/* Builds a release of the PC port as a zip, ready to publish by hand: node port/package.mjs
 *
 * It holds the program, its README and the licences, and never the game's data. It checks that: no sample of the game's archive may be
 * in the program, nor this machine's paths. It publishes nothing. Needs TC in the environment for the toolchain (docs/PORT.md, Building). */
import { execFileSync } from 'node:child_process';
import crypto from 'node:crypto';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const PORT = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.dirname(PORT);
const BUILD = path.join(ROOT, 'build/port/windows-release');
const OUT = path.join(ROOT, 'build/port/release');
const DATA = path.join(ROOT, 'build/disc/files/CRASHBSH/CRASHBSH.DAT');
const SAMPLES = 2000;
const SAMPLE_BYTES = 64;

const toolchain = process.env.TC;
if (!toolchain) {
    throw new Error('Set TC to the toolchain (docs/PORT.md, Building)');
}
const env = { ...process.env, PATH: `${path.join(toolchain, 'bin')}${path.delimiter}${process.env.PATH}` };
const run = (cmd, args, cwd = ROOT) => execFileSync(cmd, args, { cwd, env, stdio: ['ignore', 'pipe', 'inherit'] }).toString();

run('cmake', ['--preset', 'windows-release', `-DCMAKE_PREFIX_PATH=${path.join(toolchain, 'deps')}`], PORT);
run('cmake', ['--build', '--preset', 'windows-release'], PORT);

const exe = path.join(BUILD, 'crashbash.exe');
const version = run(exe, ['--version']).trim().split(' ').at(-1);
const name = `crash-bash-pc-${version}-windows-x64`;
const dir = path.join(OUT, name);

fs.rmSync(dir, { recursive: true, force: true });
fs.mkdirSync(dir, { recursive: true });
fs.copyFileSync(exe, path.join(dir, 'crashbash.exe'));
fs.writeFileSync(path.join(dir, 'README.txt'), fs.readFileSync(path.join(PORT, 'release/README.txt'), 'utf8').replaceAll('@VERSION@', version).replace(/\r?\n/g, '\r\n'));
const sdl = fs.readFileSync(path.join(toolchain, 'deps/share/licenses/SDL3/LICENSE.txt'), 'utf8');
/* zlib's licence is the notice at the top of its header */
const zlib_h = fs.readFileSync(path.join(toolchain, 'deps/include/zlib.h'), 'utf8');
const zlib = zlib_h.slice(zlib_h.indexOf('Copyright'), zlib_h.indexOf('The data format')).replace(/^ {2}/gm, '').trim();
const rule = '\n-------------------------------------------------------------------------------\n\n';
fs.writeFileSync(path.join(dir, 'LICENSE.txt'), `${fs.readFileSync(path.join(ROOT, 'LICENSE'), 'utf8')}${rule}This program includes SDL3, under this licence:\n\n${sdl}${rule}This program includes zlib, under this licence:\n\n${zlib}\n`.replace(/\r?\n/g, '\r\n'));
/* where mods go, so it's there to drop them in */
fs.mkdirSync(path.join(dir, 'mods'));
fs.writeFileSync(path.join(dir, 'mods', 'README.txt'), 'Put mods here, as their zips. They show up on the Mods page, under Settings in\r\nthe menu the game opens with.\r\n');

/* What it mustn't hold */
const program = fs.readFileSync(exe);
const home = process.env.USERPROFILE ?? process.env.HOME ?? '';
for (const text of [ROOT, ROOT.replaceAll('\\', '/'), home, home.replaceAll('\\', '/')]) {
    if (text !== '' && program.includes(text)) {
        throw new Error(`The program holds this machine's path: ${text}`);
    }
}
if (fs.existsSync(DATA)) {
    const data = fs.readFileSync(DATA);
    /* The code entries (ASSETS.md: 1E7, 2CC to 2D8 and 3DF) are the overlays, whose strings and tables are the decomp's own C and so in
       the program too: the samples are of everything else. Where each entry is comes from the port's copy of the game's table. */
    const table = fs.readFileSync(path.join(PORT, 'game/main/main.c'), 'utf8').match(/file DatIndex\[\d+\] = \{([^}]*)\}/)[1];
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

const zip = path.join(OUT, `${name}.zip`);
fs.rmSync(zip, { force: true });
run('powershell.exe', ['-NoProfile', '-Command', `Compress-Archive -Path '${dir}\\*' -DestinationPath '${zip}'`]);
const sha = crypto.createHash('sha256').update(fs.readFileSync(zip)).digest('hex');
fs.writeFileSync(`${zip}.sha256`, `${sha}  ${path.basename(zip)}\n`);
console.log(`${path.relative(ROOT, zip)}: ${fs.statSync(zip).size} bytes, sha256 ${sha}`);
