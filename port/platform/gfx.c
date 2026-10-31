/* The native renderer on SDL3's GPU API (gfx.h). On Windows it runs on Direct3D 12, with its shaders compiled from the HLSL here by
 * d3dcompiler_47.dll, which comes with Windows. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "gfx.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#define MAX_TRIS 100000
#define ATLAS_SIZE 4096
#define MAX_TEXTURES 16384
#define MAX_CLIPS 64
#define MAX_DIRTY 1024
#define ATLAS_UPLOAD_BYTES (8 * 1024 * 1024)

typedef struct {
    float x, y, z, w;
    float u, v;
    uint32_t rgba;
} GpuVert;

typedef struct {
    float x, y, w;
} GfxPos;

typedef struct {
    GfxVert v[3];
    GfxPos from[3]; /* where it was in the game's last frame, if moving */
    const GfxTex *tex;
    uint64_t object;
    int face;
    int key;
    short blend;
    short clip;
    char depth;
    char moving;
} Tri;

typedef struct {
    const void *pixels, *palette;
    int w, h, bpp;
    uint64_t hash;
    GfxTex t;
} Cached;

static SDL_Window *Window;
static SDL_GPUDevice *Device;
static SDL_GPUTexture *Target, *Base, *Depth, *Atlas;
static SDL_GPUSampler *Sampler;
static SDL_GPUBuffer *VertexBuf;
static SDL_GPUTransferBuffer *VertexXfer, *AtlasXfer, *ShotXfer;
/* solid or textured for each blend, by how the fragment shader is told to treat alpha */
static SDL_GPUGraphicsPipeline *PipeOpaque, *PipeDepth, *PipeHalf, *PipeAdd, *PipeSub, *PipeQuarter;
static int TargetW, TargetH;
static int ScreenW = 640, ScreenH = 480;
static uint8_t ClearRGB[3];
static int ClearPending = 1;

/* The game's frame being added to, and the one being shown. */
static Tri *Tris, *Shown;
static int *Order;
static int NumTris, NumShown;
static SDL_Rect Clips[MAX_CLIPS], ShownClips[MAX_CLIPS];
static int NumClips, CurClip;
static int ShownClear; /* the shown frame starts from a cleared picture */

static int DepthOn;
static uint64_t MarkObject;
static int MarkFace;

/* Above the game's own rate: how far through the step to the shown frame the next picture is. */
static int Stepping;
static int StepsDone, StepsPerFrame;
static int BaseStale;

static uint8_t *AtlasPixels;
static Cached *Textures;
static int NumTextures;
static int ShelfX, ShelfY, ShelfH;
static SDL_Rect Dirty[MAX_DIRTY];
static int NumDirty;
static const char *ShotPath;

static const char *const VS_SRC =
    "struct In { float4 pos : TEXCOORD0; float2 uv : TEXCOORD1; float4 col : TEXCOORD2; };\n"
    "struct Out { float2 uv : TEXCOORD0; float4 col : TEXCOORD1; float4 pos : SV_Position; };\n"
    "Out main(In i) { Out o; o.pos = i.pos; o.uv = i.uv; o.col = i.col; return o; }\n";

/* mode.x: the alpha written, for the blend. mode.y: 0 solid, 1 textured keeping all but clear texels, 2 textured keeping only
 * opaque ones, 3 textured keeping only the semi-transparent ones (a texel's alpha: 0 clear, 0.5 semi, 1 opaque). */
static const char *const FS_SRC =
    "Texture2D<float4> tex : register(t0, space2);\n"
    "SamplerState smp : register(s0, space2);\n"
    "cbuffer U : register(b0, space3) { float4 mode; };\n"
    "struct In { float2 uv : TEXCOORD0; float4 col : TEXCOORD1; };\n"
    "float4 main(In i) : SV_Target0 {\n"
    "  float3 c = i.col.rgb;\n"
    "  if (mode.y > 0.5) {\n"
    "    float4 t = tex.Sample(smp, i.uv);\n"
    "    if (t.a < 0.25) discard;\n"
    "    if (mode.y > 1.5 && mode.y < 2.5 && t.a < 0.75) discard;\n"
    "    if (mode.y > 2.5 && t.a > 0.75) discard;\n"
    "    c = saturate(c * t.rgb * 2.0);\n"
    "  }\n"
    "  return float4(c, mode.x);\n"
    "}\n";

#ifdef _WIN32
typedef struct ID3DBlobVtbl {
    long(__stdcall *QueryInterface)(void *, const void *, void **);
    unsigned long(__stdcall *AddRef)(void *);
    unsigned long(__stdcall *Release)(void *);
    void *(__stdcall *GetBufferPointer)(void *);
    size_t(__stdcall *GetBufferSize)(void *);
} ID3DBlobVtbl;
typedef struct { ID3DBlobVtbl *v; } ID3DBlob_;
typedef long(__stdcall *D3DCompileFn)(const void *, size_t, const char *, const void *, void *, const char *, const char *, unsigned, unsigned,
                                      ID3DBlob_ **, ID3DBlob_ **);

