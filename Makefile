# main targets and particle libraries
TARGETS := project0
PARTICLES := tio2
#
# Targte MCPU
MCPU := cortex-m4
MFPU := fpv4-sp-d16
MFLOAT-ABI := softfp

CFLAGS +=   -D PART_LM4F120H5QR -D gcc -DTARGET_IS_BLIZZARD_RA2 -Os

STELLARISWARE := /home/sebastian/build/stellarisware
DRIVERLIB := $(STELLARISWARE)/driverlib/gcc-cm4f/libdriver-cm4f.a

# default compiler/linker flags
override CPPFLAGS += -I include -I $(STELLARISWARE)

ifdef RELEASE
else
endif

CCFLAGS += -ffunction-sections -fdata-sections -Wall -pedantic -std=c99
CFLAGS += $(CCFLAGS)
CXXFLAGS += $(CCFLAGS)

LDFLAGS +=

# default values for internal variables
AS := arm-none-eabi-as
CC := arm-none-eabi-gcc
CXX := arm-none-eabi-g++
LD := arm-none-eabi-ld
CPP := $(CXX) -E
SED := sed
FIND := find
DOXYGEN := doxygen
CP := cp
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
NM := arm-none-eabi-nm

LMFLASH := /home/sebastian/projects/stellaris/tools/lm4tools/lm4flash/lm4flash

OBJDIR = obj
BINDIR = bin

# extensions of files considered to be code
# supported so far:
# C++: cpp
# C: c
# Assembler: S
override CODE_EXTS := c cpp s

# external libraries used
override LIBRARIES :=
override LIBRARIES_WITHOUT_PKGCONFIG :=

# library version requirements
define LIBRARY_VERSION_CHECK =
endef


###
# here be internals
###

#ifdef V
#override V :=
#else
#V := @
#endif

# result: cleaned name of particle
# argument 1: makefile name or directory name of particle root
define submk_name =
$(subst /,_,$(subst _,__,$(1:/dir.mk=)))
endef

# result: particles library filename
# argument 1: makefile name or directory name of particle root
define sublib_name =
lib$(call submk_name,$1).a
endef

# result: shell code to check for existence of a certain system header. generates a return code.
# argument 1: name of a header file required by the program
define require_header =
(echo '#include <$1>' | $(CPP) - >/dev/null 2>&1)
endef

# result: shell code to check for existence of a certain symbol. generates a return code.
# argument 1: name of a header file containing the symbol
# argument 2: name of the symbol
define require_symbol_definition =
((echo '#include <$1>'; echo '#ifndef $2'; echo '#error'; echo '#endif') | $(CPP) - >/dev/null 2>&1)
endef

ifndef MAKE_RESTARTS
$(shell touch -r Makefile -d yesterday .depend-check)
include .depend-check
else
$(shell $(RM) .depend-check)
DEPEND_CHECK_DONE := 1
endif



SRC := $(foreach ext,$(CODE_EXTS),$(wildcard *.$(ext)))

DEP_SRC := $(SRC)

DIRS := $(BINDIR) $(OBJDIR) $(subst ./,,$(sort $(patsubst %,$(OBJDIR)/%,$(dir $(DEP_SRC)))))

PARTICLE_MAKEFILES := $(patsubst %,%/dir.mk,$(PARTICLES))

TARGET_ELF := $(patsubst %,$(BINDIR)/%.elf,$(TARGETS))
TARGET_BIN := $(patsubst %,$(BINDIR)/%.bin,$(TARGETS))
TARGET_LSS := $(patsubst %,$(BINDIR)/%.lss,$(TARGETS))
TARGET_SYM := $(patsubst %,$(BINDIR)/%.sym,$(TARGETS))

all: $(TARGET_ELF) $(TARGET_BIN) $(TARGET_LSS) $(TARGET_SYM)

# result: text of rule to program a target to a device
# argument 1: target name
define def_program =
@echo 'program-$1: $$(BINDIR)/$1.bin' > program-$1.mk
@echo '		@echo -e "[PROG]\t" $1' >> program-$1.mk
@echo '		$$(LMFLASH) $$(BINDIR)/$1.bin' >> program-$1.mk
endef

program-%.mk: Makefile | $(DIRS)
	@echo -e '[PMK]\t' $*
	$(call def_program,$*)

PROGRAM_MAKEFILES := $(patsubst %,program-%.mk,$(TARGETS))

-include $(PROGRAM_MAKEFILES)

ifeq "$(word 2,$(TARGETS))" ""
program: program-$(TARGETS)
endif


.depend-check: Makefile
	@$(LIBRARY_VERSION_CHECK)
	@touch $@

deps:

ifdef DEPEND_CHECK_DONE
-include $(PARTICLE_MAKEFILES)
-include $(foreach ext,$(CODE_EXTS),$(patsubst %.$(ext),$(OBJDIR)/%.o.d,$(filter %.$(ext),$(DEP_SRC))))
endif

