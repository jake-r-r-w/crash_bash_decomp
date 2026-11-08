#!/bin/sh
# Makes the renderer's shaders for Vulkan and Metal from gfx.vert and gfx.frag, as the C headers gfx.c includes: SPIR-V for Vulkan
# (gfx_vert.h, gfx_frag.h), and MSL for Metal made from that by SPIRV-Cross (gfx_vert_msl.h, gfx_frag_msl.h), whose entry points it calls
# main0. Run it in the port's Linux container after changing them: tools/dl sh port/platform/shaders/make.sh
set -e
cd "$(dirname "$0")"
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

for stage in vert frag; do
    glslangValidator -V --vn "gfx_${stage}_spv" -o "gfx_${stage}.h" "gfx.${stage}"
    glslangValidator -V -o "$tmp/gfx.$stage.spv" "gfx.$stage"
    spirv-cross --msl --output "$tmp/gfx.$stage.metal" "$tmp/gfx.$stage.spv"
    {
        echo "/* gfx.$stage as MSL, made by make.sh with SPIRV-Cross from its SPIR-V */"
        echo "static const char gfx_${stage}_msl[] ="
        sed -e 's/\\/\\\\/g' -e 's/"/\\"/g' -e 's/^/    "/' -e 's/$/\\n"/' "$tmp/gfx.$stage.metal"
        echo "    ;"
    } > "gfx_${stage}_msl.h"
done
