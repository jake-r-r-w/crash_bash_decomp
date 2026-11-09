#!/usr/bin/env node
/* Play tests: the real game, played through its own pad, by Node on the host. They need your own game data, and leave what they
 * make in build/port/playtest/. The exit code is 0 if all passed.
 *
 *   node port/tests/playtest.mjs bosses [papu fox bear oxide] [--watch]
 *       Beats each boss from a real Adventure, with the player in bossbot.c: the fight's won by its own rules, and back in the
 *       warp room. NO HARM HERO is on, for the luck a fight can't plan for. --watch plays them one by one at normal speed, in
 *       a window with sound.
 *   node port/tests/playtest.mjs modes [--vblanks N] [--slots N] [--from M] [--to M] [--asan]
 *       Every game mode, played by the computer, some at a time tiled on the screen, with three shots of each. --asan uses the
 *       AddressSanitizer build, and fails on anything it reports.
 *   node port/tests/playtest.mjs adventure [--seconds N] [--resume]
 *       Plays a new Adventure with no cheats, by adventureplayer.c and bossbot.c: door by door by the game's rules until there's nothing left
 *       to win, then saves at the computer, and passes at 200% if a new run loads that save with everything still there. 30 minutes at most
 *       by default. It saves every few wins, and --resume carries on from the last one. Its log says how far it's got, as it goes. A challenge
 *       it keeps losing gets weaker computer opponents, and the log and the result say how many were won that way.
 *   node port/tests/playtest.mjs challenges [--seconds N] [--at-once N] [LEVEL | LEVEL.CHOICE ...]
 *       Each Adventure challenge alone (the door's choice: 0 trophy, 1 and 2 the gem and crystal, 3 and 4 the relics) from everything else won,
 *       some at once, up to 60 seconds each. For working on the player: which it wins, and in how many goes.
 *   node port/tests/playtest.mjs same [--save] [--all] [--hero ID]
 *   node port/tests/playtest.mjs image
 *   node port/tests/playtest.mjs packs
 *       Frames and sound of a few modes against a saved baseline (--save makes it), about a minute: whether a change that shouldn't alter
 *       anything didn't. Between big steps of cleaning up, in place of the long tests. --hero ID plays the hero check's games with
 *       four of that extra hero instead, against a baseline of its own: for a change to its making or to extra heroes' code.
 *   node port/tests/playtest.mjs hero ID [--one]
 *       An extra hero (docs/EXTRA_HEROES.md) in every game, four of it at once, each borrowing another base (--one: only player 1):
 *       its model of every kind made, each game drawing it in its own kind, none on foot for want of one, nothing crashed, and three
 *       shots of each game on a contact sheet to look over. The checklist's automatic part, for work on that hero.
 *   node port/tests/playtest.mjs heroes [ID ...]
 *       Whether the extra heroes load (all of herolist.h's by default), in a few seconds: one short game with them as the players, each
 *       made from its disc and drawn as itself. What a release or a change elsewhere checks of them, in place of `hero ID`.
 *   node port/tests/playtest.mjs mods
 *       Mods from a mods folder: zips of textures, of archive files and of patches to them, broken ones, and which wins, by the
 *       pictures of one mode.
 *   node port/tests/playtest.mjs menu
 *       From the title screen through the menus, by the pad alone, into the first game of a new Adventure's first room.
 *   node port/tests/playtest.mjs shots FROM STEP COUNT [crashbash options...]
 *       One run, a shot every STEP vblanks, on a contact sheet (Windows).
 *
 * For bosses, --no-cheats leaves NO HARM HERO off, --vblanks N lets it try for that long, --shots FROM STEP COUNT takes those shots instead of eight spread over the run,
 * and --hero ID plays it as that extra hero (roo: --extra-hero 0 ID, docs/EXTRA_HEROES.md).
 *
 * --exe FILE picks another build (by default windows-clang's, or linux-clang's on Linux). A crash's addresses become source lines if
 * llvm-symbolizer is on the PATH (Windows). On Linux they run in the port's container, on its virtual screen (docs/PORT.md, Linux):
 *   tools/dl xvfb-run -a -s "-screen 0 1920x1080x24" node port/tests/playtest.mjs modes */
import crypto from 'node:crypto';
import { spawn, spawnSync } from 'node:child_process';
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import { fileURLToPath } from 'node:url';
import { bps_make } from '../mods/bps.mjs';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '../..');
const WINDOWS = process.platform === 'win32';
/* --android: the game in Android's emulator (or a phone) by adb, one run at a time (docs/PORT.md, Android). The app takes its arguments
   with its launch; a file it's told to write goes in its folder on the device and is fetched after; what it printed is its log.txt there.
   Its exit code can't be had, so a run's is the signal if Android logged one, else 0 if its log reached the vblanks asked for, else 1. */
const ANDROID = process.argv.includes('--android');
const ADB = process.env.ADB ?? (WINDOWS ? path.join(process.env.LOCALAPPDATA ?? '', 'Android/Sdk/platform-tools/adb.exe') : 'adb');
const ANDROID_APP = 'io.github.jakerrw.crashbash';
const ANDROID_DIR = `/sdcard/Android/data/${ANDROID_APP}/files`;
/* A folder for each system's runs, as a suite empties its own first: two systems' suites at once shared one, and one emptied it under
   the other */
const OUT = path.join(ROOT, `build/port/playtest${ANDROID ? '-android' : WINDOWS ? '' : `-${process.platform}`}`);
const DEFAULT_EXE = ANDROID ? 'android' : WINDOWS ? 'build/port/windows-clang/crashbash.exe' : 'build/port/linux-clang/crashbash';
const ASAN_EXE = WINDOWS ? 'build/port/asan/crashbash.exe' : 'build/port/linux-asan/crashbash';
const BOSS_TIMEOUT = 280, MODE_TIMEOUT = 200;

/* mode: the level selector's row and the game mode's number; warp: the room a win leaves for; vblanks: long enough to get there */
const BOSSES = {
    papu: { mode: 4, warp: 33, vblanks: 14000 },
    fox: { mode: 9, warp: 33, vblanks: 13000 },
    bear: { mode: 22, warp: 33, vblanks: 9000 },
    oxide: { mode: 31, warp: 37, vblanks: 16500 },
};
const CROSS = '4000', START = '8', DOWN = '40';

/* New game, the story skipped, the hub, pause, the cheat menu, the level selector, that row, and past the rules. */
function route_to_boss(mode) {
    const press = (vblank, button) => ['--press', String(vblank), button];
    const args = [];

    for (const v of [60, 120, 180, 240, 300, 400, 500, 700]) {
        args.push(...press(v, CROSS));
    }
    args.push(...press(1150, START));
    for (const v of [1200, 1220, 1240, 1260]) {
        args.push(...press(v, DOWN));
    }
    args.push(...press(1280, CROSS), ...press(1320, CROSS));
    let t = 1360;
    for (let i = 0; i < mode; i++, t += 14) {
        args.push(...press(t, DOWN));
    }
    t += 20;
    args.push(...press(t, CROSS));
    for (let v = t + 150; v < t + 500; v += 40) {
        args.push(...press(v, CROSS));
    }
    return args;
}

/* adb, given a limit: one adb shell call hung for ten minutes once, and a suite waited on it for good */
function adb(args, limit = 60000) {
    const r = spawnSync(ADB, args, { encoding: 'utf8', env: { ...process.env, MSYS_NO_PATHCONV: '1' }, timeout: limit });

    return { ...r, stdout: r.stdout ?? '' };
}

