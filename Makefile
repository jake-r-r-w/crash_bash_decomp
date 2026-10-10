# Crash Bash (SCUS_945.70) matching decomp.
# Runs inside the toolchain container (tools/dk make <target>) or natively with the tools on PATH.
#
#   make split           - splat every configs/USA yaml (regenerates asm/ and linkers/)
#   make build           - compile and link build/main.exe and every overlay build/<ovl>.bin
#   make check           - build, then compare every target with configs/USA/checksum.sha
#   make verify          - clean rebuild + check (use after structural/config/header changes)
#   make diff FUNC=name  - build one function's file and diff it against the original
#   make permute FUNC=name [MINUTES=15] - run decomp-permuter on one function
#   make progress        - dashboard (docs/index.html), docs/progress.json and docs/badges/
#   make objdiff-config  - objdiff.json + the original (all-asm) object of every unit
#   make report          - objdiff report in build/report.json
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
CC1FLAGS := -quiet $(OPT_FLAGS) -G$(GVAL) $(EXTRA_CC1FLAGS)
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
S_FILES   := $(shell find asm -name '*.s' -not -path 'asm/nonmatchings/*' 2>/dev/null)
BIN_FILES := $(shell find assets -name '*.bin' 2>/dev/null)
O_FILES   := $(C_FILES:%=build/%.o) $(S_FILES:%=build/%.o) $(BIN_FILES:%=build/%.o)

.PHONY: all build split check verify diff permute progress objdiff-config report clean

# Two passes: splat must have produced asm/ before the object list above is computed.
build all:
	@$(MAKE) --no-print-directory $(LD_SCRIPT) $(OVL_LDS)
	@$(MAKE) --no-print-directory $(TARGET) $(OVL_BINS)

split:
	@for y in $(YAML) $(OVL_YAMLS); do $(PYTHON) -m splat split $$y || exit 1; done

$(LD_SCRIPT): $(YAML) $(CFG)/sym.main.txt
	$(PYTHON) -m splat split $(YAML)

linkers/%.ld: $(CFG)/overlays/%.yaml $(CFG)/sym.main.txt
	$(PYTHON) -m splat split $<

# .i is kept next to the object so decomp.py can show the line a compile error refers to.
# Headers aren't tracked per file: any change under include/ rebuilds the C objects.
build/%.c.o: %.c $(shell find include -name '*.h' 2>/dev/null)
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) $< | tee $(@:.o=.i) | $(CC1) $(CC1FLAGS) | $(DMPSX) | $(MASPSX) $(MASPSX_ARGS) | $(AS) $(ASFLAGS) -o $@

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
	  -T $(CFG)/undefined_syms_manual.txt -o $@

build/%.bin: build/%.ovl.elf
	$(OBJCOPY) -O binary $< $@

check: build
	@sha1sum -c $(CFG)/checksum.sha

verify:
	@$(MAKE) --no-print-directory clean
	@$(MAKE) --no-print-directory check

# ---- matching helpers (tools/scripts/decomp.py) ----
diff:
	@test -n "$(FUNC)" || { echo "usage: make diff FUNC=name"; exit 2; }
	@$(DECOMP) try $(FUNC)

MINUTES ?= 15
permute:
	@test -n "$(FUNC)" || { echo "usage: make permute FUNC=name [MINUTES=15]"; exit 2; }
	@$(DECOMP) permute $(FUNC) --minutes $(MINUTES)

progress:
	@$(DECOMP) progress

# ---- objdiff ----
# splat runs again with configs/USA/objdiff.yaml merged in, disassembling every function (matched
# ones too) into build/objdiff/asm/. tools/scripts/objdiff_config.py then writes objdiff.json
# (generated, not committed) and build/objdiff/target/<unit>.c (every function of the unit as
# INCLUDE_ASM from there); those compile with the normal pipeline into the target objects objdiff
# compares build/src/<unit>.c.o against.
OBJDIFF_VER := v3.8.2
OBJDIFF_CLI := ext/objdiff/objdiff-cli-$(OBJDIFF_VER)

build/objdiff/target/%.o: build/objdiff/target/%.c
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) $< | $(CC1) $(CC1FLAGS) | $(DMPSX) | $(MASPSX) $(MASPSX_ARGS) | $(AS) $(ASFLAGS) -o $@

objdiff-config: build
	rm -rf build/objdiff/asm build/objdiff/target
	@for y in $(YAML) $(OVL_YAMLS); do $(PYTHON) -m splat split $$y $(CFG)/objdiff.yaml > /dev/null || exit 1; done
	$(PYTHON) tools/scripts/objdiff_config.py
	@$(MAKE) --no-print-directory $$(find build/objdiff/target -name '*.c' | sed 's/[.]c$$/.o/')

$(OBJDIFF_CLI):
	@mkdir -p $(@D)
	curl -sSfL -o $@ https://github.com/encounter/objdiff/releases/download/$(OBJDIFF_VER)/objdiff-cli-linux-x86_64
	chmod +x $@

report: objdiff-config $(OBJDIFF_CLI)
	$(OBJDIFF_CLI) report generate -o build/report.json
	@$(PYTHON) -c "import json;m=json.load(open('build/report.json'))['measures'];print('objdiff: code %.2f%% (%s/%s bytes), functions %s/%s'%(m.get('matched_code_percent',0),m.get('matched_code',0),m.get('total_code',0),m.get('matched_functions',0),m.get('total_functions',0)))"

clean:
	rm -rf build
