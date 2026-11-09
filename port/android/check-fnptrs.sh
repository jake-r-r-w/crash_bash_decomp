#!/bin/sh
# In tools/da: compiles the game's C for arm64 as the Android build does, but with Clang's function-pointer mismatch warnings on, keeping
# nothing, and list them. Run port/android/build.sh first (it makes the build directory).
#   tools/da sh port/android/check-fnptrs.sh > build/port/arm64-fnptr.txt
# A call that reads a small type's result (u8, u16, bool, or a pointer read as an s32) through a pointer of another type is the kind
# to fix (docs/PORT.md, Android); one that differs only in arguments of the same size, or in a result nobody reads, is not.
cd /work/build/port/android/arm64-v8a || exit 1
ninja -t commands crashbash | grep -E ' -c [^ ]*/port/game/' \
    | sed -e 's/-Wno-incompatible-function-pointer-types/-Wincompatible-function-pointer-types -Wcast-function-type-strict/g' \
          -e 's/ -o [^ ]*/ -o \/dev\/null/' > /tmp/cmds.txt
echo "$(wc -l < /tmp/cmds.txt) files" >&2
tr '\n' '\0' < /tmp/cmds.txt | xargs -0 -P 16 -I{} sh -c '{} 2>&1' | grep -E "function (pointer|type)" | sort -u
