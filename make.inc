#
# Re-usable build rules for projects using lpc11xx_classlib
#
# Set BIN to build a binary.
# Set LIB to build a static library.
# Set CHIP to the chip to be supported, one of LPC1114FN28 (add more)
# Set SRCS to source files to be built.
# Set PORT to a serial port to upload to.
# Set INCLUDES to a list of include paths.
# Set DEFINES to a list of symbols to define.
# Set LIBS with BIN to link additional libraries.
# Set EXTRA_FMT_SRCS to reformat other source files (e.g. headers)
#
# Targets:
# $(BIN)	builds the binary if BIN was set
# $(LIB)	builds the library if LIB was set
# upload	builds and uploads the binary if BIN was set
# clean		cleans out the build directory
# reformat	reformats $(SRCS) and $(EXTRA_FMT_SRCS)
#

# Sanity-check variables
ifeq ($(SRCS),)
$(error No source files listed in SRCS)
endif
ifeq ($(if $(filter upload,$(MAKECMDGOALS)),$(PORT),NOT_REQUIRED),)
$(error Must set PORT before attempting to upload)
endif

# Computed paths
_LIBDIR		:= $(dir $(lastword $(MAKEFILE_LIST)))
_OBJS		 = $(foreach src,$(SRCS),$(_OBJDIR)/$(src).o)
_DEPS		 = $(_OBJS:.o=.d)

# Tools
CC		 = arm-none-eabi-gcc
AR		 = arm-none-eabi-ar
OBJCOPY		 = arm-none-eabi-objcopy
SIZE		 = arm-none-eabi-size
PROGRAMMER	 = /Users/msmith/work/github/lpc21isp/lpc21isp
ASTYLE		 = astyle

# Common compile arguments
EXTRA_DEPS	+= $(MAKEFILE_LIST)
INCLUDES	+= $(_LIBDIR)/include \
		   $(_LIBDIR)/include/CMSIS \
		   $(_LIBDIR)/etl/include
DEFINES		+= $(CHIP)

# Compiler options
_ARCH_FLAGS	 = -march=armv6-m \
		   -mthumb \
		   -mcpu=cortex-m0plus

_COMMON_FLAGS    = $(addprefix -D,$(DEFINES)) \
		   $(addprefix -I,$(INCLUDES)) \
		   $(_ARCH_FLAGS) \
		   -ffunction-sections \
		   -fno-builtin \
		   -ffreestanding \
		   -MD -MP \
		   -Os \
		   -g \
		   -Wall

_CFLAGS      	 = -std=gnu11 \
		   $(CFLAGS)

_CXXFLAGS	 = -std=gnu++11 \
		   -fno-exceptions \
		   -fno-rtti \
		   $(CXXFLAGS)

_LDFLAGS	= $(_ARCH_FLAGS) \
		  -nostartfiles \
		  -nostdlib \
		  -Wl,-gc-sections \
		  -L/usr/local/arm-none-eabi/lib \
		  -L$(_LIBDIR) \
		  -Wl,-Map,$@.map \
		  --specs=nano.specs \
		  $(LIBS) \
		  -lgcc \
		  $(LDFLAGS)

#
# If building a binary
#
ifneq ($(BIN),)
_ELF		 = $(BIN:.bin=.elf)
_OBJDIR		 = $(dir $(abspath $(BIN)))
_LDSCRIPT	 = $(_LIBDIR)/$(CHIP).ld

$(BIN): $(_ELF) $(_EXTRA_DEPS)
	@echo $@
	@$(OBJCOPY) -Obinary $< $@

$(_ELF): $(_OBJS) $(_EXTRA_DEPS) $(_LDSCRIPT)
	@echo $@
	@$(CC) -o $@ $(_OBJS) $(_LDFLAGS) -T$(_LDSCRIPT)
	@$(SIZE) $@

.PHONY: upload
upload: $(BIN)
	$(PROGRAMMER) -bin -control -verify -term $< $(PORT) 115200 12000

endif

#
# If building a library
#
ifneq ($(LIB),)
_OBJDIR		 = $(dir $(abspath $(LIB)))

$(LIB): $(_OBJS) $(_EXTRA_DEPS)
	@echo $@
	@$(AR) ru $@ $(_OBJS)
endif


#
# Rules for building object files.
#
$(filter %.c.o,$(_OBJS)): $(_OBJDIR)/%.o: % $(_EXTRA_DEPS)
	@echo $@
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(_COMMON_FLAGS) $(_CFLAGS)

$(filter %.cpp.o,$(_OBJS)): $(_OBJDIR)/%.o: % $(_EXTRA_DEPS)
	@echo $@
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(_COMMON_FLAGS) $(_CXXFLAGS)

#
# Utility rules
#
.PHONY: clean reformat

clean:
	rm -rf $(_OBJDIR)

reformat:
	@$(ASTYLE) \
	--style=kr \
	--indent=spaces=4 \
	--indent-cases \
	--indent-preprocessor \
	--break-blocks \
	--pad-oper \
	--pad-header \
	--unpad-paren \
	--add-brackets \
	--convert-tabs \
	--align-pointer=name \
	--keep-one-line-blocks \
	--formatted \
	--suffix=none \
	$(wildcard $(SRCS)) \
	$(wildcard $(EXTRA_FMT_SRCS))

-include $(_DEPS)
