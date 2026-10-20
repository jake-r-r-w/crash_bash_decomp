#!/usr/bin/env python3
"""decomp-permuter with the compile pipeline's Python filters run in-process.

The permuter compiles each candidate by running the directory's compile.sh:
cpp | cc1 | dmpsx.py | maspsx.py [| aspsx_fill.py] | as. Starting those Python interpreters
for every candidate costs about three times as much as cc1 itself. This runs the permuter
(PERMUTER_PY) with its Compiler.compile replaced. The pipeline is still read from compile.sh,
but each `python3 SCRIPT.py ARGS` stage is called through SCRIPT's main() inside the worker
(argv, stdin and stdout swapped), and the other stages run as plain subprocesses.

Two more savings, both for files the bind mount (/work, 9p on Docker Desktop) makes slow to
open. The `-I` directories are snapshotted into /tmp once per run. cpp is skipped when it
couldn't change the source: base.c is already preprocessed, so a candidate with no directive,
comment, continuation or macro name in it goes straight to cc1. A function using GTE macros
still gets cpp for their #defines.

The first compile of each compile.sh is also run through bash, and the two objects must be
byte-identical; otherwise (or for a compile.sh it can't read) it says so and falls back to bash.

    python3 tools/scripts/permuter_fast.py DIR [permuter args...]
"""
import atexit
import importlib.util
import io
import os
import re
import shlex
import shutil
import subprocess
import sys
import tempfile

PERMUTER = os.environ.get("PERMUTER_PY", "/opt/permuter/permuter.py")
sys.path.insert(0, os.path.dirname(PERMUTER))
from src import compiler as _compiler  # noqa: E402
from src.helpers import try_remove  # noqa: E402

SHELL_SYNTAX = {";", "&&", "||", ">", ">>", "<", "&", "2>", "2>&1"}
# what cpp would act on in a preprocessed file: names it defines itself even with -undef
CPP_BUILTINS = {"__LINE__", "__FILE__", "__DATE__", "__TIME__", "__TIMESTAMP__", "__STDC__", "__STDC_VERSION__",
                "__STDC_HOSTED__", "__COUNTER__", "__INCLUDE_LEVEL__", "__BASE_FILE__", "__FILE_NAME__",
                "_Pragma", "__has_include", "__has_include_next"}
IDENT = re.compile(r"[A-Za-z_]\w*")
_plans = {}      # compile.sh path -> (cwd, stages, skip_cpp) or None (use bash)
_checked = set()
_modules = {}
_mirrors = {}
_bash_compile = _compiler.Compiler.compile


def mirror(d):
    """A snapshot of include directory d in /tmp (once per run), or d if it can't be copied."""
    if d not in _mirrors:
        dst = d
        if os.path.isdir(d):
            tmp = tempfile.mkdtemp(prefix="permuter_inc")
            atexit.register(lambda pid=os.getpid(): os.getpid() == pid and shutil.rmtree(tmp, ignore_errors=True))
            try:
                dst = os.path.join(tmp, "i")
                shutil.copytree(d, dst, symlinks=True)
            except OSError:
                dst = d
        _mirrors[d] = dst
    return _mirrors[d]


def local_includes(argv, cwd):
    out, it = [], iter(argv)
    for a in it:
        if a == "-I":
            out += [a, mirror(os.path.join(cwd, next(it, "")))]
        elif a.startswith("-I"):
            out.append("-I" + mirror(os.path.join(cwd, a[2:])))
        else:
            out.append(a)
    return out


def cpp_names(argv):
    """The -D names of a cpp stage, if it's one whose run could be skipped (-P: no line markers)."""
    if not os.path.basename(argv[0]).endswith("cpp") or "-P" not in argv or "$1" not in argv:
        return None
    return {a[2:].split("=")[0] for a in argv if a.startswith("-D")} | CPP_BUILTINS


def cpp_noop(source, names):
    """True if cpp would pass SOURCE through unchanged but for whitespace."""
    if "/*" in source or "//" in source or "\\\n" in source or "??" in source:
        return False
    if any(line.lstrip().startswith("#") for line in source.split("\n")):
        return False
    return names.isdisjoint(IDENT.findall(source))


