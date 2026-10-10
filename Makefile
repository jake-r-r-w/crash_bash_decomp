# PSX matching decomp build. Runs inside the toolchain container: tools/dk make [target]
#   make split   - run splat on every config/*.yaml (regenerates asm/, assets/, linker scripts)
#   make         - build build/$(NAME).exe and every overlay build/<ovl>.bin
#   make check   - build and compare sha1 of every target against the original
#   make objdiff - objdiff.json + the original (all-asm) object of every unit, for objdiff
#   make report  - objdiff progress report in build/report.json (the format decomp.dev reads)
SHELL := /bin/bash -o pipefail
.DEFAULT_GOAL := all

include config/project.mk
# project.mk provides: NAME, EXE, CC_VER, OPT_FLAGS, GVAL, MASPSX_ARGS, EXTRA_CC1FLAGS

CROSS   := mipsel-linux-gnu-
AS      := $(CROSS)as
LD      := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy
CPP     := $(CROSS)cpp
CC1     := /opt/gcc/$(CC_VER)/cc1
MASPSX  := python3 /opt/maspsx/maspsx.py

CPPFLAGS := -P -nostdinc -undef -Iinclude -Iext/psyq47/include -D__GNUC__=2 -D_LANGUAGE_C -DPSX -DGVAL=$(GVAL)
CC1FLAGS := -quiet $(OPT_FLAGS) -G$(GVAL) $(EXTRA_CC1FLAGS)
ASFLAGS  := -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0

YAML      := config/$(NAME).yaml
LD_SCRIPT := build/$(NAME).ld
ELF       := build/$(NAME).elf
TARGET    := build/$(NAME).exe

