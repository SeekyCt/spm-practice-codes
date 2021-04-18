#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

ifeq ($(strip $(TTYDTOOLS)),)
$(error "Please set TTYDTOOLS in your environment. export TTYDTOOLS=<path to>ttyd-tools")
endif

include $(DEVKITPPC)/wii_rules

export ELF2REL	:=	$(TTYDTOOLS)/bin/elf2rel

ifeq ($(VERSION),)
all: us0 us1 us2 jp0 jp1 eu0 eu1 kr0
us0:
	@$(MAKE) --no-print-directory VERSION=us0
us1:
	@$(MAKE) --no-print-directory VERSION=us1
us2:
	@$(MAKE) --no-print-directory VERSION=us2
jp0:
	@$(MAKE) --no-print-directory VERSION=jp0
jp1:
	@$(MAKE) --no-print-directory VERSION=jp1
eu0:
	@$(MAKE) --no-print-directory VERSION=eu0
eu1:
	@$(MAKE) --no-print-directory VERSION=eu1
kr0:
	@$(MAKE) --no-print-directory VERSION=kr0

clean:
	@$(MAKE) --no-print-directory clean_target VERSION=us0
	@$(MAKE) --no-print-directory clean_target VERSION=us1
	@$(MAKE) --no-print-directory clean_target VERSION=us2
	@$(MAKE) --no-print-directory clean_target VERSION=jp0
	@$(MAKE) --no-print-directory clean_target VERSION=jp1
	@$(MAKE) --no-print-directory clean_target VERSION=eu0
	@$(MAKE) --no-print-directory clean_target VERSION=eu1
	@$(MAKE) --no-print-directory clean_target VERSION=kr0

.PHONY: all clean us0 us1 us2 jp0 jp1 eu0 eu1 kr0
else

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR)).$(VERSION)
BUILD		:=	build.$(VERSION)
SOURCES		:=	source $(wildcard source/*)
DATA		:=	data  
INCLUDES	:=	include

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

MACHDEP		= -mno-sdata -mgcn -DGEKKO -mcpu=750 -meabi -mhard-float

CFLAGS		= -nostdlib -ffreestanding -ffunction-sections -fdata-sections -g -O3 -Wall $(MACHDEP) $(INCLUDE)
CXXFLAGS	= -fno-exceptions -fno-rtti -std=gnu++17 $(CFLAGS)

LDFLAGS		= -r -e _prolog -u _prolog -u _epilog -u _unresolved -Wl,--gc-sections -nostdlib -g $(MACHDEP) -Wl,-Map,$(notdir $@).map

# Platform options
ifeq ($(VERSION),us0)
	CFLAGS += -DSPM_US0
	GAMECODE = "R8PE"
else ifeq ($(VERSION),us1)
	CFLAGS += -DSPM_US1
	GAMECODE = "R8PE"
else ifeq ($(VERSION),us2)
	CFLAGS += -DSPM_US2
	GAMECODE = "R8PE"
else ifeq ($(VERSION),jp0)
	CFLAGS += -DSPM_JP0
	GAMECODE = "R8PJ"
else ifeq ($(VERSION),jp1)
	CFLAGS += -DSPM_JP1
	GAMECODE = "R8PJ"
else ifeq ($(VERSION),eu0)
	CFLAGS += -DSPM_EU0
	GAMECODE = "R8PP"
else ifeq ($(VERSION),eu1)
	CFLAGS += -DSPM_EU1
	GAMECODE = "R8PP"
else ifeq ($(VERSION),kr0)
	CFLAGS += -DSPM_KR0
	GAMECODE = "R8PK"
endif

# PAL revision 1's dol is identical to revision 0, so no need for another lst
ifeq ($(VERSION),eu1)
	LST = eu0
else
	LST = $(VERSION)
endif

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
#LIBS	:= -lm

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(sFILES:.s=.o) $(SFILES:.S=.o)
export OFILES := $(OFILES_BIN) $(OFILES_SOURCES)

export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES)))

# For REL linking
export LDFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.ld)))
export MAPFILE		:= $(CURDIR)/include/spm.$(LST).lst

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD) \
			-I$(LIBOGC_INC)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
			-L$(LIBOGC_LIB)

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean_target

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean_target:
	@echo clean ... $(VERSION)
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol $(OUTPUT).rel

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).rel: $(OUTPUT).elf $(MAPFILE)
$(OUTPUT).elf: $(LDFILES) $(OFILES)

$(OFILES_SOURCES) : $(HFILES)

# REL linking
%.rel: %.elf
	@echo output ... $(notdir $@)
	@$(ELF2REL) $< -s $(MAPFILE)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .jpg extension
#---------------------------------------------------------------------------------
%.jpg.o	%_jpg.h :	%.jpg
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	$(bin2o)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
endif