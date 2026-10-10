# PSX matching-decomp toolchain. Built automatically by tools/dk on first use.
FROM --platform=linux/amd64 ubuntu:24.04

ARG OLD_GCC_RELEASE=0.17
ARG GCC_VERSIONS="2.5.7-psx 2.6.0-psx 2.6.3-psx 2.7.2-psx 2.7.2-cdk 2.8.1-psx"

ENV DEBIAN_FRONTEND=noninteractive \
    PIP_BREAK_SYSTEM_PACKAGES=1 \
    PIP_NO_CACHE_DIR=1 \
    PYTHONDONTWRITEBYTECODE=1

RUN apt-get update && apt-get install -y --no-install-recommends \
        binutils-mipsel-linux-gnu gcc-mipsel-linux-gnu \
        make python3 python3-pip git curl ca-certificates \
        cmake g++ libc6-i386 \
    && rm -rf /var/lib/apt/lists/* \
    && ln -s /usr/bin/mipsel-linux-gnu-objdump /usr/local/bin/mips-linux-gnu-objdump
# (decomp-permuter looks for mips-linux-gnu-objdump; the mipsel build disassembles both endians)

# Old GCC builds (cc1 per version) from decompals/old-gcc -> /opt/gcc/<version>/cc1
RUN set -e; mkdir -p /opt/gcc; ok=0; \
    for v in $GCC_VERSIONS; do \
      d=/opt/gcc/$v; mkdir -p $d; \
      if curl -fsSL "https://github.com/decompals/old-gcc/releases/download/${OLD_GCC_RELEASE}/gcc-$v.tar.gz" | tar xz -C $d; then \
        f=$(find $d -name cc1 -type f | head -1); \
        if [ -n "$f" ] && [ "$(dirname $f)" != "$d" ]; then mv "$(dirname $f)"/* $d/; fi; \
        if [ -x $d/cc1 ]; then ok=$((ok+1)); else echo "WARN: no cc1 in gcc-$v"; rm -rf $d; fi; \
      else echo "WARN: could not download gcc-$v"; rm -rf $d; fi; \
    done; \
    [ $ok -gt 0 ] || { echo "no compilers installed"; exit 1; }; ls /opt/gcc

RUN git clone --depth 1 https://github.com/mkst/maspsx /opt/maspsx \
 && git clone --depth 1 https://github.com/matt-kempster/m2c /opt/m2c \
 && git clone --depth 1 https://github.com/simonlindholm/decomp-permuter /opt/permuter \
 && pip3 install "splat64[mips]" pycparser graphviz toml Levenshtein pyyaml

# dumpsxiso / mkpsxiso for disc extraction (and optional disc rebuild)
RUN git clone --depth 1 --recursive https://github.com/Lameguy64/mkpsxiso /tmp/mkpsxiso \
 && cmake -S /tmp/mkpsxiso -B /tmp/mkpsxiso/build -DCMAKE_BUILD_TYPE=Release \
 && cmake --build /tmp/mkpsxiso/build -j"$(nproc)" \
 && cmake --install /tmp/mkpsxiso/build \
 && rm -rf /tmp/mkpsxiso

# The repo is bind-mounted from the host with a different owner.
RUN git config --system --add safe.directory '*'

WORKDIR /work