const sleep = ms => new Promise(r => setTimeout(r, ms));

/* run's Android side (--android, above) */
async function run_android(args, logfile, timeout) {
    const fetch = [];
    const sent = args.slice();

    for (let i = 0; i < sent.length; i++) {
        const at = sent[i] === '--shot' ? i + 2 : sent[i] === '--wav' ? i + 1 : -1;

        if (at > 0 && at < sent.length) {
            const device = `${ANDROID_DIR}/out/${path.basename(sent[at])}`;

            fetch.push([device, path.resolve(ROOT, sent[at])]);
            sent[at] = device;
            i = at;
        }
    }
    adb(['shell', 'am', 'force-stop', ANDROID_APP]);
    adb(['shell', 'rm', '-rf', `${ANDROID_DIR}/out`, `${ANDROID_DIR}/log.txt`]);
    adb(['shell', 'mkdir', '-p', `${ANDROID_DIR}/out`]);
    adb(['logcat', '-c']);
    adb(['shell', 'am', 'start', '-n', `${ANDROID_APP}/.CrashBashActivity`, '--esa', 'args', sent.join(',')]);
    const started = Date.now();
    await sleep(2000);
    while (adb(['shell', 'pidof', ANDROID_APP]).stdout.trim() !== '') {
        if (timeout && Date.now() - started > timeout * 1000) {
            adb(['shell', 'am', 'force-stop', ANDROID_APP]);
            break;
        }
        await sleep(1000);
    }
    adb(['pull', `${ANDROID_DIR}/log.txt`, logfile]);
    for (const [device, local] of fetch) {
        adb(['pull', device, local]);
    }
    const text = fs.existsSync(logfile) ? fs.readFileSync(logfile, 'latin1') : '';
    const fatal = adb(['logcat', '-d', '-s', 'libc:F', 'DEBUG:F']).stdout.match(/Fatal signal \d+ \((\w+)\)/);
    const frames = args.includes('--frames') ? args[args.indexOf('--frames') + 1] : null;

    return { code: fatal != null ? fatal[1] : frames != null && new RegExp(`^${frames} vblanks`, 'm').test(text) ? 0 : 1, text };
}

/* Runs the game; resolves with its exit code (or the signal that ended it, on Linux), and what it printed, also kept in logfile. */
function run(exe, args, logfile, { timeout = 0, env = {} } = {}) {
    if (ANDROID) {
        return run_android(args, logfile, timeout);
    }
    return new Promise(resolve => {
        const log = fs.openSync(logfile, 'w');
        const child = spawn(path.resolve(ROOT, exe), args, { cwd: ROOT, stdio: ['ignore', log, log], env: { ...process.env, ...env } });
        const timer = timeout ? setTimeout(() => child.kill(), timeout * 1000) : null;
        const done = code => {
            clearTimeout(timer);
            fs.closeSync(log);
            resolve({ code, text: fs.readFileSync(logfile, 'latin1') });
        };

        child.on('error', () => done(-1));
        child.on('exit', (code, signal) => done(code ?? signal));
    });
}

function crash_where(exe, text) {
    const crash = text.match(/crash: code.*$/m);
    const image = text.match(/image ([0-9A-F]+)/);
    const stacks = [...text.matchAll(/^stack: (.*)$/gm)].flatMap(m => m[1].trim().split(/\s+/));

    if (crash == null) {
        return null;
    }
    if (image == null || stacks.length === 0) {
        return crash[0];
    }
    const addrs = stacks.map(a => '0x' + (BigInt('0x' + a) - BigInt('0x' + image[1]) + 0x140000000n).toString(16));
    const sym = spawnSync('llvm-symbolizer', [`--obj=${path.resolve(ROOT, exe)}`, '--output-style=GNU', ...addrs], { encoding: 'utf8' });
    if (sym.status !== 0) {
        return crash[0];
    }
    const lines = sym.stdout.split(/\r?\n/);
    const frames = [];
    for (let i = 0; i + 1 < lines.length && frames.length < 6; i += 2) {
        frames.push(`${lines[i]} ${path.basename(lines[i + 1])}`);
    }
    return `${crash[0]}\n    ${frames.join('\n    ')}`;
}

/* Won: the boss's flag came up in the fight, and the run then left for the warp room. */
function boss_result(boss, { code, text }, exe) {
    const crash = crash_where(exe, text);
    const entered = text.search(new RegExp(`^mode ${boss.mode} at vblank`, 'm'));
    const left = text.match(new RegExp(`^mode ${boss.warp} at vblank (\\d+)`, 'gm'))?.at(-1);
    const flags = [...text.matchAll(new RegExp(`mode ${boss.mode} boss health -?\\d+ flags ([0-9A-F]+)`, 'g'))];
    const won = flags.some(m => parseInt(m[1], 16) & 0x8000);
    const falls = [...text.matchAll(/^exit flags 1,/gm)].length;

    if (crash != null) {
        return { pass: false, what: crash };
    }
    if (entered < 0) {
        return { pass: false, what: 'never reached the fight' };
    }
    if (won && left != null && text.lastIndexOf(left) > entered) {
        return { pass: true, what: `won, ${left}, ${falls} falls` };
    }
    if (text.includes('stalled')) {
        return { pass: false, what: 'no progress in the fight' };
    }
    return { pass: false, what: `not won (exit ${code}${won ? ', boss beaten but stayed in the fight' : ''})` };
}

function option(args, name, fallback) {
    const i = args.indexOf(name);

    if (i < 0) {
        return fallback;
    }
    const value = args[i + 1];
    args.splice(i, 2);
    return value;
}

function flag(args, name) {
    const i = args.indexOf(name);

    if (i < 0) {
        return false;
    }
    args.splice(i, 1);
    return true;
}

function report(results) {
    let failed = 0;

    for (const [name, r] of results) {
        console.log(`${r.pass ? 'ok  ' : 'FAIL'} ${name}: ${r.what}`);
        failed += r.pass ? 0 : 1;
    }
    console.log(`${results.length - failed} of ${results.length} passed`);
    return failed === 0 ? 0 : 1;
}

async function play_boss(name, exe, watch, tile, cheats, shot_at, vblanks, hero) {
    const boss = { ...BOSSES[name], vblanks: vblanks || BOSSES[name].vblanks };
    const dir = path.join(OUT, 'bosses', name);
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    const args = ['--cheat-menu', ...(cheats ? ['--cheats', '1'] : []), '--aim', '--skip-intro', '--skip-loading', '--saves', rel('saves'), ...route_to_boss(boss.mode)];
    if (hero != null) {
        args.push('--extra-hero', '0', hero);
    }
    if (watch) {
        args.push('--sound', '--window', '1280', '960');
    } else {
        args.push('--turbo', '--draw-every', '60', '--tile', String(tile), String(Object.keys(BOSSES).length), '--stall', '20000', rel('stalled.png'));
    }
    if (shot_at != null) {
        for (let i = 0; i < shot_at.count; i++) {
            const v = shot_at.from + i * shot_at.step;

            args.push('--shot', String(v), rel(`v${String(v).padStart(6, '0')}.png`));
        }
    } else {
        for (let i = 0; i < 8; i++) {
            args.push('--shot', String(Math.floor(boss.vblanks * (i + 1) / 9)), rel(`s${i}.png`));
        }
    }
    args.push('--frames', String(shot_at != null ? Math.max(boss.vblanks, shot_at.from + shot_at.step * shot_at.count + 5) : boss.vblanks));
    return [name, boss_result(boss, await run(exe, args, path.join(dir, 'log.txt'), { timeout: watch ? 0 : BOSS_TIMEOUT }), exe)];
}

