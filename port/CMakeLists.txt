# The PC build of Crash Bash (docs/PORT.md). Everything PC-only lives under port/; the game's C is
# compiled from ../src and ../include, and src/ps1/ (the PS1-only files) is left out.
#
#   cmake -S port -B build/pc && cmake --build build/pc
#
# BASH_BUILD_GAME is off until 6.1 (ROADMAP-B): today this only builds the entry point.
cmake_minimum_required(VERSION 3.16)
project(crash_bash_pc C)

option(BASH_BUILD_GAME "Compile the game's C as well (ROADMAP-B 6.1)" OFF)

get_filename_component(REPO "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

add_executable(crashbash main.c)
target_compile_definitions(crashbash PRIVATE BASH_PC)

if(BASH_BUILD_GAME)
  file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS "${REPO}/src/main/*.c" "${REPO}/src/overlays/*.c")
  target_sources(crashbash PRIVATE ${GAME_SOURCES})
  # sdk/include comes first: the PC versions of PsyQ's headers, under the same names
  target_include_directories(crashbash PRIVATE sdk/include "${REPO}/include")
endif()
