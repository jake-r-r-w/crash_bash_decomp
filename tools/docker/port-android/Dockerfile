# The PC port's Android build (docs/PORT.md, Android). Built by tools/da on first use.
# Google's Android SDK command-line tools, then with them a platform to build against, the build tools and the NDK (Google's licence for
# the SDK accepted at the user's word, 2026-11-09); a JDK for SDL's Java side; and SDL3 built with the NDK for each kind of processor the
# app has (arm64 for phones, x86_64 for the emulator), with its Java sources kept for the app's build.
FROM --platform=linux/amd64 debian:12

ARG SDL3_VERSION=3.4.18
ARG CMDLINE_TOOLS=16111833
ARG NDK_VERSION=30.0.16248370
ARG PLATFORM=android-36
ARG BUILD_TOOLS=37.0.0
ARG MIN_SDK=26

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        openjdk-17-jdk-headless cmake ninja-build make curl ca-certificates unzip zip git nodejs \
    && rm -rf /var/lib/apt/lists/*

ENV ANDROID_HOME=/opt/android-sdk
RUN mkdir -p $ANDROID_HOME/cmdline-tools \
    && curl -fsSL "https://dl.google.com/android/repository/commandlinetools-linux-${CMDLINE_TOOLS}_latest.zip" -o /tmp/tools.zip \
    && unzip -q /tmp/tools.zip -d /tmp/tools && mv /tmp/tools/cmdline-tools $ANDROID_HOME/cmdline-tools/latest \
    && rm -rf /tmp/tools /tmp/tools.zip
RUN yes | $ANDROID_HOME/cmdline-tools/latest/bin/sdkmanager --licenses > /dev/null \
    && $ANDROID_HOME/cmdline-tools/latest/bin/sdkmanager "platforms;${PLATFORM}" "build-tools;${BUILD_TOOLS}" "ndk;${NDK_VERSION}" > /dev/null
ENV ANDROID_NDK=$ANDROID_HOME/ndk/${NDK_VERSION} \
    ANDROID_PLATFORM_JAR=$ANDROID_HOME/platforms/${PLATFORM}/android.jar \
    ANDROID_BUILD_TOOLS=$ANDROID_HOME/build-tools/${BUILD_TOOLS} \
    ANDROID_MIN_SDK=${MIN_SDK}

# SDL3 for Android, a shared library for each ABI (its Java side loads libSDL3.so, then the game's libmain.so)
RUN curl -fsSL --retry 5 --retry-all-errors "https://github.com/libsdl-org/SDL/releases/download/release-${SDL3_VERSION}/SDL3-${SDL3_VERSION}.tar.gz" | tar xz -C /opt \
    && for abi in arm64-v8a x86_64; do \
         cmake -S /opt/SDL3-${SDL3_VERSION} -B /tmp/sdl-$abi -G Ninja -DCMAKE_BUILD_TYPE=Release \
             -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$abi -DANDROID_PLATFORM=${MIN_SDK} \
             -DSDL_SHARED=ON -DSDL_STATIC=OFF -DSDL_TEST_LIBRARY=OFF -DSDL_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/opt/sdl3-android/$abi \
         && cmake --build /tmp/sdl-$abi && cmake --install /tmp/sdl-$abi && rm -rf /tmp/sdl-$abi || exit 1; \
       done
ENV SDL3_SOURCE=/opt/SDL3-${SDL3_VERSION}
WORKDIR /work
