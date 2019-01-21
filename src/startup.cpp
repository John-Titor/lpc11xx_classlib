// Copyright (c) 2015 Michael Smith, All Rights Reserved
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
extern funcp_t _fini_array_start;
extern funcp_t _fini_array_end;

extern "C" void main() __attribute__((noreturn));

extern "C"
void __attribute__((naked))
_start()
{
    // Load .data from rom image.
    auto rp = &_data_rom;
    auto wp = &_data_start;

    while (wp < &_data_end) {
        *wp++ = *rp++;
    }

    // Clear .bss.
    wp = &_bss_start;

    while (wp < &_bss_end) {
        *wp++ = 0;
    }

    // Switch to maximum clock speed (be nice to do this earlier...)
    Syscon::init_48MHz();

    // Fill the stack with 1s
    register unsigned long sp asm("sp");
    wp = (uint32_t *)(sp - 16);

    while (wp >= &_bss_end) {
        *wp-- = 0xffffffff;
    }

    // Call constructors.
    auto fp = &_init_array_start;

    while (fp < &_init_array_end) {
        (*fp++)();
    }

    // run the app
    main();
}
