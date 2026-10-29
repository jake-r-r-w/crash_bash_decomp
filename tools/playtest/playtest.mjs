#!/usr/bin/env node
// Plays every minigame of a built disc through to a win with four computer players, driving the
// game through RetroArch's network commands (UDP 55355, RAM reads and writes): no screenshots and
// no input device.
//
//   node tools/playtest/playtest.mjs [--build build/shiftall] [--modes 0-32] [--minutes 10] [--keep]
//
// It reads the build's link maps for the addresses, boots the disc with fast-forward on, waits for
// the titles, then for each mode writes what the menus would leave: four CPU heroes (heroOut, which
// the bot hook runs for), NextGameMode, and exitFlags 0x8008, HandleLevelExit's "load this mode".
// Once the level has loaded it resets exitFlags to what a clean load leaves (the intro's 0x1000):
// at this speed the previous level can move on before the 0x8008 lands, and a stray 0x8008 keeps the
// new level paused (UpdatePause). Then it polls GameState until the level is decided (exitFlags
// 0x800 or 0x1, with winner[0] the winning character); the end menu is skipped, the next mode's
// 0x8008 leaves it. While the level's frame count stands still (the rules pages, which wait for
// Cross) it taps Cross on the network RetroPad (player 1, UDP 55400; tools/playtest/playtest.cfg).
// A level whose frame count stops for a minute is a hang. Results go to <build>/playtest.json and
// a screenshot of each mode's end to RetroArch's screenshot folder. PT_TRACE=1 prints every poll.
import { spawn } from "node:child_process";
import dgram from "node:dgram";
import fs from "node:fs";
import path from "node:path";

const RA = process.env.RETROARCH || "C:/RetroArch-Win64/retroarch.exe";
const CORE = process.env.RETROARCH_CORE || "C:/RetroArch-Win64/cores/pcsx_rearmed_libretro.dll";
const PORT = 55355;
const CFG = path.resolve("tools/playtest/playtest.cfg");
const PAD_PORT = 55400;  // the network RetroPad, player 1
const CROSS = 0;         // RETRO_DEVICE_ID_JOYPAD_B, the PlayStation's Cross

// GameState (GameInfo) offsets, include/bash/nas.h
const GS = { frames: 0x0C, exitFlags: 0x30, mode: 0x34, heroChoice: 0x4C, heroOut: 0x5C, winner: 0x60 };
// the boss fights. Each boss is beaten its own way (Papu: crates thrown at him and his minions'
// waves), which the bots don't play, and each then sets BossState.flags 0x8000, which the engine's
// hero code takes as the boss out of play (heroes.c, ProcessHeroes): from there the level ends as a
// battle does (the last hero standing, ChooseWinner, the win sequence). So the harness lets the
// fight run BOSS_FIGHT_FRAMES (game frames, so the speed doesn't matter), holding the heroes at their starting health, then sets that flag; the
// mode passes if a hero wins. What it doesn't cover is each boss's own defeat.
const BOSS = new Set([4, 9, 22, 31]);
const BOSS_FIGHT_FRAMES = 150;
const HERO_SIZE = 0x6C, HERO_HEALTH = 0x36, BOSS_FLAGS = 4, BOSS_HEALTH = 6;
// the level names, in mode order (main.c's LevName* strings)
const NAMES = ["Jungle Bash", "Space Bash", "Snow Bash", "Drain Bash", "Papu Pummel", "Swamp Fox",
  "Desert Fox", "Jungle Fox", "Metal Fox", "Big Bad Fox", "Pogo Painter", "Pogo-a-Gogo", "El Pogo Loco",
  "Pogo Padlock", "Crashball", "Beach Ball", "N. Ballism", "Sky Balls", "Tilt Panic", "Polar Panic",
  "Melt Panic", "Manic Panic", "Bearminator", "Dot Dash", "Toxic Dash", "Splash Dash", "Dante's Dash",
  "Ring Ding", "Dragon Drop", "Mallet Mash", "Keg Kaboom", "Oxide Ride", "Tie-Breaker"];