# Overlays: every other config/<ovl>.yaml is a raw binary loaded at runtime (see decomp.py add-overlay).
# Each links from build/<ovl>.ld into build/<ovl>.bin and is checked against config/<ovl>.sha1.
OVL_YAMLS := $(filter-out $(YAML),$(wildcard config/*.yaml))
OVLS      := $(basename $(notdir $(OVL_YAMLS)))
OVL_LDS   := $(OVLS:%=build/%.ld)
OVL_BINS  := $(OVLS:%=build/%.bin)

C_FILES   := $(shell find src -name '*.c' 2>/dev/null)
S_FILES   := $(shell find asm -name '*.s' -not -path 'asm/nonmatchings/*' 2>/dev/null)
BIN_FILES := $(shell find assets -name '*.bin' 2>/dev/null)
O_FILES   := $(C_FILES:%=build/%.o) $(S_FILES:%=build/%.o) $(BIN_FILES:%=build/%.o)

.PHONY: all split check clean objdiff report

# Two passes: splat must have produced asm/ before the object list above is computed.
all:
	@$(MAKE) --no-print-directory $(LD_SCRIPT) $(OVL_LDS)
	@$(MAKE) --no-print-directory $(TARGET) $(OVL_BINS)

split:
	@for y in $(YAML) $(OVL_YAMLS); do python3 -m splat split $$y || exit 1; done

$(LD_SCRIPT): $(YAML) config/symbol_addrs.txt
	python3 -m splat split $(YAML)

build/%.ld: config/%.yaml config/symbol_addrs.txt
	python3 -m splat split $<

# .i is kept next to the object so tools/decomp.py can show the line a compile error refers to.
# headers aren't tracked per file: any change under include/ rebuilds the C objects
build/%.c.o: %.c $(wildcard include/*.h)
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) $< | tee $(@:.o=.i) | $(CC1) $(CC1FLAGS) | $(MASPSX) $(MASPSX_ARGS) | $(AS) $(ASFLAGS) -o $@

build/%.s.o: %.s
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) -o $@ $<

build/%.bin.o: %.bin
	@mkdir -p $(@D)
	$(LD) -r -b binary -o $@ $<

# Sony's PsyQ objects linked in place (decomp.py link-psyq): their data sections are pinned at the
# original addresses by config/psyq_sections.ld, spliced into splat's script ahead of its /DISCARD/.
PSYQ_LD := $(wildcard config/psyq_sections.ld)
FULL_LD := $(if $(PSYQ_LD),build/$(NAME).full.ld,$(LD_SCRIPT))
# symbols the objects place differently from the exe; read last so these assignments win
PSYQ_SYMS := $(wildcard config/psyq_symbols.ld)

# Members are extracted to build/psyq/<lib>/<obj>.o: a plain object path in a linker script is
# loaded by ld, an archive:member one only if something else pulled the member in. Each is then
# repaired for GNU ld (tools/psyq_objs.py fix: HI16 addends the converter left without carry).
PSYQ_DIR  := ext/psyq47/lib
PSYQ_OBJS := $(shell sed -n 's/.*, lib, \([^,]*\), \([^,]*\), .*/build\/psyq\/\1\/\2.o/p' $(YAML) | sort -u)

build/psyq/%.o: tools/psyq_objs.py
	@mkdir -p $(@D)
	cd $(@D) && $(CROSS)ar x $(CURDIR)/$(PSYQ_DIR)/$(notdir $(*D)).a $(notdir $@)
	python3 tools/psyq_objs.py fix $@

build/$(NAME).full.ld: $(LD_SCRIPT) $(PSYQ_LD) Makefile
	sed '/^    \/DISCARD\/ :/e cat $(PSYQ_LD)' $(LD_SCRIPT) \
	  | sed 's#$(PSYQ_DIR)/\([A-Za-z0-9_]*\)\.a:\([A-Za-z0-9_]*\.o\)#build/psyq/\1/\2#g' > $@

$(ELF): $(FULL_LD) $(O_FILES) $(PSYQ_OBJS) $(PSYQ_SYMS)
	$(LD) -nostdlib --no-check-sections -Map $(ELF:.elf=.map) \
	  -T $(FULL_LD) -T build/undefined_syms_auto.txt -T build/undefined_funcs_auto.txt \
	  -T config/undefined_syms.txt $(PSYQ_SYMS:%=-T %) -o $@

$(TARGET): $(ELF)
	$(OBJCOPY) -O binary $< $@

# Overlays call into the main exe (and each other) at fixed addresses: splat lists those as
# undefined symbols with absolute values, so each overlay links on its own.
build/%.ovl.elf: build/%.ld $(O_FILES)
	$(LD) -nostdlib --no-check-sections -Map build/$*.map \
	  -T $< -T build/$*_undefined_syms_auto.txt -T build/$*_undefined_funcs_auto.txt \
	  -T config/undefined_syms.txt -o $@

build/%.bin: build/%.ovl.elf
	$(OBJCOPY) -O binary $< $@

check: all
	@sha1sum -c config/$(NAME).sha1 $(OVLS:%=config/%.sha1)

# objdiff: tools/objdiff_gen.py writes objdiff.json and build/objdiff/target/<unit>.c (every
# function of the unit as INCLUDE_ASM); those compile with the normal pipeline into the target
# objects objdiff compares build/src/<unit>.c.o against.
OBJDIFF_VER := v3.8.2
OBJDIFF_CLI := ext/objdiff/objdiff-cli-$(OBJDIFF_VER)

build/objdiff/target/%.o: build/objdiff/target/%.c
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) $< | $(CC1) $(CC1FLAGS) | $(MASPSX) $(MASPSX_ARGS) | $(AS) $(ASFLAGS) -o $@

objdiff: all
	python3 tools/objdiff_gen.py
	@$(MAKE) --no-print-directory $$(find build/objdiff/target -name '*.c' | sed 's/[.]c$$/.o/')

$(OBJDIFF_CLI):
	@mkdir -p $(@D)
	curl -sSfL -o $@ https://github.com/encounter/objdiff/releases/download/$(OBJDIFF_VER)/objdiff-cli-linux-x86_64
	chmod +x $@

report: objdiff $(OBJDIFF_CLI)
	$(OBJDIFF_CLI) report generate -o build/report.json
	@python3 -c "import json;m=json.load(open('build/report.json'))['measures'];print('objdiff: code %.2f%% (%s/%s bytes), functions %s/%s'%(m.get('matched_code_percent',0),m.get('matched_code',0),m.get('total_code',0),m.get('matched_functions',0),m.get('total_functions',0)))"

clean:
	rm -rf build
