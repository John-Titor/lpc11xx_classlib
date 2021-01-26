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

#pragma once

#include <LPC11xx.h>

#include "interrupt.h"
#include "syscon.h"

extern "C" void CT16_0_IRQHandler();
extern "C" void CT16_1_IRQHandler();
extern "C" void CT32_0_IRQHandler();
extern "C" void CT32_1_IRQHandler();

class Timer
{
public:
    typedef void (* Callback)(unsigned index);

    constexpr Timer(unsigned index) :
        _index(index),
        _regs((index == 0) ? * LPC_TMR16B0 :
              (index == 1) ? * LPC_TMR16B1 :
              (index == 2) ? * LPC_TMR32B0 : * LPC_TMR32B1),
        _syscon((index == 0) ? SYSCON_CT16B0 :
                (index == 1) ? SYSCON_CT16B1 :
                (index == 2) ? SYSCON_CT32B0 : SYSCON_CT32B1),
        _irq((index == 0) ? TIMER_16_0_IRQ :
             (index == 1) ? TIMER_16_1_IRQ :
             (index == 2) ? TIMER_32_0_IRQ : TIMER_32_1_IRQ)
    {}

    void                cancel();
    uint32_t            max_count() { return (_index < 2) ? 0xffff : 0xffffffff; }

    static void         handler(unsigned index);

protected:
    static Callback     _callbacks[4];

    const unsigned      _index;
    LPC_TMR_TypeDef     &_regs;
    Syscon              _syscon;
    Interrupt           _irq;

    enum IR : uint32_t {
        IR_MR0_MASK                 = 0x00000001, // Interrupt flag for match channel 0
        IR_MR0                      = 0x00000001,
        IR_MR1_MASK                 = 0x00000002, // Interrupt flag for match channel 1
        IR_MR1                      = 0x00000002,
        IR_MR2_MASK                 = 0x00000004, // Interrupt flag for match channel 2
        IR_MR2                      = 0x00000004,
        IR_MR3_MASK                 = 0x00000008, // Interrupt flag for match channel 3
        IR_MR3                      = 0x00000008,
        IR_CR0_MASK                 = 0x00000010, // Interrupt flag for capture channel 0 event
        IR_CR0                      = 0x00000010,
        IR_MASK_ALL                 = 0x0000001F,
    };

    enum TCR : uint32_t {
        TCR_COUNTERENABLE_MASK      = 0x00000001, // Counter enable
        TCR_COUNTERENABLE_ENABLED   = 0x00000001,
        TCR_COUNTERENABLE_DISABLED  = 0x00000000,
        TCR_COUNTERRESET_MASK       = 0x00000002,
        TCR_COUNTERRESET_ENABLED    = 0x00000002,
        TCR_COUNTERRESET_DISABLED   = 0x00000000,
    };

    enum MCR : uint32_t {
        MCR_MR0_INT_MASK            = 0x00000001, // Interrupt on MRO
        MCR_MR0_INT_ENABLED         = 0x00000001,
        MCR_MR0_INT_DISABLED        = 0x00000000,
        MCR_MR0_RESET_MASK          = 0x00000002, // Reset on MR0
        MCR_MR0_RESET_ENABLED       = 0x00000002,
        MCR_MR0_RESET_DISABLED      = 0x00000000,
        MCR_MR0_STOP_MASK           = 0x00000004, // Stop on MR0
        MCR_MR0_STOP_ENABLED        = 0x00000004,
        MCR_MR0_STOP_DISABLED       = 0x00000000,
        MCR_MR1_INT_MASK            = 0x00000008, // Interrupt on MR1
        MCR_MR1_INT_ENABLED         = 0x00000008,
        MCR_MR1_INT_DISABLED        = 0x00000000,
        MCR_MR1_RESET_MASK          = 0x00000010, // Reset on MR1
        MCR_MR1_RESET_ENABLED       = 0x00000010,
        MCR_MR1_RESET_DISABLED      = 0x00000000,
        MCR_MR1_STOP_MASK           = 0x00000020, // Stop on MR1
        MCR_MR1_STOP_ENABLED        = 0x00000020,
        MCR_MR1_STOP_DISABLED       = 0x00000000,
        MCR_MR2_INT_MASK            = 0x00000040, // Interrupt on MR2
        MCR_MR2_INT_ENABLED         = 0x00000040,
        MCR_MR2_INT_DISABLED        = 0x00000000,
        MCR_MR2_RESET_MASK          = 0x00000080, // Reset on MR2
        MCR_MR2_RESET_ENABLED       = 0x00000080,
        MCR_MR2_RESET_DISABLED      = 0x00000000,
        MCR_MR2_STOP_MASK           = 0x00000100, // Stop on MR2
        MCR_MR2_STOP_ENABLED        = 0x00000100,
        MCR_MR2_STOP_DISABLED       = 0x00000000,
        MCR_MR3_INT_MASK            = 0x00000200, // Interrupt on MR3
        MCR_MR3_INT_ENABLED         = 0x00000200,
        MCR_MR3_INT_DISABLED        = 0x00000000,
        MCR_MR3_RESET_MASK          = 0x00000400, // Reset on MR3
        MCR_MR3_RESET_ENABLED       = 0x00000400,
        MCR_MR3_RESET_DISABLED      = 0x00000000,
        MCR_MR3_STOP_MASK           = 0x00000800, // Stop on MR3
        MCR_MR3_STOP_ENABLED        = 0x00000800,
        MCR_MR3_STOP_DISABLED       = 0x00000000,
    };