static SDL_GPUShader *make_shader(const char *src, const char *profile, SDL_GPUShaderStage stage, int samplers, int uniforms) {
    static D3DCompileFn compile;
    ID3DBlob_ *code = NULL, *err = NULL;
    SDL_GPUShaderCreateInfo info;
    SDL_GPUShader *s;

    if (compile == NULL) {
        HMODULE dll = LoadLibraryA("d3dcompiler_47.dll");
        compile = dll ? (D3DCompileFn)GetProcAddress(dll, "D3DCompile") : NULL;
        if (compile == NULL) {
            fprintf(stderr, "No d3dcompiler_47.dll\n");
            return NULL;
        }
    }
    if (compile(src, strlen(src), "gfx", NULL, NULL, "main", profile, 0, 0, &code, &err) < 0) {
        fprintf(stderr, "Shader: %s\n", err ? (char *)err->v->GetBufferPointer(err) : "failed");
        return NULL;
    }
    SDL_zero(info);
    info.code = code->v->GetBufferPointer(code);
    info.code_size = code->v->GetBufferSize(code);
    info.entrypoint = "main";
    info.format = SDL_GPU_SHADERFORMAT_DXBC;
    info.stage = stage;
    info.num_samplers = samplers;
    info.num_uniform_buffers = uniforms;
    s = SDL_CreateGPUShader(Device, &info);
    if (s == NULL) {
        fprintf(stderr, "SDL_CreateGPUShader: %s\n", SDL_GetError());
    }
    return s;
}
#else
static SDL_GPUShader *make_shader(const char *src, const char *profile, SDL_GPUShaderStage stage, int samplers, int uniforms) {
    (void)src; (void)profile; (void)stage; (void)samplers; (void)uniforms;
    fprintf(stderr, "No shaders for this system yet\n");
    return NULL;
}
#endif

