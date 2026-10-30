# The PC build of Crash Bash (docs/PORT.md). Everything PC-only lives under port/; the game's C is
# compiled from ../src and ../include, and src/ps1/ (the PS1-only files) is left out.
#
#   cmake -S port -B build/pc -DCMAKE_C_COMPILER=i686-w64-mingw32-clang && cmake --build build/pc
cmake_minimum_required(VERSION 3.16)
project(crash_bash_pc C)

# The game's C assumes 32-bit pointers for now (docs/PORT.md).
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "The PC build is 32-bit for now: use a 32-bit compiler (docs/PORT.md, Building).")
endif()

get_filename_component(REPO "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS "${REPO}/src/main/*.c" "${REPO}/src/overlays/*.c")

add_executable(crashbash main.c link_names.c sdk/stubs.c ${GAME_SOURCES})
# INCLUDE_ASM_H keeps out include_asm.h, the PS1 assembler's macros (splat rewrites that file).
target_compile_definitions(crashbash PRIVATE BASH_PC INCLUDE_ASM_H)
# sdk/include comes first: the PC versions of PsyQ's headers, under the same names
target_include_directories(crashbash PRIVATE sdk/include "${REPO}/include")
# The dialect the game was written in, and what its compiler did with the rest: PS1 GCC's char is
# signed (ARM's isn't), and signed overflow wraps. The rest allow its loose pointers, which work when
# pointers and ints are the same size; a 64-bit build needs -Wint-conversion back (ROADMAP-B 6.5).
target_compile_options(crashbash PRIVATE
  -std=gnu89 -fno-builtin -fno-strict-aliasing -fwrapv -fsigned-char
  -Wno-incompatible-pointer-types -Wno-incompatible-function-pointer-types -Wno-int-conversion
  -Wno-deprecated-non-prototype -Wno-pointer-sign -Wno-format-security)

# The one name three overlays each define for themselves; they all link into one program here.
foreach(ovl polar race dino)
  set_property(SOURCE "${REPO}/src/overlays/${ovl}/${ovl}.c" APPEND PROPERTY
    COMPILE_DEFINITIONS XZBoxToSphere=${ovl}_XZBoxToSphere)
endforeach()
