/* Builds a release of the PC port, ready to publish by hand: node port/package.mjs
 *
 * On Windows it's a zip, and needs TC in the environment for the toolchain (docs/PORT.md, Building). On Linux it's a .tar.gz, made in the
 * port's container: tools/dl node port/package.mjs. It holds the program, its README and the licences, and never the game's data. It checks
 * that: no sample of the game's archive may be in the program, nor this machine's paths. It publishes nothing. */
import { execFileSync } from 'node:child_process';
import crypto from 'node:crypto';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const PORT = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.dirname(PORT);
const LINUX = process.platform === 'linux';
const PRESET = LINUX ? 'linux-release' : 'windows-release';
const BUILD = path.join(ROOT, 'build/port', PRESET);
const OUT = path.join(ROOT, 'build/port/release');
const DATA = path.join(ROOT, 'build/disc/files/CRASHBSH/CRASHBSH.DAT');
const SAMPLES = 2000;
const SAMPLE_BYTES = 64;

/* Where SDL3 and zlib are, for their licences: the toolchain's on Windows, the container's on Linux (SDL3 built into its image) */
let env = process.env, sdl_dir, zlib_include;
if (LINUX) {
    sdl_dir = '/usr/local';
    zlib_include = '/usr/include';
} else {
    const toolchain = process.env.TC;
    if (!toolchain) {
        throw new Error('Set TC to the toolchain (docs/PORT.md, Building)');
    }
    env = { ...process.env, PATH: `${path.join(toolchain, 'bin')}${path.delimiter}${process.env.PATH}` };
    sdl_dir = path.join(toolchain, 'deps');
    zlib_include = path.join(toolchain, 'deps/include');
}
const run = (cmd, args, cwd = ROOT) => execFileSync(cmd, args, { cwd, env, stdio: ['ignore', 'pipe', 'inherit'] }).toString();

run('cmake', ['--preset', PRESET, ...(LINUX ? [] : [`-DCMAKE_PREFIX_PATH=${sdl_dir}`])], PORT);
run('cmake', ['--build', '--preset', PRESET], PORT);

const program_name = LINUX ? 'crashbash' : 'crashbash.exe';
const exe = path.join(BUILD, program_name);
const version = run(exe, ['--version']).trim().split(' ').at(-1);
const name = `crash-bash-port-${version}-${LINUX ? 'linux' : 'windows'}-x64`;
const dir = path.join(OUT, name);
const eol = text => text.replace(/\r?\n/g, LINUX ? '\n' : '\r\n');

/* What the README says that's the system's own. The oldest glibc it runs on is the newest one any of its imports asks for. */
const glibc = LINUX ? run('objdump', ['-T', exe]).match(/GLIBC_[0-9.]+/g).map(v => v.slice(6))
    .sort((a, b) => a.localeCompare(b, 'en', { numeric: true })).at(-1) : null;
const SYSTEM = LINUX ? {
    SYSTEM: 'Linux',
    PROGRAM: 'crashbash',
    RUN: './crashbash',
    NEEDS: `It needs 64-bit Linux with glibc ${glibc} or newer and a graphics card with Vulkan.\n` +
        'Asking for the disc image takes a file chooser: a desktop portal, or zenity.',
    SAVES: 'They\'re kept in ~/.local/share/CrashBash/CrashBash, or in\n$XDG_DATA_HOME/CrashBash/CrashBash if that\'s set.',
} : {
    SYSTEM: 'Windows',
    PROGRAM: 'crashbash.exe',
    RUN: 'crashbash.exe',
    NEEDS: 'It needs 64-bit Windows 10 or 11 and a graphics card with Direct3D 12. Windows\nmay warn about an unrecognised program, as this one isn\'t signed.',
    SAVES: 'They\'re kept in %APPDATA%\\CrashBash\\CrashBash.',
};

