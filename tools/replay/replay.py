#!/usr/bin/env python3
"""Replay harness: play an input recording on a disc image and compare two discs frame by frame.

Runs inside the toolchain container:
    tools/dk make replay [REC=tools/replay/recordings/battle.txt]        retail vs build disc
    tools/dk python3 tools/replay/replay.py run DISC.cue REC [-o trace] [--shot N ...] [--video out.mp4]
    tools/dk python3 tools/replay/replay.py compare A.cue B.cue REC [--syms-b build/main.elf]

The emulator is the PCSX-ReARMed libretro core, driven headless through ctypes: no window, no
audio, no wall clock, so a run depends only on the disc and the recording. Its HLE BIOS is used
unless --bios-dir names a directory with a real one (scph1001.bin etc.). Every frame gives one
trace line: the frame number, a hash of the image and the game values of values.txt, read from
RAM at addresses looked up per disc (a sym file for the matching build, the ELF for a shifted
one, where everything sits elsewhere). compare runs both discs and reports the first frame
whose line differs, with screenshots of both (PNG) at that frame.

A recording (recordings/*.txt) is lines of `<frames> [buttons]`: hold those buttons (none if
empty) for that many frames. `#` starts a comment. Buttons: cross circle square triangle start
select up down left right l1 r1 l2 r2 l3 r3. Port 1 only for now.
"""
import argparse
import ctypes as C
import hashlib
import os
import re
import struct
import subprocess
import sys
import zlib

HERE = os.path.dirname(os.path.abspath(__file__))
CORE = os.environ.get("REPLAY_CORE", "/opt/libretro/pcsx_rearmed_libretro.so")
VALUES = os.path.join(HERE, "values.txt")
RETAIL_SYMS = "configs/USA/sym.main.txt,configs/USA/sym.engine.txt"

# Pinned so a run is deterministic and the same on every machine: the interpreter (no dynarec),
# no worker threads, no CD read-ahead, NTSC timing. Anything not listed keeps the core default.
OPTIONS = {
    "pcsx_rearmed_drc": "disabled",
    "pcsx_rearmed_drc_thread": "disabled",
    "pcsx_rearmed_gpu_thread_rendering": "disabled",
    "pcsx_rearmed_spu_thread": "disabled",
    "pcsx_rearmed_cd_readahead": "0",
    "pcsx_rearmed_region": "NTSC",
    "pcsx_rearmed_show_bios_bootlogo": "disabled",
    "pcsx_rearmed_frameskip_type": "disabled",
    "pcsx_rearmed_memcard1": "libretro",    # a freshly formatted card in memory, never written out
    "pcsx_rearmed_memcard2": "disabled",
}

# RETRO_DEVICE_ID_JOYPAD_*
BUTTONS = {"cross": 0, "square": 1, "select": 2, "start": 3, "up": 4, "down": 5, "left": 6,
           "right": 7, "circle": 8, "triangle": 9, "l1": 10, "r1": 11, "l2": 12, "r2": 13,
           "l3": 14, "r3": 15}
RAM_BASE, RAM_SIZE = 0x80000000, 0x200000
MEM_SYSTEM_RAM = 2
ENV_GET_CAN_DUPE, ENV_GET_SYSTEM_DIRECTORY, ENV_SET_PIXEL_FORMAT = 3, 9, 10
ENV_GET_VARIABLE, ENV_SET_VARIABLES, ENV_GET_VARIABLE_UPDATE, ENV_GET_SAVE_DIRECTORY = 15, 16, 17, 31

ENV_CB = C.CFUNCTYPE(C.c_bool, C.c_uint, C.c_void_p)
VIDEO_CB = C.CFUNCTYPE(None, C.c_void_p, C.c_uint, C.c_uint, C.c_size_t)
AUDIO_CB = C.CFUNCTYPE(None, C.c_int16, C.c_int16)
AUDIO_BATCH_CB = C.CFUNCTYPE(C.c_size_t, C.c_void_p, C.c_size_t)
POLL_CB = C.CFUNCTYPE(None)
STATE_CB = C.CFUNCTYPE(C.c_int16, C.c_uint, C.c_uint, C.c_uint, C.c_uint)


