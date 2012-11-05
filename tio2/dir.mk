DIRS += $(OBJDIR)/tio2

tio2_SRC := $(wildcard tio2/*.c) $(wildcard tio2/*.cpp) $(wildcard tio2/*.s)

tio2_OBJ := $(patsubst %.c,$(OBJDIR)/%.o,$(filter %.c,$(tio2_SRC))) $(patsubst %.cpp,$(OBJDIR)/%.o,$(filter %.cpp,$(tio2_SRC))) $(patsubst %.s,$(OBJDIR)/%.o,$(filter %.s,$(tio2_SRC)))

DEP_SRC += $(tio2_SRC)

$(OBJDIR)/libtio2.a: $(tio2_OBJ)
	@echo -e "[AR]\t" $@
	$V$(AR) -rcs $@ $^
