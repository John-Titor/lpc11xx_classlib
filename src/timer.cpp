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

#include <timer.h>
#include <syscon.h>
#include <interrupt.h>

Timer::Callback Timer::_callbacks[4];

void
Timer::cancel()
{
    _regs.TCR = TCR_COUNTERENABLE_DISABLED | TCR_COUNTERRESET_ENABLED;
    _regs.IR = 0;
    _regs.MR0 = 0;
    _regs.MR1 = 0;
    _regs.MR2 = 0;
    _regs.MR3 = 0;
    _regs.CCR = 0;
    _regs.EMR = 0;
    _regs.CTCR = 0;
    _regs.PWMC = 0;
    _callbacks[_index] = nullptr;
}


void
Timer::handler(unsigned index)
{
    if (_callbacks[index] != nullptr) {
        _callbacks[index](index);
    } else {
        Timer(index).cancel();
    }
}

extern "C" {
    void CT16_0_IRQHandler() { Timer::handler(0); }
    void CT16_1_IRQHandler() { Timer::handler(1); }
    void CT32_0_IRQHandler() { Timer::handler(2); }
    void CT32_1_IRQHandler() { Timer::handler(3); }
}

uint64_t        Timebase::_time;

void
Timebase::configure()
{
    cancel();
    _time = 0;
    _regs.CTCR = CTCR_CTMODE_TIMER;
    _regs.PR = (Syscon::PCLK_FREQ / 1000000) - 1;        // count microseconds
    _regs.MR0 = 0xffffffff;                              // interrupt around about wrap time
    _regs.MR1 = 0x8000;                                  // and again at ~halftime for a 16-bit timer
    _regs.MCR = MCR_MR0_INT_ENABLED | MCR_MR1_INT_ENABLED;
    _callbacks[_index] = &Timebase::handler;
    _regs.TCR = TCR_COUNTERENABLE_ENABLED | TCR_COUNTERRESET_DISABLED;
}

uint64_t
Timebase::time()
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
Timebase::handler(unsigned index)
{
    Timebase(index).time();
}