    enum CCR : uint32_t {
        CCR_CAP0RE_MASK             = 0x00000001, // Capture on rising edge
        CCR_CAP0RE_ENABLED          = 0x00000001,
        CCR_CAP0RE_DISABLED         = 0x00000000,
        CCR_CAP0FE_MASK             = 0x00000002, // Capture on falling edge
        CCR_CAP0FE_ENABLED          = 0x00000002,
        CCR_CAP0FE_DISABLED         = 0x00000000,
        CCR_CAP0I_MASK              = 0x00000004, // Interrupt on CAP0 event
        CCR_CAP0I_ENABLED           = 0x00000004,
        CCR_CAP0I_DISABLED          = 0x00000000,
    };

    enum EMR : uint32_t {
        EMR_EM0_MASK                = 0x00000001, // External match 0
        EMR_EM0                     = 0x00000001,
        EMR_EMC0_MASK               = 0x00000030,
        EMR_EMC0_DONOTHING          = 0x00000000,
        EMR_EMC0_LOW                = 0x00000010,
        EMR_EMC0_HIGH               = 0x00000020,
        EMR_EMC0_TOGGLE             = 0x00000030,
        EMR_EM1_MASK                = 0x00000002, // External match 1
        EMR_EM1                     = 0x00000002,
        EMR_EMC1_MASK               = 0x000000C0,
        EMR_EMC1_DONOTHING          = 0x00000000,
        EMR_EMC1_LOW                = 0x00000040,
        EMR_EMC1_HIGH               = 0x00000080,
        EMR_EMC1_TOGGLE             = 0x000000C0,
        EMR_EM2_MASK                = 0x00000004, // External match 2
        EMR_EM2                     = 0x00000004,
        EMR_EMC2_MASK               = 0x00000300,
        EMR_EMC2_DONOTHING          = 0x00000000,
        EMR_EMC2_LOW                = 0x00000100,
        EMR_EMC2_HIGH               = 0x00000200,
        EMR_EMC2_TOGGLE             = 0x00000300,
        EMR_EM3_MASK                = 0x00000008, // External match 3
        EMR_EM3                     = 0x00000008,
        EMR_EMC3_MASK               = 0x00000C00,
        EMR_EMC3_DONOTHING          = 0x00000000,
        EMR_EMC3_LOW                = 0x00000400,
        EMR_EMC3_HIGH               = 0x00000800,
        EMR_EMC3_TOGGLE             = 0x00000C00,
    };

    enum CTCR : uint32_t {
        CTCR_CTMODE_MASK            = 0x00000003, // Counter/Timer mode
        CTCR_CTMODE_TIMER           = 0x00000000, // Timer Mode: Every rising PCLK edge
        CTCR_CTMODE_COUNTERRISING   = 0x00000001, // Counter: TC increments on rising edge of input
        CTCR_CTMODE_COUNTERFALLING  = 0x00000002, // Counter: TC increments on falling edge of input
        CTCR_CTMODE_COUNTERBOTH     = 0x00000003, // Counter: TC increments on both edges of input
        CTCR_CINPUTSELECT_MASK      = 0x0000000C,
        CTCR_CINPUTSELECT           = 0x00000000, // CINPUTSELECT must be set to 00
    };

    enum PWMC : uint32_t {
        PWMC_PWM0_MASK              = 0x00000001,
        PWMC_PWM0_ENABLED           = 0x00000001, // PWM mode is enabled for CT32Bn_MAT0
        PWMC_PWM0_DISABLED          = 0x00000000,
        PWMC_PWM1_MASK              = 0x00000002,
        PWMC_PWM1_ENABLED           = 0x00000002, // PWM mode is enabled for CT32Bn_MAT1
        PWMC_PWM1_DISABLED          = 0x00000000,
        PWMC_PWM2_MASK              = 0x00000004,
        PWMC_PWM2_ENABLED           = 0x00000004, // PWM mode is enabled for CT32Bn_MAT2
        PWMC_PWM2_DISABLED          = 0x00000000,
        PWMC_PWM3_MASK              = 0x00000008,
        PWMC_PWM3_ENABLED           = 0x00000008,
        PWMC_PWM3_DISABLED          = 0x00000000,
    };
};

class Timebase : public Timer
{
public:
    typedef uint64_t    microseconds;

    constexpr Timebase(unsigned index) :
        Timer(index)
    {}

    void                configure();
    microseconds        time();

private:
    static microseconds _time;

    static void         handler(unsigned index);
};


#define CT16B0  Timer(0)
#define CT16B1  Timer(1)
#define CT32B0  Timer(2)
#define CT32B1  Timer(3)
