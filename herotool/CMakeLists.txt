# The hero tool (docs/HERO_PACKS.md): makes anyone's glTF model a hero pack, a mod the game plays as a hero of its own.
#
#   cmake --preset windows-clang       (or linux-clang in tools/dl)
#   cmake --build --preset windows-clang
#   build/herotool/windows-clang/herotool import MODEL.glb --id ID --markers MARKERS.json --out mods/ID
#
# It reads models with the game's own reader (port/platform/heropack.c, cgltf), so a pack it writes is one the game reads.
cmake_minimum_required(VERSION 3.25)
project(herotool VERSION 0.1.0 LANGUAGES C CXX)

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
# Dear ImGui, the tool's window (src/gui.cpp): MIT, at one version checked by its hash, with its SDL3 and SDL renderer backends
FetchContent_Declare(imgui
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9b.tar.gz
  URL_HASH SHA256=8e1bbc76c71d74fef2fb85db7e7ca8eba13d6a86623c54992b60162db554ffdb
  SOURCE_SUBDIR not-built-from-here)
FetchContent_MakeAvailable(imgui)
# Fredoka, the window's font (gui.cpp, ui_scale; the user's pick, 2026-11-22): SIL Open Font License 1.1, from google/fonts at one
# commit, checked by its hash, built into the tool as bytes (no file beside it); its licence goes beside the tool and in the release
set(FREDOKA_URL https://raw.githubusercontent.com/google/fonts/a60a77e14f28abd4ef243a1b5dfc48df0cec5205/ofl/fredoka)
set(FONT_DIR ${CMAKE_CURRENT_BINARY_DIR}/font)
file(DOWNLOAD "${FREDOKA_URL}/Fredoka%5Bwdth%2Cwght%5D.ttf" ${FONT_DIR}/Fredoka.ttf
  EXPECTED_HASH SHA256=2ba02e68b152868aef9ba28e24b3648c7d457fe6f25c761f2c2c53fb61a73fc8)
file(DOWNLOAD ${FREDOKA_URL}/OFL.txt ${FONT_DIR}/Fredoka-OFL.txt
  EXPECTED_HASH SHA256=5c9e7eee5c6b25f4b05b8d53b2e470ea4962f9ced742d044a98f7d95d1375bab)
if(NOT EXISTS ${FONT_DIR}/fredoka.c)
  file(READ ${FONT_DIR}/Fredoka.ttf FREDOKA_HEX HEX)
  string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," FREDOKA_BYTES "${FREDOKA_HEX}")
  file(WRITE ${FONT_DIR}/fredoka.c "/* Fredoka (SIL Open Font License 1.1, Fredoka-OFL.txt), made by CMakeLists.txt */\n"
    "const unsigned char FredokaTtf[] = {${FREDOKA_BYTES}};\nconst unsigned int FredokaTtfSize = sizeof(FredokaTtf);\n")
endif()
set(IMGUI_SOURCES ${imgui_SOURCE_DIR}/imgui.cpp ${imgui_SOURCE_DIR}/imgui_draw.cpp ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp)

set(PORT ${CMAKE_CURRENT_SOURCE_DIR}/../port)
add_executable(herotool src/herotool.c src/model.c src/atlas.c src/rig.c src/moves.c src/write.c src/preview.c src/pose.c src/gui.cpp
  src/sound.c ${FONT_DIR}/fredoka.c ${IMGUI_SOURCES} ${PORT}/platform/heropack.c ${PORT}/platform/heroadjust.c)
add_custom_command(TARGET herotool POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy_if_different
  "${FONT_DIR}/Fredoka-OFL.txt" "$<TARGET_FILE_DIR:herotool>/Fredoka-OFL.txt")
target_include_directories(herotool PRIVATE src ${PORT}/platform ${cgltf_SOURCE_DIR} ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_compile_options(herotool PRIVATE $<$<COMPILE_LANGUAGE:C>:-std=gnu17> $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++17>
  -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers)
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
# its pictures are made on several threads (gui.cpp, in_bands)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(herotool PRIVATE Threads::Threads)
