# The PC port

Everything PC-only lives here. The rules and the plan are in [docs/PORT.md](../docs/PORT.md).

| Folder | What goes in it |
|---|---|
| `sdk/` | PC versions of Sony's PsyQ functions, under the same names |
| `platform/` | The host: window, renderer, audio device, input, files |

```sh
cmake -S port -B build/pc
cmake --build build/pc
```
