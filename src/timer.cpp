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

#include <timer.h>
#include <syscon.h>
#include <interrupt.h>
#include <uart.h>

Timer::Callback Timer::_callbacks[4];
uint64_t        _Timebase::_time;

void
_Timebase::configure()
{
    cancel();
    _time = 0;
    _regs.CTCR = CTCR_CTMODE_TIMER;
    _regs.PR = (Syscon::PCLK_FREQ / 1000000) - 1;        // count microseconds
    _regs.MR0 = max_count();                             // interrupt around about wrap time
    _regs.MR1 = max_count() / 2;                         // and again at ~halftime
    _regs.MCR = MCR_MR0_INT_ENABLED | MCR_MR1_INT_ENABLED;
    _callbacks[_index] = &_Timebase::handler;
    _regs.TCR = TCR_COUNTERENABLE_ENABLED | TCR_COUNTERRESET_DISABLED;
    _irq.enable();
}

uint64_t
_Timebase::time()
{
    BEGIN_CRITICAL_SECTION;

    // clear any match interrupts - we're about to do the same work
    _regs.IR = _regs.IR;

    // work out whether this is a 16 or 32-bit timer
    uint64_t mask = _regs.MR0;

    // split the time into bits we own and bits that the timer owns
    uint32_t old_count = _time & mask;
    uint64_t tb_high = _time ^ old_count;

    // get the current low bits
    uint32_t count = _regs.TC;

    // handle possible wrap since the last time we were called
    if (count < old_count) {
        tb_high += mask + 1;
    }

    _time = tb_high | count;

    return _time;

    END_CRITICAL_SECTION;
}

void
_Timebase::handler(unsigned index)
{
    _Timebase(index).time();
}

void
TIMER_16_0_Handler(void)
{
    Timer(0).interrupt();
}

void
TIMER_16_1_Handler(void)
{
    Timer(1).interrupt();
}

void
TIMER_32_0_Handler(void)
{
    Timer(2).interrupt();
}

void
TIMER_32_1_Handler(void)
{
    Timer(3).interrupt();
}
