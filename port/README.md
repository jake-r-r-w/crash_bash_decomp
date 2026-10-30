# The PC port

Everything PC-only lives here. The rules and the plan are in [docs/PORT.md](../docs/PORT.md).

| Folder | What goes in it |
|---|---|
| `sdk/` | PC versions of Sony's PsyQ functions, under the same names |
| `platform/` | The host: window, renderer, audio device, input, files |

It builds and links the game's C, but doesn't start the game yet. It needs a 32-bit compiler on the
host ([PORT.md](../docs/PORT.md), Building):

```sh
cmake -S port -B build/pc -G Ninja -DCMAKE_C_COMPILER=i686-w64-mingw32-clang
cmake --build build/pc
```
