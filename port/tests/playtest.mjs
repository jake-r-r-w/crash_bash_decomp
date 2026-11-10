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
 *   node port/tests/playtest.mjs controls
 *       The launcher's CONTROLS page, with pretend keys and a pretend controller: new keys and buttons, swaps, Esc and the reset. And a
 *       pretend mouse through the menus: rows pointed at and clicked, right clicks back, the wheel.
 *   node port/tests/playtest.mjs pads
 *       Couch play, with pretend controllers: the launcher's players screen, Battle, Adventure and Tournament offering as many players as
 *       there are controllers, each choosing its own hero, and controllers pulled out and plugged in without anyone else's player changing.
 *       And the stick's deadzone: a stick a third of the way out moves the hero with a deadzone of 20%, and not with the game's own.
 *   node port/tests/playtest.mjs identical [--all] [--against FILE]
 *       Whether the game repeats itself exactly, for online play: the same presses from two pretend controllers under each picture
 *       setting, flat out and in real time, with sound and without, compared by the game's fingerprint after every step. About two
 *       minutes. --all adds every game mode under every picture setting at once, a few minutes more. --against FILE checks this
 *       system's fingerprints against another's (its fingerprints-SYSTEM.json from a run there).
 *   node port/tests/playtest.mjs online
 *       Online play, with every machine a copy of the game on this PC: two and four of them through a Battle and back to the title,
 *       flat out and in real time, with packets late and lost, a machine that vanishes and one that stalls, all compared by the
 *       game's fingerprint after every step; and through the launcher's ONLINE screens, hosting, finding and joining. About two
 *       minutes.
 *   node port/tests/playtest.mjs online --android
 *       An online game between this PC, hosting, and Android's emulator, joining it: the Battle above, matched step for step.
 *   node port/tests/playtest.mjs online --play [2|3|4] [--demo] [--delay STEPS] [--lag MS] [--loss N]
 *       Online play on this PC to try yourself: copies of the game side by side, each at its launcher, to host in one and join in the
 *       others. The first has the keyboard and each other a controller. --delay, --lag and --loss are for feeling what a game over
 *       the internet is like. --demo hosts in one and joins by its code in the other by itself, to watch, with a pretend router.
 *   node port/tests/playtest.mjs fuzz [--seconds N]
 *       Online play's packet fuzzer (tests/netfuzz.c), for a change to platform/net.c: packets of every kind, and of none, at a host and
 *       a joiner in each of their states, with ASan and UBSan watching, for 2 minutes by default. It's built apart, first:
 *       cmake --build --preset windows-clang --target netfuzz (Windows: the toolchain's libFuzzer). It keeps what it's learnt in
 *       build/port/playtest/fuzz/corpus, so each run carries on from the last, and plays again whatever an earlier run found.
 *   node port/tests/playtest.mjs shots FROM STEP COUNT [crashbash options...]
 *       One run, a shot every STEP vblanks, on a contact sheet (Windows).
 *
 * For bosses, --no-cheats leaves NO HARM HERO off, --vblanks N lets it try for that long, --shots FROM STEP COUNT takes those shots instead of eight spread over the run,
 * and --hero ID plays it as that extra hero (roo: --extra-hero 0 ID, docs/EXTRA_HEROES.md).
 *
 * --exe FILE picks another build (by default windows-clang's, or linux-clang's on Linux). A crash's addresses become source lines if
 * llvm-symbolizer is on the PATH (Windows). On Linux they run in the port's container, on its virtual screen (docs/PORT.md, Linux):
 *   tools/dl sh -c 'xvfb-run -a -s "-screen 0 1920x1080x24" node port/tests/playtest.mjs modes'
 * (through sh: xvfb-run run straight by tools/dl can wait for ever on its screen, 2026-11-10). */
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
    /* the launch splits its arguments at commas: a macro's steps go with semicolons (main.c, read_macro) */
    const sent = args.map(a => String(a).replaceAll(',', ';'));

    for (let i = 0; i < sent.length; i++) {
        const at = sent[i] === '--shot' ? i + 2 : sent[i] === '--wav' ? i + 1 : -1;

        if (at > 0 && at < sent.length) {
            /* in the app's own folder: it can't write in one adb made */
            const device = `${ANDROID_DIR}/out-${path.basename(sent[at])}`;

            fetch.push([device, path.resolve(ROOT, sent[at])]);
            sent[at] = device;
            i = at;
        }
    }
    adb(['shell', 'am', 'force-stop', ANDROID_APP]);
    adb(['shell', 'rm', '-rf', `${ANDROID_DIR}/out`, `${ANDROID_DIR}/out-*`, `${ANDROID_DIR}/log.txt`]);
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

/* Runs the game; resolves with its exit code (or the signal that ended it, on Linux), and what it printed, also kept in logfile. here: on
   this machine even with --android. What's returned can be stopped early (.stop). */
function run(exe, args, logfile, { timeout = 0, env = {}, here = false } = {}) {
    let child = null;

    if (ANDROID && !here) {
        const p = run_android(args, logfile, timeout);

        p.stop = () => adb(['shell', 'am', 'force-stop', ANDROID_APP]);
        return p;
    }
    const p = new Promise(resolve => {
        const log = fs.openSync(logfile, 'w');

        child = spawn(path.resolve(ROOT, exe), args, { cwd: ROOT, stdio: ['ignore', log, log], env: { ...process.env, ...env } });
        const timer = timeout ? setTimeout(() => child.kill(), timeout * 1000) : null;
        const done = code => {
            clearTimeout(timer);
            fs.closeSync(log);
            resolve({ code, text: fs.readFileSync(logfile, 'latin1') });
        };

        child.on('error', () => done(-1));
        child.on('exit', (code, signal) => done(code ?? signal));
    });

    p.stop = () => child?.kill();
    return p;
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

/* Couch play, with pretend controllers (--pretend-pads: SDL's virtual gamepads, plugged in, pressed and pulled out as real ones are):
   the launcher's players screen after PLAY with two or more, the game's own players menus offering as many players as there are
   controllers, as the PlayStation's did, each controller choosing for its own player, and one pulled out and put back. From the log:
   the controllers' players (sdl.c's "pads:" lines) and, as each game starts, who's playing (main.c's "players" line). */
async function pads(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const dir = path.join(OUT, 'pads');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const UP = '10', TRIANGLE = '1000';
    /* every controller's Cross on the menus' beat: player 1's by --press, the others by --pad-press */
    const crosses = (count, at) => at.flatMap(v => ['--press', String(v), CROSS, ...Array.from({ length: count - 1 }, (_, p) => ['--pad-press', String(p + 2), String(v), CROSS]).flat()]);
    const downs = (n, from) => Array.from({ length: n }, (_, i) => ['--press', String(from + i * 20), DOWN]).flat();
    const BEAT = [470, 590, 850, 1150, 1250, 1550, 1650, 1750];
    /* the main menu's Battle, its players menu set to as many as asked, then the character select and on into the first game */
    const battle = (count, ask) => [...downs(1, 170), '--press', '230', CROSS, ...downs(ask - 1, 280), '--press', '350', CROSS, ...crosses(count, BEAT)];
    /* who's playing as the first game starts, by main.c's line */
    const playing = text => {
        const m = [...text.matchAll(/^mode (\d+) at vblank \d+\r?\nplayers (\d+), computer (\d) (\d) (\d) (\d)/gm)].find(x => x[1] !== '40');
        return m == null ? null : { players: Number(m[2]), computer: m.slice(3, 7).join('') };
    };
    const lines = (text, re) => text.split(/\r?\n/).filter(l => re.test(l));
    const cases = [
        {
            name: 'one controller: PLAY goes straight to the game',
            args: ['--launcher', '--pretend-pads', '1', '--press', '120', CROSS, '--frames', '260'],
            check: t => /^launcher: play/m.test(t) && !/players screen/.test(t) ? null : 'a players screen, or no game',
        },
        {
            name: 'two: the players screen, back, again, and pad 2 to player 1',
            args: ['--launcher', '--pretend-pads', '2', '--press', '120', CROSS, '--pad-press', '1', '160', TRIANGLE, '--press', '200', CROSS,
                   '--pad-press', '2', '240', UP, '--press', '290', START, '--frames', '400'],
            check: t => lines(t, /players screen/).length !== 2 ? `the players screen ${lines(t, /players screen/).length} times, not 2`
                : !/^pads: Pretend pad 2 is now player 1/m.test(t) || !/^pads: Pretend pad 1 is now player 2/m.test(t) ? 'pad 2 never moved up'
                : !/^launcher: play/m.test(t) ? 'START never played' : null,
        },
        {
            name: 'Battle, one controller: only one player, as on the PlayStation',
            args: ['--pretend-pads', '1', ...battle(1, 2), '--frames', '1500'],
            check: t => (p => p == null ? 'no game' : p.players !== 1 || p.computer !== '0111' ? `players ${p.players}, computer ${p.computer}` : null)(playing(t)),
        },
        {
            name: 'Battle, two controllers: two players',
            args: ['--pretend-pads', '2', ...battle(2, 2), '--frames', '1500'],
            check: t => (p => p == null ? 'no game' : p.players !== 2 || p.computer !== '0011' ? `players ${p.players}, computer ${p.computer}` : null)(playing(t)),
        },
        {
            name: 'Battle, four controllers: four players, through the multitap',
            args: ['--pretend-pads', '4', ...battle(4, 4), '--frames', '1500'],
            check: t => (p => p == null ? 'no game' : p.players !== 4 || p.computer !== '0000' ? `players ${p.players}, computer ${p.computer}` : null)(playing(t)),
        },
        {
            name: 'Adventure, two controllers: two players',
            args: ['--pretend-pads', '2', '--press', '230', CROSS, '--press', '350', CROSS, ...downs(1, 440), ...crosses(2, [470, 590, 710, 830, 950, 1070]), '--frames', '1300'],
            check: t => (p => p == null ? 'no game' : p.players !== 2 || p.computer !== '0011' ? `players ${p.players}, computer ${p.computer}` : null)(playing(t)),
        },
        {
            name: 'Tournament, two controllers: two players',
            args: ['--pretend-pads', '2', ...downs(2, 160), '--press', '230', CROSS, ...downs(1, 320), ...crosses(2, [350, 470, 590, 710, 830, 950, 1070]), '--frames', '1300'],
            check: t => (p => p == null ? 'no game' : p.players !== 2 || p.computer !== '0011' ? `players ${p.players}, computer ${p.computer}` : null)(playing(t)),
        },
        {
            /* in the game pad 1 comes out and a new one, pad 3, goes in, taking the empty player 1; pad 1 goes back in as player 3 */
            name: 'pulled out and put back: nobody else changes player',
            args: ['--pretend-pads', '2', ...battle(2, 2), '--pad-unplug', '1', '1350', '--pad-plug', '3', '1400', '--pad-plug', '1', '1450', '--frames', '1500'],
            check: t => {
                const after = lines(t, /^pads: .* at vblank (1[3-4]\d\d)$/);
                const want = ['Pretend pad 1 is out: no controller for player 1', 'Pretend pad 3 is player 1', 'Pretend pad 1 is player 3'];
                const got = after.map(l => l.replace(/^pads: /, '').replace(/ at vblank \d+$/, ''));
                return got.join('|') === want.join('|') && !/^pads: Pretend pad 2 is (now )?player [^2]/m.test(t) ? null : `pads: ${got.join(', ') || 'nothing'}`;
            },
        },
        {
            name: 'pulled out and plugged in again: its own player',
            args: ['--pretend-pads', '2', ...battle(2, 2), '--pad-unplug', '2', '1350', '--pad-plug', '2', '1400', '--frames', '1450'],
            check: t => {
                const got = lines(t, /^pads: .* at vblank 1[3-4]\d\d$/).map(l => l.replace(/^pads: /, '').replace(/ at vblank \d+$/, ''));
                return got.join('|') === 'Pretend pad 2 is out: no controller for player 2|Pretend pad 2 is player 2' ? null : `pads: ${got.join(', ') || 'nothing'}`;
            },
        },
    ];

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    const slots = Math.min(AT_ONCE, cases.length);
    const one = async (c, i) => {
        const saves = ANDROID ? [] : ['--saves', rel(`saves${i}`)];
        const r = await run(exe, ['--turbo', '--skip-intro', ...saves, '--tile', String(i % slots), String(slots), ...c.args],
                            path.join(dir, `case${i}.txt`), { timeout: MODE_TIMEOUT });
        const crash = crash_where(exe, r.text);
        const wrong = crash ?? (r.code !== 0 ? `exit ${r.code}` : c.check(r.text));

        return [c.name, { pass: wrong == null, what: wrong ?? `case${i}.txt` }];
    };
    const results = [];
    for (let i = 0; i < cases.length; i += slots) {
        results.push(...await Promise.all(cases.slice(i, i + slots).map((c, j) => one(c, i + j))));
    }
    /* The stick's deadzone (controls.c, controls_stick): the same Battle four times, its one player's stick held a third of the way to
       the right once the rules are read and it's playing, or not at all. The computer players do the same each time, so the heroes and the objects
       are the same as with no stick unless that player's hero moved: a third out it stands still with the game's own deadzone, and
       moves with 20%; nearly all the way out it moves with the game's own. */
    if (!ANDROID) {
        const stick = (name, extra) => run(exe, ['--turbo', '--skip-intro', '--saves', rel(`stick-${name}`), '--tile', String(results.length % 4), '4',
            '--pretend-pads', '1', ...battle(1, 2), '--press', '2000', CROSS, '--fingerprints', '1', ...extra, '--frames', '3000'], path.join(dir, `stick-${name}.txt`), { timeout: MODE_TIMEOUT });
        const moved = (a, b) => {
            const parts = l => Object.fromEntries(l.split(' ').slice(2).reduce((p, w, i, all) => (i % 2 === 0 ? [...p, [w, all[i + 1]]] : p), []));
            const pa = fingerprints(a.text), pb = fingerprints(b.text);

            for (let i = 0; i < Math.min(pa.length, pb.length); i++) {
                const x = parts(pa[i]), y = parts(pb[i]);

                if (x.heroes !== y.heroes || x.objects !== y.objects) {
                    return i + 1;
                }
            }
            return null;
        };
        const [none, third, third20, most] = await Promise.all([stick('none', []), stick('third', ['--pad-stick', '1', '2050', '35', '0']),
            stick('third-20', ['--pad-stick', '1', '2050', '35', '0', '--stick-deadzone', '20']), stick('most', ['--pad-stick', '1', '2050', '90', '0'])]);
        const runs = [none, third, third20, most];
        const wrong = runs.map(r => crash_where(exe, r.text) ?? (r.code !== 0 ? `exit ${r.code}` : null)).find(x => x != null)
            ?? (fingerprints(none.text).length < 300 ? `the game's short: ${fingerprints(none.text).length} steps`
            : moved(none, third) != null ? `a third out with the game's own deadzone, the hero moved, from step ${moved(none, third)}`
            : moved(none, third20) == null ? 'a third out with a 20% deadzone, the hero never moved'
            : moved(none, most) == null ? 'nearly all the way out with the game\'s own deadzone, the hero never moved' : null);

        results.push(['the stick\'s deadzone: a third out moves the hero at 20%, not at the game\'s own', { pass: wrong == null,
            what: wrong ?? `moved from step ${moved(none, third20)} at 20%, and from ${moved(none, most)} nearly all the way out` }]);
    }
    return report(results);
}

/* Whether the game repeats itself exactly, which online play rests on (docs/PORT.md, Online play): every machine runs the whole game
   from everyone's presses, so the same presses have to give the same game whatever the machine's own settings. Each run here has the
   same presses, from two pretend controllers, through the menus into a Battle and on through it, and prints the game's fingerprint
   after every step (--fingerprints, game/main/fingerprint.c), with its sound mixed by the game's clock (--game-clock-sound), as
   online play's is. Under each picture setting, flat out and in real time, with the sound played and not, the fingerprints have to
   match the first run's step for step. A run with one press more has to differ, or the fingerprint isn't looking. Then what's known
   to change the game, which online has to be the same for everyone: the settings that are the host's, a wait in the launcher, and
   sound mixed by the sound device. What each does is said, and none of those can fail.

   --all adds every game mode, played by the computer: flat out as it comes, against every picture setting at once in real time.

   Each game's last fingerprint is kept in fingerprints-SYSTEM.json, and --against FILE checks them against another system's. */
const IDENTICAL_VBLANKS = 4000;

function identical_presses({ shift = 0, one_more = false } = {}) {
    const UP = 0x10, RIGHT = 0x20, LEFT = 0x80, SQUARE = 0x8000, CIRCLE = 0x2000;
    const down = parseInt(DOWN, 16), cross = parseInt(CROSS, 16);
    const args = [];
    const pad1 = (v, b) => args.push('--press', String(v + shift), b.toString(16));
    const pad2 = (v, b) => args.push('--pad-press', '2', String(v + shift), b.toString(16));
    let seed = 12345;
    const next = n => {
        seed = (Math.imul(seed, 1103515245) + 12345) >>> 0;
        return (seed >>> 16) % n;
    };

    /* the menus: Battle, two players, each controller's Cross through the character select and on, and past the first game's rules */
    pad1(170, down);
    pad1(230, cross);
    pad1(280, down);
    pad1(350, cross);
    for (const v of [470, 590, 850, 1150, 1250, 1550, 1650, 1750, 2000]) {
        pad1(v, cross);
        pad2(v, cross);
    }
    /* the game: each controller a direction held for a while and a button now and then, the same ones every time */
    for (const press of [pad1, pad2]) {
        let dir = 0, left = 0;

        for (let v = 2100; v < IDENTICAL_VBLANKS - 12; v += 6) {
            if (left-- <= 0) {
                dir = [UP, RIGHT, down, LEFT, UP | RIGHT, down | LEFT, 0][next(7)];
                left = 2 + next(8);
            }
            const button = [0, 0, 0, SQUARE, cross, CIRCLE][next(6)];

            if ((dir | button) !== 0) {
                press(v, dir | button);
            }
        }
    }
    if (one_more) {
        pad2(3003, SQUARE | UP);
    }
    return args;
}

/* A run's fingerprint lines, from its last start (the launcher's PLAY starts them again) */
function fingerprints(text) {
    const lines = text.split(/\r?\n/);
    const from = lines.lastIndexOf('fp start');

    return lines.slice(from + 1).filter(l => l.startsWith('fp '));
}

/* Where two runs' fingerprints part, and in what: null if they never do */
function fingerprints_part(a, b) {
    const n = Math.min(a.length, b.length);

    for (let i = 0; i < n; i++) {
        if (a[i] !== b[i]) {
            const wa = a[i].split(' '), wb = b[i].split(' ');
            const parts = [];

            for (let k = 4; k + 1 < wa.length; k += 2) {
                if (wa[k + 1] !== wb[k + 1]) {
                    parts.push(wa[k]);
                }
            }
            return `step ${wa[1]}, in ${parts.join(', ') || 'an earlier step'}`;
        }
    }
    return a.length !== b.length || n === 0 ? `${a.length} steps against ${b.length}` : null;
}

async function identical(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const against = option(args, '--against', null);
    const all = flag(args, '--all');
    const dir = path.join(OUT, 'identical');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const presses = identical_presses();
    /* PLAY at that vblank and START on the players screen, which starts the game again from its beginning (--restart-on-play), as an
       online game's START does; then the same presses, for that start (--then) */
    const launcher = play => ['--launcher', '--restart-on-play', '--press', String(play), CROSS, '--press', String(play + 170), START, '--then', ...presses];
    const cases = [
        { name: 'flat out', args: ['--turbo', ...presses] },
        { name: 'flat out, again', args: ['--turbo', ...presses] },
        { name: 'in real time, its sound played', args: ['--sound', ...presses] },
        { name: '60 FPS, its sound played', args: ['--fps', '60', '--sound', ...presses] },
        { name: '120 FPS', args: ['--fps', '120', ...presses] },
        { name: 'widescreen', args: ['--widescreen', '--turbo', ...presses] },
        { name: 'textures made bigger and smoothed', args: ['--upscale', '2', '--smooth', '--turbo', ...presses] },
        { name: 'anti-aliased', args: ['--antialias', '4', '--turbo', ...presses] },
        { name: 'sharp icons', args: ['--sharp-icons', '--turbo', ...presses] },
        { name: 'drawn one frame in 60', args: ['--draw-every', '60', '--turbo', ...presses] },
        { name: 'every picture setting at once, in real time', args: ['--fps', '120', '--widescreen', '--upscale', '4', '--smooth', '--antialias', '8', '--sharp-icons', '--sound', ...presses] },
        { name: 'one press more', args: ['--turbo', ...identical_presses({ one_more: true })], want: 'differs' },
        /* the launcher's part of the game's own menus, so its steps count, and a game played on from it is a different game the longer
           it was open (its random numbers have moved on). So an online game's START starts the game again from its very beginning,
           in the same window, which has to be the game a fresh start gives, however long the launcher was open (docs/PORT.md) */
        { name: 'started again from the launcher', args: ['--turbo', ...launcher(120)] },
        { name: 'started again from the launcher, after a wait', args: ['--turbo', ...launcher(333)] },
        /* what changes the game: the host's, online */
        { name: 'the cheat menu on', args: ['--turbo', '--cheat-menu', ...presses], want: 'said' },
        { name: 'loading skipped', args: ['--turbo', '--skip-loading', ...presses], want: 'said' },
        { name: 'extra heroes on', args: ['--turbo', '--extra-heroes', ...presses], want: 'said' },
        /* and why the sound's mixed by the game's clock */
        { name: 'sound mixed by the sound device, as offline', args: ['--sound', ...presses], device_sound: true, want: 'said' },
    ];
    const results = [];
    const prints = [];

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    const slots = Math.min(AT_ONCE, cases.length);
    const one = async (c, i) => {
        const saves = ANDROID ? [] : ['--saves', rel(`saves${i}`)];
        const last = Math.max(...presses.map((a, k) => (presses[k - 1] === '--press' ? Number(a) : 0)));
        const r = await run(exe, ['--skip-intro', '--pretend-pads', '2', '--fingerprints', '1', ...(c.device_sound ? [] : ['--game-clock-sound']), ...saves,
            '--tile', String(i % slots), String(slots), ...c.args, '--frames', String(last + 30)], path.join(dir, `case${i}.txt`), { timeout: MODE_TIMEOUT });

        prints[i] = fingerprints(r.text);
        return crash_where(exe, r.text) ?? (r.code !== 0 ? `exit ${r.code}` : !/^players 2, computer 0 0 1 1/m.test(r.text) ? 'never reached the two-player game' : null);
    };
    const wrong = [];
    for (let i = 0; i < cases.length; i += slots) {
        wrong.push(...await Promise.all(cases.slice(i, i + slots).map((c, j) => one(c, i + j))));
    }
    cases.forEach((c, i) => {
        const first = 0;
        const parted = fingerprints_part(prints[first], prints[i]);

        if (wrong[i] != null) {
            results.push([c.name, { pass: false, what: wrong[i] }]);
        } else if (c.want === 'said') {
            results.push([c.name, { pass: true, what: parted == null ? 'no different, in this game' : `a different game from ${parted}` }]);
        } else if (c.want === 'differs') {
            results.push([c.name, { pass: parted != null, what: parted == null ? 'no different: the fingerprint missed it' : `a different game from ${parted}` }]);
        } else {
            results.push([c.name, { pass: parted == null, what: parted == null ? `the same, ${prints[i].length} steps` : `differs from ${parted} (case${first}.txt, case${i}.txt)` }]);
        }
    });
    /* what another system's run is checked against: each game's steps and the last one's fingerprint of them all */
    const mine = { 'the Battle': { steps: prints[0].length, all: prints[0].at(-1)?.split(' ')[3] } };

    if (all) {
        /* every mode, played by the computer: flat out as it comes, against every picture setting at once in real time with its sound */
        const modes = [...Array(41).keys()];
        const flat = [], varied = [], bad = [];
        const mode = async (m, slot, into, more) => {
            const r = await run(exe, ['--mode', String(m), '--skip-intro', '--skip-loading', '--fingerprints', '1', '--game-clock-sound', '--tile', String(slot),
                String(AT_ONCE), ...more, '--frames', '1400'], path.join(dir, `mode${m}${into === flat ? '' : '-varied'}.txt`), { timeout: MODE_TIMEOUT });

            into[m] = fingerprints(r.text);
            bad[m] ??= crash_where(exe, r.text) ?? (r.code !== 0 ? `exit ${r.code}` : null);
        };

        for (let i = 0; i < modes.length; i += AT_ONCE) {
            await Promise.all(modes.slice(i, i + AT_ONCE).map((m, slot) => mode(m, slot, flat, ['--turbo'])));
        }
        for (let i = 0; i < modes.length; i += AT_ONCE) {
            await Promise.all(modes.slice(i, i + AT_ONCE).map((m, slot) => mode(m, slot, varied,
                ['--fps', '120', '--widescreen', '--upscale', '4', '--smooth', '--antialias', '8', '--sharp-icons', '--sound'])));
        }
        for (const m of modes) {
            const parted = bad[m] ?? fingerprints_part(flat[m], varied[m]);

            mine[`mode ${m}`] = { steps: flat[m].length, all: flat[m].at(-1)?.split(' ')[3] };
            results.push([`mode ${m}, every picture setting`, { pass: parted == null, what: parted == null ? `the same, ${flat[m].length} steps`
                : bad[m] ?? `differs from ${parted} (mode${m}.txt, mode${m}-varied.txt)` }]);
        }
    }
    fs.writeFileSync(path.join(dir, `fingerprints-${ANDROID ? 'android' : process.platform}.json`), JSON.stringify(mine, null, 1));
    if (against != null) {
        const theirs = JSON.parse(fs.readFileSync(against, 'utf8'));
        const both = Object.keys(mine).filter(k => theirs[k] != null);
        const differ = both.filter(k => theirs[k].steps !== mine[k].steps || theirs[k].all !== mine[k].all);

        results.push([`against ${path.basename(against)}`, { pass: both.length !== 0 && differ.length === 0, what: both.length === 0 ? 'nothing of the same in it'
            : differ.length === 0 ? `the same: ${both.length === 1 ? both[0] : `${both.length} games`}` : `differs: ${differ.join(', ')}` }]);
    }
    return report(results);
}

/* Online play (docs/PORT.md, Online play), with every machine a copy of the game on this PC: a host and the others joining it at
   127.0.0.1 (--host, --join), each with one pretend controller, so each is a player. The host's player goes through the menus into a
   Battle for as many players as there are machines; everyone picks a hero and plays, by a pattern of presses each (--macro), through
   a round, its result and into the next; then the host pauses and quits, and they're all back at the title. Every machine prints the
   game's fingerprint after every step, and they have to match step for step: flat out, with packets late and lost (--net-lag,
   --net-loss), with two machines and with four. The host of one keeps the game's presses (--net-record), and playing that again alone
   (--replay) has to give the same game once more. A machine made to disagree (--net-differ) has to be caught, and both stop.

   Then in real time, as it's played: late and lost packets again, a machine that vanishes without a word (the host plays on without
   its player after a wait), and one that stalls, as a window being dragged does, and is back.

   And through the launcher's ONLINE screens, as players start it: one copy hosts (HOST GAME), another finds its game and joins it
   (JOIN GAME), or types its address, and the host's START starts the game again from its beginning on both, where the same Battle
   is played. The main menu's EXIT, and the host leaving, each bring a machine back to its launcher. Those copies' games are this
   PC's alone (--online-local), each on a port of its own (--online-port). */
const ONLINE_END = 7700;
const ONLINE_MACROS = ['10:40,4010:6,20:50,8020:6,40:30,2040:6,80:60,4000:6,0:20', '80:35,4080:6,10:45,8010:6,20:55,4000:6,40:25,2040:6,0:14',
    '40:50,8040:6,80:30,4000:6,10:40,2010:6,20:44,4020:6,0:10', '20:30,4020:6,40:56,8040:6,10:36,4000:6,80:48,2080:6,0:26'];

function online_presses(machine, machines) {
    const press = (v, b) => ['--press', String(v), b];
    const LEFT = '80';
    const args = [];

    if (machine === 0) {
        /* Battle, and its players menu: it's for everyone in the game and nobody fewer, so up, for one player less, does nothing */
        args.push(...press(170, DOWN), ...press(230, CROSS), ...press(290, '10'), ...press(350, CROSS));
    }
    for (const v of [470, 590, 850, 1150, 1250, 1550, 1650, 1750, 2000]) {
        args.push(...press(v, CROSS));
    }
    args.push('--macro', '2100', ONLINE_MACROS[machine], '--macro-until', '6900');
    if (machine === 0) {
        /* paused, down to QUIT GAME, and YES */
        args.push(...press(7000, START), ...[7060, 7090, 7120, 7150].flatMap(v => press(v, DOWN)), ...press(7200, CROSS), ...press(7250, LEFT), ...press(7300, CROSS));
    }
    return args;
}

/* online --play [2|3|4] [--delay STEPS] [--lag MS] [--loss N]: online play on this PC to try yourself, with your own settings. Copies
   of the game in windows side by side, each at its launcher: choose ONLINE and HOST GAME in one, and ONLINE and JOIN GAME in the
   others, where the game hosted shows by itself, then START in the host's. The first copy has the keyboard and no controller, and is
   the one with sound; each other copy has a controller, the first, second and third found, read even while its window isn't the one
   in front, and the keyboard too once its window's clicked. So with the first window in front, the keyboard is one player and a
   controller another, both at once. The games are this PC's alone (--online-local), so Windows doesn't ask about its firewall.
   --delay sets how many steps late a press is used, and --lag and --loss make every packet late and lose some, to feel a game over
   the internet. The host keeps the game's presses in game.txt, to play again (--replay). It ends when the windows are closed, and
   says what each copy said of the game.

   --demo shows a join code at work, with nobody pressing anything for the first half minute: the left copy hosts, and its router
   "opens a port" (a pretend router: none is asked), so it shows a code; the right copy goes to JOIN GAME, down to CODE, types the
   code a letter at a time and joins; the host presses START, and the game begins, yours to play from there. */
async function online_play(args, exe) {
    const delay = option(args, '--delay', null), lag = option(args, '--lag', null), loss = option(args, '--loss', null);
    const demo = flag(args, '--demo');
    const copies = demo ? 2 : Math.min(4, Math.max(2, Number(args.find(a => /^\d$/.test(a)) ?? 2)));
    const dir = path.join(OUT, 'online-play');
    const press = (v, b) => ['--press', String(v), b];
    /* the demo's presses, by vblank (60 a second), slow enough to follow */
    const code = demo ? spawnSync(path.resolve(ROOT, exe), ['--make-code', '127.0.0.1', '0'], { encoding: 'utf8' }).stdout.trim() : '';
    const shown = [
        ['--online-pretend-router', 'open', ...press(240, DOWN), ...press(300, CROSS), ...press(380, CROSS), ...press(1500, START)],
        [...press(420, DOWN), ...press(480, CROSS), ...press(540, DOWN), ...press(600, CROSS), ...press(720, DOWN), ...press(780, CROSS),
            ...[...code.replace('-', '')].flatMap((key, i) => ['--key-press', String(860 + i * 45), key]), ...press(1300, START)],
    ];
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    if (demo) {
        console.log(`A join code at work, by itself, for about 25 seconds: hands off till the game starts.`);
        console.log(`  left:  ONLINE, HOST GAME, and it shows the code ${code}`);
        console.log('  right: ONLINE, JOIN GAME, down to CODE, types it, and Start');
        console.log('  left:  START, and the game begins on both, yours to play. Close the windows to end it.');
    } else {
        console.log(`${copies} copies of the game, side by side. In the left one choose ONLINE, then HOST GAME; in ${copies === 2 ? 'the other' : 'each other'}, ONLINE, then JOIN GAME and the game it finds; then START in the left one.`);
        console.log('The left copy has the keyboard; the others a controller each, or the keyboard when clicked. Close the windows to end it.');
    }
    const runs = await Promise.all(Array.from({ length: copies }, (_, m) => run(exe, ['--tile', String(m), '4', '--online-local',
        ...(m === 0 ? ['--controllers', 'none', '--net-record', rel('game.txt'), ...(delay != null ? ['--net-delay', delay] : [])] : ['--controllers', String(m), '--no-sound']),
        ...(lag != null ? ['--net-lag', lag] : []), ...(loss != null ? ['--net-loss', loss] : []), ...(demo ? shown[m] : [])], path.join(dir, `copy${m + 1}.txt`))));

    runs.forEach((r, m) => {
        console.log(`--- copy ${m + 1} (exit ${r.code})`);
        console.log(r.text.split(/\r?\n/).filter(l => /^(online|pads|crash|stack):|^Online play/.test(l)).join('\n'));
    });
    return 0;
}

/* online --android: a game between this PC, hosting, and Android's emulator, joining it at 10.0.2.2 (the emulator's way to the PC): the
   online test's two-machine Battle, matched step for step by fingerprint. The PC's copy hosts on 127.0.0.1, so Windows doesn't ask about
   its firewall, and is stopped if the phone hasn't joined in a minute and a half, as a host's window is black till someone does. The
   phone plays on past its own --frames: catching up with the host, it plays steps without waiting for vblanks (net_behind), and the
   emulator is slow enough to be catching up most of the time. So its game ends with the host's last step, as "the host has left". */
async function online_android(args) {
    const pc = option(args, '--pc-exe', WINDOWS ? 'build/port/windows-clang/crashbash.exe' : 'build/port/linux-clang/crashbash');
    const dir = path.join(OUT, 'online');
    const port = '47310';
    const common = ['--skip-intro', '--pretend-pads', '1', '--fingerprints', '1', '--turbo', '--draw-every', '60'];

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(path.join(dir, 'saves'), { recursive: true });
    const host = run(pc, [...common, '--tile', '0', '4', '--no-sound', '--saves', path.relative(ROOT, path.join(dir, 'saves')).replaceAll('\\', '/'),
        '--host', `127.0.0.1:${port}`, '--net-joiners', '1', ...online_presses(0, 2), '--frames', String(ONLINE_END + 200)],
        path.join(dir, 'pc.txt'), { timeout: MODE_TIMEOUT, here: true });
    const phone = run('android', [...common, '--join', `10.0.2.2:${port}`, ...online_presses(1, 2), '--frames', String(ONLINE_END)], path.join(dir, 'phone.txt'),
        { timeout: MODE_TIMEOUT });
    const guard = setTimeout(() => {
        if (!/has joined/.test(fs.readFileSync(path.join(dir, 'pc.txt'), 'latin1'))) {
            host.stop();
            phone.stop();
        }
    }, 90000);
    const [a, b] = await Promise.all([host, phone]);
    const pa = fingerprints(a.text), pb = fingerprints(b.text);
    const parted = fingerprints_part(pa, pb);

    clearTimeout(guard);
    const wrong = !/has joined/.test(a.text) ? 'the phone never joined'
        : crash_where(pc, a.text) ?? (/^crash: /m.test(b.text) ? b.text.match(/^crash: .*$/m)[0] : null)
        ?? (pb.length < 3800 ? `the phone's game is short: ${pb.length} steps` : parted != null ? `the phone's game differs from the PC's from ${parted}` : null);

    return report([['this PC hosting, Android\'s emulator joining', { pass: wrong == null, what: wrong ?? `the same on both, ${pb.length} steps` }]]);
}

async function online(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    if (flag(args, '--play')) {
        return online_play(args, exe);
    }
    if (ANDROID) {
        return online_android(args);
    }
    const dir = path.join(OUT, 'online');
    const rel = p => path.relative(ROOT, path.join(dir, p)).replaceAll('\\', '/');
    const late = ['--net-lag', '40', '--net-loss', '10'];
    /* flat out, a press has to be used many steps late for the steps to come as fast as they're played: a packet takes as long as ever */
    const cases = [
        { name: 'two machines', machines: 2, save: 'good' },
        { name: 'a save the game couldn\'t have made is refused', machines: 2, save: 'bad', want: 'refused' },
        { name: 'two machines, packets late and lost', machines: 2, all: late, host: ['--net-delay', '30', '--net-record', rel('late.txt')] },
        { name: 'four machines, packets late and lost', machines: 4, all: late, host: ['--net-delay', '30'] },
        { name: 'a machine that disagrees is caught', machines: 2, per: { 1: ['--net-differ', '600'] }, want: 'differs' },
        { name: 'in real time, packets late and lost', machines: 2, real: true, all: ['--net-lag', '30', '--net-loss', '5'] },
        { name: 'in real time, a machine vanishes', machines: 3, real: true, per: { 2: ['--net-vanish', '800'] }, host: ['--net-patience', '1500', '--shot', '2300', rel('vanished.png')], want: 'vanishes' },
        { name: 'in real time, a machine stalls and is back', machines: 3, real: true, per: { 2: ['--net-stall', '800', '3000'] }, host: ['--net-patience', '1000'], want: 'back' },
        /* the joiner's player pauses in the round, and in OPTIONS, SOUND turns the music down three steps, the effects one, and
           chooses mono: that machine's own change, the host's don't, and the games stay the same */
        { name: 'a player\'s sound options are their own', machines: 2, want: 'own sound', end: 3600, presses: m => [
            ...(m === 0 ? [...press(170, DOWN), ...press(230, CROSS), ...press(350, CROSS)] : []),
            ...[470, 590, 850, 1150, 1250, 1550, 1650, 1750, 2000].flatMap(v => press(v, CROSS)),
            ...(m === 1 ? [[3000, START], [3040, DOWN], [3070, DOWN], [3100, CROSS], [3140, CROSS], [3180, DOWN], [3210, '80'], [3240, '80'], [3270, '80'],
                [3300, DOWN], [3330, '80'], [3380, '10'], [3410, '10'], [3440, '80']].flatMap(([v, b]) => press(v, b)) : []),
            '--shot', '3500', rel(`sound-machine${m + 1}.png`)] },
    ];
    const press = (v, b) => ['--press', String(v), b];
    /* the launcher's presses: ONLINE, then HOST GAME and in time START; or JOIN GAME and the first game found, or down to ADDRESS
       and the host's typed. In real time, as finding and joining take as long as they take */
    /* a code typed with the controller (settingsmenu.c, update_join): right for a character more, up and down through the letters to
       it, then its Cross. From vblank 400, a press every 8; how long it takes, for the host to wait till then to start */
    const PICK = '0123456789ABCDEFGHJKMNPQRSTVWXYZ.';
    const pad_typing = text => {
        const presses = [];
        let last = null;

        for (const ch of text) {
            presses.push('20');
            last = last ?? '0';
            const d = (PICK.indexOf(ch) - PICK.indexOf(last) + PICK.length) % PICK.length;
            presses.push(...Array(d <= PICK.length / 2 ? d : 0).fill(UP_PAD), ...Array(d > PICK.length / 2 ? PICK.length - d : 0).fill(DOWN));
            last = ch;
        }
        presses.push(CROSS);
        return presses.flatMap((b, i) => ['--pad-press', '1', String(400 + i * 8), b]);
    };
    const UP_PAD = '10';
    const typing_ends = text => 400 + (pad_typing(text).length / 4) * 8;
    const lobby = (m, typed, pad, start = 720) => m === 0 ? [...press(100, DOWN), ...press(130, CROSS), ...press(170, CROSS), ...press(start, START)]
        : [...press(100, DOWN), ...press(130, CROSS), ...press(170, DOWN), ...press(200, CROSS), ...(typed == null ? press(360, CROSS)
            : [...press(330, DOWN), ...press(360, CROSS), ...(pad ? pad_typing(typed)
                : [...[...typed].flatMap((key, i) => ['--key-press', String(400 + i * 14), key]), ...press(560, START)])])];
    /* the join code of a game hosted on this PC, as the program makes it: what its pretend router gives the host to show */
    const code = spawnSync(path.resolve(ROOT, exe), ['--make-code', '127.0.0.1', '0'], { encoding: 'utf8' }).stdout.trim();
    /* the game after it: the same Battle; or the main menu's EXIT, by the host; or nothing, till the host's gone */
    const exit = m => m === 0 ? [300, 320, 340, 360].flatMap(v => press(v, DOWN)).concat(press(400, CROSS)) : [];
    const after = (c, m) => c.launcher === 'exit' ? [...exit(m), '--then', '--frames', '200']
        /* the main menu's EXIT, back in the launcher, hosted and joined again, and the same Battle played */
        : c.launcher === 'second' ? [...exit(m), '--then', ...lobby(m), '--then', '--turbo', ...online_presses(m, c.machines), '--frames', String(ONLINE_END)]
        : c.launcher === 'host leaves' ? ['--frames', m === 0 ? '600' : '100000', ...(m === 0 ? [] : ['--then', '--shot', '150', rel('left.png'), '--frames', '200'])]
        : [...online_presses(m, c.machines), '--frames', String(ONLINE_END)];
    const through = [
        { name: 'from the launcher: hosted, found and joined', machines: 2, launcher: 'play' },
        { name: 'from the launcher: the host\'s address typed', machines: 2, launcher: 'play', typed: '127.0.0.1' },
        /* the host's router "opens its port" (a pretend one: none is asked), the host shows the code, and the joiner types it: its
           letters are also keys the game's buttons are set to, which mustn't act as them */
        { name: `from the launcher: joined by the host's code, ${code}`, machines: 2, launcher: 'play', typed: code.replace('-', ''), router: 'open' },
        { name: 'from the launcher: the host\'s code typed with a controller', machines: 2, launcher: 'play', typed: code.replace('-', ''), pad: true, router: 'open' },
        { name: 'from the launcher: EXIT brings each machine back to it', machines: 2, launcher: 'exit' },
        { name: 'from the launcher: a second game after the first, in the same run', machines: 2, launcher: 'second' },
        { name: 'from the launcher: the host leaving brings the joiner back to it', machines: 2, launcher: 'host leaves' },
    ];
    const results = [];

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    const play_through = (c, k, slot, slots) => Promise.all(Array.from({ length: c.machines }, (_, m) => run(exe, ['--launcher', '--online-local', '--online-port',
        String(47250 + k), '--skip-intro', '--pretend-pads', '1', '--fingerprints', '1', '--draw-every', '60', '--tile', String(slot + m), String(slots),
        ...(c.router != null && m === 0 ? ['--online-pretend-router', c.router] : []),
        '--saves', rel(`through${k}-${m}`), ...lobby(m, c.typed, c.pad, c.pad ? typing_ends(c.typed) + 200 : 720), '--then', '--turbo', ...after(c, m)],
        path.join(dir, `through${k}-machine${m + 1}.txt`), { timeout: MODE_TIMEOUT })));
    const judge_through = (c, runs) => {
        const prints = runs.map(r => fingerprints(r.text));
        const count = (m, re) => [...runs[m].text.matchAll(re)].length;
        let wrong = runs.map(r => crash_where(exe, r.text)).find(x => x != null) ?? null;

        for (let m = 0; wrong == null && m < c.machines; m++) {
            wrong = runs[m].code !== 0 ? `machine ${m + 1}: exit ${runs[m].code}`
                : count(m, /^restart: the game from its beginning/gm) < 1 ? `machine ${m + 1}'s game never started` : null;
            if (wrong == null && c.launcher === 'second') {
                /* the second game's fingerprints are the ones after its start */
                const parted = fingerprints_part(prints[0], prints[m]);

                wrong = count(m, /^restart: the game from its beginning/gm) !== 3 ? `machine ${m + 1} didn't start a second game`
                    : count(m, /^players 2, /gm) < 2 ? `machine ${m + 1} never reached the second two-player game`
                    : parted != null ? `machine ${m + 1}'s second game differs from the host's from ${parted}`
                    : prints[m].length < 3000 ? `machine ${m + 1}'s second game is short: ${prints[m].length} steps` : null;
            } else if (wrong == null && c.launcher === 'play') {
                const parted = fingerprints_part(prints[0], prints[m]);

                wrong = !/^players 2, /m.test(runs[m].text) ? `machine ${m + 1} never reached the two-player game`
                    : parted != null ? `machine ${m + 1}'s game differs from the host's from ${parted}`
                    : count(m, /^mode 40 at vblank/gm) !== 3 ? `machine ${m + 1} wasn't back at the title at the end` : null;
            } else if (wrong == null && (c.launcher === 'exit' || m !== 0)) {
                /* back in the launcher: the game started again a second time, offline */
                wrong = count(m, /^restart: the game from its beginning/gm) !== 2 ? `machine ${m + 1} never came back to its launcher` : null;
            }
        }
        if (wrong == null && c.launcher === 'host leaves' && !/^online: Online play: the host has left the game/m.test(runs[1].text)) {
            wrong = 'the joiner never said the host had left';
        }
        if (wrong == null && c.launcher === 'play' && !/^launcher: the lobby has 2, this one 2$/m.test(runs[1].text)) {
            wrong = 'the joiner was never shown who\'s in the lobby';
        }
        if (wrong == null && c.router === 'open' && !runs[0].text.includes(`the join code is ${code}`)) {
            wrong = 'the host never had its code';
        }
        if (wrong == null && c.router === 'open' && !/^launcher: joining [0-9A-Z]{8}$/m.test(runs[1].text)) {
            wrong = 'the joiner never joined by the code';
        }
        return [c.name, { pass: wrong == null, what: wrong ?? (c.launcher === 'play' || c.launcher === 'second' ? `the same on both, ${prints[0].length} steps` : 'back in the launcher') }];
    };
    /* a game's machines, all at once: what each printed */
    const play = async (c, k, slot, slots) => {
        const end = c.end ?? (c.real ? 2400 : ONLINE_END);

        if (c.save != null) {
            /* the host has a save, which the others are to play with: a slot's file as saves.c writes it. One the game could have made
               (an Adventure saved in its first warp room, with a few things won), or one it couldn't, which hosting refuses */
            const data = c.save === 'bad' ? Buffer.alloc(0x194, 0x21) : adventure_save();
            const head = Buffer.alloc(16);

            head.write('CBSV');
            head.writeUInt32LE(1, 4);
            head.writeUInt32LE(data.length, 8);
            head.writeUInt32LE(crc32(data), 12);
            fs.mkdirSync(path.join(dir, `saves${k}-0`), { recursive: true });
            fs.writeFileSync(path.join(dir, `saves${k}-0`, 'slot2.sav'), Buffer.concat([head, data]));
        }
        return Promise.all(Array.from({ length: c.machines }, (_, m) => run(exe, ['--skip-intro', '--pretend-pads', '1', '--fingerprints', '1',
            ...(c.real ? [] : ['--turbo', '--draw-every', '60']), '--tile', String(slot + m), String(slots), '--saves', rel(`saves${k}-${m}`),
            ...(m === 0 ? ['--host', `127.0.0.1:${47200 + k}`, '--net-joiners', String(c.machines - 1), ...(c.host ?? [])] : ['--join', `127.0.0.1:${47200 + k}`]),
            ...(c.all ?? []), ...(c.per?.[m] ?? []), ...(c.presses != null ? c.presses(m) : online_presses(m, c.machines)), '--frames', String(end)],
            path.join(dir, `game${k}-machine${m + 1}.txt`), { timeout: MODE_TIMEOUT })));
    };
    const judge = (c, k, runs) => {
        const prints = runs.map(r => fingerprints(r.text));
        const said = (m, re) => re.test(runs[m].text);
        const crash = runs.map(r => crash_where(exe, r.text)).find(x => x != null);
        const how = runs[0].text.match(/^online: \d+ steps; waited.*$/m)?.[0].replace(/^online: /, '').replace(/; \d+ packets.*/, '') ?? '';
        let wrong = crash ?? null;

        if (wrong == null && c.want === 'refused') {
            wrong = /^online: save slot 2 isn't one the game could have made/m.test(runs[0].text) ? null : 'the host didn\'t refuse it';
            return [c.name, { pass: wrong == null, what: wrong ?? 'the host said so, and didn\'t host' }];
        }
        if (wrong == null && c.want === 'differs') {
            wrong = runs.every(r => /^online: the games differ at step 600/m.test(r.text)) ? null : 'not every machine said the games differ at step 600';
            return [c.name, { pass: wrong == null, what: wrong ?? 'caught at step 600, and both stopped' }];
        }
        for (let m = 0; wrong == null && m < c.machines; m++) {
            const gone = c.want === 'vanishes' && m === 2;
            const parted = gone ? fingerprints_part(prints[0].slice(0, prints[m].length), prints[m]) : fingerprints_part(prints[0], prints[m]);

            wrong = runs[m].code !== 0 ? `machine ${m + 1}: exit ${runs[m].code}`
                : !new RegExp(`^players ${c.machines}, `, 'm').test(runs[m].text) ? `machine ${m + 1} never reached the ${c.machines}-player game`
                : parted != null ? `machine ${m + 1}'s game differs from the host's from ${parted}`
                : !c.real && c.presses == null && !gone && [...runs[m].text.matchAll(/^mode 40 at vblank/gm)].length !== 2 ? `machine ${m + 1} wasn't back at the title at the end` : null;
        }
        if (wrong == null && c.want === 'own sound') {
            wrong = !said(1, /^own sound: music 55, fx 95, mono$/m) ? 'the joiner\'s own sound options didn\'t change as pressed'
                : said(0, /^own sound:/m) ? 'the host\'s sound options changed with the joiner\'s' : null;
        }
        if (wrong == null && c.save === 'good') {
            const saves = runs.map(r => r.text.match(/saves ([0-9A-F]{8})$/m)?.[1]);

            wrong = saves.some(s => s == null || s !== saves[0]) || saves[0] === '9AE73935' ? `the host's save didn't reach the others: ${saves.join(', ')}` : null;
        }
        if (wrong == null && (c.want === 'vanishes' || c.want === 'back')) {
            /* the third copy's machine, by the order they happened to join in */
            const n = runs[2].text.match(/^online: joined as machine (\d)/m)?.[1];

            wrong = !said(0, new RegExp(`^online: machine ${n} is slow`, 'm')) ? 'the host never said it was slow, while it waited for it'
                : !said(0, new RegExp(`^online: machine ${n} isn't answering`, 'm')) ? 'the host never played on without it'
                : c.want === 'back' && !said(0, new RegExp(`^online: machine ${n} is back`, 'm')) ? 'it never came back' : null;
        }
        return [c.name, { pass: wrong == null, what: wrong ?? `the same on all ${c.machines}, ${prints[0].length} steps; ${how}` }];
    };
    /* flat out first, then the ones in real time: each lot together, in a grid */
    for (const real of [false, true]) {
        const lot = cases.map((c, k) => [c, k]).filter(([c]) => !!c.real === real);
        const slots = lot.reduce((n, [c]) => n + c.machines, 0);
        let slot = 0;

        results.push(...await Promise.all(lot.map(([c, k]) => {
            const from = slot;

            slot += c.machines;
            return play(c, k, from, slots).then(runs => judge(c, k, runs));
        })));
    }
    /* through the launcher: all together, its screens in real time and the games flat out */
    {
        const slots = through.reduce((n, c) => n + c.machines, 0);
        let slot = 0;

        results.push(...await Promise.all(through.map((c, k) => {
            const from = slot;

            slot += c.machines;
            return play_through(c, k, from, slots).then(runs => judge_through(c, runs));
        })));
    }
    /* a router that won't help, each way it can: the host's screen says which (a pretend router again), with a shot of each page.
       And the same PC on a virtual network (a pretend adapter, called as Radmin VPN's is): the screen says the address to join at there */
    {
        const routers = { refused: 'a router that won\'t open a port', none: 'no router that opens ports', shared: 'an address shared between homes',
            network: 'no router, on a virtual network' };

        results.push(...await Promise.all(Object.keys(routers).map(async (router, i) => {
            const network = router === 'network';
            const r = await run(exe, ['--launcher', '--online-local', '--online-port', String(47270 + i), '--online-pretend-router', network ? 'none' : router,
                ...(network ? ['--online-pretend-network', 'Radmin VPN (Famatech Radmin VPN Ethernet Adapter)=26.41.7.9'] : []), '--skip-intro',
                '--pretend-pads', '1', '--turbo', '--tile', String(i), '4', ...press(100, DOWN), ...press(130, CROSS), ...press(170, CROSS),
                '--shot', '700', rel(`router-${router}-1.png`), '--shot', '1000', rel(`router-${router}-2.png`), '--frames', '1020'],
                path.join(dir, `router-${router}.txt`), { timeout: MODE_TIMEOUT });
            const said = new RegExp(`^launcher: router ${network ? 'none' : router}$`, 'm').test(r.text)
                && /^launcher: on Radmin VPN, they join at 26\.41\.7\.9$/m.test(r.text) === network;

            return [`hosting with ${routers[router]}`, { pass: r.code === 0 && said, what: crash_where(exe, r.text) ?? (said ? 'said so' : `not said (exit ${r.code})`) }];
        })));
    }
    /* the recorded game again, with nobody else */
    {
        const r = await run(exe, ['--skip-intro', '--pretend-pads', '1', '--fingerprints', '1', '--turbo', '--draw-every', '60', '--replay', rel('late.txt'),
            '--frames', String(ONLINE_END)], path.join(dir, 'again.txt'), { timeout: MODE_TIMEOUT });
        const parted = crash_where(exe, r.text) ?? (r.code !== 0 ? `exit ${r.code}`
            : fingerprints_part(fingerprints(fs.readFileSync(path.join(dir, `game${cases.findIndex(c => c.host?.includes(rel('late.txt')))}-machine1.txt`), 'latin1')), fingerprints(r.text)));

        results.push(['a recorded game played again', { pass: parted == null, what: parted == null ? 'the same game' : `differs: ${parted}` }]);
    }
    /* and what a joiner kept of its game by itself, beside its saves: the host's saves were in it, which it played with */
    {
        const k = cases.findIndex(c => c.save === 'good');
        const kept = path.join(dir, `saves${k}-1`, 'last-online-game.txt');
        const r = fs.existsSync(kept) ? await run(exe, ['--skip-intro', '--pretend-pads', '1', '--fingerprints', '1', '--turbo', '--draw-every', '60',
            '--replay', path.relative(ROOT, kept).replaceAll('\\', '/'), '--frames', String(ONLINE_END)], path.join(dir, 'kept-again.txt'), { timeout: MODE_TIMEOUT }) : null;
        const parted = r == null ? 'none was kept' : crash_where(exe, r.text) ?? (r.code !== 0 ? `exit ${r.code}`
            : fingerprints_part(fingerprints(fs.readFileSync(path.join(dir, `game${k}-machine1.txt`), 'latin1')), fingerprints(r.text)));

        results.push(['a joiner\'s own record of its game, played again', { pass: parted == null, what: parted == null ? 'the same game' : `differs: ${parted}` }]);
    }
    return report(results);
}

/* The launcher's CONTROLS page, with pretend keys (--key-press) and a pretend controller: a key chosen for Cross, a key
   another button had (they swap), the new key choosing PLAY, a controller button chosen and used the same way, and RESET TO
   DEFAULTS. From the log's "controls:" lines (settingsmenu.c). */
async function controls(args) {
    const exe = option(args, '--exe', DEFAULT_EXE);
    const dir = path.join(OUT, 'controls');
    const UP = '10', TRIANGLE = '1000', CIRCLE = '2000';
    const press = (...pairs) => pairs.flatMap(([v, b]) => ['--press', String(v), b]);
    /* SETTINGS, then that many down the categories (CONTROLS is 2), into its rows, and down past STICK DEADZONE and the line to Cross's */
    const page = n => press([85, DOWN], [100, DOWN], [130, CROSS], ...Array.from({ length: n }, (_, i) => [160 + i * 20, DOWN]), [230, CROSS], [245, DOWN]);
    /* back out to the top and up onto PLAY */
    const out = from => press([from, TRIANGLE], [from + 30, TRIANGLE], [from + 60, UP], [from + 75, UP]);
    const cases = [
        {
            name: 'keyboard: J for Cross, J for Circle (Cross gets X), K for Cross, K plays',
            args: [...page(2), ...press([260, CROSS]), '--key-press', '300', 'J', ...press([360, DOWN], [390, CROSS]), '--key-press', '420', 'J',
                   ...press([480, UP], [510, CROSS]), '--key-press', '540', 'K', ...out(570), '--key-press', '660', 'K', '--frames', '720'],
            want: ['CROSS is key J', 'CIRCLE is key J', 'CROSS is key K', 'play'],
        },
        {
            name: 'gamepad: the right-hand button for Cross, and it plays',
            args: ['--pretend-pads', '1', ...page(2), ...press([260, CROSS]), '--pad-press', '1', '290', CIRCLE, ...out(350), '--pad-press', '1', '440', CIRCLE,
                   '--frames', '500'],
            want: ['CROSS is button b', 'play'],
        },
        {
            name: 'keyboard: Esc keeps a key, and RESET TO DEFAULTS puts them back: Z plays',
            args: [...page(2), ...press([260, CROSS]), '--key-press', '300', 'ESCAPE', ...press([330, CROSS]), '--key-press', '360', 'J',
                   ...Array.from({ length: 14 }, (_, i) => [390 + i * 12, DOWN]).flatMap(([v, b]) => ['--press', String(v), b]), ...press([580, CROSS]),
                   ...out(610), '--key-press', '700', 'Z', '--frames', '760'],
            want: ['CROSS is key J', 'keys and buttons as they were', 'play'],
        },
        /* the mouse, in the launcher's own units (a 640 x 480 picture's from its middle): rows are pointed at and clicked */
        {
            name: 'mouse: SETTINGS, CONTROLS and STICK DEADZONE clicked, back by two right clicks, and PLAY clicked',
            args: ['--mouse', '100', '0', '22', 'click', '--mouse', '150', '-240', '-30', 'click', '--mouse', '200', '0', '-102', 'click',
                   '--mouse', '240', '0', '-102', 'right', '--mouse', '270', '0', '-102', 'right', '--mouse', '300', '0', '-46', 'click', '--frames', '360'],
            want: ['stick deadzone 10', 'play'],
        },
        {
            name: 'mouse: CONTROLS pointed at, its STICK DEADZONE clicked to open it, clicked twice, and a click on nothing does nothing',
            args: ['--mouse', '100', '0', '22', 'click', '--mouse', '150', '-240', '-30', 'move', '--mouse', '180', '0', '-102', 'click',
                   '--mouse', '210', '0', '-102', 'click', '--mouse', '240', '0', '-102', 'click', '--mouse', '270', '-100', '200', 'down',
                   '--mouse', '290', '-100', '200', 'up', '--mouse', '310', '-100', '200', 'click', '--frames', '360'],
            want: ['stick deadzone 10', 'stick deadzone 20'],
        },
    ];

    fs.rmSync(dir, { recursive: true, force: true });
    fs.mkdirSync(dir, { recursive: true });
    const results = await Promise.all(cases.map(async (c, i) => {
        const r = await run(exe, ['--launcher', '--skip-intro', '--turbo', '--tile', String(i), String(cases.length), ...c.args], path.join(dir, `case${i}.txt`),
                            { timeout: MODE_TIMEOUT });
        const crash = crash_where(exe, r.text);
        const got = r.text.split(/\r?\n/).filter(l => /^(controls|launcher): /.test(l)).map(l => l.replace(/^\w+: /, '').replace(/ at vblank \d+$/, ''));
        const wrong = crash ?? (r.code !== 0 ? `exit ${r.code}` : got.join('|') !== c.want.join('|') ? `got ${got.join(', ') || 'nothing'}` : null);

        return [c.name, { pass: wrong == null, what: wrong ?? `case${i}.txt` }];
    }));
    return report(results);
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
/* A save slot as the game makes one (main.h, CardSlot: the heroes' summary, then GameState, nas.h): an Adventure begun with Crash and
   saved in its first warp room, with the first few things won. For the online test's host to have. */
function adventure_save() {
    const d = Buffer.alloc(0x194);
    const s = 8; /* GameState, after the summary */

    d.set([1, 2, 3, 4, 0, 1, 1, 1], 0);
    d.writeUInt32LE(0x80000000, s + 0x04);              /* cheats: none */
    d.write('A       ', s + 0x18, 'latin1');            /* name */
    d.writeInt8(60, s + 0x21);                          /* fps */
    d.set([1, 2], s + 0x26);                            /* players, maxPlayers */
    d.writeUInt16LE(0x200, s + 0x30);                   /* exitFlags */
    d.writeUInt16LE(0x214, s + 0x32);                   /* levelFlags */
    d.writeInt16LE(33, s + 0x34);                       /* mode: the first warp room */
    d.writeInt16LE(1, s + 0x38);                        /* aiLevel */
    d.writeUInt16LE(3, s + 0x3A);                       /* battleWins */
    d.writeInt16LE(206, s + 0x3C);                      /* levelLoadModel */
    d.writeInt16LE(512, s + 0x3E);
    d.writeInt16LE(-1, s + 0x42);                       /* cupLeader */
    d.writeInt16LE(1, s + 0x44);                        /* teamSide */
    d.writeInt16LE(2, s + 0x46);
    d.writeInt16LE(0, s + 0x48);                        /* teamLeader */
    d.writeInt16LE(2, s + 0x4A);
    d.set([1, 2, 3, 4], s + 0x4C);                      /* heroChoice */
    d.set([1, 9, 9, 9], s + 0x50);                      /* heroes */
    d.fill(0xFF, s + 0x54, s + 0x5C);                   /* teammateChoice, teammates: none */
    d.set([0, 1, 1, 1], s + 0x5C);                      /* heroCpu */
    d.writeInt8(-1, s + 0x63);                          /* gateway */
    d.set([0, 1, 2, 3], s + 0xA7);                      /* adventure.rank */
    d.writeInt32LE(70, s + 0xD0);                       /* adventure.options: music, sfx, stereo, vibration */
    d.writeInt32LE(100, s + 0xD4);
    d.writeInt32LE(1, s + 0xD8);
    d.set([1, 1, 1, 1], s + 0xDC);
    d.set([1, 1, 0, 1], s + 0xE4);                      /* adventure.won: a trophy or two */
    return d;
}

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

/* Online play's packet fuzzer, which is libFuzzer's program: the sanitizers' runtime is beside the compiler, so that's on the PATH. */
async function fuzz(args) {
    const seconds = Number(option(args, '--seconds', '120'));
    const exe = path.join(ROOT, WINDOWS ? 'build/port/windows-clang/netfuzz.exe' : 'build/port/linux-clang/netfuzz');
    const dir = path.join(OUT, 'fuzz');
    const corpus = path.join(dir, 'corpus'), found = path.join(dir, 'found');
    const cache = path.join(path.dirname(exe), 'CMakeCache.txt');
    const compiler = fs.existsSync(cache) ? fs.readFileSync(cache, 'utf8').match(/^CMAKE_C_COMPILER:\w+=(.*)$/m)?.[1] : null;
    /* (Windows has it as Path) */
    const key = Object.keys(process.env).find(k => k.toUpperCase() === 'PATH') ?? 'PATH';
    const env = { ...process.env, [key]: `${compiler != null ? path.dirname(compiler) : ''}${path.delimiter}${process.env[key]}`, NETFUZZ_SEEDS: corpus };
    const results = [];

    if (!fs.existsSync(exe)) {
        console.log(`no fuzzer at ${path.relative(ROOT, exe)}: cmake --build --preset windows-clang --target netfuzz (it needs a compiler with libFuzzer)`);
        return 2;
    }
    fs.mkdirSync(corpus, { recursive: true });
    fs.mkdirSync(found, { recursive: true });
    /* what earlier runs found, first: each has to be put right by now */
    for (const name of fs.readdirSync(found).filter(n => /^(crash|timeout|oom|leak)-/.test(n))) {
        const r = spawnSync(exe, [path.join(found, name)], { env, encoding: 'latin1' });
        const what = (r.stdout + r.stderr).match(/^(SUMMARY: .*|.*runtime error: .*)$/m)?.[1] ?? `exit ${r.status}`;

        results.push([`found before: ${name}`, { pass: r.status === 0, what: r.status === 0 ? 'fixed' : what }]);
    }
    const r = await run(exe, [corpus, `-artifact_prefix=${found}${path.sep}`, `-max_total_time=${seconds}`, '-timeout=3', '-rss_limit_mb=2048', '-print_final_stats=1'],
        path.join(dir, 'run.txt'), { env });
    const text = r.text;
    const runs = text.match(/stat::number_of_executed_units:\s*(\d+)/)?.[1] ?? '?';
    const cov = text.match(/^#\d+\s+DONE\s+cov: (\d+)/m)?.[1] ?? '?';
    const problem = text.match(/^(SUMMARY: .*|.*runtime error: .*|==\d+== ERROR: libFuzzer: .*)$/m)?.[1];

    results.push([`${seconds} seconds of packets`, { pass: r.code === 0 && problem == null, what: problem ?? (r.code !== 0 ? `exit ${r.code}`
        : `${runs} inputs, ${cov} edges of code reached, nothing wrong`) }]);
    if (problem != null) {
        console.log(`what it found is in ${path.relative(ROOT, found)}; run.txt says where`);
    }
    return report(results);
}

const commands = { bosses, modes, adventure, challenges, same, hero, heroes, menu, pads, controls, identical, online, fuzz, shots, image, packs, mods };
if (commands[command] == null) {
    console.log('node port/tests/playtest.mjs bosses|modes|adventure|challenges|same|hero|heroes|menu|pads|controls|identical|online|fuzz|shots|image|packs|mods: see the top of this file');
    process.exit(2);
}
process.exit(await commands[command](rest));
