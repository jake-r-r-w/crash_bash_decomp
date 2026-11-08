# The PC port's Linux build, releases and play tests (docs/PORT.md, Linux). Built by tools/dl on first use.
# Clang and GCC, CMake and Ninja, SDL3 (built here, the version port/CMakeLists.txt fetches), glslang and SPIRV-Cross for the shaders
# (Vulkan's, and the Mac's Metal ones made from them), and for the play tests a virtual screen (Xvfb), Mesa's software Vulkan (lavapipe)
# and Node. Debian 12 for its glibc: a program runs only where glibc is as new as the one it was built with, and Ubuntu 24.04's asked
# for 2.38, which leaves out Ubuntu 22.04, Debian 12 and Mint 21 (its CMake, 3.25, is also just new enough).
FROM --platform=linux/amd64 debian:12

ARG SDL3_VERSION=3.4.18

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        clang gcc g++ cmake ninja-build pkg-config make git curl ca-certificates xz-utils \
        zlib1g-dev glslang-tools spirv-tools spirv-cross \
        libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxss-dev libxfixes-dev libxtst-dev libxkbcommon-dev \
        libwayland-dev wayland-protocols libdecor-0-dev libegl-dev libgl-dev libvulkan-dev \
        libasound2-dev libpulse-dev libpipewire-0.3-dev libudev-dev libdbus-1-dev libdrm-dev libgbm-dev \
        xvfb xauth mesa-vulkan-drivers vulkan-tools nodejs \
    && rm -rf /var/lib/apt/lists/*

# SDL3, installed to /usr/local (find_package finds it there). Static, so a release is the one program: SDL opens the system's own
# libraries (X11 or Wayland, the sound servers, Vulkan) as it runs, so it needs none of them to start.
RUN curl -fsSL "https://github.com/libsdl-org/SDL/releases/download/release-${SDL3_VERSION}/SDL3-${SDL3_VERSION}.tar.gz" | tar xz -C /tmp \
    && cmake -S /tmp/SDL3-${SDL3_VERSION} -B /tmp/sdl-build -G Ninja -DCMAKE_BUILD_TYPE=Release -DSDL_TEST_LIBRARY=OFF -DSDL_TESTS=OFF \
        -DSDL_SHARED=OFF -DSDL_STATIC=ON -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    && cmake --build /tmp/sdl-build \
    && cmake --install /tmp/sdl-build \
    && rm -rf /tmp/SDL3-${SDL3_VERSION} /tmp/sdl-build

# the play tests' screen is Xvfb's, with no sound device
ENV SDL_VIDEO_DRIVER=x11
ENV SDL_AUDIO_DRIVER=dummy
WORKDIR /work
