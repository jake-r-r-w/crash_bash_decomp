"""Reconcile a header after a swarm patch with the declarations already in the tree.

Workers match against an older tip; by the time their patch reaches the gate, other matches may
have declared the same struct, global or function. A 3-way apply then keeps both sides' lines, and
cc1 stops on `conflicting types`, `redefinition` or `duplicate member`. In every such case on
2026-10-19 the worker's C was right and the existing declaration worked for it: keep the tree's
declaration, drop the patch's (decomp.py reconcile; tools/swarm runs it before a gate fix)."""
import re

IDENT = re.compile(r"[A-Za-z_]\w*")
KEYWORDS = {"extern", "static", "const", "volatile", "unsigned", "signed", "struct", "union", "enum",
            "typedef", "register", "void", "char", "short", "int", "long", "float", "double"}


def items(text):
    """Split a header into top-level pieces: ("decl", text) for each declaration or definition
    ending in `;` outside braces, ("text", text) for everything else (comments between them,
    blank lines, preprocessor lines). Joining the texts gives TEXT back."""
    out, i, start, depth, n = [], 0, 0, 0, len(text)

    def flush(kind, end):
        nonlocal start
        if end > start:
            out.append((kind, text[start:end]))
        start = end

    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            if depth == 0 and not text[start:i].strip():
                flush("text", j)
            i = j
            continue
        if text.startswith("//", i):
            j = text.find("\n", i)
            i = n if j < 0 else j
            continue
        if c == "#" and depth == 0 and (i == 0 or text[i - 1] == "\n") and not text[start:i].strip():
            j = i
            while True:                         # a #define may continue with backslashes
                k = text.find("\n", j)
                if k < 0:
                    j = n
                    break
                if text[k - 1] != "\\":
                    j = k + 1
                    break
                j = k + 1
            flush("text", i)
            flush("text", j)
            i = j
            continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
        elif c == ";" and depth == 0:
            # the declaration takes its own line's end (a trailing comment) with it
            j = i + 1
            m = re.match(r"[ \t]*(/\*.*?\*/)?[ \t]*\n?", text[j:])
            j += m.end() if m else 0
            lead = len(text[start:i]) - len(text[start:i].lstrip())
            flush("text", start + lead)
            flush("decl", j)
            i = j
            continue
        i += 1
    flush("text", n)
    return out


def names(decl):
    """The names a top-level declaration introduces: a typedef's name and struct tag, a global's or
    function's name."""
    d = re.sub(r"/\*.*?\*/", " ", decl, flags=re.S)
    out = set()
    tag = re.match(r"\s*(typedef\s+)?(?:struct|union|enum)\s+([A-Za-z_]\w*)\s*\{", d)
    if tag:
        out.add("struct " + tag.group(2))
        if not tag.group(1):                    # `struct X { ... };` names only the tag
            return out
    if re.match(r"\s*[A-Za-z_]\w*\s*\(", d):
        return set()                            # a top-level macro call: BASH_SIZE_CHECK(T, 0x1C);
    body = d
    while "{" in body:                          # drop brace bodies, innermost first
        body = re.sub(r"\{[^{}]*\}", " ", body)
    body = body.strip().rstrip(";").strip()
    if re.match(r"(?:typedef\s+)?(?:struct|union|enum)\s+\w+\s*$", body) and "{" not in d:
        return {"struct " + body.split()[-1]}   # a forward declaration
    fp = re.search(r"\(\s*\*\s*([A-Za-z_]\w*)\s*\)", body)
    if fp:                                      # function pointer: typedef void (*Fn)(...);
        out.add(fp.group(1))
        return out
    m = re.match(r"(.*?)\b([A-Za-z_]\w*)\s*\(", body)
    if m and "=" not in m.group(1):             # a prototype
        out.add(m.group(2))
        return out
    for part in body.split(","):                # extern T a, *b[4];
        part = part.split("=")[0]
        ids = [x for x in IDENT.findall(re.sub(r"\[[^\]]*\]", "", part)) if x not in KEYWORDS]
        if ids:
            out.add(ids[-1])
    return out


def declared(texts):
    """Every name declared at top level in any of TEXTS, mapped to its first declaration."""
    out = {}
    for t in texts:
        for kind, s in items(t):
            if kind == "decl":
                for n in names(s):
                    out.setdefault(n, s)
    return out


def reconcile(base, new, known):
    """NEW (a header after a patch) with the patch's declarations of names the tree already
    declares dropped. Declarations already in BASE (this header before the patch) are never
    touched. A changed one takes BASE's form back; one KNOWN (name -> declaration in any other
    header before the patch) declares, or that the patch adds twice, goes. Returns
    (text, [dropped declarations])."""
    base_decls = {s for kind, s in items(base) if kind == "decl"}
    mine = declared([base])
    new_items = items(new)
    present = {s for kind, s in new_items if kind == "decl"}
    out, added, restored, dropped = [], set(), set(), []
    for kind, s in new_items:
        if kind != "decl" or s in base_decls:
            out.append(s)
            continue
        ns = names(s)
        old = sorted(ns & set(mine))
        if old:
            keep = mine[old[0]]
            dropped.append(s)
            if keep not in present and keep not in restored:
                restored.add(keep)              # the patch rewrote it: put the tree's form back
                out.append(keep)
        elif ns & set(known) or ns & added:
            dropped.append(s)
        else:
            added |= ns
            out.append(s)
    return "".join(out), dropped


INCLUDE = re.compile(r'^\s*#\s*include\s+"([^"]+)"', re.M)


def closure(text, heads):
    """The headers (paths as in HEADS: include/...) a file with TEXT includes, directly or through
    other headers in HEADS."""
    seen, todo = set(), ["include/" + p for p in INCLUDE.findall(text)]
    while todo:
        p = todo.pop()
        if p in seen or p not in heads:
            continue
        seen.add(p)
        todo += ["include/" + q for q in INCLUDE.findall(heads[p])]
    return seen


def visible(header, sources, heads):
    """The other headers every one of SOURCES (C file texts) that includes HEADER also includes:
    only their declarations are safe to keep over the patch's, since each file that sees HEADER
    sees them too."""
    out = None
    for text in sources:
        seen = closure(text, heads)
        if header in seen:
            out = seen if out is None else out & seen
    return (out or set()) - {header}
