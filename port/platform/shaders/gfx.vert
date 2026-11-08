#version 450
// The renderer's vertex shader for Vulkan (gfx.c's VS_SRC, its HLSL, is the Direct3D 12 one): the positions come in clip space.
// make.sh compiles it to SPIR-V, gfx_vert.h.
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 col;
layout(location = 0) centroid out vec2 v_uv;
layout(location = 1) centroid out vec4 v_col;

void main() {
    gl_Position = pos;
    v_uv = uv;
    v_col = col;
}