static SDL_GPUGraphicsPipeline *make_pipeline(SDL_GPUShader *vs, SDL_GPUShader *fs, SDL_GPUBlendFactor src, SDL_GPUBlendFactor dst, SDL_GPUBlendOp op, int blend, int depth) {
    SDL_GPUGraphicsPipelineCreateInfo p;
    SDL_GPUVertexBufferDescription vb;
    SDL_GPUVertexAttribute a[3];
    SDL_GPUColorTargetDescription c;

    SDL_zero(p);
    SDL_zero(vb);
    SDL_zero(a);
    SDL_zero(c);
    vb.slot = 0;
    vb.pitch = sizeof(GpuVert);
    vb.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    a[0].location = 0;
    a[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
    a[0].offset = 0;
    a[1].location = 1;
    a[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
    a[1].offset = 16;
    a[2].location = 2;
    a[2].format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM;
    a[2].offset = 24;
    c.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    if (blend) {
        c.blend_state.enable_blend = true;
        c.blend_state.color_blend_op = op;
        c.blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
        c.blend_state.src_color_blendfactor = src;
        c.blend_state.dst_color_blendfactor = dst;
        c.blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO;
        c.blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
    }
    p.vertex_shader = vs;
    p.fragment_shader = fs;
    p.vertex_input_state.vertex_buffer_descriptions = &vb;
    p.vertex_input_state.num_vertex_buffers = 1;
    p.vertex_input_state.vertex_attributes = a;
    p.vertex_input_state.num_vertex_attributes = 3;
    p.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    p.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
    p.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    p.target_info.color_target_descriptions = &c;
    p.target_info.num_color_targets = 1;
    p.target_info.has_depth_stencil_target = true;
    p.target_info.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
    p.depth_stencil_state.enable_depth_test = depth != 0;
    p.depth_stencil_state.enable_depth_write = depth != 0;
    p.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;
    return SDL_CreateGPUGraphicsPipeline(Device, &p);
}

int gfx_init(void *window, int width, int height, int vsync, int fps) {
    SDL_GPUShader *vs, *fs;
    SDL_GPUTextureCreateInfo t;
    SDL_GPUSamplerCreateInfo sc;
    SDL_GPUBufferCreateInfo bc;
    SDL_GPUTransferBufferCreateInfo tc;

    Window = window;
    TargetW = width;
    TargetH = height;
    Device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXBC, false, "direct3d12");
    if (Device == NULL) {
        fprintf(stderr, "SDL_CreateGPUDevice: %s\n", SDL_GetError());
        return 0;
    }
    if (!SDL_ClaimWindowForGPUDevice(Device, Window)) {
        fprintf(stderr, "SDL_ClaimWindowForGPUDevice: %s\n", SDL_GetError());
        return 0;
    }
    if (!vsync && SDL_WindowSupportsGPUPresentMode(Device, Window, SDL_GPU_PRESENTMODE_IMMEDIATE)) {
        SDL_SetGPUSwapchainParameters(Device, Window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_IMMEDIATE);
    }
    vs = make_shader(VS_SRC, "vs_5_1", SDL_GPU_SHADERSTAGE_VERTEX, 0, 0);
    fs = make_shader(FS_SRC, "ps_5_1", SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 1);
    if (vs == NULL || fs == NULL) {
        return 0;
    }
    PipeOpaque = make_pipeline(vs, fs, 0, 0, 0, 0, 0);
    PipeDepth = make_pipeline(vs, fs, 0, 0, 0, 0, 1);
    PipeHalf = make_pipeline(vs, fs, SDL_GPU_BLENDFACTOR_SRC_ALPHA, SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_GPU_BLENDOP_ADD, 1, 0);
    PipeAdd = make_pipeline(vs, fs, SDL_GPU_BLENDFACTOR_ONE, SDL_GPU_BLENDFACTOR_ONE, SDL_GPU_BLENDOP_ADD, 1, 0);
    PipeSub = make_pipeline(vs, fs, SDL_GPU_BLENDFACTOR_ONE, SDL_GPU_BLENDFACTOR_ONE, SDL_GPU_BLENDOP_REVERSE_SUBTRACT, 1, 0);
    PipeQuarter = make_pipeline(vs, fs, SDL_GPU_BLENDFACTOR_SRC_ALPHA, SDL_GPU_BLENDFACTOR_ONE, SDL_GPU_BLENDOP_ADD, 1, 0);
    SDL_ReleaseGPUShader(Device, vs);
    SDL_ReleaseGPUShader(Device, fs);
    if (!PipeOpaque || !PipeDepth || !PipeHalf || !PipeAdd || !PipeSub || !PipeQuarter) {
        fprintf(stderr, "Pipeline: %s\n", SDL_GetError());
        return 0;
    }

    SDL_zero(t);
    t.type = SDL_GPU_TEXTURETYPE_2D;
    t.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    t.layer_count_or_depth = 1;
    t.num_levels = 1;
    t.width = width;
    t.height = height;
    t.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
    Target = SDL_CreateGPUTexture(Device, &t);
    t.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    Base = SDL_CreateGPUTexture(Device, &t);
    t.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
    t.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    Depth = SDL_CreateGPUTexture(Device, &t);
    t.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    t.width = ATLAS_SIZE;
    t.height = ATLAS_SIZE;
    t.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    Atlas = SDL_CreateGPUTexture(Device, &t);

    SDL_zero(sc);
    sc.min_filter = SDL_GPU_FILTER_NEAREST;
    sc.mag_filter = SDL_GPU_FILTER_NEAREST;
    sc.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    sc.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    Sampler = SDL_CreateGPUSampler(Device, &sc);

    SDL_zero(bc);
    bc.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    bc.size = MAX_TRIS * 3 * sizeof(GpuVert);
    VertexBuf = SDL_CreateGPUBuffer(Device, &bc);
    SDL_zero(tc);
    tc.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    tc.size = bc.size;
    VertexXfer = SDL_CreateGPUTransferBuffer(Device, &tc);
    tc.size = ATLAS_UPLOAD_BYTES;
    AtlasXfer = SDL_CreateGPUTransferBuffer(Device, &tc);
    tc.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
    tc.size = width * height * 4;
    ShotXfer = SDL_CreateGPUTransferBuffer(Device, &tc);
    if (!Target || !Base || !Depth || !Atlas || !Sampler || !VertexBuf || !VertexXfer || !AtlasXfer || !ShotXfer) {
        fprintf(stderr, "GPU resources: %s\n", SDL_GetError());
        return 0;
    }
    Stepping = fps > 30;
    StepsPerFrame = 2 * fps / 60;
    Tris = malloc(sizeof(Tri) * MAX_TRIS);
    Shown = malloc(sizeof(Tri) * MAX_TRIS);
    Order = malloc(sizeof(int) * MAX_TRIS);
    AtlasPixels = calloc(ATLAS_SIZE * ATLAS_SIZE, 4);
    Textures = malloc(sizeof(Cached) * MAX_TEXTURES);
    gfx_begin_frame();
    return 1;
}

void gfx_quit(void) {
    if (Device != NULL) {
        SDL_WaitForGPUIdle(Device);
    }
}

void gfx_clear(uint8_t r, uint8_t g, uint8_t b) {
    ClearPending = 1;
    ClearRGB[0] = r;
    ClearRGB[1] = g;
    ClearRGB[2] = b;
}

void gfx_set_screen(int width, int height) {
    ScreenW = width;
    ScreenH = height;
}

void gfx_set_clip(int x, int y, int w, int h) {
    float sx = (float)TargetW / ScreenW, sy = (float)TargetH / ScreenH;
    SDL_Rect r;

    r.x = (int)(x * sx);
    r.y = (int)(y * sy);
    r.w = (int)(w * sx);
    r.h = (int)(h * sy);
    if (NumClips > 0 && memcmp(&Clips[CurClip], &r, sizeof(r)) == 0) {
        return;
    }
    if (NumClips < MAX_CLIPS) {
        CurClip = NumClips++;
        Clips[CurClip] = r;
    }
}

void gfx_set_depth(int layer) {
    DepthOn = layer;
}

void gfx_mark(uint64_t object, int face) {
    MarkObject = object;
    MarkFace = face;
}

void gfx_begin_frame(void) {
    NumTris = 0;
    NumClips = 0;
    CurClip = 0;
    gfx_set_clip(0, 0, ScreenW, ScreenH);
}

void gfx_tri(int key, const GfxVert v[3], const GfxTex *tex, int blend) {
    Tri *t;

    if (NumTris >= MAX_TRIS) {
        return;
    }
    t = &Tris[NumTris++];
    memcpy(t->v, v, sizeof(t->v));
    t->tex = tex;
    t->key = key;
    t->blend = (short)blend;
    t->clip = (short)CurClip;
    t->depth = (char)DepthOn;
    t->moving = 0;
    t->object = MarkObject;
    t->face = MarkFace++;
}

void gfx_state_blend(int key, int blend) {
    GfxVert none[3];

    memset(none, 0, sizeof(none));
    gfx_tri(key, none, NULL, 100 + blend);
}

void gfx_quad(int key, const GfxVert v[4], const GfxTex *tex, int blend) {
    GfxVert a[3];

    /* the PS1 draws the first triangle first, and a later add in a bucket draws earlier */
    a[0] = v[1];
    a[1] = v[3];
    a[2] = v[2];
    gfx_tri(key, a, tex, blend);
    gfx_tri(key, v, tex, blend);
}

/* the texture atlas */
static void atlas_reset(void) {
    gfx_forget_textures();
    NumTextures = 0;
    ShelfX = ShelfY = ShelfH = 0;
}

static uint32_t decode_colour(uint16_t c) {
    uint32_t r, g, b, a;

    if (c == 0) {
        return 0;
    }
    r = (c & 0x1F) * 255 / 31;
    g = ((c >> 5) & 0x1F) * 255 / 31;
    b = ((c >> 10) & 0x1F) * 255 / 31;
    a = (c & 0x8000) ? 128 : 255;
    return r | (g << 8) | (b << 16) | (a << 24);
}

/* Two levels: ids seen since the last gfx_forget_textures, then the pixels' own content, for when the same pixels load again. */
#define ID_SLOTS 65536
typedef struct {
    uint64_t id;
    int gen, index;
} IdSlot;
static IdSlot IdCache[ID_SLOTS];
static int IdGen = 1;

static uint64_t hash_bytes(const uint8_t *p, size_t n, uint64_t h) {
    size_t i;

    for (i = 0; i < n; i++) {
        h = (h ^ p[i]) * 0x100000001B3ULL;
    }
    return h;
}

/* Copies the rect of the atlas to the GPU with the frame. */
static void mark_dirty(int x, int y, int w, int h) {
    if (NumDirty < MAX_DIRTY) {
        Dirty[NumDirty].x = x;
        Dirty[NumDirty].y = y;
        Dirty[NumDirty].w = w;
        Dirty[NumDirty].h = h;
        NumDirty++;
    }
}

const GfxTex *gfx_texture(uint64_t id, const void *pixels, int w, int h, int bpp, const void *palette) {
    Cached *c;
    uint32_t *out;
    int i, x, y;
    uint64_t hash;
    size_t bytes;
    IdSlot *slot = &IdCache[(id * 0x9E3779B97F4A7C15ULL) >> 48];

    if (id != 0 && slot->gen == IdGen && slot->id == id) {
        return &Textures[slot->index].t;
    }
    if (w <= 0 || h <= 0 || w > 1024 || h > 1024) {
        return NULL;
    }
    bytes = bpp == 16 ? (size_t)w * h * 2 : bpp == 8 ? (size_t)((w + 1) & ~1) * h : (size_t)((w + 3) / 4 * 2) * h;
    hash = hash_bytes(pixels, bytes, 0xCBF29CE484222325ULL);
    if (palette != NULL) {
        hash = hash_bytes(palette, bpp == 8 ? 512 : 32, hash);
    }
    for (i = NumTextures - 1; i >= 0; i--) {
        c = &Textures[i];
        if (c->hash == hash && c->w == w && c->h == h) {
            slot->gen = IdGen;
            slot->id = id;
            slot->index = i;
            return &c->t;
        }
    }
    if (w <= 0 || h <= 0 || w > 1024 || h > 1024) {
        return NULL;
    }
    if (NumTextures >= MAX_TEXTURES) {
        atlas_reset();
    }
    if (ShelfX + w + 2 > ATLAS_SIZE) {
        ShelfX = 0;
        ShelfY += ShelfH;
        ShelfH = 0;
    }
    if (ShelfY + h + 2 > ATLAS_SIZE) {
        atlas_reset();
    }
    slot->gen = IdGen;
    slot->id = id;
    slot->index = NumTextures;
    c = &Textures[NumTextures++];
    c->hash = hash;
    c->pixels = pixels;
    c->palette = palette;
    c->w = w;
    c->h = h;
    c->bpp = bpp;
    /* one texel of border all round, copied from the edges below */
    c->t.u0 = (float)(ShelfX + 1) / ATLAS_SIZE;
    c->t.v0 = (float)(ShelfY + 1) / ATLAS_SIZE;
    c->t.us = 1.0f / ATLAS_SIZE;
    c->t.vs = 1.0f / ATLAS_SIZE;
    for (y = 0; y < h; y++) {
        out = (uint32_t *)AtlasPixels + (ShelfY + 1 + y) * ATLAS_SIZE + ShelfX + 1;
        if (bpp == 16) {
            const uint16_t *row = (const uint16_t *)pixels + y * w;
            for (x = 0; x < w; x++) {
                out[x] = decode_colour(row[x]) | 0xFF000000u; /* a full-screen picture: black is black here */
            }
        } else if (bpp == 8) {
            const uint8_t *row = (const uint8_t *)pixels + y * ((w + 1) & ~1);
            const uint16_t *pal = palette;
            for (x = 0; x < w; x++) {
                out[x] = decode_colour(pal ? pal[row[x]] : 0x7FFF);
            }
        } else {
            const uint8_t *row = (const uint8_t *)pixels + y * ((w + 3) / 4 * 2);
            const uint16_t *pal = palette;
            for (x = 0; x < w; x++) {
                int n = (row[x / 2] >> ((x & 1) * 4)) & 15;
                out[x] = decode_colour(pal ? pal[n] : 0x7FFF);
            }
        }
    }
    {
        uint32_t *px = (uint32_t *)AtlasPixels;

        for (y = 1; y <= h; y++) {
            out = px + (ShelfY + y) * ATLAS_SIZE + ShelfX;
            out[0] = out[1];
            out[w + 1] = out[w];
        }
        memcpy(px + ShelfY * ATLAS_SIZE + ShelfX, px + (ShelfY + 1) * ATLAS_SIZE + ShelfX, (size_t)(w + 2) * 4);
        memcpy(px + (ShelfY + h + 1) * ATLAS_SIZE + ShelfX, px + (ShelfY + h) * ATLAS_SIZE + ShelfX, (size_t)(w + 2) * 4);
    }
    mark_dirty(ShelfX, ShelfY, w + 2, h + 2);
    ShelfX += w + 2;
    if (h + 2 > ShelfH) {
        ShelfH = h + 2;
    }
    return &c->t;
}

void gfx_forget_textures(void) {
    IdGen++;
}

void gfx_screenshot(const char *path) {
    ShotPath = path;
}

static void sort_tris(void) {
    int i;
    int minK = 0x7FFFFFFF, maxK = -0x7FFFFFFF;
    static int *counts;
    static int countsSize;
    int range, pos;

    for (i = 0; i < NumShown; i++) {
        if (Shown[i].key < minK) minK = Shown[i].key;
        if (Shown[i].key > maxK) maxK = Shown[i].key;
    }
    if (NumShown == 0) {
        return;
    }
    range = maxK - minK + 1;
    if (range > countsSize) {
        free(counts);
        counts = malloc(sizeof(int) * range);
        countsSize = range;
    }
    memset(counts, 0, sizeof(int) * range);
    for (i = 0; i < NumShown; i++) {
        counts[maxK - Shown[i].key]++;
    }
    pos = 0;
    for (i = 0; i < range; i++) {
        int n = counts[i];
        counts[i] = pos;
        pos += n;
    }
    /* within a bucket the last added draws first, as addPrim's list has it */
    for (i = NumShown - 1; i >= 0; i--) {
        Order[counts[maxK - Shown[i].key]++] = i;
    }
}

/* Depth for the triangles that test it, from 1 to the GTE's furthest. */
#define DEPTH_NEAR 1.0f
#define DEPTH_FAR 65536.0f

static void put_vert(GpuVert *o, const GfxVert *v, const GfxPos *from, float step, const GfxTex *tex, int depth) {
    float x = v->x, y = v->y, w = v->w;

    if (from != NULL) {
        x = from->x + (x - from->x) * step;
        y = from->y + (y - from->y) * step;
        w = from->w + (w - from->w) * step;
    }
    if (w < 0.001f) {
        w = 0.001f;
    }
    o->x = (x / ScreenW * 2.0f - 1.0f) * w;
    o->y = (1.0f - y / ScreenH * 2.0f) * w;
    /* linear in w, so it interpolates across the triangle like 1/w's depth does */
    o->z = depth ? ((depth - 1) * w + (w - DEPTH_NEAR) * DEPTH_FAR / (DEPTH_FAR - DEPTH_NEAR)) / GFX_DEPTH_LAYERS : 0.5f * w;
    o->w = w;
    if (tex != NULL) {
        o->u = tex->u0 + v->u * tex->us;
        o->v = tex->v0 + v->v * tex->vs;
    } else {
        o->u = 0;
        o->v = 0;
    }
    o->rgba = v->rgba;
}

/* One run of triangles with the same state, in draw order. */
typedef struct {
    int first, count;
    short blend, textured, clip;
} Run;

/* The largest part of a window with the picture's shape, centred. */
static void fit_picture(Uint32 sw, Uint32 sh, SDL_GPUBlitRegion *r) {
    Uint32 w = sw, h = sh;

    if ((Uint64)sw * TargetH > (Uint64)sh * TargetW) {
        w = (Uint32)((Uint64)sh * TargetW / TargetH);
    } else {
        h = (Uint32)((Uint64)sw * TargetH / TargetW);
    }
    r->x = (sw - w) / 2;
    r->y = (sh - h) / 2;
    r->w = w;
    r->h = h;
}

/* A blend for a depth-tested opaque triangle, past the others in Run.blend. */
#define BLEND_DEPTH (-3)

/* Draws the shown frame, step of the way from where it was, and shows it. */
static void draw_shown(float step) {
    static Run runs[MAX_TRIS];
    int numRuns = 0;
    SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(Device);
    SDL_GPUTexture *swap = NULL;
    Uint32 sw = 0, sh = 0;
    GpuVert *gv;
    SDL_GPUCopyPass *copy;
    SDL_GPURenderPass *pass;
    SDL_GPUColorTargetInfo ct;
    SDL_GPUDepthStencilTargetInfo dt;
    int i;
    Uint32 used;

    if (cmd == NULL) {
        return;
    }

    /* Each semi-transparent textured triangle draws twice: its opaque texels, then its semi-transparent ones. */
    gv = SDL_MapGPUTransferBuffer(Device, VertexXfer, true);
    used = 0;
    {
    int stateBlend = GFX_HALF;
    for (i = 0; i < NumShown; i++) {
        const Tri *t = &Shown[Order[i]];
        int textured = t->tex != NULL;
        int passes = (t->blend != GFX_OPAQUE && textured) ? 2 : 1;
        int p;

        if (t->blend >= 100) {
            stateBlend = t->blend - 100;
            continue;
        }

        for (p = 0; p < passes && used + 3 <= MAX_TRIS * 3; p++) {
            int b = (passes == 2 && p == 0) ? -2 : t->blend == GFX_STATE_BLEND ? stateBlend : t->blend; /* -2: the opaque texels of a semi-transparent triangle */
            int depth = b == GFX_OPAQUE ? t->depth : 0;
            Run *r = numRuns ? &runs[numRuns - 1] : NULL;

            if (depth) {
                b = BLEND_DEPTH;
            }
            if (r == NULL || r->blend != b || r->textured != textured || r->clip != t->clip) {
                r = &runs[numRuns++];
                r->first = used;
                r->count = 0;
                r->blend = (short)b;
                r->textured = (short)textured;
                r->clip = t->clip;
            }
            put_vert(&gv[used++], &t->v[0], t->moving ? &t->from[0] : NULL, step, t->tex, depth);
            put_vert(&gv[used++], &t->v[1], t->moving ? &t->from[1] : NULL, step, t->tex, depth);
            put_vert(&gv[used++], &t->v[2], t->moving ? &t->from[2] : NULL, step, t->tex, depth);
            r->count += 3;
        }
    }
    }
    SDL_UnmapGPUTransferBuffer(Device, VertexXfer);

    copy = SDL_BeginGPUCopyPass(cmd);
    if (used != 0) {
        SDL_GPUTransferBufferLocation src;
        SDL_GPUBufferRegion dst;

        SDL_zero(src);
        SDL_zero(dst);
        src.transfer_buffer = VertexXfer;
        dst.buffer = VertexBuf;
        dst.size = used * sizeof(GpuVert);
        SDL_UploadToGPUBuffer(copy, &src, &dst, true);
    }
    if (NumDirty != 0) {
        uint8_t *a = SDL_MapGPUTransferBuffer(Device, AtlasXfer, true);
        Uint32 offset = 0;
        int done = 0;

        /* what doesn't fit this frame goes in the next */
        while (done < NumDirty) {
            const SDL_Rect *r = &Dirty[done];
            Uint32 size = (Uint32)r->w * r->h * 4;
            SDL_GPUTextureTransferInfo src;
            SDL_GPUTextureRegion dst;
            int y;

            if (offset + size > ATLAS_UPLOAD_BYTES) {
                break;
            }
            for (y = 0; y < r->h; y++) {
                memcpy(a + offset + (size_t)y * r->w * 4, AtlasPixels + ((size_t)(r->y + y) * ATLAS_SIZE + r->x) * 4, (size_t)r->w * 4);
            }
            SDL_zero(src);
            SDL_zero(dst);
            src.transfer_buffer = AtlasXfer;
            src.offset = offset;
            src.pixels_per_row = r->w;
            src.rows_per_layer = r->h;
            dst.texture = Atlas;
            dst.x = r->x;
            dst.y = r->y;
            dst.w = r->w;
            dst.h = r->h;
            dst.d = 1;
            SDL_UploadToGPUTexture(copy, &src, &dst, false); /* no cycling: the rest of the atlas has to stay */
            offset += size;
            done++;
        }
        SDL_UnmapGPUTransferBuffer(Device, AtlasXfer);
        NumDirty -= done;
        memmove(Dirty, Dirty + done, sizeof(Dirty[0]) * NumDirty);
    }
    if (Stepping && !ShownClear) {
        /* each step starts from the same picture, so what blends over it doesn't build up */
        SDL_GPUTextureLocation a, b;

        SDL_zero(a);
        SDL_zero(b);
        a.texture = Target;
        b.texture = Base;
        if (BaseStale) {
            SDL_CopyGPUTextureToTexture(copy, &a, &b, TargetW, TargetH, 1, false);
            BaseStale = 0;
        }
        SDL_CopyGPUTextureToTexture(copy, &b, &a, TargetW, TargetH, 1, false);
    }
    SDL_EndGPUCopyPass(copy);

    SDL_zero(ct);
    ct.texture = Target;
    ct.clear_color.r = ClearRGB[0] / 255.0f;
    ct.clear_color.g = ClearRGB[1] / 255.0f;
    ct.clear_color.b = ClearRGB[2] / 255.0f;
    ct.clear_color.a = 1;
    ct.load_op = ShownClear ? SDL_GPU_LOADOP_CLEAR : SDL_GPU_LOADOP_LOAD;
    ct.store_op = SDL_GPU_STOREOP_STORE;
    SDL_zero(dt);
    dt.texture = Depth;
    dt.clear_depth = 1;
    dt.load_op = SDL_GPU_LOADOP_CLEAR;
    dt.store_op = SDL_GPU_STOREOP_DONT_CARE;
    dt.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
    dt.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
    pass = SDL_BeginGPURenderPass(cmd, &ct, 1, &dt);
    if (numRuns != 0) {
        SDL_GPUBufferBinding vb;
        SDL_GPUTextureSamplerBinding tb;

        vb.buffer = VertexBuf;
        vb.offset = 0;
        SDL_BindGPUVertexBuffers(pass, 0, &vb, 1);
        tb.texture = Atlas;
        tb.sampler = Sampler;
        for (i = 0; i < numRuns; i++) {
            const Run *r = &runs[i];
            float mode[4];
            SDL_GPUGraphicsPipeline *pipe;

            mode[1] = 1;
            mode[2] = mode[3] = 0;
            switch (r->blend) {
            case GFX_HALF:
                pipe = PipeHalf;
                mode[0] = 0.5f;
                mode[1] = 3;
                break;
            case GFX_ADD:
                pipe = PipeAdd;
                mode[0] = 1;
                mode[1] = 3;
                break;
            case GFX_SUB:
                pipe = PipeSub;
                mode[0] = 1;
                mode[1] = 3;
                break;
            case GFX_ADD_QUARTER:
                pipe = PipeQuarter;
                mode[0] = 0.25f;
                mode[1] = 3;
                break;
            case -2:
                pipe = PipeOpaque;
                mode[0] = 1;
                mode[1] = 2;
                break;
            case BLEND_DEPTH:
                pipe = PipeDepth;
                mode[0] = 1;
                mode[1] = 1;
                break;
            default:
                pipe = PipeOpaque;
                mode[0] = 1;
                mode[1] = 1;
                break;
            }
            if (!r->textured) {
                mode[1] = 0;
            }
            SDL_BindGPUGraphicsPipeline(pass, pipe);
            SDL_SetGPUScissor(pass, &ShownClips[r->clip]);
            SDL_BindGPUFragmentSamplers(pass, 0, &tb, 1);
            SDL_PushGPUFragmentUniformData(cmd, 0, mode, sizeof(mode));
            SDL_DrawGPUPrimitives(pass, r->count, 1, r->first, 0);
        }
    }
    SDL_EndGPURenderPass(pass);

    if (ShotPath != NULL) {
        SDL_GPUCopyPass *c2 = SDL_BeginGPUCopyPass(cmd);
        SDL_GPUTextureRegion src;
        SDL_GPUTextureTransferInfo dst;

        SDL_zero(src);
        SDL_zero(dst);
        src.texture = Target;
        src.w = TargetW;
        src.h = TargetH;
        src.d = 1;
        dst.transfer_buffer = ShotXfer;
        dst.pixels_per_row = TargetW;
        dst.rows_per_layer = TargetH;
        SDL_DownloadFromGPUTexture(c2, &src, &dst);
        SDL_EndGPUCopyPass(c2);
    }

    /* above the display's rate a picture that doesn't fit is dropped, and the game's clock doesn't wait */
    if ((Stepping ? SDL_AcquireGPUSwapchainTexture(cmd, Window, &swap, &sw, &sh)
                  : SDL_WaitAndAcquireGPUSwapchainTexture(cmd, Window, &swap, &sw, &sh)) && swap != NULL) {
        SDL_GPUBlitInfo b;

        SDL_zero(b);
        b.source.texture = Target;
        b.source.w = TargetW;
        b.source.h = TargetH;
        b.destination.texture = swap;
        fit_picture(sw, sh, &b.destination);
        b.load_op = SDL_GPU_LOADOP_CLEAR;
        b.filter = SDL_GPU_FILTER_LINEAR;
        SDL_BlitGPUTexture(cmd, &b);
    }
    if (ShotPath != NULL) {
        SDL_GPUFence *f = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);
        SDL_Surface *s;
        void *px;

        SDL_WaitForGPUFences(Device, true, &f, 1);
        SDL_ReleaseGPUFence(Device, f);
        px = SDL_MapGPUTransferBuffer(Device, ShotXfer, false);
        s = SDL_CreateSurfaceFrom(TargetW, TargetH, SDL_PIXELFORMAT_ABGR8888, px, TargetW * 4);
        if (s != NULL) {
            SDL_SavePNG(s, ShotPath);
            SDL_DestroySurface(s);
        }
        SDL_UnmapGPUTransferBuffer(Device, ShotXfer);
        ShotPath = NULL;
    } else {
        SDL_SubmitGPUCommandBuffer(cmd);
    }
}

