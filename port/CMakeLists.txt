# The native PC port of Crash Bash (docs/PORT.md): 64-bit Windows and Linux, and the Mac to come, on SDL3.
#
#   cmake --preset windows-clang       (or linux-clang in tools/dl, linux-gcc, linux-asan; macos-clang)
#   cmake --build --preset windows-clang
#   ctest --preset windows-clang
cmake_minimum_required(VERSION 3.25)
project(crash_bash_pc VERSION 1.4.1 LANGUAGES C)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "The port is 64-bit: use a 64-bit compiler (docs/PORT.md, Building).")
endif()

# A release (node port/package.mjs) has no console window or debug information, and the optimisation everything's been tested at.
# It keeps its symbol table, which a crash's report reads to name the functions and data it was in (platform/crashlog.c): on Windows
# that's about 1.3 MB more. Android's library keeps it too.
option(BASH_RELEASE "A build for publishing" OFF)
if(BASH_RELEASE)
  add_link_options(-Wl,--strip-debug)
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

# On Android the game is a shared library, so its objects are position-independent too.
if(ANDROID)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Online play asks the player's router to let the others in (platform/portmap.c): by UPnP with miniupnpc, and by NAT-PMP with
# libnatpmp. Both are small, BSD-licensed and by the same author; each is fetched at one version, checked by its hash, and built into
# the program. Only their sources are taken (SOURCE_SUBDIR names nothing, so their own builds, with their tools and tests, aren't
# run), and the targets are made here.
include(FetchContent)
FetchContent_Declare(miniupnp
  URL https://github.com/miniupnp/miniupnp/archive/refs/tags/miniupnpc_2_3_3.tar.gz
  URL_HASH SHA256=8cf2c833b3e76fc4893ff29c2a376e3394962449e5970e373c0a91421724d222
  SOURCE_SUBDIR not-built-from-here)
FetchContent_Declare(libnatpmp
  URL https://github.com/miniupnp/libnatpmp/archive/134fc89e2781e154e40042641f4d8bcbe42579f1.tar.gz
  URL_HASH SHA256=be1864ed4e54a122590fd2de3824b085386b6e4aa9a91738c277c280ab8bf188
  SOURCE_SUBDIR not-built-from-here)
