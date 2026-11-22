"""Bobby's ears cut free of his shoulders, in Blender (the user's word, 2026-11-22: the ear was a problem in the model itself, so fix the
model in blender, then re-import into the tool).

His model (bobby-model.glb) had the bottom edge of each long ear flap welded to his chest and upper arm: 13 faces a side joined them,
so any turn of an ear stretched those faces into a fin down to his shoulder, whatever its weights. This gives each ear its own copies of
the corners it shared with his body (not with his head, where an ear grows from) and moves them 2 mm into the ear, so they're places
of its own: the same 1365 triangles, nothing else moved, the flap now hanging over his shoulder instead of grown into it.

Which faces are an ear's and which his body's came from his rig as the hero tool made it then: bobby_ears.json, each face's middle (in
the model's units and axes) as an ear's or his body's, by the bone most of its corners went with. It was run once, on the model from
before (git history), as:
    blender -b --factory-startup --python port/mods/bobby_ears.py -- bobby-model.glb port/mods/bobby_ears.json bobby-model.glb
"""
import json
import sys

import bmesh
import bpy
from mathutils import Vector

argv = sys.argv[sys.argv.index("--") + 1:]
src, faces_json, out = argv[:3]
marks = json.load(open(faces_json))

bpy.ops.wm.read_factory_settings(use_empty=True)
bpy.ops.import_scene.gltf(filepath=src)
obj = next(o for o in bpy.context.scene.objects if o.type == "MESH")
me = obj.data
bm = bmesh.new()
bm.from_mesh(me)
bm.faces.ensure_lookup_table()
uv = bm.loops.layers.uv.active


def gltf(v):
    """Blender's axes (z up) as glTF's (y up)"""
    return Vector((v.x, v.z, -v.y))


def blender(p):
    return Vector((p[0], -p[2], p[1]))


# each face as an ear's (left, right) or his body's, by the nearest marked middle
marked = [(Vector(p), "ear_l") for p in marks["ear_l"]] + [(Vector(p), "ear_r") for p in marks["ear_r"]] + [(Vector(p), "body") for p in marks["body"]]
kind = {}
for f in bm.faces:
    mid = gltf(f.calc_center_median())
    best, d = None, 1e9
    for p, k in marked:
        e = (p - mid).length
        if e < d:
            best, d = k, e
    kind[f] = best if d < 0.002 else "head"
print("bobby_ears: faces", {k: sum(1 for v in kind.values() if v == k) for k in ("ear_l", "ear_r", "body", "head")})

made = 0
for side in ("ear_l", "ear_r"):
    ear_faces = [f for f in bm.faces if kind[f] == side]
    middle = sum((f.calc_center_median() for f in ear_faces), Vector()) / max(1, len(ear_faces))
    # the corners it shares with his body
    shared = [v for v in bm.verts if any(kind[f] == side for f in v.link_faces) and any(kind[f] == "body" for f in v.link_faces)]
    copy = {}
    for v in shared:
        nv = bm.verts.new(v.co + (middle - v.co).normalized() * 0.002)
        copy[v] = nv
    # its faces made again on its copies, their texture's corners kept
    for f in [f for f in ear_faces if any(v in copy for v in f.verts)]:
        verts = [copy.get(v, v) for v in f.verts]
        uvs = [l[uv].uv.copy() for l in f.loops] if uv is not None else None
        mat, smooth = f.material_index, f.smooth
        bm.faces.remove(f)
        nf = bm.faces.new(verts)
        nf.material_index, nf.smooth = mat, smooth
        kind[nf] = side
        if uvs is not None:
            for l, t in zip(nf.loops, uvs):
                l[uv].uv = t
        made += 1
    print("bobby_ears:", side, "corners cut free", len(shared))
bm.to_mesh(me)
bm.free()
me.update()
print("bobby_ears: faces made again", made, "triangles", sum(len(p.vertices) - 2 for p in me.polygons))
bpy.ops.export_scene.gltf(filepath=out, export_format="GLB", export_yup=True, export_texcoords=True, export_normals=True,
                          export_materials="EXPORT", use_selection=False)
