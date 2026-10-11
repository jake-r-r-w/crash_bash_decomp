# Notes for agents

Read [docs/AGENT_LOG.md](docs/AGENT_LOG.md) first (latest state and next
steps), then [CONTRIBUTING.md](CONTRIBUTING.md) and
[docs/CONVENTIONS.md](docs/CONVENTIONS.md).

- Everything runs through `tools/dk` (Docker); the host has no Python.
- This repo follows the `psx-matching-decomp` skill with different paths:
  `tools/scripts/decomp.py` (not `tools/decomp.py`), `configs/USA/` (not
  `config/`), symbols in `configs/USA/sym.main.txt`, exe target `build/main.exe`.
- Never commit game data or `ext/`; never commit a match except via
  `decomp.py done NAME --commit`.
- Matching is by hand, one function per commit, through the validation gate
  `decomp.py done NAME --commit` ([docs/MATCHING.md](docs/MATCHING.md); skill
  `/match-function`). `decomp.py auto` is retired.
- At the end of every session, update `docs/AGENT_LOG.md`: Current state,
  Next and Open leads, any new gotcha, and one short entry at the top of its
  Session log. Matching idioms go in the Diff patterns table in
  `docs/MATCHING.md`, game facts in `docs/ARCHITECTURE.md`.
