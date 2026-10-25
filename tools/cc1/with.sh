#!/usr/bin/env bash
# Run a command in the toolchain container with a cc1 variant from tools/cc1/build.sh in place
# of /opt/gcc/2.8.1-psx/cc1 (tools/cc1/README.md). The other compilers stay as they are.
#   tools/cc1/with.sh NAME make check
#   tools/cc1/with.sh NAME python3 tools/scripts/decomp.py try func_800B4048 --file F.c
# It sets GCC_ROOT, which the Makefile (GCC_ROOT ?=) and decomp.py both read.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
if [ $# -lt 2 ]; then echo "usage: $0 NAME COMMAND..." >&2; exit 2; fi
NAME="$1"; shift
if [ ! -f "$ROOT/build/cc1/$NAME/cc1" ]; then     # -f: Git Bash doesn't count an ELF as executable
  echo "ERROR: no build/cc1/$NAME/cc1 (run tools/cc1/build.sh $NAME ... first)" >&2; exit 1
fi

# A container-local root: links to every /opt/gcc version, with 2.8.1-psx copied and its cc1
# replaced (the repo mount may not take symlinks).
exec "$ROOT/tools/dk" sh -c '
  set -e
  r=/tmp/gcc-root
  mkdir -p "$r"
  for d in /opt/gcc/*; do ln -s "$d" "$r/"; done
  rm "$r/2.8.1-psx"
  mkdir "$r/2.8.1-psx"
  cp /opt/gcc/2.8.1-psx/* "$r/2.8.1-psx/"
  cp "build/cc1/$0/cc1" "$r/2.8.1-psx/cc1"
  GCC_ROOT="$r" exec "$@"' "$NAME" "$@"