class Variable(C.Structure):
    _fields_ = [("key", C.c_char_p), ("value", C.c_char_p)]


class GameInfo(C.Structure):
    _fields_ = [("path", C.c_char_p), ("data", C.c_void_p), ("size", C.c_size_t), ("meta", C.c_char_p)]


class AvInfo(C.Structure):
    _fields_ = [("base_width", C.c_uint), ("base_height", C.c_uint), ("max_width", C.c_uint),
                ("max_height", C.c_uint), ("aspect_ratio", C.c_float), ("fps", C.c_double),
                ("sample_rate", C.c_double)]


def die(msg):
    print(f"replay: {msg}", file=sys.stderr)
    sys.exit(1)


# ---------------------------------------------------------------- inputs, values, symbols

def load_recording(path):
    """[(frames, button mask)] from a recording file."""
    out = []
    for n, line in enumerate(open(path), 1):
        words = line.split("#", 1)[0].split()
        if not words:
            continue
        if not words[0].isdigit():
            die(f"{path}:{n}: expected a frame count, got {words[0]!r}")
        mask = 0
        for b in words[1:]:
            if b.lower() not in BUTTONS:
                die(f"{path}:{n}: unknown button {b!r} (one of {' '.join(BUTTONS)})")
            mask |= 1 << BUTTONS[b.lower()]
        out.append((int(words[0]), mask))
    return out