async function bosses(args) {
    const watch = flag(args, '--watch');
    const exe = option(args, '--exe', DEFAULT_EXE);
    const cheats = !flag(args, '--no-cheats');
    const vblanks = Number(option(args, '--vblanks', 0));
    const hero = option(args, '--hero', null);
    let shot_at = null;
    const at = args.indexOf('--shots');
    if (at >= 0) {
        const [from, step, count] = args.splice(at, 4).slice(1).map(Number);

        shot_at = { from, step, count };
    }
    const names = args.length !== 0 ? args : Object.keys(BOSSES);

    for (const name of names) {
        if (BOSSES[name] == null) {
            throw new Error(`no boss called ${name}: ${Object.keys(BOSSES).join(', ')}`);
        }
    }
    let results = [];
    if (watch) {
        for (const name of names) {
            console.log(`${name}...`);
            results.push(await play_boss(name, exe, true, 0, cheats, shot_at, vblanks, hero));
        }
    } else {
        for (let i = 0; i < names.length; i += AT_ONCE) {
            results.push(...await Promise.all(names.slice(i, i + AT_ONCE).map((name, k) => play_boss(name, exe, false, i + k, cheats, shot_at, vblanks, hero))));
        }
    }
    return report(results);
}

async function modes(args) {
    const asan = flag(args, '--asan');
    const exe = option(args, '--exe', asan ? ASAN_EXE : DEFAULT_EXE);
    const vblanks = Number(option(args, '--vblanks', 900));
    const slots = Math.min(AT_ONCE, Number(option(args, '--slots', 8)));
    const from = Number(option(args, '--from', 1));
    const to = Number(option(args, '--to', 39));
    const dir = path.join(OUT, 'modes');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const env = asan ? { ASAN_OPTIONS: 'detect_leaks=0:halt_on_error=0' } : {};
    const results = [];

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    const one = async (m, slot) => {
        const shots = [[3, 'a'], [1.5, 'b']].flatMap(([d, n]) => ['--shot', String(Math.floor(vblanks / d)), rel(`m${m}-${n}.png`)]);
        const r = await run(exe, ['--frames', String(vblanks), '--turbo', '--skip-intro', '--skip-loading', '--mode', String(m), '--tile', String(slot), String(slots),
            ...shots, '--shot', String(vblanks - 5), rel(`m${m}-c.png`)], path.join(dir, `m${m}.txt`), { timeout: MODE_TIMEOUT, env });
        const crash = crash_where(exe, r.text);
        const sanitized = [...r.text.matchAll(/ERROR: AddressSanitizer: (\S+)/g)].map(a => a[1]);

        if (crash != null) {
            return [`mode ${m}`, { pass: false, what: crash }];
        }
        if (sanitized.length !== 0) {
            return [`mode ${m}`, { pass: false, what: `AddressSanitizer: ${[...new Set(sanitized)].join(', ')}` }];
        }
        return [`mode ${m}`, { pass: r.code === 0, what: `exit ${r.code}` }];
    };
    for (let m = from; m <= to; m += slots) {
        const batch = [];
        for (let s = 0; s < slots && m + s <= to; s++) {
            batch.push(one(m + s, s));
        }
        results.push(...await Promise.all(batch));
    }
    return report(results);
}

async function adventure(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const seconds = Number(option(args, '--seconds', 1800));
    const resume = flag(args, '--resume');
    const dir = path.join(OUT, 'adventure');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');

    if (!resume) {
        fs.rmSync(dir, { recursive: true, force: true });
    }
    fs.mkdirSync(dir, { recursive: true });
    const r = await run(exe, [resume ? '--adventure-resume' : '--adventure', '--turbo', '--draw-every', '120', '--skip-intro', '--skip-loading', '--saves', rel('saves'), '--frames', '2000000000'],
        path.join(dir, 'log.txt'), { timeout: seconds });
    const crash = crash_where(exe, r.text);
    const progress = text => [...text.matchAll(/^adventure: vblank.*$/gm)].at(-1)?.[0] ?? 'nothing';
    const state = text => progress(text).replace(/vblank \d+ /, '');

    if (crash != null || !/^adventure: nothing left to win/m.test(r.text)) {
        return report([['adventure', { pass: false, what: crash ?? `didn't finish in the time; ${progress(r.text)}` }]]);
    }
    const load = await run(exe, ['--adventure-load', '--turbo', '--skip-intro', '--skip-loading', '--saves', rel('saves'), '--frames', '3000'],
        path.join(dir, 'load.txt'), { timeout: MODE_TIMEOUT });
    const load_crash = crash_where(exe, load.text);
    const same = /^adventure: loaded/m.test(load.text) && state(load.text) === state(r.text);
    const weak = r.text.match(/^adventure: nothing left to win, (\d+) of it/m)?.[1] ?? '?';

    return report([['adventure', { pass: load_crash == null && same && / complete 200%/.test(progress(r.text)), what: load_crash ?? `${progress(r.text)}; ${weak} won against weak opponents; ${same ? 'loaded the same' : `loaded ${state(load.text)}`}` }]]);
}

/* One challenge at a time from everything else won, some at once: which it wins, in how many goes. */
async function challenges(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const seconds = Number(option(args, '--seconds', 60));
    const at_once = Math.min(AT_ONCE, Number(option(args, '--at-once', 8)));
    const dir = path.join(OUT, 'challenges');
    const levels = [...Array(31).keys()].filter(l => ![4, 9, 22].includes(l));
    const wanted = args.length !== 0 ? args : levels.flatMap(l => [0, 1, 2, 3, 4].map(c => `${l}.${c}`));
    const list = wanted.flatMap(w => (w.includes('.') ? [w] : [0, 1, 2, 3, 4].map(c => `${w}.${c}`)));
    const results = [];

    fs.mkdirSync(dir, { recursive: true });
    const one = async (w, slot) => {
        const [level, choice] = w.split('.');
        const r = await run(exe, ['--challenge', level, choice, '--tile', String(slot), String(at_once), '--turbo', '--draw-every', '120', '--skip-intro', '--skip-loading', '--frames', '2000000000'],
            path.join(dir, `${w}.txt`), { timeout: seconds });
        const crash = crash_where(exe, r.text);
        const tries = [...r.text.matchAll(/^adventure: try (\d+) of level/gm)].length;
        const rounds = [...r.text.matchAll(/^round:/gm)].length;

        const weak = /^adventure: challenge won against weak/m.test(r.text) ? ', weak opponents' : '';

        return [w, { pass: crash == null && /^adventure: challenge won/m.test(r.text), what: crash ?? `${tries} tries, ${rounds} rounds${weak}` }];
    };
    const queue = [...list];
    await Promise.all(Array.from({ length: at_once }, async (_, slot) => {
        while (queue.length !== 0) {
            results.push(await one(queue.shift(), slot));
        }
    }));
    return report(results.sort((a, b) => a[0].split('.')[0] - b[0].split('.')[0] || a[0].split('.')[1] - b[0].split('.')[1]));
}

/* Whether a build still draws and sounds the same, for a change that shouldn't alter either: frames and sound of a few modes against a
   saved baseline, in about a minute. */
/* The most copies of the game at once, small and in a grid (--tile): 37 together at full size took down the whole PC. */
const AT_ONCE = ANDROID ? 1 : 12; /* one app at a time on a phone */

