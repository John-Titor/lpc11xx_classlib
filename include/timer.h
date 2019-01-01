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

#pragma once

#include <LPC11xx.h>
#include "_compiler.h"
#include "syscon.h"

extern "C" void CT16_0_IRQHandler();
extern "C" void CT16_1_IRQHandler();
extern "C" void CT32_0_IRQHandler();
extern "C" void CT32_1_IRQHandler();

class Timer
{
public:
    typedef void (* Callback)();

    constexpr Timer(unsigned index) :
        _index(index),
        _regs((index == 0) ? *LPC_TMR16B0 :
              (index == 1) ? *LPC_TMR16B1 :
              (index == 2) ? *LPC_TMR32B0 : *LPC_TMR32B1),
        _syscon((index == 0) ? SYSCON_CT16B0 :
                (index == 1) ? SYSCON_CT16B1 :
                (index == 2) ? SYSCON_CT32B0 : SYSCON_CT32B1)
    {}

    void configure(Callback callback) const __always_inline
    {
        _callbacks[_index] = callback;      // save the callback
        _syscon.clock(true);
    }

    void                cancel() 
    {
        _syscon.clock(false);
        _callbacks[_index] = nullptr;
    }

    LPC_TMR_TypeDef     &reg() { return _regs; }

    static void         handler(unsigned index);

    // XXX TODO make this useful

private:
    const unsigned      _index;
    LPC_TMR_TypeDef     &_regs;
    Syscon              _syscon;

    static Callback     _callbacks[4];
};

#define CT16B0  Timer(0)
#define CT16B1  Timer(1)
#define CT32B0  Timer(2)
#define CT32B1  Timer(3)