const TRACE = !!process.env.PT_TRACE;

function args() {
  const a = { build: "build/shiftall", modes: "0-32", minutes: 10, keep: false };
  const v = process.argv.slice(2);
  for (let i = 0; i < v.length; i++) {
    if (v[i] === "--build") a.build = v[++i];
    else if (v[i] === "--modes") a.modes = v[++i];
    else if (v[i] === "--minutes") a.minutes = Number(v[++i]);
    else if (v[i] === "--keep") a.keep = true;
    else { console.error("unknown argument " + v[i]); process.exit(2); }
  }
  a.modeList = a.modes.split(",").flatMap(r => {
    const [lo, hi] = r.split("-").map(x => parseInt(x, 0));
    return Array.from({ length: (hi ?? lo) - lo + 1 }, (_, k) => lo + k);
  });
  return a;
}

function symbols(dir) {
  const s = {};
  for (const m of ["main.map", "engine.map"]) {
    for (const line of fs.readFileSync(path.join(dir, m), "utf8").split("\n")) {
      const r = line.match(/^\s+0x([0-9a-f]{8})\s+([A-Za-z_]\w*)\s*$/);
      if (r && !(r[2] in s)) s[r[2]] = parseInt(r[1], 16);
    }
  }
  for (const n of ["GameInfo", "NextGameMode", "ProcessControl", "RetryProcess", "FrameCount", "BossState", "Heroes"])
    if (!(n in s)) throw new Error(`no ${n} in ${dir}'s maps`);
  return s;
}

class Ra {
  constructor() {
    this.sock = dgram.createSocket("udp4");
    this.waiting = null;
    this.sock.on("message", m => { if (this.waiting) { const w = this.waiting; this.waiting = null; w(m.toString()); } });
  }
  send(cmd) { this.sock.send(cmd, PORT, "127.0.0.1"); }
  ask(cmd, ms = 1000) {
    return new Promise(res => {
      const t = setTimeout(() => { this.waiting = null; res(null); }, ms);
      this.waiting = r => { clearTimeout(t); res(r); };
      this.send(cmd);
    });
  }
  async read(addr, n) {
    const r = await this.ask(`READ_CORE_RAM ${(addr & 0x1FFFFF).toString(16)} ${n}`);
    if (!r) return null;
    const p = r.trim().split(/\s+/).slice(2);
    if (p.length < n || p[0] === "-1") return null;
    return Buffer.from(p.slice(0, n).map(h => parseInt(h, 16)));
  }
  // a button's state on the network RetroPad: struct remote_message {int port, device, index, id;
  // uint16_t state;} (20 bytes with its padding), device 1 = RETRO_DEVICE_JOYPAD
  pad(id, down) {
    const b = Buffer.alloc(20);
    b.writeInt32LE(1, 4); b.writeInt32LE(id, 12); b.writeUInt16LE(down ? 1 : 0, 16);
    this.sock.send(b, PAD_PORT, "127.0.0.1");
  }
  async tap(id) { this.pad(id, true); await sleep(60); this.pad(id, false); this.pad(id, false); await sleep(60); }
  write(addr, bytes) {
    this.send(`WRITE_CORE_RAM ${(addr & 0x1FFFFF).toString(16)} ${[...bytes].map(b => b.toString(16).padStart(2, "0")).join(" ")}`);
  }
  close() { this.sock.close(); }
}

const sleep = ms => new Promise(r => setTimeout(r, ms));
const u32 = v => { const b = Buffer.alloc(4); b.writeUInt32LE(v >>> 0); return b; };
const u16 = v => { const b = Buffer.alloc(2); b.writeUInt16LE(v & 0xFFFF); return b; };
const s16 = v => { const b = Buffer.alloc(2); b.writeInt16LE(v); return b; };

async function state(ra, s) {
  const g = await ra.read(s.GameInfo, 0x64);
  const pc = await ra.read(s.ProcessControl, 8);
  if (!g || !pc) return null;
  return { frames: g.readUInt32LE(GS.frames), exitFlags: g.readUInt16LE(GS.exitFlags), mode: g.readInt16LE(GS.mode),
           winner: [g[GS.winner], g[GS.winner + 1]], process: pc.readUInt32LE(0), next: pc.readUInt32LE(4) };
}