const SAME_MODES = [0, 1, 4, 5, 10, 14, 18, 23, 27, 28, 30, 31, 33, 40];
const SAME_SHOTS = [500, 900, 1300];
const SAME_VBLANKS = 1400;
/* With --all, every mode too, a few at a time, and what's on screen outside the levels: the logos, the Adventure hub and the pause text. */
const SAME_EXTRAS = { intro: ['--frames', '1400'], hub: ['--adventure', '--frames', '1400', '--skip-intro', '--skip-loading'], pause: ['--mode', '1', '--press', '700', '8', '--frames', '1400', '--skip-intro', '--skip-loading'] };

async function same(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const save = flag(args, '--save');
    const all = flag(args, '--all');
    const hero = option(args, '--hero', null);
    const dir = path.join(OUT, 'same', ...(hero != null ? [`hero-${hero}`] : []));
    /* one for each system, as their GPU APIs draw a pixel here and there differently: the sound is the same on both */
    const base = path.join(dir, `baseline${all ? '-all' : ''}${WINDOWS ? '' : `-${process.platform}`}.json`);
    const extra = hero != null ? [0, 1, 2, 3].flatMap(p => ['--extra-hero', String(p), hero]) : [];
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const sha = file => crypto.createHash('sha256').update(fs.readFileSync(file)).digest('hex');
    const old = !save && fs.existsSync(base) ? JSON.parse(fs.readFileSync(base, 'utf8')) : null;
    const now = {};

    if (!save && old == null) {
        throw new Error('No baseline: run it with --save first');
    }
    fs.mkdirSync(dir, { recursive: true });
    const one = async (m, slot) => {
        const shots = SAME_SHOTS.flatMap(v => ['--shot', String(v), rel(`m${m}-${v}.png`)]);
        const what = SAME_EXTRAS[m] ?? ['--frames', String(SAME_VBLANKS), '--skip-intro', '--skip-loading', '--mode', String(m)];

        /* a run that doesn't start mustn't be judged by the last one's files */
        for (const v of SAME_SHOTS) {
            fs.rmSync(path.join(dir, `m${m}-${v}.png`), { force: true });
        }
        fs.rmSync(path.join(dir, `m${m}.wav`), { force: true });
        const r = await run(exe, [...what, '--turbo', '--tile', String(slot), String(SAME_MODES.length),
            ...shots, '--wav', rel(`m${m}.wav`), ...extra], path.join(dir, `m${m}.txt`), { timeout: MODE_TIMEOUT });
        const crash = crash_where(exe, r.text);

        if (crash != null) {
            return [`mode ${m}`, { pass: false, what: crash }];
        }
        if (r.code !== 0) {
            return [`mode ${m}`, { pass: false, what: `exit ${r.code}` }];
        }
        for (const v of SAME_SHOTS) {
            now[`m${m}-${v}`] = sha(path.join(dir, `m${m}-${v}.png`));
        }
        now[`m${m}-sound`] = sha(path.join(dir, `m${m}.wav`));
        const differ = Object.keys(now).filter(k => k.startsWith(`m${m}-`) && old != null && old[k] !== now[k]);

        return [`mode ${m}`, { pass: differ.length === 0, what: differ.length === 0 ? 'the same' : `differs: ${differ.map(k => k.slice(k.indexOf('-') + 1)).join(', ')}` }];
    };
    const results = [];
    const list = hero != null ? HERO_GAMES.map(g => g.mode) : all ? [...Array(41).keys(), ...Object.keys(SAME_EXTRAS)] : SAME_MODES;

    for (let i = 0; i < list.length; i += AT_ONCE) {
        results.push(...await Promise.all(list.slice(i, i + AT_ONCE).map((m, slot) => one(m, slot))));
    }

    if (save) {
        fs.writeFileSync(base, JSON.stringify(now, null, 1));
    }
    return report(results);
}

/* An extra hero in every game (docs/EXTRA_HEROES.md, The checklist): a game of each kind of model the games' heroes have, and the
   games with something of their own (Ring Ding on foot, Manic Panic on foot and on the bear, the swamp's hover tanks). */
const HERO_GAMES = [
    { mode: 0, game: 'Jungle Bash' }, { mode: 27, game: 'Ring Ding' }, { mode: 18, game: 'Tilt Panic' }, { mode: 21, game: 'Manic Panic' },
    { mode: 10, game: 'Pogo Painter' }, { mode: 14, game: 'Crashball' }, { mode: 6, game: 'Desert Fox' }, { mode: 5, game: 'Swamp Fox' },
    { mode: 23, game: 'Dot Dash' }, { mode: 25, game: 'Splash Dash' }, { mode: 29, game: 'Mallet Mash' }, { mode: 28, game: 'Dragon Drop' },
    { mode: 30, game: 'Keg Kaboom' },
    /* the tournament's podium (its test run has the four ranked, player 1 first); its lines are the ones there, as the main menu's after
       it start heroes on foot, and its shots are before it ends */
    { mode: 43, game: 'Podium', only: 'on the podium', shots: [300, 600, 900] },
];
const HERO_SHOTS = [500, 900, 1300];
const HERO_VBLANKS = 1400;

/* Its model of each kind made (bonusheroes.c's log), each game drawing every player who's it in that game's kind (extraheroes.c's),
   none on foot for want of a model, each on a base of its own, every kind it has played somewhere, and nothing crashed; the shots on
   one sheet. What's left is looking and playing. */
