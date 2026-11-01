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
 *   node port/tests/playtest.mjs same [--save] [--all]
 *   node port/tests/playtest.mjs image
 *   node port/tests/playtest.mjs packs
 *       Frames and sound of a few modes against a saved baseline (--save makes it), about a minute: whether a change that shouldn't alter
 *       anything didn't. Between big steps of cleaning up, in place of the long tests.
 *   node port/tests/playtest.mjs menu
 *       From the title screen through the menus, by the pad alone, into the first game of a new Adventure's first room.
 *   node port/tests/playtest.mjs shots FROM STEP COUNT [crashbash options...]
 *       One run, a shot every STEP vblanks, on a contact sheet (Windows).
 *
 * For bosses, --no-cheats leaves NO HARM HERO off, --vblanks N lets it try for that long, and --shots FROM STEP COUNT takes those shots instead of eight spread over the run.
 *
 * --exe FILE picks another build. A crash's addresses become source lines if llvm-symbolizer is on the PATH. */
import crypto from 'node:crypto';
import { spawn, spawnSync } from 'node:child_process';
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '../..');
const OUT = path.join(ROOT, 'build/port/playtest');
const SUFFIX = process.platform === 'win32' ? '.exe' : '';
const DEFAULT_EXE = `build/port/windows-clang/crashbash${SUFFIX}`;
const ASAN_EXE = `build/port/asan/crashbash${SUFFIX}`;
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

/* Runs the game; resolves with its exit code, and what it printed, also kept in logfile. */
function run(exe, args, logfile, { timeout = 0, env = {} } = {}) {
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
        child.on('exit', code => done(code));
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

async function play_boss(name, exe, watch, tile, cheats, shot_at, vblanks) {
    const boss = { ...BOSSES[name], vblanks: vblanks || BOSSES[name].vblanks };
    const dir = path.join(OUT, 'bosses', name);
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    const args = ['--cheat-menu', ...(cheats ? ['--cheats', '1'] : []), '--aim', '--skip-intro', '--skip-loading', '--saves', rel('saves'), ...route_to_boss(boss.mode)];
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
            results.push(await play_boss(name, exe, true, 0, cheats, shot_at, vblanks));
        }
    } else {
        for (let i = 0; i < names.length; i += AT_ONCE) {
            results.push(...await Promise.all(names.slice(i, i + AT_ONCE).map((name, k) => play_boss(name, exe, false, i + k, cheats, shot_at, vblanks))));
        }
    }
    return report(results);
}

async function modes(args) {
    const asan = flag(args, '--asan');
    const exe = option(args, '--exe', asan ? ASAN_EXE : DEFAULT_EXE);
    const vblanks = Number(option(args, '--vblanks', 900));
    const slots = Math.min(AT_ONCE, Number(option(args, '--slots', AT_ONCE)));
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
    const at_once = Math.min(AT_ONCE, Number(option(args, '--at-once', AT_ONCE)));
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
/* The most copies of the game at once: 37 together took down the whole PC. */
const AT_ONCE = 4;

const SAME_MODES = [0, 1, 4, 5, 10, 14, 18, 23, 27, 28, 30, 31, 33, 40];
const SAME_SHOTS = [500, 900, 1300];
const SAME_VBLANKS = 1400;
/* With --all, every mode too, a few at a time, and what's on screen outside the levels: the logos, the Adventure hub and the pause text. */
const SAME_EXTRAS = { intro: ['--frames', '1400'], hub: ['--adventure', '--frames', '1400', '--skip-intro', '--skip-loading'], pause: ['--mode', '1', '--press', '700', '8', '--frames', '1400', '--skip-intro', '--skip-loading'] };

async function same(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const save = flag(args, '--save');
    const all = flag(args, '--all');
    const dir = path.join(OUT, 'same');
    const base = path.join(dir, all ? 'baseline-all.json' : 'baseline.json');
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
            ...shots, '--wav', rel(`m${m}.wav`)], path.join(dir, `m${m}.txt`), { timeout: MODE_TIMEOUT });
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
    const list = all ? [...Array(41).keys(), ...Object.keys(SAME_EXTRAS)] : SAME_MODES;

    for (let i = 0; i < list.length; i += AT_ONCE) {
        results.push(...await Promise.all(list.slice(i, i + AT_ONCE).map((m, slot) => one(m, slot))));
    }

    if (save) {
        fs.writeFileSync(base, JSON.stringify(now, null, 1));
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

const [command, ...rest] = process.argv.slice(2);
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
    const run_image = async (name, img) => {
        const out = `${name}.dat`;
        const r = await run(exe, ['--image', img, '--data', rel(out), '--frames', '1', '--no-window'], path.join(dir, `${name}.txt`), { timeout: MODE_TIMEOUT });

        return { code: r.code, text: r.text, out: path.join(dir, out) };
    };
    for (const [name, img] of [['cue', 'build/CrashBash.cue'], ['bin', 'build/CrashBash.bin'], ['iso', rel('disc.iso')]]) {
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
    fs.copyFileSync(path.resolve(ROOT, exe), path.join(beside, 'crashbash.exe'));
    fs.linkSync(raw, path.join(beside, 'CrashBash.bin'));
    fs.copyFileSync(path.join(ROOT, 'build/CrashBash.cue'), path.join(beside, 'CrashBash.cue'));
    const out = path.join(dir, 'beside.dat');
    const r = await run(path.join(beside, 'crashbash.exe'), ['--data', out, '--frames', '1', '--no-window'], path.join(dir, 'beside.txt'), { timeout: MODE_TIMEOUT });
    const found = fs.existsSync(out) && sha(out) === want;

    results.push(['beside', { pass: r.code === 0 && found, what: found ? 'found and the same as the disc\'s' : `exit ${r.code}: ${r.text.trim().split('\n').at(-1)}` }]);
    fs.rmSync(dir, { recursive: true, force: true });
    return report(results);
}

/* A PNG, for a pack: solid colour, by alpha or not (colour type 6 or 2). */
function png_of(w, h, rgba, colour_type = 6) {
    const crc = b => {
        let c = 0xFFFFFFFF;
        for (const x of b) {
            c ^= x;
            for (let k = 0; k < 8; k++) c = c & 1 ? (c >>> 1) ^ 0xEDB88320 : c >>> 1;
        }
        return (c ^ 0xFFFFFFFF) >>> 0;
    };
    const chunk = (type, data) => {
        const t = Buffer.concat([Buffer.from(type, 'latin1'), data]);
        const len = Buffer.alloc(4), sum = Buffer.alloc(4);
        len.writeUInt32BE(data.length);
        sum.writeUInt32BE(crc(t));
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

const commands = { bosses, modes, adventure, challenges, same, menu, shots, image, packs };
if (commands[command] == null) {
    console.log('node port/tests/playtest.mjs bosses|modes|adventure|challenges|same|menu|shots|image|packs: see the top of this file');
    process.exit(2);
}
process.exit(await commands[command](rest));