// fast-forward is a toggle that RetroArch drops when it comes too early: measure the speed by
// FrameCount (the vsync count, 60 a second at 1x) and toggle until it runs well above that
async function ensureFast(ra, s) {
  for (let i = 0; i < 4; i++) {
    const a = await ra.read(s.FrameCount, 4); await sleep(500); const b = await ra.read(s.FrameCount, 4);
    if (!a || !b) { await sleep(500); continue; }
    const rate = (b.readUInt32LE(0) - a.readUInt32LE(0)) * 2;
    if (rate > 150) return rate;
    ra.send("FAST_FORWARD");
    await sleep(300);
  }
  return 0;
}

async function waitFor(ra, s, test, ms, what) {
  const end = Date.now() + ms;
  let st = null;
  while (Date.now() < end) {
    st = await state(ra, s);
    if (TRACE && st) console.log(`   wait process ${st.process.toString(16)} next ${st.next.toString(16)} mode ${st.mode} frames ${st.frames} exit ${st.exitFlags.toString(16)}`);
    if (st && test(st)) return st;
    await sleep(100);
  }
  throw new Error(`timed out waiting for ${what} (last state ${JSON.stringify(st)})`);
}

async function runMode(ra, s, mode, minutes) {
  const G = s.GameInfo;
  await ensureFast(ra, s);
  const start = Date.now();
  // what the menus leave behind for a 4-CPU battle: the heroes, all of them computer players
  ra.write(G + GS.heroChoice, [1, 2, 3, 4]);
  ra.write(G + GS.heroOut, [1, 1, 1, 1]);
  ra.write(s.NextGameMode, u32(mode));
  ra.write(G + GS.exitFlags, u16(0x8008));
  let st = await waitFor(ra, s, st => st.mode === mode && st.process === s.RetryProcess && st.next === 0, 180000,
                         `mode ${mode} to load`);
  // a clean load leaves exitFlags 0 or the intro's 0x1000; anything else is the handover's
  if (st.exitFlags & ~0x1000) ra.write(G + GS.exitFlags, u16(st.exitFlags & 0x1000));
  const loaded = (Date.now() - start) / 1000;
  const boss = BOSS.has(mode);
  let heroHealth = null;
  if (boss) {
    const hb = await ra.read(s.Heroes, HERO_SIZE * 4);
    heroHealth = [0, 1, 2, 3].map(i => Math.max(hb.readInt16LE(i * HERO_SIZE + HERO_HEALTH), 1));
  }
  let last = -1, still = Date.now(), taps = 0, fight0 = null;
  const end = Date.now() + minutes * 60000;
  while (true) {
    ra.write(G + GS.heroOut, [1, 1, 1, 1]);
    if (boss) {
      for (let i = 0; i < 4; i++) ra.write(s.Heroes + i * HERO_SIZE + HERO_HEALTH, s16(heroHealth[i]));
      const bf = await ra.read(s.BossState + BOSS_FLAGS, 2);
      if (bf && fight0 !== null && last - fight0 >= BOSS_FIGHT_FRAMES && !(bf.readUInt16LE(0) & 0x8000)) {
        ra.write(s.BossState + BOSS_FLAGS, u16(bf.readUInt16LE(0) | 0x8000));
        ra.write(s.BossState + BOSS_HEALTH, s16(0));
      }
    }
    st = await state(ra, s);
    if (TRACE && st) console.log(`   play ${((Date.now() - start) / 1000).toFixed(1)}s frames ${st.frames} exit ${st.exitFlags.toString(16)}`);
    if (st) {
      // decided: a winner (0x800) or the level finished (0x1). The next mode's 0x8008 leaves it.
      if (st.exitFlags & 0x801) break;
      if (st.frames !== last) { last = st.frames; still = Date.now(); }
      if (boss && fight0 === null && last >= 0) fight0 = last;  // frames stand still on the rules pages, so they count only play
      if (st.process !== s.RetryProcess && st.next === 0 && st.mode !== mode)
        return { mode, result: "LEFT", detail: `the mode changed to ${st.mode} without a winner`, loaded };
    }
    // stalled: the rules pages wait for Cross
    if (Date.now() - still > 300 && Date.now() - still < 60000) { await ra.tap(CROSS); taps++; continue; }
    if (Date.now() - still > 60000 || Date.now() > end) {
      ra.send("SCREENSHOT"); await sleep(1500);
      return { mode, result: Date.now() > end ? "TIMEOUT" : "HANG",
               detail: `frames stopped at ${last} after ${taps} taps, exitFlags 0x${st?.exitFlags.toString(16)}`, state: st, loaded };
    }
    await sleep(boss ? 0 : 250);  // a boss fight: keep the heroes topped up as often as it can
  }
  ra.send("SCREENSHOT");
  // the games' choose-winner code sets 0x800 and winner[0] (the character); a draw leaves winner 0.
  // A boss fight is won when the boss was beaten (its flag) and a hero won.
  let result = st.winner[0] ? "WIN" : "DRAW", bossHealth;
  if (boss) {
    const b = await ra.read(s.BossState, 8);
    bossHealth = b.readInt16LE(BOSS_HEALTH);
    result = (b.readUInt16LE(BOSS_FLAGS) & 0x8000) && st.winner[0] ? "WIN" : "LOST";
  }
  return { mode, result, bossHealth, exitFlags: "0x" + st.exitFlags.toString(16), winner: st.winner, frames: st.frames,
           loaded, taps, seconds: Math.round((Date.now() - start) / 1000) };
}