async function hero(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const one = flag(args, '--one');
    const id = args.shift();
    const dir = path.join(OUT, 'hero', id ?? '');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const players = one ? [1] : [1, 2, 3, 4];
    const extra = players.flatMap(p => ['--extra-hero', String(p - 1), id]);
    const results = [];
    const played = new Set();
    let made = null;

    if (id == null) {
        console.log('node port/tests/playtest.mjs hero ID [--one]: which extra hero? (roo)');
        return 2;
    }
    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    const name_of = g => `m${g.mode}-${g.game.toLowerCase().replace(/[^a-z]+/g, '-')}`;
    const shots_of = g => (g.shots ?? HERO_SHOTS).map((v, i) => rel(`${name_of(g)}-${'abc'[i]}.png`));
    const play = async (g, slot) => {
        const name = name_of(g);
        const shots = shots_of(g);
        const r = await run(exe, ['--mode', String(g.mode), '--skip-intro', '--skip-loading', '--turbo', '--tile', String(slot), String(AT_ONCE),
            '--frames', String(HERO_VBLANKS), ...(g.shots ?? HERO_SHOTS).flatMap((v, i) => ['--shot', String(v), shots[i]]), ...extra],
            path.join(dir, `${name}.txt`), { timeout: MODE_TIMEOUT });
        const crash = crash_where(exe, r.text);
        /* a game can start its heroes more than once (a round, the game proper after its intro): each player's last (of its kind, for a
           game that names one) */
        const lines = [...new Map([...r.text.matchAll(new RegExp(`^extra heroes: player (\\d) is ${id}, in hero (\\d+)'s place:(.*)$`, 'gm'))]
            .filter(m => g.only == null || m[3].trim() === g.only)
            .map(m => [m[1], m])).values()].sort((a, b) => Number(a[1]) - Number(b[1]));
        const missing = players.filter(p => !lines.some(m => Number(m[1]) === p));
        const bases = new Set(lines.map(m => m[2]));
        const kinds = [...new Set(lines.map(m => m[3].trim()))];
        const wanting = lines.filter(m => /it has no model|no model loaded/.test(m[3]));

        made ??= r.text.match(new RegExp(`^bonus heroes: ${id} (is made: a model .*|isn't made.*)$`, 'm'))?.[1] ?? null;
        for (const k of kinds.flatMap(k => k.split(', '))) {
            played.add(k);
        }
        if (crash != null) {
            return [g.game, { pass: false, what: crash }];
        }
        if (r.code !== 0) {
            return [g.game, { pass: false, what: `exit ${r.code}` }];
        }
        if (missing.length !== 0) {
            return [g.game, { pass: false, what: `player ${missing.join(', ')} isn't ${id} (not made, or nobody's base was free)` }];
        }
        if (wanting.length !== 0) {
            return [g.game, { pass: false, what: wanting.map(m => `player ${m[1]}${m[3]}`).join('; ') }];
        }
        if (bases.size !== lines.length) {
            return [g.game, { pass: false, what: `two of them on one base: ${lines.map(m => m[2]).join(', ')}` }];
        }
        return [g.game, { pass: true, what: `${kinds.join('; ')} (bases ${lines.map(m => m[2]).join(', ')})` }];
    };
    for (let i = 0; i < HERO_GAMES.length; i += AT_ONCE) {
        results.push(...await Promise.all(HERO_GAMES.slice(i, i + AT_ONCE).map((g, slot) => play(g, slot))));
    }
    /* what it was made with, and that each kind it has was played */
    const has = made?.startsWith('is made') ? made.replace(/^is made: a model /, '').split('; none ')[0].split(', ') : [];
    const none = made?.includes('; none ') ? made.split('; none ')[1] : null;
    results.unshift(['made', { pass: made?.startsWith('is made') === true && none == null,
        what: made == null ? 'no log line: is EXTRA HEROES there?' : none != null ? `no model ${none}` : made }]);
    const unplayed = has.filter(k => !played.has(k));
    results.push(['every kind played', { pass: has.length > 0 && unplayed.length === 0, what: unplayed.length === 0 ? `${has.length} kinds` : `not: ${unplayed.join(', ')}` }]);
    if (process.platform === 'win32') {
        const sheet = rel('sheet.png');

        spawnSync('powershell.exe', ['-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', 'port/tests/sheet.ps1', sheet, String(HERO_SHOTS.length), ...HERO_GAMES.flatMap(shots_of)], { cwd: ROOT, stdio: 'inherit' });
        console.log(`shots: ${sheet}`);
    }
    return report(results);
}

/* Whether the extra heroes load, which is all a release or a change elsewhere needs to know of them (the user's word, 2026-11-09):
   one short game with each of them a player, four at a time, each made from the player's disc and drawn as itself, and nothing
   crashed. `hero ID` plays one in every game, for work on that hero. The heroes are herolist.h's, by its names. */
async function heroes(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const list = fs.readFileSync(path.join(ROOT, 'port/game/include/bash/herolist.h'), 'utf8');
    const ids = args.length !== 0 ? args : [...list.matchAll(/^\s*X\((\w+), \w+\)/gm)].map(m => m[1].toLowerCase());
    const dir = path.join(OUT, 'heroes');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const results = [];

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    for (let i = 0; i < ids.length; i += 4) {
        const group = ids.slice(i, i + 4);
        const name = `game${i / 4 + 1}`;
        const r = await run(exe, ['--mode', String(HERO_GAMES[0].mode), '--skip-intro', '--skip-loading', '--turbo', '--frames', '600',
            '--shot', '590', rel(`${name}.png`), ...group.flatMap((id, p) => ['--extra-hero', String(p), id])],
            path.join(dir, `${name}.txt`), { timeout: MODE_TIMEOUT });
        const crash = crash_where(exe, r.text);

        group.forEach((id, p) => {
            const made = r.text.match(new RegExp(`^bonus heroes: ${id} (is made: .*|isn't made.*)$`, 'm'))?.[1];
            /* a game starts its heroes more than once: the last says what it's drawn as */
            const drawn = [...r.text.matchAll(new RegExp(`^extra heroes: player ${p + 1} is ${id}, in hero \\d+'s place: (.*)$`, 'gm'))].at(-1)?.[1];

            if (crash != null || r.code !== 0) {
                results.push([id, { pass: false, what: crash ?? `exit ${r.code}` }]);
            } else if (made == null || !made.startsWith('is made') || made.includes('; none ')) {
                results.push([id, { pass: false, what: made ?? 'not made: no log line (is its disc there?)' }]);
            } else if (drawn == null || /no model/.test(drawn)) {
                results.push([id, { pass: false, what: `player ${p + 1}: ${drawn ?? 'not this hero'}` }]);
            } else {
                results.push([id, { pass: true, what: `made from its disc, player ${p + 1} ${drawn} in ${HERO_GAMES[0].game}` }]);
            }
        });
    }
    return report(results);
}

async function menu(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const dir = path.join(OUT, 'menu');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const press = ['--press', '170', DOWN];

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    for (const v of [230, 350, 470, 590, 850, 1150, 1250, 1550, 1650, 1750, 2050, 2150]) {
        press.push('--press', String(v), CROSS);
    }
    const r = await run(exe, ['--turbo', '--skip-intro', '--saves', rel('saves'), ...press, '--shot', '3195', rel('game.png'), '--frames', '3200'], path.join(dir, 'log.txt'), { timeout: MODE_TIMEOUT });
    const crash = crash_where(exe, r.text);
    const started = /^mode 0 at vblank/m.test(r.text);

    return report([['menu', { pass: crash == null && r.code === 0 && started, what: crash ?? (started ? 'reached a game' : `never reached a game (exit ${r.code})`) }]]);
}

async function shots(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const [first, step, count] = args.splice(0, 3).map(Number);
    const dir = path.join(OUT, 'shots');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const files = [];
    const shot = [];

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    for (let i = 0; i < count; i++) {
        const v = first + i * step;
        const name = rel(`v${String(v).padStart(6, '0')}.png`);

        shot.push('--shot', String(v), name);
        files.push(name);
    }
    const r = await run(exe, ['--frames', String(first + step * count + 5), ...shot, ...args], path.join(dir, 'log.txt'));
    const crash = crash_where(exe, r.text);
    if (crash != null) {
        console.log(crash);
    }
    if (process.platform === 'win32') {
        const sheet = rel('sheet.png');

        spawnSync('powershell.exe', ['-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', 'port/tests/sheet.ps1', sheet, '5', ...files], { cwd: ROOT, stdio: 'inherit' });
        console.log(sheet);
    }
    return r.code === 0 && crash == null ? 0 : 1;
}

