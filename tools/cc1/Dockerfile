# cc1 2.8.1-psx from source (tools/cc1/README.md), as decompals/old-gcc 0.17 builds it.
# tools/cc1/build.sh fills the build context: the FSF tarball, old-gcc's patches/ and tests/,
# and extra/ (our own source patches for an analysis variant; empty for the release build).
# HOST_CFLAGS adds host compiler flags for a variant; empty reproduces the release.
FROM ubuntu:focal AS build
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y build-essential gcc gcc-multilib

WORKDIR /work
COPY gcc-2.8.1.tar.gz /work/
RUN tar xzf gcc-2.8.1.tar.gz

WORKDIR /work/gcc-2.8.1
COPY patches /work/patches
RUN sed -i -- 's/include <varargs.h>/include <stdarg.h>/g' *.c
RUN patch -u -p1 obstack.h -i ../patches/obstack-2.8.1.h.patch
RUN patch -u -p1 config/mips/mips.h -i ../patches/mips.patch
RUN patch -su -p1 < ../patches/psx.patch

COPY extra /work/extra
RUN set -e; for p in /work/extra/*.patch; do [ -e "$p" ] || continue; echo "applying $p"; patch -p1 < "$p"; done

RUN ./configure \
    --target=mips-sony-psx \
    --prefix=/opt/cross \
    --with-endian-little \
    --with-gnu-as \
    --disable-gprof \
    --disable-gdb \
    --disable-werror \
    --host=i386-pc-linux \
    --build=i386-pc-linux

RUN touch insn-config.h

ARG HOST_CFLAGS=""
RUN make --jobs $(nproc) cpp cc1 xgcc cc1plus g++ CFLAGS="-std=gnu89 -m32 -static $HOST_CFLAGS"

# old-gcc's checks: little-endian loads, section attributes, 32-bit HOST_WIDE_INT, the PSX
# target's default switches
COPY tests /work/tests
RUN ./cc1 -quiet -O2 /work/tests/little_endian.c && grep -E 'lbu\s\$2,0\(\$4\)' /work/tests/little_endian.s
RUN ./cc1 -quiet -O2 /work/tests/section_attribute.c
RUN ./cc1 -quiet -O2 /work/tests/host_wide_int.c && grep -E '# 0x80000000$' /work/tests/host_wide_int.s
RUN ./cc1 -version </dev/null 2>&1 | grep -- -msoft-float
RUN ./cc1 -version </dev/null 2>&1 | grep -- -msplit-addresses
RUN ./cc1 -version </dev/null 2>&1 | grep -- -mgpopt

RUN mv xgcc gcc && mkdir /build && cp cpp cc1 gcc cc1plus g++ /build/ \
 && { gcc --version | head -1; dpkg -l gcc-9 libc6-dev-i386 | tail -2; \
      echo "HOST_CFLAGS=$HOST_CFLAGS"; ls /work/extra; } > /build/BUILDINFO

FROM scratch AS export
COPY --from=build /build/* .