async function main() {
  const a = args();
  const s = symbols(a.build);
  const cue = path.resolve(a.build, "CrashBash.cue");
  const proc = spawn(RA, ["-L", CORE, "--appendconfig", CFG, cue], { stdio: "ignore", detached: a.keep });
  if (a.keep) proc.unref();
  const ra = new Ra();
  const results = [];
  const t0 = Date.now();
  try {
    // fast-forward from boot: the logos and the intro too
    for (let i = 0; i < 80 && !(await ra.ask("GET_STATUS", 500))?.includes("PLAYING"); i++) await sleep(250);
    ra.send("FAST_FORWARD");
    await waitFor(ra, s, st => st.mode === 0x28 && st.process === s.RetryProcess && st.frames > 60, 240000, "the titles");
    console.log(`   at the titles after ${Math.round((Date.now() - t0) / 1000)}s, running at ${await ensureFast(ra, s)} vsyncs a second`);
    for (const m of a.modeList) {
      let r;
      try { r = await runMode(ra, s, m, a.minutes); }
      catch (e) { r = { mode: m, result: "ERROR", detail: e.message }; }
      r.name = NAMES[m] ?? `mode ${m}`;
      results.push(r);
      const done = ["WIN", "DRAW", "LOST"].includes(r.result);
      console.log(`${String(m).padStart(2)} ${r.name.padEnd(13)} ${r.result.padEnd(7)} ` +
                  (done ? `winner ${r.winner[0]} in ${r.seconds}s (loaded in ${r.loaded.toFixed(1)}s, ${r.taps} taps, exitFlags ${r.exitFlags}` +
                          `${r.bossHealth !== undefined ? `, boss health ${r.bossHealth}` : ""})` : r.detail));
      if (!done) break;  // the game's state is unknown now: stop
    }
  } finally {
    fs.writeFileSync(path.join(a.build, "playtest.json"), JSON.stringify(results, null, 1));
    if (!a.keep) { ra.send("QUIT"); await sleep(1000); proc.kill(); }
    ra.close();
  }
  const bad = results.filter(r => r.result !== "WIN").length + (a.modeList.length - results.length);
  console.log(bad ? `${bad} mode(s) without a win` : `all ${results.length} modes played to a win in ${Math.round((Date.now() - t0) / 1000)}s`);
  process.exit(bad ? 1 : 0);
}

main().catch(e => { console.error(e); process.exit(1); });
