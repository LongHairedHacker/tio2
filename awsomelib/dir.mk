DIRS += $(OBJDIR)/awsomelib

awsomelib_SRC := $(wildcard awsomelib/*.c) $(wildcard awsomelib/*.cpp) $(wildcard awsomelib/*.s)

awsomelib_OBJ := $(patsubst %.c,$(OBJDIR)/%.o,$(filter %.c,$(awsomelib_SRC))) $(patsubst %.cpp,$(OBJDIR)/%.o,$(filter %.cpp,$(awsomelib_SRC))) $(patsubst %.s,$(OBJDIR)/%.o,$(filter %.s,$(awsomelib_SRC)))

DEP_SRC += $(awsomelib_SRC)

$(OBJDIR)/libawsomelib.a: $(awsomelib_OBJ)
	@echo -e "[AR]\t" $@
	$V$(AR) -rcs $@ $^