const [command, ...rest] = process.argv.slice(2).filter(a => a !== '--android');
/* adb waits for a device that isn't there, for some of what a run asks */
if (ANDROID && adb(['get-state']).stdout.trim() !== 'device') {
    console.log(`No Android device or emulator for adb (${ADB}): start one (docs/PORT.md, Android)`);
    process.exit(2);
}
/* A disc image's data comes out the same as the disc's, from each kind of image, and a wrong image is turned away. Needs make disc's image. */
async function image(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const dir = path.join(OUT, 'image');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const raw = path.join(ROOT, 'build/CrashBash.bin');
    const sha = file => crypto.createHash('sha256').update(fs.readFileSync(file)).digest('hex');
    const want = sha(path.join(ROOT, 'build/disc/files/CRASHBSH/CRASHBSH.DAT'));
    const results = [];

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    /* the same disc as an .iso: each raw sector's data */
    const bin = fs.readFileSync(raw);
    const iso = Buffer.alloc(bin.length / 2352 * 2048);
    for (let i = 0; i < bin.length / 2352; i++) {
        bin.copy(iso, i * 2048, i * 2352 + (bin[i * 2352 + 15] === 2 ? 24 : 16), i * 2352 + (bin[i * 2352 + 15] === 2 ? 24 : 16) + 2048);
    }
    fs.writeFileSync(path.join(dir, 'disc.iso'), iso);
    /* the same image with another game's executable in it, as another region would be */
    const other = Buffer.from(iso);
    const at = other.indexOf('SCUS_945.70;1');
    other.write('SCES_000.00;1', at);
    fs.writeFileSync(path.join(dir, 'other.iso'), other);
    fs.writeFileSync(path.join(dir, 'junk.bin'), Buffer.alloc(1 << 20, 0x55));
    /* a cue sheet with its file's name in another case, which Windows finds anyway and other systems must look for (but not the Linux
       container in a Windows folder, which ignores case too: there it's tried by hand in /tmp) */
    fs.writeFileSync(path.join(dir, 'case.cue'), 'FILE "DISC.ISO" BINARY\n  TRACK 01 MODE1/2048\n    INDEX 01 00:00:00\n');
    const run_image = async (name, img) => {
        const out = `${name}.dat`;
        const r = await run(exe, ['--image', img, '--data', rel(out), '--frames', '1', '--no-window'], path.join(dir, `${name}.txt`), { timeout: MODE_TIMEOUT });

        return { code: r.code, text: r.text, out: path.join(dir, out) };
    };
    for (const [name, img] of [['cue', 'build/CrashBash.cue'], ['bin', 'build/CrashBash.bin'], ['iso', rel('disc.iso')], ['cue-case', rel('case.cue')]]) {
        const r = await run_image(name, img);
        const same_data = fs.existsSync(r.out) && sha(r.out) === want;

        results.push([name, { pass: r.code === 0 && same_data, what: same_data ? 'the same as the disc\'s' : `exit ${r.code}: ${r.text.trim().split('\n').at(-1)}` }]);
        fs.rmSync(r.out, { force: true });
    }
    for (const [name, img] of [['other', rel('other.iso')], ['junk', rel('junk.bin')], ['missing', rel('nothing.cue')]]) {
        const r = await run_image(name, img);

        results.push([name, { pass: r.code === 1 && !fs.existsSync(r.out) && !fs.existsSync(`${r.out}.part`), what: r.text.trim().split('\n').at(-1) }]);
    }
    /* next to the program, it's found with nothing asked */
    const beside = path.join(dir, 'beside');
    fs.mkdirSync(beside);
    const beside_exe = path.join(beside, path.basename(exe));
    fs.copyFileSync(path.resolve(ROOT, exe), beside_exe);
    fs.chmodSync(beside_exe, 0o755);
    /* a link if the folder can have one (the Linux container's view of a Windows folder can't) */
    try {
        fs.linkSync(raw, path.join(beside, 'CrashBash.bin'));
    } catch {
        fs.copyFileSync(raw, path.join(beside, 'CrashBash.bin'));
    }
    fs.copyFileSync(path.join(ROOT, 'build/CrashBash.cue'), path.join(beside, 'CrashBash.cue'));
    const out = path.join(dir, 'beside.dat');
    const r = await run(beside_exe, ['--data', out, '--frames', '1', '--no-window'], path.join(dir, 'beside.txt'), { timeout: MODE_TIMEOUT });
    const found = fs.existsSync(out) && sha(out) === want;

    results.push(['beside', { pass: r.code === 0 && found, what: found ? 'found and the same as the disc\'s' : `exit ${r.code}: ${r.text.trim().split('\n').at(-1)}` }]);
    fs.rmSync(dir, { recursive: true, force: true });
    return report(results);
}

/* The CRC that PNGs and zips have. */
function crc32(b) {
    let c = 0xFFFFFFFF;
    for (const x of b) {
        c ^= x;
        for (let k = 0; k < 8; k++) c = c & 1 ? (c >>> 1) ^ 0xEDB88320 : c >>> 1;
    }
    return (c ^ 0xFFFFFFFF) >>> 0;
}

/* A PNG, for a pack: solid colour, by alpha or not (colour type 6 or 2). */
function png_of(w, h, rgba, colour_type = 6) {
    const chunk = (type, data) => {
        const t = Buffer.concat([Buffer.from(type, 'latin1'), data]);
        const len = Buffer.alloc(4), sum = Buffer.alloc(4);
        len.writeUInt32BE(data.length);
        sum.writeUInt32BE(crc32(t));
        return Buffer.concat([len, t, sum]);
    };
    const n = colour_type === 6 ? 4 : 3;
    const raw = Buffer.alloc((w * n + 1) * h);
    for (let y = 0; y < h; y++) {
        for (let x = 0; x < w; x++) {
            for (let c = 0; c < n; c++) raw[y * (w * n + 1) + 1 + x * n + c] = rgba[c];
        }
    }
    const head = Buffer.alloc(13);
    head.writeUInt32BE(w, 0);
    head.writeUInt32BE(h, 4);
    head[8] = 8;
    head[9] = colour_type;
    return Buffer.concat([Buffer.from([0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A]), chunk('IHDR', head), chunk('IDAT', zlib.deflateSync(raw)), chunk('IEND', Buffer.alloc(0))]);
}