FetchContent_MakeAvailable(miniupnp libnatpmp)
set(UPNPC_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(UPNPC_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(UPNPC_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(UPNPC_BUILD_SAMPLE OFF CACHE BOOL "" FORCE)
set(UPNPC_NO_INSTALL ON CACHE BOOL "" FORCE)
add_subdirectory(${miniupnp_SOURCE_DIR}/miniupnpc ${miniupnp_BINARY_DIR}/miniupnpc EXCLUDE_FROM_ALL)
target_compile_options(libminiupnpc-static PRIVATE -w)
set(natpmp_sources ${libnatpmp_SOURCE_DIR}/natpmp.c ${libnatpmp_SOURCE_DIR}/getgateway.c)
if(WIN32)
  list(APPEND natpmp_sources ${libnatpmp_SOURCE_DIR}/wingettimeofday.c)
endif()
# Recomp Achievements, RecompLauncher's achievements (platform/achievements.c, docs/PORT.md, Achievements): its small C library, with the
# rcheevos and cJSON it carries, at one commit, checked by its hash. Free for any game under its own licence (in the release's
# LICENSE.txt). Not on Android, where there's no RecompLauncher.
if(NOT ANDROID)
  FetchContent_Declare(recompachievements
    URL https://github.com/elripalda/RecompAchievements/archive/faf094acdf1efb57d8db7c8187b4b47aa1501805.tar.gz
    URL_HASH SHA256=a1f00f193436e060a8a7fd38d6614a7c07742e451184c0244c31034bb80cba31
    SOURCE_SUBDIR not-built-from-here)
  FetchContent_MakeAvailable(recompachievements)
  add_subdirectory(${recompachievements_SOURCE_DIR}/sdk ${recompachievements_BINARY_DIR}/sdk EXCLUDE_FROM_ALL)
  target_compile_options(rl_achievements PRIVATE -w)
endif()
# Hero packs are glTF (platform/heropack.c, docs/HERO_PACKS.md): cgltf, one header, MIT, at one version checked by its hash.
FetchContent_Declare(cgltf
  URL https://github.com/jkuhlmann/cgltf/archive/refs/tags/v1.15.tar.gz
  URL_HASH SHA256=84e352092e5cd6aab7f66de62ddb66beb5e6f18d412ca9d12950d7a55bfef25a
  SOURCE_SUBDIR not-built-from-here)
FetchContent_MakeAvailable(cgltf)
add_library(natpmp STATIC ${natpmp_sources})
target_include_directories(natpmp PUBLIC ${libnatpmp_SOURCE_DIR})
target_compile_definitions(natpmp PUBLIC NATPMP_STATICLIB PRIVATE ENABLE_STRNATPMPERR)
target_compile_options(natpmp PRIVATE -w)
if(WIN32)
  target_link_libraries(natpmp PUBLIC ws2_32 iphlpapi)
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
  # An address in an integer is an error (the void * ones are Clang's alone: GCC's two above cover them).
  -Werror=int-conversion -Werror=pointer-to-int-cast -Werror=int-to-pointer-cast
  $<$<C_COMPILER_ID:Clang>:-Werror=void-pointer-to-int-cast> $<$<C_COMPILER_ID:Clang>:-Werror=int-to-void-pointer-cast>
  $<$<C_COMPILER_ID:Clang>:-ferror-limit=0>)

# Each slot overlay's data goes in a section of its own (platform/overlays.c).
foreach(ovl arkenoid bash bonk chase dino hub n2o papu pogo polar race spin tank titles)
  file(GLOB ovl_sources CONFIGURE_DEPENDS game/overlays/${ovl}/*.c)
  set_property(SOURCE ${ovl_sources} APPEND PROPERTY COMPILE_OPTIONS "-include;${CMAKE_CURRENT_SOURCE_DIR}/platform/ovlsection.h;-fno-zero-initialized-in-bss")
  set_property(SOURCE ${ovl_sources} APPEND PROPERTY COMPILE_DEFINITIONS "OVL_NAME=${ovl}")
endforeach()
# And the rest of the game's data, main's and the engine's, in one: all of it is put back when the game starts again from its
# beginning in the same window (platform/overlays.c, overlays_restart).
file(GLOB_RECURSE core_sources CONFIGURE_DEPENDS game/main/*.c game/overlays/engine/*.c)
list(APPEND core_sources tests/bossbot.c tests/adventureplayer.c tests/pogoplayer.c tests/bashplayer.c)
set_property(SOURCE ${core_sources} APPEND PROPERTY COMPILE_OPTIONS "-include;${CMAKE_CURRENT_SOURCE_DIR}/platform/ovlsection.h;-fno-zero-initialized-in-bss")
set_property(SOURCE ${core_sources} APPEND PROPERTY COMPILE_DEFINITIONS "OVL_NAME=game")

set(PORT_SOURCES main.c platform/overlays.c platform/maths.c platform/archive.c platform/discimage.c platform/clock.c platform/pad.c platform/saves.c platform/folders.c
  platform/audio.c platform/sdl.c platform/gfx.c platform/upscale.c platform/texpack.c platform/mods.c platform/heropack.c platform/zipread.c platform/bps.c platform/settings.c
  platform/touch.c platform/controls.c platform/prompts.c platform/net.c platform/portmap.c platform/crashlog.c platform/achievements.c ${CMAKE_CURRENT_BINARY_DIR}/prompt_pngs.c)
# The button prompts' pictures, Kenney's Input Prompts (CC0, assets/prompts/License.txt), built in as data (platform/prompts.c)
file(GLOB PROMPT_PNGS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets/prompts/*.png)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/prompt_pngs.c
  COMMAND ${CMAKE_COMMAND} -DDIR=${CMAKE_CURRENT_SOURCE_DIR}/assets/prompts -DOUT=${CMAKE_CURRENT_BINARY_DIR}/prompt_pngs.c
          -P ${CMAKE_CURRENT_SOURCE_DIR}/assets/prompts/embed.cmake
  DEPENDS ${PROMPT_PNGS} ${CMAKE_CURRENT_SOURCE_DIR}/assets/prompts/embed.cmake)
if(ANDROID)
  # Android loads the game as libmain.so, and SDL's Java side calls its main (port/android/, docs/PORT.md, Android)
  add_library(crashbash SHARED ${PORT_SOURCES} $<TARGET_OBJECTS:game>)
  set_target_properties(crashbash PROPERTIES OUTPUT_NAME main)
else()
  add_executable(crashbash ${PORT_SOURCES} $<TARGET_OBJECTS:game>)
  # the example mods in the build's mods folder, as the downloads have them (package.mjs): Bobby, the hero packs' example
  add_custom_command(TARGET crashbash POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/mods/bobby" "$<TARGET_FILE_DIR:crashbash>/mods/bobby")
endif()
# The Windows program's own icon, a drawn one (platform/crashbash.rc)
if(WIN32)
  enable_language(RC)
  target_sources(crashbash PRIVATE platform/crashbash.rc)
endif()
# Mods are zips, which zlib unpacks (platform/zipread.c).
find_package(ZLIB REQUIRED)
target_link_libraries(crashbash PRIVATE ZLIB::ZLIB)
target_include_directories(crashbash PRIVATE ${cgltf_SOURCE_DIR})
# Online play is plain sockets (platform/net.c), which Windows keeps in a library of their own, and the two libraries above
if(WIN32)
  target_link_libraries(crashbash PRIVATE ws2_32)
endif()
target_link_libraries(crashbash PRIVATE libminiupnpc-static natpmp)
if(TARGET rl_achievements)
  target_link_libraries(crashbash PRIVATE rl_achievements)
  set_property(SOURCE platform/achievements.c APPEND PROPERTY COMPILE_DEFINITIONS BASH_ACHIEVEMENTS)
endif()
# The maths library is its own on Linux and the Mac (Windows links it by itself)
if(UNIX)
  target_link_libraries(crashbash PRIVATE m)
endif()
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
# The commit it's built from, for the log's first line: a crash's addresses are this build's (+ when there were changes not committed)
execute_process(COMMAND git rev-parse --short=8 HEAD WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE BASH_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET RESULT_VARIABLE commit_failed)
execute_process(COMMAND git status --porcelain --untracked-files=no -- . WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE commit_dirty OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
if(commit_failed OR BASH_COMMIT STREQUAL "")
  set(BASH_COMMIT "unknown")
elseif(NOT commit_dirty STREQUAL "")
  string(APPEND BASH_COMMIT "+")
endif()
set_property(SOURCE platform/crashlog.c APPEND PROPERTY COMPILE_DEFINITIONS "BASH_COMMIT=\"${BASH_COMMIT}\"")
if(BASH_RELEASE AND WIN32)
  set_target_properties(crashbash PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

if(ANDROID)
  return()
endif()
enable_testing()
add_test(NAME version COMMAND crashbash --version)
set_tests_properties(version PROPERTIES PASS_REGULAR_EXPRESSION "crashbash ${PROJECT_VERSION}")
add_test(NAME sdl COMMAND crashbash --check-sdl)
add_test(NAME codes COMMAND crashbash --check-codes)
set_tests_properties(codes PROPERTIES PASS_REGULAR_EXPRESSION "codes ok")
add_test(NAME sticks COMMAND crashbash --check-sticks)
set_tests_properties(sticks PROPERTIES PASS_REGULAR_EXPRESSION "sticks ok")

# The fuzzer for online play's packets (tests/netfuzz.c; PORT.md, Online play, Hardening), with ASan and UBSan: built only when
# asked for (cmake --build --preset windows-clang --target netfuzz), and only where the compiler has libFuzzer, as the Windows
# toolchain's has. Its driver refuses -fsanitize=fuzzer for Windows, so the coverage is asked for and libFuzzer linked by hand.
execute_process(COMMAND ${CMAKE_C_COMPILER} -print-resource-dir OUTPUT_VARIABLE BASH_CLANG_RESOURCES OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
set(BASH_LIBFUZZER "${BASH_CLANG_RESOURCES}/lib/$<IF:$<BOOL:${WIN32}>,windows,linux>/libclang_rt.fuzzer-x86_64.a")
if(EXISTS "${BASH_CLANG_RESOURCES}/lib/windows/libclang_rt.fuzzer-x86_64.a" OR EXISTS "${BASH_CLANG_RESOURCES}/lib/linux/libclang_rt.fuzzer-x86_64.a")
  add_executable(netfuzz EXCLUDE_FROM_ALL tests/netfuzz.c platform/net.c platform/saves.c)
  target_compile_definitions(netfuzz PRIVATE NET_FUZZ BASH_VERSION="${PROJECT_VERSION}")
  target_compile_options(netfuzz PRIVATE -std=gnu17 -fwrapv -g -O1 -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize-recover=all
    -fsanitize-coverage=inline-8bit-counters,trace-cmp,pc-table)
  target_link_options(netfuzz PRIVATE -fsanitize=address,undefined)
  target_link_libraries(netfuzz PRIVATE "${BASH_LIBFUZZER}" c++)
  if(WIN32)
    target_link_libraries(netfuzz PRIVATE ws2_32 iphlpapi)
  endif()
  if(TARGET SDL3::SDL3-static)
    target_link_libraries(netfuzz PRIVATE SDL3::SDL3-static)
  else()
    target_link_libraries(netfuzz PRIVATE SDL3::SDL3)
  endif()
endif()

# Checks that need the user's own game data, from assets/ (dat.py extract).
file(GLOB MODE_TEXT_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/../assets/CRASHBSH/*.txt/data.bin)
if(MODE_TEXT_FILES)
  add_executable(modetext_test tests/modetext_test.c game/overlays/engine/modetext.c)
  target_include_directories(modetext_test PRIVATE game/include)
  target_compile_options(modetext_test PRIVATE -std=gnu17 -fsigned-char -fno-builtin -w -Wno-error=int-conversion -Wno-incompatible-pointer-types -Wno-incompatible-function-pointer-types)
  add_test(NAME modetext COMMAND modetext_test ${MODE_TEXT_FILES})
endif()
