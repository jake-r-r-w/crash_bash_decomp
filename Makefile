# Crash Bash (SCUS_945.70) matching decomp.
# Runs inside the toolchain container (tools/dk make <target>) or natively with the tools on PATH.
#
#   make split           - splat every configs/USA yaml (regenerates asm/ and linkers/)
#   make build           - compile and link build/main.exe and every overlay build/<ovl>.bin
#   make check           - build, then compare every target with configs/USA/checksum.sha
#   make disc            - build, then pack build/CrashBash.bin/.cue (bootable; tools/scripts/pack_disc.py)
#   make disc-check      - make disc, then compare the image with the retail one (configs/USA/disc.sha)
#   make test-tools      - unit tests of the pipeline's asm filters (tools/scripts/tests)
#                          (the swarm's on the host: node --test tools/swarm/swarm.test.mjs)
#   make verify          - clean rebuild + check (use after structural/config/header changes)
#   make diff FUNC=name  - build one function's file and diff it against the original
#   make permute FUNC=name [MINUTES=15] - run decomp-permuter on one function
#   make progress        - dashboard (docs/index.html), docs/progress.json and docs/badges/
#   make objdiff-config  - objdiff.json + the original (all-asm) object of every unit
SHELL := /bin/bash -o pipefail
.DEFAULT_GOAL := build

CFG := configs/USA
include $(CFG)/project.mk
# project.mk (written by decomp.py probe/config) provides:
#   NAME, EXE, CC_VER, OPT_FLAGS, GVAL, MASPSX_ARGS, EXTRA_CC1FLAGS

# ---- toolchain (defaults are the container's paths; override for a native setup) ----
CROSS    ?= mipsel-linux-gnu-
GCC_ROOT ?= /opt/gcc
CC1      ?= $(GCC_ROOT)/$(CC_VER)/cc1
MASPSX   ?= python3 /opt/maspsx/maspsx.py
# DMPSX: turns the GTE placeholder words of inline_c.h into cop2 instructions
DMPSX    ?= python3 tools/scripts/dmpsx.py
PSYQ_SDK ?= ext/psyq47
PYTHON   ?= python3
AS      := $(CROSS)as
LD      := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy
CPP     := $(CROSS)cpp
DECOMP  := $(PYTHON) tools/scripts/decomp.py

CPPFLAGS := -P -nostdinc -undef -Iinclude -I$(PSYQ_SDK)/include -D__GNUC__=2 -D_LANGUAGE_C -DPSX -DGVAL=$(GVAL)
CC1FLAGS = -quiet $(OPT_FLAGS) -G$(GVAL) $(EXTRA_CC1FLAGS)
ASFLAGS  := -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0

# ---- targets ----
YAML      := $(CFG)/$(NAME).yaml
LD_SCRIPT := linkers/$(NAME).ld
ELF       := build/$(NAME).elf
TARGET    := build/$(NAME).exe