/* Making a pack and using it: the textures save, a pack replaces them, and what's broken in one doesn't stop the game. */
async function packs(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const dir = path.join(OUT, 'packs');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const sha = file => crypto.createHash('sha256').update(fs.readFileSync(file)).digest('hex');
    const results = [];
    const go = (name, more) => run(exe, ['--mode', '1', '--skip-intro', '--skip-loading', '--turbo', '--window', '480', '360', '--shot', '500', rel(`${name}.png`), '--frames', '520', ...more], path.join(dir, `${name}.txt`), { timeout: MODE_TIMEOUT });
    const fine = (name, r) => r.code === 0 && crash_where(exe, r.text) == null && fs.existsSync(path.join(dir, `${name}.png`));

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    const base = await go('base', []);
    const dumped = await go('dumped', ['--dump-textures', rel('dump')]);
    const names = fs.existsSync(path.join(dir, 'dump')) ? fs.readdirSync(path.join(dir, 'dump')).filter(n => /^[0-9a-f]{16}\.png$/.test(n)) : [];
    results.push(['dump', { pass: fine('dumped', dumped) && names.length > 20 && sha(path.join(dir, 'dumped.png')) === sha(path.join(dir, 'base.png')), what: `${names.length} textures saved, picture unchanged` }]);

    /* any pack makes the atlas bigger, which moves a few sprites' edge texels by one (as UPSCALE 4X does), so this one replacing nothing is what the rest compare to */
    fs.mkdirSync(path.join(dir, 'idle'));
    fs.writeFileSync(path.join(dir, 'idle', '0000000000000000.png'), png_of(4, 4, [255, 0, 255, 255]));
    const idle = await go('idle', ['--texture-pack', rel('idle')]);
    const idle_sha = fine('idle', idle) ? sha(path.join(dir, 'idle.png')) : '';

    /* the dump reloaded as a pack of itself is the same picture */
    const same_pack = await go('same', ['--texture-pack', rel('dump')]);
    results.push(['own textures as a pack', { pass: fine('same', same_pack) && sha(path.join(dir, 'same.png')) === idle_sha, what: 'the same picture' }]);

    /* everything loud pink, at a size of its own */
    fs.mkdirSync(path.join(dir, 'pink'));
    for (const n of names) fs.writeFileSync(path.join(dir, 'pink', n), png_of(64, 32, [255, 0, 255, 255]));
    const pink = await go('pink', ['--texture-pack', rel('pink')]);
    results.push(['replaces', { pass: fine('pink', pink) && sha(path.join(dir, 'pink.png')) !== sha(path.join(dir, 'base.png')), what: 'a different picture' }]);

    /* no alpha channel */
    fs.mkdirSync(path.join(dir, 'rgb'));
    for (const n of names) fs.writeFileSync(path.join(dir, 'rgb', n), png_of(32, 32, [0, 255, 0], 2));
    const rgb = await go('rgb', ['--texture-pack', rel('rgb')]);
    results.push(['RGB PNG', { pass: fine('rgb', rgb) && sha(path.join(dir, 'rgb.png')) !== sha(path.join(dir, 'base.png')), what: 'loads' }]);

    /* broken ones: not PNGs, cut short, a stated size far too big, a name that isn't one. All stay as the game's own */
    fs.mkdirSync(path.join(dir, 'bad'));
    names.forEach((n, i) => {
        const good = png_of(16, 16, [0, 0, 255, 255]);
        const kinds = [Buffer.from('not a png'), good.subarray(0, 40), Buffer.alloc(0)];
        const huge = Buffer.from(good);
        huge.writeUInt32BE(100000, 16);
        huge.writeUInt32BE(100000, 20);
        kinds.push(huge);
        fs.writeFileSync(path.join(dir, 'bad', n), kinds[i % kinds.length]);
    });
    fs.writeFileSync(path.join(dir, 'bad', 'readme.png'), 'hello');
    const bad = await go('bad', ['--texture-pack', rel('bad')]);
    results.push(['broken files', { pass: fine('bad', bad) && sha(path.join(dir, 'bad.png')) === idle_sha, what: 'the game\'s own textures, no crash' }]);

    const none = await go('none', ['--texture-pack', rel('nothing-here')]);
    results.push(['missing pack', { pass: fine('none', none) && sha(path.join(dir, 'none.png')) === sha(path.join(dir, 'base.png')), what: 'as with no pack' }]);
    fs.mkdirSync(path.join(dir, 'empty'));
    const empty = await go('empty', ['--texture-pack', rel('empty')]);
    results.push(['empty pack', { pass: fine('empty', empty) && sha(path.join(dir, 'empty.png')) === sha(path.join(dir, 'base.png')), what: 'as with no pack' }]);
    return report(results);
}

/* A zip of [name, bytes] pairs as a zip tool makes them, a name ending in / being a folder: deflated, or stored. Or broken in one of the
   ways a download can be: a wrong CRC, encrypted, or sizes that say it's ZIP64. */
function zip_of(files, { stored = false, bad_crc = false, locked = false, zip64 = false } = {}) {
    const locals = [], list = [];
    let at = 0;

    for (const [name, data] of files) {
        const nameb = Buffer.from(name, 'utf8'), keep = stored || name.endsWith('/');
        const packed = keep ? data : zlib.deflateRawSync(data), method = keep ? 0 : 8, flags = locked ? 1 : 0;
        const crc = (crc32(data) ^ (bad_crc ? 1 : 0)) >>> 0;
        const local = Buffer.alloc(30), entry = Buffer.alloc(46);

        local.writeUInt32LE(0x04034B50, 0);
        local.writeUInt16LE(20, 4);
        local.writeUInt16LE(flags, 6);
        local.writeUInt16LE(method, 8);
        local.writeUInt32LE(crc, 14);
        local.writeUInt32LE(packed.length, 18);
        local.writeUInt32LE(data.length, 22);
        local.writeUInt16LE(nameb.length, 26);
        entry.writeUInt32LE(0x02014B50, 0);
        entry.writeUInt16LE(20, 4);
        entry.writeUInt16LE(20, 6);
        entry.writeUInt16LE(flags, 8);
        entry.writeUInt16LE(method, 10);
        entry.writeUInt32LE(crc, 16);
        entry.writeUInt32LE(zip64 ? 0xFFFFFFFF : packed.length, 20);
        entry.writeUInt32LE(zip64 ? 0xFFFFFFFF : data.length, 24);
        entry.writeUInt16LE(nameb.length, 28);
        entry.writeUInt32LE(at, 42);
        locals.push(local, nameb, packed);
        list.push(entry, nameb);
        at += 30 + nameb.length + packed.length;
    }
    const listed = Buffer.concat(list), end = Buffer.alloc(22);
    end.writeUInt32LE(0x06054B50, 0);
    end.writeUInt16LE(files.length, 8);
    end.writeUInt16LE(files.length, 10);
    end.writeUInt32LE(listed.length, 12);
    end.writeUInt32LE(at, 16);
    return Buffer.concat([...locals, listed, end]);
}

/* Mods from a mods folder: zips and a folder of textures (as the packs above), on and in order, and archive files in place of the
   game's. Those are the game's own bytes, so the picture's the same if they're read right: at the same size, bigger in a group, and
   one far before the rest that moves them all. What's broken in a zip, or not allowed, leaves the game as it was. */
