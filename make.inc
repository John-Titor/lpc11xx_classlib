#
# Re-usable build rules for projects using lpc11xx_classlib
#
# Set BIN to the binary to be built.
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
# scmRTOS
#
# Thread main routines are main0..mainN for N+1 processes (max N is 8);
#
# Options in DEFINES:
#
# scmRTOS_PROCESS_COUNT		Set the number of processes. If 0, scmRTOS is not
#				enabled and main() must be supplied.
# scmRTOS_PROCn_STACKSIZE	Set the stack size for process 'n'. If not set,
#				the default stack size will be used.
# scmRTOS_DEFAULT_STACKSIZE	Set the default stack size, defaults to 512.
#

# Sanity-check variables
ifeq ($(BIN),)
$(error No binary to build)
endif
ifeq ($(SRCS),)
$(error No source files listed in SRCS)
endif
ifeq ($(if $(filter upload,$(MAKECMDGOALS)),$(PORT),NOT_REQUIRED),)
$(error Must set PORT before attempting to upload)
endif

ifeq ($(VERBOSE),)
_V		 = @
endif

# Computed paths
_PROJDIR	:= $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
_LIBDIR		:= $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
_OBJDIR		:= $(dir $(abspath $(BIN)))

# Library sources
_SRCDIRS	 = src 
_INCDIRS	 = include \
		   include/CMSIS \
		   etl/include

ifneq ($(findstring scmRTOS_PROCESS_COUNT,$(DEFINES)),)
$(info WITH_SCMRTOS)
_SRCDIRS	+= scmrtos/core \
		   scmrtos/ext \
		   scmrtos/port/cortex/mx-gcc
_INCDIRS	+= scmrtos/core scmrtos/port/cortex/mx-gcc
endif

# Tools
CC		 = arm-none-eabi-gcc
AR		 = arm-none-eabi-ar
OBJCOPY		 = arm-none-eabi-objcopy
SIZE		 = arm-none-eabi-size
PROGRAMMER	 = /Users/msmith/work/github/lpc21isp/lpc21isp
ASTYLE		 = astyle

# Common compile arguments
EXTRA_DEPS	+= $(MAKEFILE_LIST)
_INCLUDES	+= $(foreach dir,$(_INCDIRS),-I$(_LIBDIR)$(dir))
DEFINES		+= $(CHIP)
$(info _INCDIRS $(_INCDIRS))
$(info _INCLUDES $(_INCLUDES))

# Compiler options
_ARCH_FLAGS	 = -march=armv6-m \
		   -mthumb \
		   -mcpu=cortex-m0plus

_COMMON_FLAGS    = $(addprefix -D,$(DEFINES)) \
		   $(_INCLUDES) \
		   $(_ARCH_FLAGS) \
		   -ffunction-sections \
		   -fdata-sections \
		   -fno-builtin \
		   -ffreestanding \
		   -flto \
		   -MD -MP \
		   -Os \
		   -g \
		   -Wall

_CFLAGS      	 = -std=gnu11 \
		   $(CFLAGS)

_CXXFLAGS	 = -std=gnu++14 \
		   -fno-exceptions \
		   -fno-rtti \
		   $(CXXFLAGS)

_LDFLAGS	 = $(_ARCH_FLAGS) \
		   -nostartfiles \
		   -Wl,-gc-sections \
		   -Wl,-flto \
		   -L/usr/local/arm-none-eabi/lib \
		   -L$(_LIBDIR) \
		   -Wl,-Map,$@.map \
		   --specs=nano.specs \
		   $(LIBS) \
		   -lgcc \
		   $(LDFLAGS)


_ELF		 = $(BIN:.bin=.elf)
_LDSCRIPT	 = $(_LIBDIR)$(CHIP).ld
_OBJS		 = $(foreach src,$(SRCS),$(_OBJDIR)$(src).o)
_DEPS		 = $(_OBJS:.o=.d)

SRCS           	+= $(foreach dir,$(_SRCDIRS),$(wildcard $(_LIBDIR)$(dir)/*.cpp) $(wildcard $(_LIBDIR)$(dir)/*.c))

#$(info SRCS $(notdir $(SRCS)))
#$(info OBJS $(notdir $(_OBJS)))

$(BIN): $(_ELF) $(_EXTRA_DEPS)
	@echo BINARY $(notdir $@)
	$(_V)$(OBJCOPY) -Obinary $< $@

$(_ELF): $(_OBJS) $(_EXTRA_DEPS) $(_LDSCRIPT)
	@echo ELF $(notdir $@)
	$(_V)$(CC) -o $@ $(_OBJS) $(_LDFLAGS) -T$(_LDSCRIPT)
	$(_V)$(SIZE) $@

.PHONY: upload
upload: $(BIN)
	$(PROGRAMMER) -bin -control -verify -term $< $(PORT) 115200 12000

#
# Rules for building object files.
#
$(filter %.c.o,$(_OBJS)): $(_OBJDIR)%.o: % $(_EXTRA_DEPS)
	@echo CC $(notdir $@)
	$(_V)mkdir -p $(dir $@)
	$(_V)$(CC) -c -o $@ $< $(_COMMON_FLAGS) $(_CFLAGS)

$(filter %.cpp.o,$(_OBJS)): $(_OBJDIR)%.o: % $(_EXTRA_DEPS)
	@echo C++ $(notdir $@)
	$(_V)mkdir -p $(dir $@)
	$(_V)$(CC) -c -o $@ $< $(_COMMON_FLAGS) $(_CXXFLAGS)

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