# Overlays: every configs/USA/overlays/<ovl>.yaml is a raw binary loaded at runtime
# (decomp.py add-overlay); each links from linkers/<ovl>.ld into build/<ovl>.bin.
OVL_YAMLS := $(wildcard $(CFG)/overlays/*.yaml)
OVLS      := $(basename $(notdir $(OVL_YAMLS)))
OVL_LDS   := $(OVLS:%=linkers/%.ld)
OVL_BINS  := $(OVLS:%=build/%.bin)

C_FILES   := $(shell find src -name '*.c' 2>/dev/null)
# (pruned: asm/nonmatchings holds ~2,700 .s files that aren't objects; walking them cost ~90 ms
# on every make, which `try` and `regions` run several times)
S_FILES   := $(shell find asm -path asm/nonmatchings -prune -o -name '*.s' -print 2>/dev/null)
# splat's binary segments (configs: asset_path); assets/ is the game data dump (dat.py), not built
BIN_FILES := $(shell find asm/bin -name '*.bin' 2>/dev/null)
O_FILES   := $(C_FILES:%=build/%.o) $(S_FILES:%=build/%.o) $(BIN_FILES:%=build/%.o)

.PHONY: all build split check verify disc disc-check shift test-tools diff permute progress objdiff-config clean

# Two passes: splat must have produced asm/ before the object list above is computed.
# The splat pass is serial (-j1) even under make -j: concurrent splat runs share asm/ and gave
# intermittently short exes. Compiling and linking (the second pass) stay parallel.
build all:
	@$(MAKE) --no-print-directory -j1 $(LD_SCRIPT) $(OVL_LDS)
	@$(MAKE) --no-print-directory $(TARGET) $(OVL_BINS)

split:
	@for y in $(YAML) $(OVL_YAMLS); do $(PYTHON) -m splat split $$y || exit 1; done

# splat leaves an unchanged linker script alone, so without the touch it stays older than its
# inputs and every build re-runs every split
$(LD_SCRIPT): $(YAML) $(CFG)/sym.main.txt $(CFG)/rel.main.txt
	$(PYTHON) -m splat split $(YAML)
	@touch $@

# an overlay's yaml reads sym.main.txt, sym.engine.txt and its own sym.<ovl>.txt
linkers/%.ld: $(CFG)/overlays/%.yaml $(CFG)/sym.main.txt $(CFG)/sym.engine.txt $(CFG)/sym.%.txt
	$(PYTHON) -m splat split $<
	@touch $@

# .i is kept next to the object so decomp.py can show the line a compile error refers to.
# Headers aren't tracked per file: any change under include/ rebuilds the C objects.
# The pipeline's own inputs count too: a change to a filter script, the compiler settings or this
# Makefile (per-unit settings below) rebuilds the objects instead of leaving stale ones.
# Retail fills a `j` slot our cc1 leaves to the assembler: for gcc 2.8.1 only the frame-only
# epilogue (aspsx_fill.py --jumps-only); the gcc 2.7.2 units below use the whole filler. Not
# ASPSX's doing, though the script is named for it (docs/ARCHITECTURE.md, Toolchain).
ASPSX_FILL ?= | $(PYTHON) tools/scripts/aspsx_fill.py --jumps-only
PIPELINE_DEPS := tools/scripts/dmpsx.py tools/scripts/aspsx_fill.py $(CFG)/project.mk Makefile
build/%.c.o: %.c $(shell find include -name '*.h' 2>/dev/null) $(PIPELINE_DEPS)
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) $< | tee $(@:.o=.i) | $(CC1) $(CC1FLAGS) | $(DMPSX) | $(MASPSX) $(MASPSX_ARGS) $(ASPSX_FILL) | $(AS) $(ASFLAGS) -o $@

# Units built with other settings than project.mk's (decomp.py reads these lines too, see unit_cfg).
# main/8002d488 has every global store as `lui $at` + `sw` and ASPSX-filled jump slots: gcc 2.7.2,
# with Sony's assembler, whose delay-slot filling tools/scripts/aspsx_fill.py does.
build/src/main/8002d488.c.o: CC_VER = 2.7.2-psx
build/src/main/8002d488.c.o: ASPSX_FILL = | $(PYTHON) tools/scripts/aspsx_fill.py
# No second cse pass: with it, `D_8006788C[1]` folds to a direct `sym+1` access; retail keeps `la` +
# offsets (func_8002DE2C, func_8002D4F4). main/8002e0f0 is not built this way (func_8002E25C breaks).
build/src/main/8002d488.c.o: EXTRA_CC1FLAGS = -fno-builtin -fno-rerun-cse-after-loop
# main/8002e0f0 is the same (lui $at stores, addu moves, ASPSX-filled slots; func_8002E618 matches
# only with these), so 2.7.2 runs from 0x8002D488 to 0x8002E7A8.
build/src/main/8002e0f0.c.o: CC_VER = 2.7.2-psx
build/src/main/8002e0f0.c.o: ASPSX_FILL = | $(PYTHON) tools/scripts/aspsx_fill.py

build/%.s.o: %.s
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) -o $@ $<

build/%.bin.o: %.bin
	@mkdir -p $(@D)
	$(LD) -r -b binary -o $@ $<

# ---- Sony's PsyQ objects, linked in place (decomp.py link-psyq) ----
# configs/USA/psyq/sections.ld pins the objects' data sections at their original addresses; it is
# spliced into splat's script ahead of its /DISCARD/. symbols.ld holds symbols the objects place
# differently from the exe, read last so its assignments win.
PSYQ_LD   := $(wildcard $(CFG)/psyq/sections.ld)
FULL_LD   := $(if $(PSYQ_LD),build/$(NAME).full.ld,$(LD_SCRIPT))
PSYQ_SYMS := $(wildcard $(CFG)/psyq/symbols.ld)

# Members are extracted to build/psyq/<lib>/<obj>.o (ld only loads an archive:member reference if
# something else pulled it in), then repaired for GNU ld (HI16 addends left without carry).
PSYQ_LIB  := $(PSYQ_SDK)/lib
PSYQ_OBJS := $(shell sed -n 's/.*, lib, \([^,]*\), \([^,]*\), .*/build\/psyq\/\1\/\2.o/p' $(YAML) | sort -u)

build/psyq/%.o: tools/scripts/psyq_objs.py
	@mkdir -p $(@D)
	cd $(@D) && $(CROSS)ar x $(CURDIR)/$(PSYQ_LIB)/$(notdir $(*D)).a $(notdir $@)
	$(PYTHON) tools/scripts/psyq_objs.py fix $@

build/$(NAME).full.ld: $(LD_SCRIPT) $(PSYQ_LD) Makefile
	@mkdir -p $(@D)
	sed '/^    \/DISCARD\/ :/e cat $(PSYQ_LD)' $(LD_SCRIPT) \
	  | sed 's#$(PSYQ_LIB)/\([A-Za-z0-9_]*\)\.a:\([A-Za-z0-9_]*\.o\)#build/psyq/\1/\2#g' > $@

$(ELF): $(FULL_LD) $(O_FILES) $(PSYQ_OBJS) $(PSYQ_SYMS)
	$(LD) -nostdlib --no-check-sections -Map $(ELF:.elf=.map) \
	  -T $(FULL_LD) -T linkers/undefined_syms_auto.txt -T linkers/undefined_funcs_auto.txt \
	  -T $(CFG)/undefined_syms_manual.txt $(PSYQ_SYMS:%=-T %) -o $@

$(TARGET): $(ELF)
	$(OBJCOPY) -O binary $< $@

# Overlays call into the exe (and each other) at fixed addresses: splat lists those as undefined
# symbols with absolute values, so each overlay links on its own.
build/%.ovl.elf: linkers/%.ld $(O_FILES)
	$(LD) -nostdlib --no-check-sections -Map build/$*.map \
	  -T $< -T linkers/$*_undefined_syms_auto.txt -T linkers/$*_undefined_funcs_auto.txt \
	  -T $(CFG)/undefined_syms_manual.txt $(if $(filter engine,$*),-T $(SLOT_SYMS)) -o $@

# The engine's AST table (src/overlays/engine/asttable.c) names the slot overlays' Init/Plot
# functions: their addresses come from the overlays' own links, so a moved overlay moves its
# entries (tools/scripts/slot_syms.py). The overlays link against the engine's fixed addresses,
# not its ELF, so they can link first.
SLOT_OVLS := $(filter-out engine,$(OVLS))
SLOT_SYMS := build/engine_slot_syms.ld
$(SLOT_SYMS): build/src/overlays/engine/asttable.c.o $(SLOT_OVLS:%=build/%.ovl.elf) tools/scripts/slot_syms.py
	python3 tools/scripts/slot_syms.py $@ build/src/overlays/engine/asttable.c.o -- $(SLOT_OVLS:%=build/%.ovl.elf)
build/engine.ovl.elf: $(SLOT_SYMS)

build/%.bin: build/%.ovl.elf
	$(OBJCOPY) -O binary $< $@

check: build
	@sha1sum -c $(CFG)/checksum.sha

verify:
	@$(MAKE) --no-print-directory clean
	@$(MAKE) --no-print-directory check

# The disc image: the user's retail image with build/main.exe and every build/<ovl>.bin put back
# at their retail sectors. Boots in an emulator; the matching build packs to the retail image.
disc: build
	@$(PYTHON) tools/scripts/pack_disc.py

# The shiftable NON_MATCHING build: OVL padded by PAD bytes before its rodata, text and data,
# the engine relinked against it, packed into build/shift/CrashBash.bin (tools/scripts/shift.py)
OVL ?= ovl5f
PAD ?= 0x40
shift: build
	@$(PYTHON) tools/scripts/shift.py $(OVL) --pad $(PAD)

disc-check: disc
	@sha1sum -c $(CFG)/disc.sha

# the asm filters (dmpsx.py, aspsx_fill.py) against known before/after cases
test-tools:
	@$(PYTHON) -m unittest discover -s tools/scripts/tests

# ---- matching helpers (tools/scripts/decomp.py) ----
diff:
	@test -n "$(FUNC)" || { echo "usage: make diff FUNC=name"; exit 2; }
	@$(DECOMP) try $(FUNC)

MINUTES ?= 15
permute:
	@test -n "$(FUNC)" || { echo "usage: make permute FUNC=name [MINUTES=15] [ARGS=--differing]"; exit 2; }
	@$(DECOMP) permute $(FUNC) --minutes $(MINUTES) $(ARGS)

progress:
	@$(DECOMP) progress

# ---- objdiff ----
# splat runs again with configs/USA/objdiff.yaml merged in, disassembling every function (matched
# ones too) into build/objdiff/asm/. tools/scripts/objdiff_config.py then writes objdiff.json
# (generated, not committed) and build/objdiff/target/<unit>.c (every function of the unit as
# INCLUDE_ASM from there); those compile with the normal pipeline into the target objects objdiff
# compares build/src/<unit>.c.o against.
build/objdiff/target/%.o: build/objdiff/target/%.c
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) $< | $(CC1) $(CC1FLAGS) | $(DMPSX) | $(MASPSX) $(MASPSX_ARGS) | $(AS) $(ASFLAGS) -o $@

objdiff-config: build
	rm -rf build/objdiff/asm build/objdiff/target
	@for y in $(YAML) $(OVL_YAMLS); do $(PYTHON) -m splat split $$y $(CFG)/objdiff.yaml > /dev/null || exit 1; done
	$(PYTHON) tools/scripts/objdiff_config.py
	@$(MAKE) --no-print-directory $$(find build/objdiff/target -name '*.c' | sed 's/[.]c$$/.o/')

# make print-VAR: a variable's value (decomp.py reads CPPFLAGS and ASFLAGS this way)
print-%:
	@echo '$($*)'

# build/ is output only: delete it all. Notes and drafts go in scratch/ (ignored), the game data
# dump in assets/ (dat.py), both outside build/.
clean:
	rm -rf build
