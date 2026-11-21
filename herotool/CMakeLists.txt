# The hero tool (docs/HERO_PACKS.md): makes anyone's glTF model a hero pack, a mod the game plays as a hero of its own.
#
#   cmake --preset windows-clang       (or linux-clang in tools/dl)
#   cmake --build --preset windows-clang
#   build/herotool/windows-clang/herotool import MODEL.glb --id ID --markers MARKERS.json --out mods/ID
#
# It reads models with the game's own reader (port/platform/heropack.c, cgltf), so a pack it writes is one the game reads.
cmake_minimum_required(VERSION 3.25)
project(herotool VERSION 0.1.0 LANGUAGES C)

option(BASH_FETCH_SDL3 "Download and build SDL3 if it isn't installed" OFF)
find_package(SDL3 QUIET CONFIG)
if(NOT SDL3_FOUND)
  if(NOT BASH_FETCH_SDL3)
    message(FATAL_ERROR "SDL3 not found: set CMAKE_PREFIX_PATH to it, or use -DBASH_FETCH_SDL3=ON.")
  endif()
  include(FetchContent)
  FetchContent_Declare(SDL3 URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.18/SDL3-3.4.18.tar.gz)
  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(SDL3)
endif()

# cgltf, as the port fetches it (port/CMakeLists.txt): one header, MIT, at one version checked by its hash
include(FetchContent)
FetchContent_Declare(cgltf
  URL https://github.com/jkuhlmann/cgltf/archive/refs/tags/v1.15.tar.gz
  URL_HASH SHA256=84e352092e5cd6aab7f66de62ddb66beb5e6f18d412ca9d12950d7a55bfef25a
  SOURCE_SUBDIR not-built-from-here)
FetchContent_MakeAvailable(cgltf)

set(PORT ${CMAKE_CURRENT_SOURCE_DIR}/../port)
add_executable(herotool src/herotool.c src/model.c src/atlas.c src/rig.c src/moves.c src/write.c ${PORT}/platform/heropack.c)
target_include_directories(herotool PRIVATE src ${PORT}/platform ${cgltf_SOURCE_DIR})
target_compile_options(herotool PRIVATE -std=gnu17 -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers)
if(TARGET SDL3::SDL3-static)
  target_link_libraries(herotool PRIVATE SDL3::SDL3-static)
else()
  target_link_libraries(herotool PRIVATE SDL3::SDL3)
  if(WIN32)
    add_custom_command(TARGET herotool POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy_if_different
      "$<TARGET_FILE:SDL3::SDL3>" "$<TARGET_FILE_DIR:herotool>")
  endif()
endif()
if(UNIX)
  target_link_libraries(herotool PRIVATE m)
endif()
