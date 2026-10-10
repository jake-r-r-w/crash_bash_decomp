# Notes for agents

Read [docs/AGENT_LOG.md](docs/AGENT_LOG.md) first (latest state and next
steps), then [CONTRIBUTING.md](CONTRIBUTING.md) and
[docs/CONVENTIONS.md](docs/CONVENTIONS.md).

- Everything runs through `tools/dk` (Docker); the host has no Python.
- This repo follows the `psx-matching-decomp` skill with different paths:
  `tools/scripts/decomp.py` (not `tools/decomp.py`), `configs/USA/` (not
  `config/`), symbols in `configs/USA/sym.main.txt`, exe target `build/main.exe`.
- Never commit game data or `ext/`; never commit a match except via
  `decomp.py done --commit`.
- Append an entry to `docs/AGENT_LOG.md` at the end of every session.
