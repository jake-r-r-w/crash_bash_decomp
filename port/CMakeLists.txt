# The PC build of Crash Bash (docs/PORT.md). Everything PC-only lives under port/; the game's C is
# compiled from ../src and ../include, and src/ps1/ (the PS1-only files) is left out.
#
#   cmake -S port -B build/pc -G Ninja -DCMAKE_C_COMPILER=i686-w64-mingw32-clang && cmake --build build/pc
cmake_minimum_required(VERSION 3.16)
project(crash_bash_pc C)

# The game's C assumes 32-bit pointers for now (docs/PORT.md).
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "The PC build is 32-bit for now: use a 32-bit compiler (docs/PORT.md, Building).")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

get_filename_component(REPO "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

# Each overlay has two generated variables that mark where its data starts and ends (overlays.c), placed
# by data_order.txt below.
file(GLOB GAME_MAIN CONFIGURE_DEPENDS "${REPO}/src/main/*.c")
file(GLOB GAME_ENGINE CONFIGURE_DEPENDS "${REPO}/src/overlays/engine/*.c")
set(GAME_SOURCES ${GAME_MAIN} ${GAME_ENGINE})
foreach(ovl arkenoid bash bonk chase dino hub n2o papu pogo polar race spin tank titles)
  file(GLOB ovl_sources CONFIGURE_DEPENDS "${REPO}/src/overlays/${ovl}/*.c")
  foreach(mark begin end)
    set(mark_file "${CMAKE_CURRENT_BINARY_DIR}/mark_${ovl}_${mark}.c")
    file(WRITE "${mark_file}" "char ovl_${ovl}_data_${mark}[4] = { 1 };\n")
  endforeach()
  list(APPEND GAME_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/mark_${ovl}_begin.c" ${ovl_sources}
       "${CMAKE_CURRENT_BINARY_DIR}/mark_${ovl}_end.c")
endforeach()

set(SDK_SOURCES sdk/cd.c sdk/etc.c sdk/gpu.c sdk/gpu_raster.c sdk/gte.c sdk/mcrd.c sdk/pad.c sdk/snd.c)

# SDL2 from the system, or unpacked in ext/sdl2 (docs/PORT.md, Building).
file(GLOB SDL2_HINTS "${REPO}/ext/sdl2/SDL2-*/i686-w64-mingw32/lib/cmake/SDL2")
find_package(SDL2 QUIET CONFIG HINTS ${SDL2_HINTS})
if(SDL2_FOUND)
  set(HOST_SOURCE platform/sdl.c)
else()
  message(STATUS "No SDL2: building without a window (docs/PORT.md, Building).")
  set(HOST_SOURCE platform/no_host.c)
endif()

add_executable(crashbash main.c overlays.c playtest.c link_names.c ${HOST_SOURCE} ${SDK_SOURCES} ${GAME_SOURCES})
if(SDL2_FOUND)
  if(TARGET SDL2::SDL2-static)
    target_link_libraries(crashbash PRIVATE SDL2::SDL2-static)
  else()
    target_link_libraries(crashbash PRIVATE SDL2::SDL2)
  endif()
endif()
# INCLUDE_ASM_H keeps out include_asm.h, the PS1 assembler's macros (splat rewrites that file).
target_compile_definitions(crashbash PRIVATE BASH_PC INCLUDE_ASM_H)
# sdk/include comes first: the PC versions of PsyQ's headers, under the same names
target_include_directories(crashbash PRIVATE sdk/include "${REPO}/include")
# The dialect the game was written in, and what its compiler did with the rest: PS1 GCC's char is
# signed (ARM's isn't), and signed overflow wraps. The rest allow its loose pointers, which work when
# pointers and ints are the same size; a 64-bit build needs -Wint-conversion back (ROADMAP-B 6.5).
target_compile_options(crashbash PRIVATE
  -std=gnu89 -fno-builtin -fno-strict-aliasing -fwrapv -fsigned-char -fno-omit-frame-pointer
  -fdata-sections -fno-zero-initialized-in-bss
  -Wno-incompatible-pointer-types -Wno-incompatible-function-pointer-types -Wno-int-conversion
  -Wno-deprecated-non-prototype -Wno-pointer-sign -Wno-format-security)

# A primitive's link keeps 24 bits of its address, so the program has to load below 16 MB, at a fixed
# place (docs/PORT.md).
#
# The game reads some globals through others, so its data has to keep the PS1's layout: each object is in
# a section of its own (above), which the linker lays out from a list (tools/scripts/pc_data_order.py).
# Nothing unused may go, and it writes to a const string, so constants are writable, as on the PS1.
set(DATA_ORDER "${CMAKE_CURRENT_SOURCE_DIR}/data_order.txt")
set_property(TARGET crashbash APPEND PROPERTY LINK_DEPENDS "${DATA_ORDER}")
if(WIN32)
  target_link_options(crashbash PRIVATE -Wl,--image-base=0x400000 -Wl,--disable-dynamicbase -Wl,--no-gc-sections
    "-Wl,-Xlink=-order:@${DATA_ORDER}" -Wl,-Xlink=-ignore:4037 "SHELL:-Xlinker --Xlink=-section:.rdata,rw")
else()
  target_link_options(crashbash PRIVATE -no-pie -Wl,-Ttext-segment=0x400000
    "-Wl,--symbol-ordering-file=${DATA_ORDER}" -Wl,--no-warn-symbol-ordering)
  target_link_libraries(crashbash PRIVATE m)
endif()

# The game's C isn't optimised. It reads through globals past their declared size (GameState over
# GameInfo, a 4-byte object), and an optimiser takes the size at its word: Clang deletes writes it
# thinks a later one covers. -O0 is fast enough for a PS1 game on any PC (docs/PORT.md).
set_source_files_properties(${GAME_SOURCES} PROPERTIES COMPILE_OPTIONS "-O0")

# The one name three overlays each define for themselves; they all link into one program here.
foreach(ovl polar race dino)
  set_property(SOURCE "${REPO}/src/overlays/${ovl}/${ovl}.c" APPEND PROPERTY
    COMPILE_DEFINITIONS XZBoxToSphere=${ovl}_XZBoxToSphere)
endforeach()