def parse(path):
    """compile.sh as (cwd, stages), each stage a ("py", module, argv) or ("exe", None, argv),
    for the generated shape: `cd DIR` and one pipeline of plain commands. None otherwise."""
    try:
        with open(path) as f:
            text = f.read()
    except OSError:
        return None
    cwd, cmd = os.path.dirname(os.path.abspath(path)), None
    for line in text.splitlines():
        s = line.strip()
        if not s or s.startswith("#") or s.startswith("set "):
            continue
        if s.startswith("cd "):
            cwd = shlex.split(s)[1]
            continue
        if cmd is not None:
            return None
        cmd = s
    if cmd is None:
        return None
    try:
        toks = shlex.split(cmd)
    except ValueError:
        return None
    stages, cur = [], []
    for t in toks:
        if t in SHELL_SYNTAX or "`" in t or ("$" in t and t not in ("$1", "$2", "$3")):
            return None
        if t == "|":
            stages.append(cur)
            cur = []
        else:
            cur.append(t)
    stages.append(cur)
    if any(not st for st in stages):
        return None
    out = []
    for st in stages:
        mod = None
        if os.path.basename(st[0]).startswith("python") and len(st) > 1 and st[1].endswith(".py"):
            mod = load(os.path.join(cwd, st[1]))
        out.append(("py", mod, st[1:]) if mod else ("exe", None, local_includes(st, cwd)))
    return cwd, out, cpp_names(out[0][2]) if out[0][0] == "exe" and len(out) > 1 else None


def load(script):
    """SCRIPT as a module, once per worker, if it has a main() (else None: run it as a process)."""
    if script not in _modules:
        mod = None
        try:
            d = os.path.dirname(script)
            if d not in sys.path:
                sys.path.append(d)          # its own imports (maspsx.py's maspsx/ package)
            spec = importlib.util.spec_from_file_location("_stage_" + re.sub(r"\W", "_", script), script)
            mod = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(mod)
            if not callable(getattr(mod, "main", None)):
                mod = None
        except Exception:
            mod = None
        _modules[script] = mod
    return _modules[script]


def run_py(mod, argv, data, show):
    """mod.main() as if it were `python3 argv...` reading data on stdin; its stdout, or None."""
    old = sys.argv, sys.stdin, sys.stdout
    out = io.StringIO()
    sys.argv = argv
    sys.stdin = io.StringIO(data.decode("utf-8", "surrogateescape"))
    sys.stdout = out
    try:
        mod.main()
    except SystemExit as e:
        if e.code not in (None, 0):
            if show and not isinstance(e.code, int):
                print(e.code, file=sys.__stderr__)
            return None
    except Exception as e:
        if show:
            print(f"{argv[0]}: {e!r}", file=sys.__stderr__)
        return None
    finally:
        sys.argv, sys.stdin, sys.stdout = old
    return out.getvalue().encode("utf-8", "surrogateescape")


def run_plan(plan, source, c_name, o_name, show):
    """The pipeline on c_name (holding source) into o_name; True if every stage succeeded."""
    cwd, stages, cpp = plan
    args = {"$1": c_name, "$2": "-o", "$3": o_name}
    data = None
    err = None if show else subprocess.DEVNULL
    if cpp is not None and cpp_noop(source, cpp):
        data = source.encode("utf-8", "surrogateescape")
        stages = stages[1:]
    for kind, mod, argv in stages:
        argv = [args.get(a, a) for a in argv]
        if kind == "py":
            data = run_py(mod, argv, data or b"", show)
            if data is None:
                return False
        else:
            try:
                r = subprocess.run(argv, cwd=cwd, input=data, stdin=None if data is not None else subprocess.DEVNULL,
                                   stdout=subprocess.PIPE, stderr=err)
            except OSError:
                return False
            if r.returncode:
                return False
            data = r.stdout
    return True


def fast_compile(self, source, *, show_errors=False):
    show = show_errors or self.show_errors or self.debug_mode
    if self.compile_cmd not in _plans:
        _plans[self.compile_cmd] = parse(self.compile_cmd)
    plan = _plans[self.compile_cmd]
    if plan is None or self.debug_mode:
        return _bash_compile(self, source, show_errors=show_errors)
    with tempfile.NamedTemporaryFile(prefix="permuter", suffix=".c", mode="w", delete=False) as f:
        c_name = f.name
        f.write(source)
    with tempfile.NamedTemporaryFile(prefix="permuter", suffix=".o", delete=False) as f2:
        o_name = f2.name
    try:
        ok = run_plan(plan, source, c_name, o_name, show)
    except BaseException:
        try_remove(c_name)
        try_remove(o_name)
        raise
    if self.compile_cmd not in _checked:
        # the first compile that builds: the bash pipeline must give the same object, or bash it is
        ref = _bash_compile(self, source, show_errors=show_errors)
        same = ok and ref is not None and open(ref, "rb").read() == open(o_name, "rb").read()
        if ref is not None:
            try_remove(ref)
        if ok or ref is not None:
            _checked.add(self.compile_cmd)
        if not same and (ok or ref is not None):
            print(f"permuter_fast: in-process compile differs from {self.compile_cmd}; using bash",
                  file=sys.__stderr__)
            _plans[self.compile_cmd] = None
            try_remove(c_name)
            try_remove(o_name)
            return _bash_compile(self, source, show_errors=show_errors)
    if ok or not show:
        try_remove(c_name)
    if not ok:
        try_remove(o_name)
        return None
    return o_name


_compiler.Compiler.compile = fast_compile

if __name__ == "__main__":
    from src.main import main
    main()
