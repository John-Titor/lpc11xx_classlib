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

class Interrupt
{
public:
    constexpr Interrupt(IRQn_Type vector) :
        _vector(vector)
    {}

    void                enable()  const __always_inline { NVIC_EnableIRQ(_vector); }
    void                disable() const __always_inline { NVIC_DisableIRQ(_vector); }
    void                set_priority(unsigned priority) const __always_inline { NVIC_SetPriority(_vector, priority); }

    static void         enable_all() __always_inline { __enable_irq(); }
    static void         disable_all() __always_inline { __disable_irq(); }
    static void         wait() __always_inline { __WFI(); }

private:
    const IRQn_Type      _vector;
};

class CriticalSection
{
public:
    CriticalSection() :
        _enabled(__get_PRIMASK() == 0)
    {
        if (_enabled) {
            __disable_irq();
        }
    }
    ~CriticalSection()
    {
        if (_enabled) {
            __enable_irq();
        }
    }

private:
    bool                _enabled = false;
};

#define BEGIN_CRITICAL_SECTION  do { CriticalSection _crit
#define END_CRITICAL_SECTION    } while(0)

#define WAKEUP0_IRQ     Interrupt(WAKEUP0_IRQn)
#define WAKEUP1_IRQ     Interrupt(WAKEUP1_IRQn)
#define WAKEUP2_IRQ     Interrupt(WAKEUP2_IRQn)
#define WAKEUP3_IRQ     Interrupt(WAKEUP3_IRQn)
#define WAKEUP4_IRQ     Interrupt(WAKEUP4_IRQn)
#define WAKEUP5_IRQ     Interrupt(WAKEUP5_IRQn)
#define WAKEUP6_IRQ     Interrupt(WAKEUP6_IRQn)
#define WAKEUP7_IRQ     Interrupt(WAKEUP7_IRQn)
#define WAKEUP8_IRQ     Interrupt(WAKEUP8_IRQn)
#define WAKEUP9_IRQ     Interrupt(WAKEUP9_IRQn)
#define WAKEUP10_IRQ    Interrupt(WAKEUP10_IRQn)
#define WAKEUP11_IRQ    Interrupt(WAKEUP11_IRQn)
#define WAKEUP12_IRQ    Interrupt(WAKEUP12_IRQn)
#define CAN_IRQ         Interrupt(CAN_IRQn)
#define SSP1_IRQ        Interrupt(SSP1_IRQn)
#define I2C_IRQ         Interrupt(I2C_IRQn)
#define TIMER_16_0      Interrupt(TIMER_16_0_IRQn)
#define TIMER_16_1      Interrupt(TIMER_16_1_IRQn)
#define TIMER_32_0      Interrupt(TIMER_32_0_IRQn)
#define TIMER_32_1      Interrupt(TIMER_32_1_IRQn)
#define SSP0_IRQ        Interrupt(SSP0_IRQn)
#define UART_IRQ        Interrupt(UART_IRQn)
#define ADC_IRQ         Interrupt(ADC_IRQn)
#define WDT_IRQ         Interrupt(WDT_IRQn)
#define BOD_IRQ         Interrupt(BOD_IRQn)
#define FMC_IRQ         Interrupt(FMC_IRQn)
#define EINT3_IRQ       Interrupt(EINT3_IRQn)
#define EINT2_IRQ       Interrupt(EINT2_IRQn)
#define EINT1_IRQ       Interrupt(EINT1_IRQn)
#define EINT0_IRQ       Interrupt(EINT0_IRQn)
