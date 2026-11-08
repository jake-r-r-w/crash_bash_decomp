#version 450
// The renderer's fragment shader for Vulkan, gfx.c's FS_SRC (HLSL, for Direct3D 12) the same in GLSL: see there for mode. SDL3's GPU API
// puts a fragment shader's samplers in set 2 and its uniform buffers in set 3. make.sh compiles it to SPIR-V, gfx_frag.h.
layout(set = 2, binding = 0) uniform sampler2D tex;
layout(set = 3, binding = 0) uniform U {
    vec4 mode;
};
layout(location = 0) centroid in vec2 v_uv;
layout(location = 1) centroid in vec4 v_col;
layout(location = 0) out vec4 o;

void main() {
    if (mode.y > 3.5) {
        o = vec4(texture(tex, v_uv).rgb, 1.0);
        return;
    }
    vec3 c = v_col.rgb;
    if (mode.y > 0.5) {
        vec4 t = texture(tex, v_uv);
        if (t.a < 0.25) {
            discard;
        }
        if (mode.y > 1.5 && mode.y < 2.5 && t.a < 0.75) {
            discard;
        }
        if (mode.y > 2.5 && t.a > 0.75) {
            discard;
        }
        c = clamp(c * t.rgb * 2.0, 0.0, 1.0);
    }
    o = vec4(c, mode.x);
}
