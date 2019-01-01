CHIP            = LPC1114FN28
LIB             = obj/lpc11xx_classlib.a
SRCS            = $(wildcard src/*.cpp) $(wildcard src/*.c)
EXTRA_FMT_SRCS  = $(wildcard include/*.h)

.PHONY: all test

all: $(LIB) test

test:
	make -C ./test

include make.inc
