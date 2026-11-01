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
 *   node port/tests/playtest.mjs adventure [--seconds N]
 *       Plays a new Adventure with no cheats, by adventureplayer.c and bossbot.c: through every door the game's rules open, as far as it can
 *       in the time (30 minutes by default), and passes if it runs out of doors. Its log says how far it's got, as it goes.
 *   node port/tests/playtest.mjs menu
 *       From the title screen through the menus, by the pad alone, into the first game of a new Adventure's first room.
 *   node port/tests/playtest.mjs shots FROM STEP COUNT [crashbash options...]
 *       One run, a shot every STEP vblanks, on a contact sheet (Windows).
 *
 * For bosses, --no-cheats leaves NO HARM HERO off, --vblanks N lets it try for that long, and --shots FROM STEP COUNT takes those shots instead of eight spread over the run.
 *
 * --exe FILE picks another build. A crash's addresses become source lines if llvm-symbolizer is on the PATH. */
import { spawn, spawnSync } from 'node:child_process';
import fs from 'node:fs';
import path from 'node:path';
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
    bear: { mode: 22, warp: 33, vblanks: 8000 },
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
        results = await Promise.all(names.map((name, i) => play_boss(name, exe, false, i, cheats, shot_at, vblanks)));
    }
    return report(results);
}

async function modes(args) {
    const asan = flag(args, '--asan');
    const exe = option(args, '--exe', asan ? ASAN_EXE : DEFAULT_EXE);
    const vblanks = Number(option(args, '--vblanks', 900));
    const slots = Number(option(args, '--slots', 8));
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
    const dir = path.join(OUT, 'adventure');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    const r = await run(exe, ['--adventure', '--turbo', '--draw-every', '120', '--skip-intro', '--skip-loading', '--saves', rel('saves'), '--frames', '2000000000'],
        path.join(dir, 'log.txt'), { timeout: seconds });
    const crash = crash_where(exe, r.text);
    const progress = [...r.text.matchAll(/^adventure: vblank.*$/gm)].at(-1)?.[0] ?? 'nothing';
    const finished = /^adventure: no door left/m.test(r.text);

    return report([['adventure', { pass: crash == null && finished, what: crash ?? `${finished ? 'no door left' : 'still going at the time limit'}; ${progress}` }]]);
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
const commands = { bosses, modes, adventure, menu, shots };
if (commands[command] == null) {
    console.log('node port/tests/playtest.mjs bosses|modes|adventure|menu|shots: see the top of this file');
    process.exit(2);
}
process.exit(await commands[command](rest));
