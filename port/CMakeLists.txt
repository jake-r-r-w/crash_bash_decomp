# The native PC port of Crash Bash (docs/PORT.md): 64-bit Windows and Linux, on SDL3.
#
#   cmake --preset windows-clang       (or linux-clang, linux-gcc, linux-asan)
#   cmake --build --preset windows-clang
#   ctest --preset windows-clang
cmake_minimum_required(VERSION 3.25)
project(crash_bash_pc VERSION 0.1.1 LANGUAGES C)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "The port is 64-bit: use a 64-bit compiler (docs/PORT.md, Building).")
endif()

# A release (node port/package.mjs) has no console window or symbols, and the optimisation everything's been tested at.
option(BASH_RELEASE "A build for publishing" OFF)
if(BASH_RELEASE)
  add_link_options(-s)
else()
  add_compile_options(-g)
  add_link_options(-g)
endif()
option(BASH_FETCH_SDL3 "Download and build SDL3 if it isn't installed" OFF)

find_package(SDL3 QUIET CONFIG)
if(NOT SDL3_FOUND)
  if(NOT BASH_FETCH_SDL3)
    message(FATAL_ERROR "SDL3 not found: set CMAKE_PREFIX_PATH to it, or use -DBASH_FETCH_SDL3=ON (docs/PORT.md, Building).")
  endif()
  include(FetchContent)
  FetchContent_Declare(SDL3
    URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.18/SDL3-3.4.18.tar.gz)
  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(SDL3)
endif()

# The game's C, as it stands: this target is what 6.3 and 6.4 clean up (docs/ROADMAP-B.md).
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS game/main/*.c game/overlays/*.c)
add_library(game OBJECT ${GAME_SOURCES} tests/bossbot.c tests/adventureplayer.c tests/pogoplayer.c tests/bashplayer.c)
target_include_directories(game PUBLIC game/include)
target_compile_options(game PRIVATE
  # GNU C for the empty-parens hooks, until they have prototypes (6.3); C23 would read them as (void).
  -std=gnu17
  # What the PS1's compiler did with the rest: a signed char, and signed overflow that wraps.
  -fsigned-char -fwrapv -fno-strict-aliasing -fno-builtin
  # The warnings stay at zero. Unused parameters are the hooks' shape, and data tables are written flat.
  -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers -Wno-empty-body -Wno-missing-braces
  # Old C, reviewed in 6.3; the warnings that matter for 64-bit stay on.
  -Wno-strict-prototypes -Wno-pointer-sign
  -Wno-incompatible-pointer-types -Wno-incompatible-function-pointer-types -Wno-format-security
  # An address in an integer is an error.
  -Werror=int-conversion -Werror=pointer-to-int-cast -Werror=int-to-pointer-cast
  -Werror=void-pointer-to-int-cast -Werror=int-to-void-pointer-cast
  $<$<C_COMPILER_ID:Clang>:-ferror-limit=0>)

# Each slot overlay's data goes in a section of its own (platform/overlays.c).
foreach(ovl arkenoid bash bonk chase dino hub n2o papu pogo polar race spin tank titles)
  file(GLOB ovl_sources CONFIGURE_DEPENDS game/overlays/${ovl}/*.c)
  set_property(SOURCE ${ovl_sources} APPEND PROPERTY COMPILE_OPTIONS "-include;${CMAKE_CURRENT_SOURCE_DIR}/platform/ovlsection.h;-fno-zero-initialized-in-bss")
  set_property(SOURCE ${ovl_sources} APPEND PROPERTY COMPILE_DEFINITIONS "OVL_NAME=${ovl}")
endforeach()

add_executable(crashbash main.c platform/overlays.c platform/maths.c platform/archive.c platform/clock.c platform/pad.c platform/saves.c
  platform/audio.c platform/sdl.c platform/gfx.c platform/settings.c $<TARGET_OBJECTS:game>)
if(TARGET SDL3::SDL3-static)
  target_link_libraries(crashbash PRIVATE SDL3::SDL3-static)
else()
  target_link_libraries(crashbash PRIVATE SDL3::SDL3)
  if(WIN32)
    add_custom_command(TARGET crashbash POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy_if_different
      "$<TARGET_FILE:SDL3::SDL3>" "$<TARGET_FILE_DIR:crashbash>")
  endif()
endif()
target_compile_options(crashbash PRIVATE -std=gnu17 -fsigned-char -fwrapv -fno-strict-aliasing -w -Wno-error=int-conversion -Wno-incompatible-pointer-types)
target_compile_options(crashbash PRIVATE -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers)
target_compile_definitions(crashbash PRIVATE BASH_VERSION="${PROJECT_VERSION}")
if(BASH_RELEASE AND WIN32)
  set_target_properties(crashbash PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

enable_testing()
add_test(NAME version COMMAND crashbash --version)
set_tests_properties(version PROPERTIES PASS_REGULAR_EXPRESSION "crashbash ${PROJECT_VERSION}")
add_test(NAME sdl COMMAND crashbash --check-sdl)

# Checks that need the user's own game data, from assets/ (dat.py extract).
file(GLOB MODE_TEXT_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/../assets/CRASHBSH/*.txt/data.bin)
if(MODE_TEXT_FILES)
  add_executable(modetext_test tests/modetext_test.c game/overlays/engine/modetext.c)
  target_include_directories(modetext_test PRIVATE game/include)
  target_compile_options(modetext_test PRIVATE -std=gnu17 -fsigned-char -fno-builtin -w -Wno-error=int-conversion -Wno-incompatible-pointer-types -Wno-incompatible-function-pointer-types)
  add_test(NAME modetext COMMAND modetext_test ${MODE_TEXT_FILES})
endif()
