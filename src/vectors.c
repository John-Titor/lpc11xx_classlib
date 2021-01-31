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

// Symbols from linker script.
extern uint8_t _stacktop;

// From startup.cpp
extern void _start(void);

static void _badhandler0(void) { for (;;) { LPC_UART->THR = '0'; } }
static void _badhandler1(void) { for (;;) { LPC_UART->THR = '1'; } }
static void _badhandler2(void) { for (;;) { LPC_UART->THR = '2'; } }
static void _badhandler3(void) { for (;;) { LPC_UART->THR = '3'; } }
static void _badhandler4(void) { for (;;) { LPC_UART->THR = '4'; } }
static void _badhandler5(void) { for (;;) { LPC_UART->THR = '5'; } }
//static void _badhandler6(void) { for (;;) { LPC_UART->THR = '6'; } }
//static void _badhandler7(void) { for (;;) { LPC_UART->THR = '7'; } }
//static void _badhandler8(void) { for (;;) { LPC_UART->THR = '8'; } }
//static void _badhandler9(void) { for (;;) { LPC_UART->THR = '9'; } }

void NonMaskableInt_Handler(void)   __attribute__((weak, alias("_badhandler1")));
void HardFault_Handler(void)        __attribute__((weak, alias("_badhandler2")));
void SVCall_Handler(void)           __attribute__((weak, alias("_badhandler3")));
void PendSV_Handler(void)           __attribute__((weak, alias("_badhandler4")));
void SysTick_Handler(void)          __attribute__((weak, alias("_badhandler5")));
void WAKEUP0_Handler(void)          __attribute__((weak, alias("_badhandler0")));
void WAKEUP1_Handler(void)          __attribute__((weak, alias("_badhandler0")));
void WAKEUP2_Handler(void)          __attribute__((weak, alias("_badhandler0")));
void WAKEUP3_Handler(void)          __attribute__((weak, alias("_badhandler0")));
void WAKEUP4_Handler(void)          __attribute__((weak, alias("_badhandler0")));
void WAKEUP5_Handler(void)          __attribute__((weak, alias("_badhandler0")));
void WAKEUP6_Handler(void)          __attribute__((weak, alias("_badhandler0")));
void WAKEUP7_Handler(void)          __attribute__((weak, alias("_badhandler0")));
void WAKEUP8_Handler(void)          __attribute__((weak, alias("_badhandler0")));
void WAKEUP9_Handler(void)          __attribute__((weak, alias("_badhandler0")));
void WAKEUP10_Handler(void)         __attribute__((weak, alias("_badhandler0")));
void WAKEUP11_Handler(void)         __attribute__((weak, alias("_badhandler0")));
void WAKEUP12_Handler(void)         __attribute__((weak, alias("_badhandler0")));
void CAN_Handler(void)              __attribute__((weak, alias("_badhandler0")));
void SSP1_Handler(void)             __attribute__((weak, alias("_badhandler0")));
void I2C_Handler(void)              __attribute__((weak, alias("_badhandler0")));
void TIMER_16_0_Handler(void)       __attribute__((weak, alias("_badhandler0")));
void TIMER_16_1_Handler(void)       __attribute__((weak, alias("_badhandler0")));
void TIMER_32_0_Handler(void)       __attribute__((weak, alias("_badhandler0")));
void TIMER_32_1_Handler(void)       __attribute__((weak, alias("_badhandler0")));
void SSP0_Handler(void)             __attribute__((weak, alias("_badhandler0")));
void UART_Handler(void)             __attribute__((weak, alias("_badhandler0")));
void ADC_Handler(void)              __attribute__((weak, alias("_badhandler0")));
void WDT_Handler(void)              __attribute__((weak, alias("_badhandler0")));
void BOD_Handler(void)              __attribute__((weak, alias("_badhandler0")));
void EINT3_Handler(void)            __attribute__((weak, alias("_badhandler0")));
void EINT2_Handler(void)            __attribute__((weak, alias("_badhandler0")));
void EINT1_Handler(void)            __attribute__((weak, alias("_badhandler0")));
void EINT0_Handler(void)            __attribute__((weak, alias("_badhandler0")));

__attribute__((section(".vectors")))
void (*_vectors[])() = {
    [0]                         = (void (*)()) &_stacktop,
    [1]                         = _start,
    [16 + NonMaskableInt_IRQn]  = NonMaskableInt_Handler,
    [16 + HardFault_IRQn]       = HardFault_Handler,
    [16 + SVCall_IRQn]          = SVCall_Handler,
    [16 + PendSV_IRQn]          = PendSV_Handler,
    [16 + SysTick_IRQn]         = SysTick_Handler,
    [16 + WAKEUP0_IRQn]         = WAKEUP0_Handler,
    [16 + WAKEUP1_IRQn]         = WAKEUP1_Handler,
    [16 + WAKEUP2_IRQn]         = WAKEUP2_Handler,
    [16 + WAKEUP3_IRQn]         = WAKEUP3_Handler,
    [16 + WAKEUP4_IRQn]         = WAKEUP4_Handler,
    [16 + WAKEUP5_IRQn]         = WAKEUP5_Handler,
    [16 + WAKEUP6_IRQn]         = WAKEUP6_Handler,
    [16 + WAKEUP7_IRQn]         = WAKEUP7_Handler,
    [16 + WAKEUP8_IRQn]         = WAKEUP8_Handler,
    [16 + WAKEUP9_IRQn]         = WAKEUP9_Handler,
    [16 + WAKEUP10_IRQn]        = WAKEUP10_Handler,
    [16 + WAKEUP11_IRQn]        = WAKEUP11_Handler,
    [16 + WAKEUP12_IRQn]        = WAKEUP12_Handler,
    [16 + CAN_IRQn]             = CAN_Handler,
    [16 + SSP1_IRQn]            = SSP1_Handler,
    [16 + I2C_IRQn]             = I2C_Handler,
    [16 + TIMER_16_0_IRQn]      = TIMER_16_0_Handler,
    [16 + TIMER_16_1_IRQn]      = TIMER_16_1_Handler,
    [16 + TIMER_32_0_IRQn]      = TIMER_32_0_Handler,
    [16 + TIMER_32_1_IRQn]      = TIMER_32_1_Handler,
    [16 + SSP0_IRQn]            = SSP0_Handler,
    [16 + UART_IRQn]            = UART_Handler,
    [16 + ADC_IRQn]             = ADC_Handler,
    [16 + WDT_IRQn]             = WDT_Handler,
    [16 + BOD_IRQn]             = BOD_Handler,
    [16 + EINT3_IRQn]           = EINT3_Handler,
    [16 + EINT2_IRQn]           = EINT2_Handler,
    [16 + EINT1_IRQn]           = EINT1_Handler,
    [16 + EINT0_IRQn]           = EINT0_Handler,
};
