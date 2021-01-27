// Copyright (c) 2019 Michael Smith, All Rights Reserved
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  o Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  o Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include <LPC11xx.h>
#include <syscon.h>

// Symbols from linker script.
typedef void (*funcp_t)();
extern uint32_t _data_rom;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;
extern funcp_t _init_array_start;
extern funcp_t _init_array_end;

extern "C" void main() __attribute__((noreturn));

extern "C" void _board_init(void) {}
void board_init(void) __attribute__((weak, alias("_board_init")));

extern "C"
void __attribute__((naked))
_start()
{
    // Load .data from rom image.
    for (auto src = &_data_rom, dst = &_data_start;
         dst < &_data_end;
         src++, dst++) {
        *dst = *src;
    }

    // Clear .bss.
    for (auto ptr = &_bss_start;
         ptr < &_bss_end;
         ptr++) {
        *ptr = 0;
    }

    // Switch to maximum clock speed (be nice to do this earlier...)
    Syscon::init_48MHz();

    // Fill the stack with 1s
    register unsigned long sp asm("sp");
    for (auto ptr = (uint32_t *)(sp - 16); 
         ptr >= &_bss_end;
         ptr--) {
        *ptr = 0xffffffff;
    }

    // Call constructors.
    for (auto fp = &_init_array_start;
         fp < &_init_array_end;
         fp++) {
        (*fp)();
    }

    // call the board init hook
    board_init();

    // run the app
    main();
}
