#!/usr/bin/env bash
# Download Sony's PsyQ 4.7 SDK (converted for GNU tools) into ext/psyq47/, which the build links
# the library from and takes the SDK headers from. Run on the host, not through tools/dk (the
# toolchain image has no 7-Zip): needs curl and one of 7z, 7za or a tar that reads .7z (bsdtar:
# the tar of Windows 10+ and macOS). Checks every file against configs/USA/psyq/sdk.sha256, the
# copy the matches were made with. Does nothing if ext/psyq47/lib is there already.
#   tools/get_psyq.sh
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
URL="http://psx.arthus.net/sdk/Psy-Q/psyq-4.7-converted-full.7z"
DEST="$ROOT/ext/psyq47"
SUMS="$ROOT/configs/USA/psyq/sdk.sha256"

if [ -d "$DEST/lib" ]; then
  echo "ext/psyq47 is already there (delete it to fetch it again)."
  exit 0
fi

TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
echo "Downloading $URL ..."
curl -fL -o "$TMP/psyq.7z" "$URL"

mkdir -p "$TMP/out"
if command -v 7z >/dev/null 2>&1; then
  7z x -y -o"$TMP/out" "$TMP/psyq.7z" >/dev/null
elif command -v 7za >/dev/null 2>&1; then
  7za x -y -o"$TMP/out" "$TMP/psyq.7z" >/dev/null
elif tar -xf "$TMP/psyq.7z" -C "$TMP/out" 2>/dev/null; then
  :
else
  echo "ERROR: no way to unpack a .7z: install 7-Zip (7z/7za), or use a tar that reads .7z (bsdtar)." >&2
  exit 1
fi

# the archive's include/ and lib/ may sit one folder down
SRC="$TMP/out"
if [ ! -d "$SRC/lib" ]; then
  SRC="$(dirname "$(find "$TMP/out" -type d -name lib -path '*/lib' | head -1)")"
fi
[ -d "$SRC/lib" ] && [ -d "$SRC/include" ] || { echo "ERROR: no include/ and lib/ in the archive" >&2; exit 1; }

echo "Checking the files against configs/USA/psyq/sdk.sha256 ..."
if command -v sha256sum >/dev/null 2>&1; then
  (cd "$SRC" && sha256sum --quiet -c "$SUMS")
else
  (cd "$SRC" && shasum -a 256 -q -c "$SUMS")
fi

mkdir -p "$DEST"
cp -R "$SRC/include" "$SRC/lib" "$DEST/"
echo "PsyQ 4.7 is in ext/psyq47."