fs.rmSync(dir, { recursive: true, force: true });
fs.mkdirSync(dir, { recursive: true });
fs.copyFileSync(exe, path.join(dir, program_name));
fs.chmodSync(path.join(dir, program_name), 0o755);
let readme = fs.readFileSync(path.join(PORT, 'release/README.txt'), 'utf8').replaceAll('@VERSION@', version);
for (const [key, text] of Object.entries(SYSTEM)) {
    readme = readme.replaceAll(`@${key}@`, text);
}
if (/@[A-Z]+@/.test(readme)) {
    throw new Error(`The README has a part with nothing for it: ${readme.match(/@[A-Z]+@/)[0]}`);
}
fs.writeFileSync(path.join(dir, 'README.txt'), eol(readme));
const sdl = fs.readFileSync(path.join(sdl_dir, 'share/licenses/SDL3/LICENSE.txt'), 'utf8');
/* zlib's licence is the notice at the top of its header */
const zlib_h = fs.readFileSync(path.join(zlib_include, 'zlib.h'), 'utf8');
const zlib = zlib_h.slice(zlib_h.indexOf('Copyright'), zlib_h.indexOf('The data format')).replace(/^ {2}/gm, '').trim();
const rule = '\n-------------------------------------------------------------------------------\n\n';
fs.writeFileSync(path.join(dir, 'LICENSE.txt'), eol(`${fs.readFileSync(path.join(ROOT, 'LICENSE'), 'utf8')}${rule}This program includes SDL3, under this licence:\n\n${sdl}${rule}This program includes zlib, under this licence:\n\n${zlib}\n`));
/* where mods go, so it's there to drop them in */
fs.mkdirSync(path.join(dir, 'mods'));
fs.writeFileSync(path.join(dir, 'mods', 'README.txt'), eol('Put mods here, as their zips. They show up on the Mods page, under Settings in\nthe menu the game opens with.\n'));

/* What it mustn't hold */
const program = fs.readFileSync(exe);
const home = process.env.USERPROFILE ?? process.env.HOME ?? '';
for (const text of [ROOT, ROOT.replaceAll('\\', '/'), home, home.replaceAll('\\', '/')]) {
    if (text !== '' && text !== '/' && program.includes(text)) {
        throw new Error(`The program holds this machine's path: ${text}`);
    }
}
if (fs.existsSync(DATA)) {
    const data = fs.readFileSync(DATA);
    /* The code entries (ASSETS.md: 1E7, 2CC to 2D8 and 3DF) are the overlays, whose strings and tables are the decomp's own C and so in
       the program too: the samples are of everything else. Where each entry is comes from the port's copy of the game's table (its size
       a sum since the extra heroes' empty entries were added after the game's 992, which come after all of those). */
    const table = fs.readFileSync(path.join(PORT, 'game/main/main.c'), 'utf8').match(/file DatIndex\[[^\]]*\] = \{([^}]*)\}/)[1];
    const words = table.replace(/\/\*.*?\*\//gs, '').match(/0x[0-9A-Fa-f]+|\d+/g).map(Number);
    const code = [0x1E7, ...Array.from({ length: 13 }, (_, i) => 0x2CC + i), 0x3DF].map(e => [words[e * 2] * 2048, words[e * 2] * 2048 + words[e * 2 + 1]]);
    let found = 0;

    for (let i = 0; i < SAMPLES; i++) {
        let at;

        do {
            at = crypto.randomInt(data.length - SAMPLE_BYTES);
        } while (code.some(([from, to]) => at + SAMPLE_BYTES > from && at < to));
        const sample = data.subarray(at, at + SAMPLE_BYTES);

        if (new Set(sample).size > 8 && program.includes(sample)) {
            found++;
        }
    }
    if (found !== 0) {
        throw new Error(`${found} of ${SAMPLES} samples of the game's archive are in the program`);
    }
    console.log(`none of ${SAMPLES} samples of the game's archive, but for its code, are in the program`);
} else {
    console.log("the game's archive isn't here, so that wasn't checked");
}
for (const file of fs.readdirSync(dir)) {
    if (/\.(dat|bin|cue|iso)$/i.test(file)) {
        throw new Error(`${file} is in the package`);
    }
}

/* A zip of the files on Windows; on Linux a .tar.gz of their folder, the program in it still a program */
const archive = path.join(OUT, `${name}${LINUX ? '.tar.gz' : '.zip'}`);
fs.rmSync(archive, { force: true });
if (LINUX) {
    run('tar', ['--sort=name', '--owner=0', '--group=0', '--numeric-owner', '-czf', archive, '-C', OUT, name]);
} else {
    run('powershell.exe', ['-NoProfile', '-Command', `Compress-Archive -Path '${dir}\\*' -DestinationPath '${archive}'`]);
}
const sha = crypto.createHash('sha256').update(fs.readFileSync(archive)).digest('hex');
fs.writeFileSync(`${archive}.sha256`, `${sha}  ${path.basename(archive)}\n`);
if (LINUX) {
    console.log(`needs glibc ${glibc}`);
}
console.log(`${path.relative(ROOT, archive)}: ${fs.statSync(archive).size} bytes, sha256 ${sha}`);
