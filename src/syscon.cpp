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

#include "syscon.h"

void
Syscon::init_48MHz()
{

    LPC_SYSCON->SYSAHBCLKCTRL |= SYSAHBCLKCTRL_ALL_MASK;  // all clocks on

    //
    // Configure PLL @ 48MHz
    //

    // swap back to the IRC first...
    LPC_SYSCON->PDRUNCFG &= ~PDRUNCFG_SYSOSC_MASK;
    LPC_SYSCON->MAINCLKSEL = MAINCLKSEL_SOURCE_INTERNALOSC;
    LPC_SYSCON->MAINCLKUEN = MAINCLKUEN_UPDATE;
    LPC_SYSCON->MAINCLKUEN = MAINCLKUEN_DISABLE;
    LPC_SYSCON->MAINCLKUEN = MAINCLKUEN_UPDATE;
    while (!(LPC_SYSCON->MAINCLKUEN & MAINCLKUEN_UPDATE)) {
        // wait for the bit to self-clear indicating the main clock has switched
    }

    // select the IRC as PLL input
    LPC_SYSCON->SYSPLLCLKSEL = PLLCLKSEL_SOURCE_INTERNALOSC;  // select internal oscillator
    LPC_SYSCON->SYSPLLCLKUEN = PLLCLKUEN_UPDATE;
    LPC_SYSCON->SYSPLLCLKUEN = PLLCLKUEN_DISABLE;
    LPC_SYSCON->SYSPLLCLKUEN = PLLCLKUEN_UPDATE;
    while (!(LPC_SYSCON->SYSPLLCLKUEN & PLLCLKUEN_UPDATE)) {
        // wait for the bit to self-clear indicating the PLL input clock has switched
    }

    // turn the PLL off
    LPC_SYSCON->PDRUNCFG |= PDRUNCFG_SYSPLL_MASK;

    // 4x multiplier, /2 post-divider 12MHz IRC -> 48MHz FCLKOUT
    LPC_SYSCON->SYSPLLCTRL = (PLLCTRL_MSEL_4 | PLLCTRL_PSEL_2);

    // turn the PLL back on
    LPC_SYSCON->PDRUNCFG &= ~PDRUNCFG_SYSPLL_MASK;
    while (!(LPC_SYSCON->SYSPLLSTAT & PLLSTAT_LOCK)) {
        // wait for the lock bit to set
    }

    // select the PLL as the main clock
    LPC_SYSCON->MAINCLKSEL = MAINCLKSEL_SOURCE_SYSPLLCLKOUT;
    LPC_SYSCON->MAINCLKUEN = MAINCLKUEN_UPDATE;     // toggle high to update clock source
    LPC_SYSCON->MAINCLKUEN = MAINCLKUEN_DISABLE;    // toggle high to update clock source
    LPC_SYSCON->MAINCLKUEN = MAINCLKUEN_UPDATE;     // toggle high to update clock source
    while (!(LPC_SYSCON->MAINCLKUEN & MAINCLKUEN_UPDATE)) {
        // wait for the bit to self-clear indicating the main clock has switched
    }

    //
    // select AHB at 1x core clock
    //
    LPC_SYSCON->SYSAHBCLKDIV = SYSAHBCLKDIV_DIV1;
}