PARTICLE_LIBRARY_NAMES := $(foreach lib,$(PARTICLES),$(call sublib_name,$(lib)))
PARTICLE_LIBRARIES := $(foreach lib,$(PARTICLES),-l$(call submk_name,$(lib)))

override LDFLAGS += -L $(OBJDIR)
ifneq (,$(LIBRARIES))
override CXXFLAGS += `pkg-config --cflags $(LIBRARIES)`
override LDFLAGS += `pkg-config --libs $(LIBRARIES)`
endif
override LDFLAGS += $(patsubst %,-l%,$(LIBRARIES_WITHOUT_PKGCONFIG)) -Wl,--whole-archive -Wl,--start-group $(PARTICLE_LIBRARIES) -Wl,--end-group -Wl,--no-whole-archive
override LDFLAGS += -Wl,--gc-sections -Wl,--entry,ResetISR $(DRIVERLIB)

override CFLAGS += -mthumb -mcpu=$(MCPU) -mfpu=$(MFPU) -mfloat-abi=$(MFLOAT-ABI)
override CXXFLAGS += -mthumb -mcpu=$(MCPU) -mfpu=$(MFPU) -mfloat-abi=$(MFLOAT-ABI)
override ASFLAGS += -mcpu=$(MCPU)


$(BINDIR)/%.elf: $(OBJDIR)/%.o $(patsubst %,$(OBJDIR)/%,$(PARTICLE_LIBRARY_NAMES)) | $(BINDIR)
	@echo -e "[LD]\t" $@
	$V$(CXX) -o $@ $<  -T $$(basename ${@:.elf=.ld}) $(LDFLAGS)

$(BINDIR)/%.bin: $(BINDIR)/%.elf | $(BINDIR)
	@echo -e "[BIN]\t" $@
	$V$(OBJCOPY) -O binary  $< $@

$(BINDIR)/%.lss: $(BINDIR)/%.elf | $(BINDIR)
	@echo -e "[LSS]\t" $@
	$V$(OBJDUMP) -h -S $< > $@

$(BINDIR)/%.sym: $(BINDIR)/%.elf | $(BINDIR)
	@echo -e "[SYM]\t" $@
	$V$(NM) -n $< > $@

.SECONDARY: $(patsubst %,$(OBJDIR)/%.o,$(TARGETS))

clean:
	-$(RM) -r $(OBJDIR) $(BINDIR) $(PARTICLE_MAKEFILES) $(PROGRAM_MAKEFILES)

depclean:
	-$(FIND) $(OBJDIR) -iname '*.d' -delete

distclean:
	-$(RM) -r $(BINDIR)

$(OBJDIR)/%.o: %.cpp Makefile | $(DIRS)
	@echo -e "[CXX]\t" $<
	$V$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
	$(call generate_depfile,$<,$@,)

$(OBJDIR)/%.o: %.c Makefile | $(DIRS)
	@echo -e "[CC]\t" $<
	$V$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
	$(call generate_depfile,$<,$@,)

$(OBJDIR)/%.o: %.S Makefile | $(DIRS)
	@echo -e "[AS]\t" $<
	$V$(AS) $(ASFLAGS) -o $@ $<

$(DIRS):
	@mkdir -p $@

$(PARTICLE_MAKEFILES): Makefile
	@echo -e "[GEN]\t" $@
	$(call generate_subdir_makefile,$@)

html-doc:
	$V$(DOXYGEN) doc/doxy.cfg

.PHONY: clean distclean depclean $(DIRS) deps all html-doc program $(patsubst %,program-%,$(TARGETS))


# result: shell command to create a dependency file
# argument 1: input source file
# argument 2: output object file
# argument 3: extra cpp flags
define generate_depfile =
	$V$(CPP) -MM -MP -MT $2 $(CPPFLAGS) $3 $1 > $2.d \
		&& $(SED) -e 's@^\(.*\)\.o:@\1.d \1.o:@' -i $2.d
endef

# result: shell commands to create a particle Makefile
# argument 1: directory
define generate_subdir_makefile =
	@echo 'DIRS += $$(OBJDIR)/$(1:/dir.mk=)' > $1
	@echo >> $1
	@echo '$(call submk_name,$1)_SRC := $(foreach ext,$(CODE_EXTS),$$(wildcard $(dir $1)*.$(ext)))' >> $1
	@echo >> $1
	@echo '$(call submk_name,$1)_OBJ := $(foreach ext,$(CODE_EXTS),$$(patsubst %.$(ext),$$(OBJDIR)/%.o,$$(filter %.$(ext),$$($(call submk_name,$1)_SRC))))' >> $1
	@echo >> $1
	@echo 'DEP_SRC += $$($(call submk_name,$1)_SRC)' >> $1
	@echo >> $1
	@echo '$$(OBJDIR)/$(call sublib_name,$1): $$($(call submk_name,$1)_OBJ)' >> $1
	@echo '	@echo -e "[AR]\t" $$@' >> $1
	@echo '	$$V$$(AR) -rcs $$@ $$^' >> $1
endef
