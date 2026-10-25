#!/usr/bin/env bash
# Build cc1 2.8.1-psx from source in Docker (tools/cc1/README.md). Runs on the host, like tools/dk.
#   tools/cc1/build.sh                    the release build, into build/cc1/release/, then checks it
#                                         is byte-identical to the toolchain's /opt/gcc/2.8.1-psx/cc1
#   tools/cc1/build.sh NAME [--patch FILE]... [--cflags "FLAGS"]
#                                         an analysis variant, into build/cc1/NAME/: our source
#                                         patches (patch -p1 in gcc-2.8.1/, in the order given) and
#                                         extra host CFLAGS (e.g. -mpc64)
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
NAME=release
PATCHES=()
CFLAGS_X=""
if [ $# -gt 0 ] && [ "${1#-}" = "$1" ]; then NAME="$1"; shift; fi
while [ $# -gt 0 ]; do
  case "$1" in
    --patch) PATCHES+=("$2"); shift 2 ;;
    --cflags) CFLAGS_X="$2"; shift 2 ;;
    *) echo "usage: $0 [NAME] [--patch FILE]... [--cflags FLAGS]" >&2; exit 2 ;;
  esac
done
if [ "$NAME" = release ] && { [ ${#PATCHES[@]} -gt 0 ] || [ -n "$CFLAGS_X" ]; }; then
  echo "ERROR: give a variant with patches or flags a name of its own" >&2; exit 2
fi

# decompals/old-gcc tag 0.17: the release the toolchain image installs as /opt/gcc/2.8.1-psx
OLDGCC=b74211c9d959e9724802f3177c8229cd67202c87
DL="$ROOT/ext/cc1"            # third-party downloads: local only, kept by make clean
CTX="$ROOT/build/cc1/ctx-$NAME"
OUT="$ROOT/build/cc1/$NAME"

sha() { if command -v sha256sum >/dev/null 2>&1; then sha256sum "$1"; else shasum -a 256 "$1"; fi | cut -d' ' -f1; }
fetch() {  # URL FILE SHA256: download unless FILE already has that hash, then check it
  if [ ! -f "$2" ] || [ "$(sha "$2")" != "$3" ]; then
    mkdir -p "$(dirname "$2")"
    curl -fsSL -o "$2.part" "$1"
    mv "$2.part" "$2"
  fi
  if [ "$(sha "$2")" != "$3" ]; then echo "ERROR: sha256 mismatch for $2" >&2; exit 1; fi
}

fetch https://ftp.gnu.org/gnu/gcc/gcc-2.8.1.tar.gz "$DL/gcc-2.8.1.tar.gz" \
  3b30fbfdf93e628373d90d174243f3267b0eec9ebe792bb64fd15b8828c2ea4c
while read -r hash file; do
  fetch "https://raw.githubusercontent.com/decompals/old-gcc/$OLDGCC/$file" "$DL/$file" "$hash"
done <<'EOF'
b7ee011f3a159cbcbf65affa04261c4a04f90301af0f37de779578a02d032d79 patches/psx.patch
98e20e8d853aa18bf4915dabdca544c0e54770a98c33d997fd928785f6edcfcf patches/mips.patch
6c048d1eb3efb2d1ca536c08fe59f2830be984ad5c4fddc8223c7e719b60d357 patches/obstack-2.8.1.h.patch
8c4cb47a9d45d0cc17cd59d3e276696b9ca4a79dd43889e5f33083b837d3b49c tests/little_endian.c
67a509a36f4cafafc5f1cd9691f41a4c3dd47667d9aa7619b2e57db6d9ebe226 tests/section_attribute.c
ce456ad2d4fe3f681d8393e94b360ae4c3afb39873b2789bc5c967a10f526789 tests/host_wide_int.c
EOF

rm -rf "$CTX"
mkdir -p "$CTX/extra"
cp -r "$DL/gcc-2.8.1.tar.gz" "$DL/patches" "$DL/tests" "$CTX/"
i=0
for p in ${PATCHES[@]+"${PATCHES[@]}"}; do
  i=$((i + 1))
  b="$(basename "$p")"
  cp "$p" "$CTX/extra/$(printf %02d $i)-${b%.patch}.patch"
done

if ! command -v docker >/dev/null 2>&1; then echo "ERROR: docker not found" >&2; exit 127; fi
win() { if command -v cygpath >/dev/null 2>&1; then cygpath -w "$1"; else echo "$1"; fi; }
export MSYS_NO_PATHCONV=1     # Git Bash: pass Windows paths through untouched
rm -rf "$OUT"
docker build --platform linux/amd64 -f "$(win "$ROOT/tools/cc1/Dockerfile")" \
  --build-arg HOST_CFLAGS="$CFLAGS_X" --target export \
  --output "type=local,dest=$(win "$OUT")" "$(win "$CTX")"
rm -rf "$CTX"
echo "built build/cc1/$NAME/ ($(tr '\n' ';' < "$OUT/BUILDINFO"))"

if [ "$NAME" = release ]; then
  "$ROOT/tools/dk" sh -c 'cmp build/cc1/release/cc1 /opt/gcc/2.8.1-psx/cc1 && echo "cc1: byte-identical to /opt/gcc/2.8.1-psx/cc1"' </dev/null
fi
