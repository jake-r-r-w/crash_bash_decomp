#!/usr/bin/env bash
# Build the whole game with a cc1 variant and check it against the retail checksums (make check),
# in a container-local copy of the build inputs, so build/ is left as it is (tools/cc1/README.md).
#   tools/cc1/check.sh NAME
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
if [ $# -ne 1 ]; then echo "usage: $0 NAME" >&2; exit 2; fi

exec "$ROOT/tools/cc1/with.sh" "$1" sh -c '
  set -e
  w=/tmp/w
  mkdir -p "$w"
  tar -C /work --exclude=./build --exclude=./assets --exclude=./protos --exclude=./disc \
      --exclude=./scratch --exclude=./docs --exclude=./.git -cf - . | tar -C "$w" -xf -
  cd "$w"
  echo "make check with GCC_ROOT=$GCC_ROOT"
  make -j"$(nproc)" check'
