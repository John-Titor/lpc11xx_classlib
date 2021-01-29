#
# Re-usable build rules for projects using lpc11xx_classlib
#
# Set BIN to the binary to be built.
# Set CHIP to the chip to be supported, one of LPC1114FN28, LPC11C24FBD48 (add more)
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
# scmRTOS_PROCESS_COUNT		Set the number of processes. If not set, scmRTOS 
#				is not enabled and main() must be supplied.
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

# LTO elides this file before we get a chance for the 
# library dependencies to be seen, so turn LTO off for 
# it.
_EXTRA_FLAGS_atomic.cpp		= -fno-lto
_EXTRA_FLAGS_cplusplus.cpp	= -fno-lto

# Tools
CC		 = arm-none-eabi-gcc
AR		 = arm-none-eabi-ar
OBJCOPY		 = arm-none-eabi-objcopy
SIZE		 = arm-none-eabi-size
PROGRAMMER	 = lpc21isp
ASTYLE		 = astyle

# Common compile arguments
EXTRA_DEPS	+= $(MAKEFILE_LIST)
_INCLUDES	+= $(foreach dir,$(_INCDIRS),-I$(_LIBDIR)$(dir))
DEFINES		+= $(CHIP)
#$(info _INCDIRS $(_INCDIRS))
#$(info _INCLUDES $(_INCLUDES))

# Compiler options
_ARCH_FLAGS	 = -mcpu=cortex-m0

_COMMON_FLAGS    = $(addprefix -D,$(DEFINES)) \
		   $(_INCLUDES) \
		   $(_ARCH_FLAGS) \
		   -ffunction-sections \
		   -fdata-sections \
		   -ffreestanding \
		   -flto \
		   -MD -MP \
		   -Os \
		   -g \
		   -Wall

_CFLAGS      	 = -std=gnu17 \
		   $(CFLAGS)

# -Wno-register for CMSIS/cmsis_gcc.h (XXX try newer CMSIS?)
_CXXFLAGS	 = -std=gnu++17 \
		   -fno-exceptions \
		   -fno-rtti \
		   -Wno-register \
		   $(CXXFLAGS)

_ASFLAGS	 = $(_ARCH_FLAGS)

_LDFLAGS	 = $(_ARCH_FLAGS) \
		   -nostartfiles \
		   -Wl,-gc-sections \
		   -flto \
		   -L$(_LIBDIR) \
		   -Wl,-Map,$@.map \
		   --specs=nano.specs \
		   $(LIBS) \
		   $(LDFLAGS)

_ELF		 = $(BIN:.bin=.elf)
_LDSCRIPT	 = $(_LIBDIR)$(CHIP).ld
_OBJS		 = $(foreach src,$(SRCS),$(_OBJDIR)$(src).o)
_DEPS		 = $(_OBJS:.o=.d)

SRCS           	+= $(foreach dir,					\
			$(_SRCDIRS),					\
				$(wildcard $(_LIBDIR)$(dir)/*.cpp) 	\
		    		$(wildcard $(_LIBDIR)$(dir)/*.c) 	\
		    		$(wildcard $(_LIBDIR)$(dir)/*.S))	\
		   $(addprefix $(_LIBDIR),$(_EXTRA_SRCS))

#$(info SRCS $(notdir $(SRCS)))
#$(info OBJS $(notdir $(_OBJS)))

$(BIN): $(_ELF) $(EXTRA_DEPS)
	@echo BINARY $(notdir $@)
	$(_V)$(OBJCOPY) -Obinary $< $@

$(_ELF): $(_OBJS) $(EXTRA_DEPS) $(_LDSCRIPT)
	@echo ELF $(notdir $@)
	$(_V)$(CC) -o $@ $(_OBJS) $(_LDFLAGS) -T$(_LDSCRIPT)
	$(_V)$(SIZE) $@

.PHONY: upload
upload: $(BIN)
	$(PROGRAMMER) -bin -control -verify -term $< $(PORT) 115200 12000

#
# Rules for building object files.
#
$(filter %.S.o,$(_OBJS)): $(_OBJDIR)%.o: % $(EXTRA_DEPS)
	@echo ASM $(notdir $@)
	$(_V)mkdir -p $(dir $@)
	$(_V)$(CC) -c -o $@ $< $(_ASFLAGS)

$(filter %.c.o,$(_OBJS)): $(_OBJDIR)%.o: % $(EXTRA_DEPS)
	@echo CC $(notdir $@)
	$(_V)mkdir -p $(dir $@)
	$(_V)$(CC) -c -o $@ $< $(_COMMON_FLAGS) $(_CFLAGS) $(_EXTRA_FLAGS_$(notdir $<))

$(filter %.cpp.o,$(_OBJS)): $(_OBJDIR)%.o: % $(EXTRA_DEPS)
	@echo C++ $(notdir $@)
	$(_V)mkdir -p $(dir $@)
	$(_V)$(CC) -c -o $@ $< $(_COMMON_FLAGS) $(_CXXFLAGS) $(_EXTRA_FLAGS_$(notdir $<))

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