def load_values(path):
    """[(name, size, signed)] from values.txt: lines `<symbol[+offset]> <u8|s8|u16|s16|u32|s32>`."""
    out = []
    for line in open(path):
        words = line.split("#", 1)[0].split()
        if words:
            t = words[1] if len(words) > 1 else "u32"
            out.append((words[0], int(t[1:]) // 8, t[0] == "s"))
    return out


def load_syms(paths):
    """{name: address} from comma-separated splat sym files and ELFs (their symbol tables)."""
    syms = {}
    for path in paths.split(","):
        syms.update(load_sym_file(path))
    return syms


def load_sym_file(path):
    if path.endswith(".elf"):
        nm = os.environ.get("CROSS", "mipsel-linux-gnu-") + "nm"
        r = subprocess.run([nm, path], capture_output=True, text=True)
        if r.returncode:
            die(f"{nm} {path} failed: {r.stderr.strip()}")
        pairs = (l.split() for l in r.stdout.splitlines())
        return {p[2]: int(p[0], 16) for p in pairs if len(p) == 3}
    syms = {}
    for line in open(path):
        m = re.match(r"\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", line)
        if m:
            syms[m.group(1)] = int(m.group(2), 16)
    return syms


def resolve(values, syms):
    out = []
    for name, size, signed in values:
        base, _, off = name.partition("+")
        addr = int(base, 16) if base.startswith("0x") else syms.get(base)
        if addr is None:
            die(f"value {name}: {base} is not in the symbol tables")
        addr += int(off, 0) if off else 0
        if not RAM_BASE <= addr < RAM_BASE + RAM_SIZE:
            die(f"value {name}: 0x{addr:08X} is outside main RAM")
        out.append((name, addr - RAM_BASE, size, signed))
    return out


# ---------------------------------------------------------------- the emulator

class Emu:
    """One libretro core instance. A core is a process-wide singleton: one Emu per process."""

    def __init__(self, disc, bios_dir=None):
        if not os.path.exists(CORE):
            die(f"core {CORE} not found (rebuild the toolchain image, or set REPLAY_CORE)")
        self.lib = C.CDLL(CORE)
        self.options = dict(OPTIONS, pcsx_rearmed_bios="auto" if bios_dir else "HLE")
        self.sysdir = C.c_char_p(os.path.abspath(bios_dir or "build/emu").encode())
        os.makedirs("build/emu", exist_ok=True)
        self.keep = []           # option strings handed to the core; must outlive the call
        self.pixfmt = 2          # RGB565 until the core says otherwise
        self.buttons = 0
        self.frame = None        # (bytes, width, height, bytes per pixel) of the last image
        self.audio = None        # a list collecting s16 stereo samples, when recording a video
        self.cbs = [ENV_CB(self._env), VIDEO_CB(self._video), AUDIO_CB(self._sample),
                    AUDIO_BATCH_CB(self._samples), POLL_CB(lambda: None), STATE_CB(self._input)]
        L = self.lib
        L.retro_set_environment(self.cbs[0])
        L.retro_set_video_refresh(self.cbs[1])
        L.retro_set_audio_sample(self.cbs[2])
        L.retro_set_audio_sample_batch(self.cbs[3])
        L.retro_set_input_poll(self.cbs[4])
        L.retro_set_input_state(self.cbs[5])
        L.retro_init()
        L.retro_load_game.restype = C.c_bool
        info = GameInfo(os.path.abspath(disc).encode(), None, 0, None)
        if not L.retro_load_game(C.byref(info)):
            die(f"the core could not load {disc}")
        L.retro_set_controller_port_device(0, 1)   # RETRO_DEVICE_JOYPAD
        L.retro_get_memory_data.restype = C.c_void_p
        L.retro_get_memory_size.restype = C.c_size_t
        self.ram = L.retro_get_memory_data(MEM_SYSTEM_RAM)
        if not self.ram or L.retro_get_memory_size(MEM_SYSTEM_RAM) < RAM_SIZE:
            die("the core exposes no system RAM")
        av = AvInfo()
        L.retro_get_system_av_info(C.byref(av))
        self.fps, self.sample_rate = av.fps, av.sample_rate

    def _env(self, cmd, data):
        cmd &= 0xFFFF   # drop RETRO_ENVIRONMENT_EXPERIMENTAL
        if cmd == ENV_GET_CAN_DUPE:
            C.cast(data, C.POINTER(C.c_bool))[0] = True
        elif cmd in (ENV_GET_SYSTEM_DIRECTORY, ENV_GET_SAVE_DIRECTORY):
            C.cast(data, C.POINTER(C.c_char_p))[0] = self.sysdir.value
        elif cmd == ENV_SET_PIXEL_FORMAT:
            self.pixfmt = C.cast(data, C.POINTER(C.c_uint))[0]
        elif cmd == ENV_SET_VARIABLES:
            pass      # defaults stay the core's own; GET_VARIABLE answers only for OPTIONS
        elif cmd == ENV_GET_VARIABLE:
            var = C.cast(data, C.POINTER(Variable))[0]
            val = self.options.get(var.key.decode())
            if val is None:
                return False
            s = C.c_char_p(val.encode())
            self.keep.append(s)
            C.cast(data, C.POINTER(Variable))[0].value = s.value
        elif cmd == ENV_GET_VARIABLE_UPDATE:
            C.cast(data, C.POINTER(C.c_bool))[0] = False
        else:
            return False
        return True

    def _video(self, data, w, h, pitch):
        if not data:
            return    # a duplicate frame: the image is the previous one
        bpp = 4 if self.pixfmt == 1 else 2
        row = w * bpp
        raw = C.string_at(data, pitch * h)
        if pitch != row:
            raw = b"".join(raw[y * pitch:y * pitch + row] for y in range(h))
        self.frame = (raw, w, h, bpp)

    def _sample(self, left, right):
        if self.audio is not None:
            self.audio.append(struct.pack("<hh", left, right))

    def _samples(self, data, frames):
        if self.audio is not None:
            self.audio.append(C.string_at(data, frames * 4))
        return frames

    def _input(self, port, device, index, id_):
        return (self.buttons >> id_) & 1 if port == 0 and device == 1 else 0

    def run_frame(self, buttons):
        self.buttons = buttons
        self.lib.retro_run()

    def read(self, off, size, signed):
        return int.from_bytes(C.string_at(self.ram + off, size), "little", signed=signed)

    def image_hash(self):
        if not self.frame:
            return "-" * 16
        raw, w, h, _ = self.frame
        return hashlib.blake2b(struct.pack("<II", w, h) + raw, digest_size=8).hexdigest()

    def screenshot(self, path):
        """The last image as an RGB PNG."""
        if not self.frame:
            return
        raw, w, h, bpp = self.frame
        if bpp == 2:
            px = struct.unpack(f"<{w * h}H", raw)
            rgb = bytearray(w * h * 3)
            for i, p in enumerate(px):
                r, g, b = (p >> 11) & 31, (p >> 5) & 63, p & 31
                rgb[i * 3:i * 3 + 3] = bytes(((r << 3) | (r >> 2), (g << 2) | (g >> 4), (b << 3) | (b >> 2)))
        else:   # XRGB8888, stored B G R X
            rgb = bytearray(w * h * 3)
            rgb[0::3], rgb[1::3], rgb[2::3] = raw[2::4], raw[1::4], raw[0::4]
        rows = b"".join(b"\0" + bytes(rgb[y * w * 3:(y + 1) * w * 3]) for y in range(h))

        def chunk(tag, body):
            return struct.pack(">I", len(body)) + tag + body + struct.pack(">I", zlib.crc32(tag + body))
        with open(path, "wb") as f:
            f.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
                    + chunk(b"IDAT", zlib.compress(rows, 6)) + chunk(b"IEND", b""))


class Video:
    """An MP4 of a run: every image scaled to one size (the PS1 changes resolution between screens),
    piped to ffmpeg; the sound is muxed in at the end."""
    W, H = 640, 480

    def __init__(self, path, emu):
        import numpy
        self.np, self.path, self.emu = numpy, path, emu
        self.tmp = path + ".noaudio.mp4"
        self.ff = subprocess.Popen(
            ["ffmpeg", "-y", "-loglevel", "error", "-f", "rawvideo", "-pix_fmt", "rgb24",
             "-s", f"{self.W}x{self.H}", "-r", f"{emu.fps:.4f}", "-i", "-", "-c:v", "libx264",
             "-pix_fmt", "yuv420p", "-crf", "20", self.tmp], stdin=subprocess.PIPE)
        self.maps = {}
        emu.audio = []

    def add(self):
        np, f = self.np, self.emu.frame
        if not f:
            img = np.zeros((self.H, self.W, 3), np.uint8)
        else:
            raw, w, h, bpp = f
            if bpp == 2:
                p = np.frombuffer(raw, "<u2").reshape(h, w).astype(np.uint32)
                r, g, b = (p >> 11) & 31, (p >> 5) & 63, p & 31
                rgb = np.stack([(r << 3) | (r >> 2), (g << 2) | (g >> 4), (b << 3) | (b >> 2)], -1)
            else:
                rgb = np.frombuffer(raw, np.uint8).reshape(h, w, 4)[:, :, 2::-1]
            if (w, h) not in self.maps:
                self.maps[(w, h)] = (np.arange(self.H) * h // self.H, np.arange(self.W) * w // self.W)
            ys, xs = self.maps[(w, h)]
            img = rgb.astype(np.uint8)[ys][:, xs]
        self.ff.stdin.write(img.tobytes())

    def close(self):
        self.ff.stdin.close()
        self.ff.wait()
        wav = self.path + ".s16"
        with open(wav, "wb") as f:
            f.write(b"".join(self.emu.audio))
        subprocess.run(["ffmpeg", "-y", "-loglevel", "error", "-i", self.tmp, "-f", "s16le", "-ar",
                        str(round(self.emu.sample_rate)), "-ac", "2", "-i", wav, "-c:v", "copy",
                        "-c:a", "aac", "-b:a", "160k", "-shortest", self.path], check=True)
        os.remove(self.tmp)
        os.remove(wav)


# ---------------------------------------------------------------- commands

def play(disc, rec, syms, trace, shots=(), stop=None, shot_dir="build/replay", bios_dir=None, tag=None,
         video=None, start=0):
    """Run the recording on disc, writing one trace line per frame; returns the frame count."""
    values = resolve(load_values(VALUES), load_syms(syms))
    steps = load_recording(rec)
    total = sum(n for n, _ in steps)
    stop = total if stop is None else min(stop, total)
    shots = set(shots)
    emu = Emu(disc, bios_dir)
    vid = None
    tag = tag or os.path.splitext(os.path.basename(disc))[0]
    os.makedirs(shot_dir, exist_ok=True)
    frame = 0
    with open(trace, "w") as out:
        out.write("# frame image " + " ".join(n for n, *_ in values) + "\n")
        for n, mask in steps:
            for _ in range(n):
                if frame >= stop:
                    break
                emu.run_frame(mask)
                frame += 1
                if video and frame > start:
                    vid = vid or Video(video, emu)
                    vid.add()
                vals = " ".join(f"{emu.read(o, s, sg):X}" for _, o, s, sg in values)
                out.write(f"{frame} {emu.image_hash()} {vals}\n")
                if frame in shots:
                    emu.screenshot(f"{shot_dir}/{tag}_{frame:06d}.png")
            else:
                continue
            break
    if vid:
        vid.close()
    return frame


def first_diff(ta, tb):
    with open(ta) as a, open(tb) as b:
        head = a.readline().split()[2:]   # after "# frame"
        b.readline()
        for la, lb in zip(a, b):
            if la != lb:
                return int(la.split()[0]), head, la.split()[1:], lb.split()[1:]
    return None


def cmd_run(a):
    trace = a.output or f"build/replay/{os.path.splitext(os.path.basename(a.disc))[0]}.trace"
    os.makedirs(os.path.dirname(trace), exist_ok=True)
    n = play(a.disc, a.rec, a.syms, trace, a.shot, a.frames, bios_dir=a.bios_dir, tag=a.tag,
             video=a.video, start=a.video_from)
    print(f"{a.disc}: {n} frames -> {trace}")


def cmd_compare(a):
    """Both runs go in child processes: the core is a singleton, so one process plays one disc."""
    os.makedirs("build/replay", exist_ok=True)
    runs = [(a.disc_a, a.syms_a, "a"), (a.disc_b, a.syms_b, "b")]

    def child(disc, syms, tag, extra=()):
        cmd = [sys.executable, os.path.abspath(__file__), "run", disc, a.rec, "--syms", syms,
               "-o", f"build/replay/{tag}.trace", "--tag", tag, *extra]
        if a.bios_dir:
            cmd += ["--bios-dir", a.bios_dir]
        if a.frames:
            cmd += ["--frames", str(a.frames)]
        return subprocess.Popen(cmd, stdout=subprocess.DEVNULL)

    for p in [child(*r) for r in runs]:
        if p.wait():
            die("a run failed (see above)")
    d = first_diff("build/replay/a.trace", "build/replay/b.trace")
    if d is None:
        n = sum(1 for _ in open("build/replay/a.trace")) - 1
        print(f"REPLAY OK {n} frames identical ({os.path.basename(a.rec)})")
        return
    frame, head, va, vb = d
    diffs = [f"{h} {x} != {y}" for h, x, y in zip(head, va, vb) if x != y]
    for p in [child(d_, s, t + "_shot", ["--shot", str(frame), "--frames", str(frame)]) for d_, s, t in runs]:
        p.wait()
    print(f"REPLAY DIFF at frame {frame}: " + ", ".join(diffs))
    print(f"  screenshots: build/replay/{{a,b}}_shot_{frame:06d}.png; traces: build/replay/{{a,b}}.trace")
    sys.exit(1)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run", help="play a recording on one disc, write its trace")
    r.add_argument("disc")
    r.add_argument("rec")
    r.add_argument("--syms", default=RETAIL_SYMS, help="sym files or ELFs giving the values' addresses, comma-separated")
    r.add_argument("-o", "--output")
    r.add_argument("--shot", type=int, action="append", default=[], help="save a PNG of this frame")
    r.add_argument("--frames", type=int, help="stop after this many frames")
    r.add_argument("--bios-dir")
    r.add_argument("--video", help="also record the run as this MP4, with sound")
    r.add_argument("--video-from", type=int, default=0, help="start the video after this frame")
    r.add_argument("--tag", help="screenshot name prefix (default: the disc's name)")
    r.set_defaults(func=cmd_run)
    c = sub.add_parser("compare", help="play a recording on two discs, report the first difference")
    c.add_argument("disc_a")
    c.add_argument("disc_b")
    c.add_argument("rec")
    c.add_argument("--syms-a", default=RETAIL_SYMS)
    c.add_argument("--syms-b", default=RETAIL_SYMS)
    c.add_argument("--frames", type=int)
    c.add_argument("--bios-dir")
    c.set_defaults(func=cmd_compare)
    a = ap.parse_args()
    a.func(a)


if __name__ == "__main__":
    main()