async function mods(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const dir = path.join(OUT, 'mods'), folder = path.join(dir, 'mods');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const sha = name => fs.existsSync(path.join(dir, `${name}.png`)) ? crypto.createHash('sha256').update(fs.readFileSync(path.join(dir, `${name}.png`))).digest('hex') : 'none';
    const go = (name, more) => run(exe, ['--mode', '1', '--skip-intro', '--skip-loading', '--turbo', '--window', '480', '360', '--shot', '500', rel(`${name}.png`), '--frames', '520', ...more], path.join(dir, `${name}.txt`), { timeout: MODE_TIMEOUT });
    const fine = (name, r) => r.code === 0 && crash_where(exe, r.text) == null && fs.existsSync(path.join(dir, `${name}.png`));
    const put = (name, bytes) => fs.writeFileSync(path.join(folder, name), bytes);
    const runs = {};
    /* each a mod or two, from the folder: four at a time */
    const play = async list => {
        for (let i = 0; i < list.length; i += 4) {
            const part = list.slice(i, i + 4);
            const done = await Promise.all(part.map(([name, ids]) => go(name, ['--mods-dir', rel('mods'), ...ids.flatMap(id => ['--mod', id])])));
            part.forEach(([name], k) => { runs[name] = done[k]; });
        }
    };
    const results = [];
    const same = (what, name, as) => results.push([what, { pass: fine(name, runs[name]) && sha(name) === sha(as), what: `the same picture as ${as}` }]);

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(folder, { recursive: true });
    runs.base = await go('base', []);
    runs.dumped = await go('dumped', ['--dump-textures', rel('dump')]);
    const dumped_files = await run(exe, ['--dump-files', rel('files')], path.join(dir, 'files.txt'), { timeout: MODE_TIMEOUT });
    const names = fs.existsSync(path.join(dir, 'dump')) ? fs.readdirSync(path.join(dir, 'dump')).filter(n => /^[0-9a-f]{16}\.png$/.test(n)) : [];
    const own = id => fs.readFileSync(path.join(dir, 'files', `${id}.bin`));
    const files = fs.existsSync(path.join(dir, 'files')) ? fs.readdirSync(path.join(dir, 'files')) : [];
    results.push(['dump', { pass: fine('dumped', runs.dumped) && names.length > 20 && dumped_files.code === 0 && files.length > 900, what: `${names.length} textures and ${files.length} files saved` }]);
    if (names.length === 0 || files.length === 0) {
        return report(results);
    }

    /* textures: in a folder, in zips stored and deflated, and in a zip of a folder as Windows or a Mac makes it */
    const pink = names.map(n => [n, png_of(64, 32, [255, 0, 255, 255])]);
    fs.mkdirSync(path.join(folder, 'pink'));
    for (const [n, png] of pink) put(path.join('pink', n), png);
    put('pink.zip', zip_of(pink));
    put('pink-stored.zip', zip_of(pink, { stored: true }));
    put('Pink Pack.zip', zip_of([['Pink Pack/', Buffer.alloc(0)], ['Pink Pack/mod.json', Buffer.from('{ "name": "Pink \\"Things\\"", "tags": ["pink", { "a": [1, "}"] }], "version": 1, "author": "The tests", "description": "Every texture pink" }')],
        ...pink.map(([n, png]) => [`Pink Pack/textures/${n}`, png]), ['__MACOSX/Pink Pack/._mod.json', Buffer.from('junk')]]));
    put('green.zip', zip_of(names.map(n => [n, png_of(32, 32, [0, 255, 0, 255])])));
    put('idle.zip', zip_of([['0000000000000000.png', png_of(4, 4, [255, 0, 255, 255])]]));
    /* broken */
    const pink_zip = zip_of(pink);
    put('bad-crc.zip', zip_of(pink, { bad_crc: true }));
    put('encrypted.zip', zip_of(pink, { locked: true }));
    put('zip64.zip', zip_of(pink, { zip64: true }));
    put('truncated.zip', pink_zip.subarray(0, pink_zip.length >> 1));
    put('not-a-zip.zip', Buffer.from('hello'));
    put('empty.zip', zip_of([]));
    /* archive files, as the game's own: mode 1 reads 097 (its text), the group 252 to 25D, 07E and 0A6, and never 010 */
    const pad = (bytes, n) => Buffer.concat([bytes, Buffer.alloc(n)]);
    put('same.zip', zip_of(['097', '252', '258', '0A6'].map(id => [`files/${id}.bin`, own(id)])));
    put('bigger.zip', zip_of(['252', '258', '07E'].map(id => [`files/${id}.bin`, pad(own(id), 6000)])));
    put('moved.zip', zip_of([['files/010.bin', pad(own('010'), 100000)]]));
    put('text-too-big.zip', zip_of([['files/097.bin', pad(own('097'), 17000 - own('097').length)]]));
    put('code.zip', zip_of([['files/3DF.bin', Buffer.alloc(4096)]]));
    /* and one that isn't: the level's texture set (ASSETS.md) with its colours turned round, but for the clear one */
    const level = Buffer.from(own('252'));
    for (let k = 0, at = 0x20; k < level.readUInt16LE(0xA); k++) {
        const n = level.readUInt32LE(at);
        for (let j = 0; j < n; j++) {
            const c = level.readUInt16LE(at + 4 + j * 2);
            if (c & 0x7FFF) level.writeUInt16LE(c ^ 0x7FFF, at + 4 + j * 2);
        }
        at += 4 + n * 2;
    }
    put('changed.zip', zip_of([['files/252.bin', level]]));
    /* the same as patches: to the game's own file, one that makes a file bigger, one for another file, and one cut about */
    put('patch.zip', zip_of([['files/252.bps', bps_make(own('252'), level).patch]]));
    put('patch-bigger.zip', zip_of([['files/010.bps', bps_make(own('010'), pad(own('010'), 100000)).patch]]));
    put('patch-other.zip', zip_of([['files/252.bps', bps_make(level, own('252')).patch]]));
    const damaged = Buffer.from(bps_make(own('252'), level).patch);
    damaged[damaged.length >> 1] ^= 0x40;
    put('patch-damaged.zip', zip_of([['files/252.bps', damaged]]));

    await play([['idle', ['idle.zip']], ['folder', ['pink']], ['deflated', ['pink.zip']], ['stored', ['pink-stored.zip']], ['zipped-folder', ['Pink Pack.zip']],
        ['green', ['green.zip']], ['order', ['green.zip', 'pink.zip']], ['bad-crc', ['bad-crc.zip']], ['encrypted', ['encrypted.zip']],
        ['zip64', ['zip64.zip']], ['truncated', ['truncated.zip']], ['not-a-zip', ['not-a-zip.zip']], ['empty', ['empty.zip']],
        ['same-files', ['same.zip']], ['bigger-files', ['bigger.zip']], ['moved-files', ['moved.zip']], ['text-too-big', ['text-too-big.zip']], ['code', ['code.zip']],
        ['changed-file', ['changed.zip']], ['patch', ['patch.zip']], ['patch-bigger', ['patch-bigger.zip']], ['patch-other', ['patch-other.zip']],
        ['patch-damaged', ['patch-damaged.zip']]]);
    results.push(['texture folder', { pass: fine('folder', runs.folder) && sha('folder') !== sha('idle') && sha('idle') !== 'none', what: 'replaces' }]);
    same('deflated zip', 'deflated', 'folder');
    same('stored zip', 'stored', 'folder');
    same('a zipped folder', 'zipped-folder', 'folder');
    results.push(['mod.json', { pass: /mod Pink Pack\.zip \(Pink "Things" by The tests\): on, \d+ textures, 0 files\r?\n/.test(runs['zipped-folder'].text), what: 'read past what it doesn\'t know' }]);
    same('higher wins', 'order', 'green');
    results.push(['green differs', { pass: fine('green', runs.green) && sha('green') !== sha('folder'), what: 'from pink' }]);
    same('bad CRC', 'bad-crc', 'idle');
    same('encrypted', 'encrypted', 'idle');
    same('ZIP64', 'zip64', 'idle');
    same('cut short', 'truncated', 'base');
    same('not a zip', 'not-a-zip', 'base');
    same('empty zip', 'empty', 'base');
    same('files, same size', 'same-files', 'base');
    same('files, bigger in a group', 'bigger-files', 'base');
    same('a file that moves the rest', 'moved-files', 'base');
    results.push(['a changed file', { pass: fine('changed-file', runs['changed-file']) && sha('changed-file') !== sha('base'), what: 'changes the picture' }]);
    results.push(['files replaced', { pass: /mods: 4 of the archive's files replaced/.test(runs['same-files'].text) && /mods: 3 of/.test(runs['bigger-files'].text) && /mods: 1 of/.test(runs['moved-files'].text), what: 'as many as there are' }]);
    same('mode text too big', 'text-too-big', 'base');
    results.push(['too big, said', { pass: /TOO BIG/.test(runs['text-too-big'].text), what: 'in the log' }]);
    same('code', 'code', 'base');
    results.push(['code, said', { pass: /ITS CODE IS LEFT OUT/.test(runs.code.text), what: 'in the log' }]);
    same('a patch', 'patch', 'changed-file');
    same('a patch that makes a file bigger', 'patch-bigger', 'base');
    same('a patch for another file', 'patch-other', 'base');
    results.push(['another file, said', { pass: /FOR ANOTHER FILE/.test(runs['patch-other'].text), what: 'in the log' }]);
    same('a damaged patch', 'patch-damaged', 'base');
    results.push(['damaged, said', { pass: /DAMAGED/.test(runs['patch-damaged'].text), what: 'in the log' }]);
    return report(results);
}

const commands = { bosses, modes, adventure, challenges, same, hero, heroes, menu, shots, image, packs, mods };
if (commands[command] == null) {
    console.log('node port/tests/playtest.mjs bosses|modes|adventure|challenges|same|hero|heroes|menu|shots|image|packs|mods: see the top of this file');
    process.exit(2);
}
process.exit(await commands[command](rest));