/* Finds each named triangle of the new frame in the one shown, to move it from there. */
#define FIND_SLOTS (1 << 18)
#define MOVE_LIMIT 0.25f /* of the screen: further than this is a jump, not movement */

typedef struct {
    uint64_t object;
    int face, index, gen;
} FindSlot;

static uint32_t find_hash(uint64_t object, int face) {
    uint64_t h = (object ^ ((uint64_t)face * 0x9E3779B1u)) * 0x9E3779B97F4A7C15ULL;

    return (uint32_t)(h >> 46);
}

static void find_movement(void) {
    static FindSlot slots[FIND_SLOTS];
    static int gen;
    float limit_x = ScreenW * MOVE_LIMIT, limit_y = ScreenH * MOVE_LIMIT;
    int i, j;

    gen++;
    for (i = 0; i < NumShown; i++) {
        const Tri *t = &Shown[i];

        if (t->object != 0) {
            uint32_t h = find_hash(t->object, t->face);

            while (slots[h].gen == gen) {
                h = (h + 1) & (FIND_SLOTS - 1);
            }
            slots[h].gen = gen;
            slots[h].object = t->object;
            slots[h].face = t->face;
            slots[h].index = i;
        }
    }
    for (i = 0; i < NumTris; i++) {
        Tri *t = &Tris[i];

        if (t->object != 0) {
            uint32_t h = find_hash(t->object, t->face);

            while (slots[h].gen == gen) {
                if (slots[h].object == t->object && slots[h].face == t->face) {
                    const Tri *old = &Shown[slots[h].index];

                    t->moving = 1;
                    for (j = 0; j < 3; j++) {
                        t->from[j].x = old->v[j].x;
                        t->from[j].y = old->v[j].y;
                        t->from[j].w = old->v[j].w;
                        if (SDL_fabsf(t->from[j].x - t->v[j].x) > limit_x || SDL_fabsf(t->from[j].y - t->v[j].y) > limit_y) {
                            t->moving = 0;
                        }
                    }
                    break;
                }
                h = (h + 1) & (FIND_SLOTS - 1);
            }
        }
    }
}

void gfx_end_frame(void) {
    Tri *swap = Tris;

    if (Stepping) {
        find_movement();
        /* as many steps as this frame took to come round */
        if (StepsDone > 0) {
            StepsPerFrame = StepsDone;
        }
        StepsDone = 0;
        BaseStale = 1;
    }
    Tris = Shown;
    Shown = swap;
    NumShown = NumTris;
    memcpy(ShownClips, Clips, sizeof(ShownClips));
    ShownClear = ClearPending;
    ClearPending = 0;
    sort_tris();
    if (!Stepping) {
        draw_shown(1);
    }
    gfx_begin_frame();
}

void gfx_present(void) {
    if (Stepping) {
        StepsDone++;
        draw_shown(StepsDone >= StepsPerFrame ? 1.0f : (float)StepsDone / StepsPerFrame);
    }
}
