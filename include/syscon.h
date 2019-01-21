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

#include "pin.h"

class Syscon
{
public:
    // we always run @ maximum speed
    static const unsigned               PCLK_FREQ = 48000000;

    enum MEMREMAP : uint32_t {
        MEMREMAP_MODE_BOOTLOADER        = 0x00000000, // Interrupt vectors are remapped to Boot ROM
        MEMREMAP_MODE_RAM               = 0x00000001, // Interrupt vectors are remapped to Static ROM
        MEMREMAP_MODE_FLASH             = 0x00000002, // Interrupt vectors are not remapped and reside in Flash
        MEMREMAP_MASK                   = 0x00000003
    };

    enum PRESETCTRL : uint32_t {
        PRESETCTRL_SSP0_RESETENABLED    = 0x00000000,
        PRESETCTRL_SSP0_RESETDISABLED   = 0x00000001,
        PRESETCTRL_SSP0_MASK            = 0x00000001,
        PRESETCTRL_I2C_RESETENABLED     = 0x00000000,
        PRESETCTRL_I2C_RESETDISABLED    = 0x00000002,
        PRESETCTRL_I2C_MASK             = 0x00000002,
        PRESETCTRL_SSP1_RESETENABLED    = 0x00000000,
        PRESETCTRL_SSP1_RESETDISABLED   = 0x00000004,
        PRESETCTRL_SSP1_MASK            = 0x00000004,
        PRESETCTRL_CAN_RESETENABLED     = 0x00000000,
        PRESETCTRL_CAN_RESETDISABLED    = 0x00000008,
        PRESETCTRL_CAN_MASK             = 0x00000008,
    };

    enum PLLCTRL : uint32_t {
        PLLCTRL_MSEL_1                  = 0x00000000,
        PLLCTRL_MSEL_2                  = 0x00000001,
        PLLCTRL_MSEL_3                  = 0x00000002,
        PLLCTRL_MSEL_4                  = 0x00000003,
        PLLCTRL_MSEL_5                  = 0x00000004,
        PLLCTRL_MSEL_6                  = 0x00000005,
        PLLCTRL_MSEL_7                  = 0x00000006,
        PLLCTRL_MSEL_8                  = 0x00000007,
        PLLCTRL_MSEL_9                  = 0x00000008,
        PLLCTRL_MSEL_10                 = 0x00000009,
        PLLCTRL_MSEL_11                 = 0x0000000A,
        PLLCTRL_MSEL_12                 = 0x0000000B,
        PLLCTRL_MSEL_13                 = 0x0000000C,
        PLLCTRL_MSEL_14                 = 0x0000000D,
        PLLCTRL_MSEL_15                 = 0x0000000E,
        PLLCTRL_MSEL_16                 = 0x0000000F,
        PLLCTRL_MSEL_17                 = 0x00000010,
        PLLCTRL_MSEL_18                 = 0x00000011,
        PLLCTRL_MSEL_19                 = 0x00000012,
        PLLCTRL_MSEL_20                 = 0x00000013,
        PLLCTRL_MSEL_21                 = 0x00000014,
        PLLCTRL_MSEL_22                 = 0x00000015,
        PLLCTRL_MSEL_23                 = 0x00000016,
        PLLCTRL_MSEL_24                 = 0x00000017,
        PLLCTRL_MSEL_25                 = 0x00000018,
        PLLCTRL_MSEL_26                 = 0x00000019,
        PLLCTRL_MSEL_27                 = 0x0000001A,
        PLLCTRL_MSEL_28                 = 0x0000001B,
        PLLCTRL_MSEL_29                 = 0x0000001C,
        PLLCTRL_MSEL_30                 = 0x0000001D,
        PLLCTRL_MSEL_31                 = 0x0000001E,
        PLLCTRL_MSEL_32                 = 0x0000001F,
        PLLCTRL_MSEL_MASK               = 0x0000001F,
        PLLCTRL_PSEL_2                  = 0x00000000,
        PLLCTRL_PSEL_4                  = 0x00000020,
        PLLCTRL_PSEL_8                  = 0x00000040,
        PLLCTRL_PSEL_16                 = 0x00000060,
        PLLCTRL_PSEL_BIT                = 5,
        PLLCTRL_PSEL_MASK               = 0x00000060,
        PLLCTRL_MASK                    = 0x0000007F,
    };

    enum PLLSTAT : uint32_t {
        PLLSTAT_LOCK                    = 0x00000001, // 0 = PLL not locked, 1 = PLL locked
        PLLSTAT_LOCK_MASK               = 0x00000001,
    };

    enum SYSOSCCTRL : uint32_t {
        SYSOSCCTRL_BYPASS_DISABLED      = 0x00000000, // Oscillator is not bypassed.
        SYSOSCCTRL_BYPASS_ENABLED       = 0x00000001, // Bypass enabled
        SYSOSCCTRL_BYPASS_MASK          = 0x00000001,
        SYSOSCCTRL_FREQRANGE_1TO20MHZ   = 0x00000000, // 1-20 MHz frequency range
        SYSOSCCTRL_FREQRANGE_15TO25MHZ  = 0x00000002, // 15-25 MHz frequency range
        SYSOSCCTRL_FREQRANGE_MASK       = 0x00000002,
    };

    enum WDTOSCCTRL : uint32_t {
        WDTOSCCTRL_DIVSEL_DIV2          = 0x00000000, // Reset value
        WDTOSCCTRL_DIVSEL_DIV4          = 0x00000001,
        WDTOSCCTRL_DIVSEL_DIV6          = 0x00000002,
        WDTOSCCTRL_DIVSEL_DIV8          = 0x00000003,
        WDTOSCCTRL_DIVSEL_DIV10         = 0x00000004,
        WDTOSCCTRL_DIVSEL_DIV12         = 0x00000005,
        WDTOSCCTRL_DIVSEL_DIV14         = 0x00000006,
        WDTOSCCTRL_DIVSEL_DIV16         = 0x00000007,
        WDTOSCCTRL_DIVSEL_DIV18         = 0x00000008,
        WDTOSCCTRL_DIVSEL_DIV20         = 0x00000009,
        WDTOSCCTRL_DIVSEL_DIV22         = 0x0000000A,
        WDTOSCCTRL_DIVSEL_DIV24         = 0x0000000B,
        WDTOSCCTRL_DIVSEL_DIV26         = 0x0000000C,
        WDTOSCCTRL_DIVSEL_DIV28         = 0x0000000D,
        WDTOSCCTRL_DIVSEL_DIV30         = 0x0000000E,
        WDTOSCCTRL_DIVSEL_DIV32         = 0x0000000F,
        WDTOSCCTRL_DIVSEL_DIV34         = 0x00000010,
        WDTOSCCTRL_DIVSEL_DIV36         = 0x00000011,
        WDTOSCCTRL_DIVSEL_DIV38         = 0x00000012,
        WDTOSCCTRL_DIVSEL_DIV40         = 0x00000013,
        WDTOSCCTRL_DIVSEL_DIV42         = 0x00000014,
        WDTOSCCTRL_DIVSEL_DIV44         = 0x00000015,
        WDTOSCCTRL_DIVSEL_DIV46         = 0x00000016,
        WDTOSCCTRL_DIVSEL_DIV48         = 0x00000017,
        WDTOSCCTRL_DIVSEL_DIV50         = 0x00000018,
        WDTOSCCTRL_DIVSEL_DIV52         = 0x00000019,
        WDTOSCCTRL_DIVSEL_DIV54         = 0x0000001A,
        WDTOSCCTRL_DIVSEL_DIV56         = 0x0000001B,
        WDTOSCCTRL_DIVSEL_DIV58         = 0x0000001C,
        WDTOSCCTRL_DIVSEL_DIV60         = 0x0000001D,
        WDTOSCCTRL_DIVSEL_DIV62         = 0x0000001E,
        WDTOSCCTRL_DIVSEL_DIV64         = 0x0000001F,
        WDTOSCCTRL_DIVSEL_MASK          = 0x0000001F,
        WDTOSCCTRL_FREQSEL_0_5MHZ       = 0x00000020,
        WDTOSCCTRL_FREQSEL_0_8MHZ       = 0x00000040,
        WDTOSCCTRL_FREQSEL_1_1MHZ       = 0x00000060,
        WDTOSCCTRL_FREQSEL_1_4MHZ       = 0x00000080,
        WDTOSCCTRL_FREQSEL_1_6MHZ       = 0x000000A0, // Reset value
        WDTOSCCTRL_FREQSEL_1_8MHZ       = 0x000000C0,
        WDTOSCCTRL_FREQSEL_2_0MHZ       = 0x000000E0,
        WDTOSCCTRL_FREQSEL_2_2MHZ       = 0x00000100,
        WDTOSCCTRL_FREQSEL_2_4MHZ       = 0x00000120,
        WDTOSCCTRL_FREQSEL_2_6MHZ       = 0x00000140,
        WDTOSCCTRL_FREQSEL_2_7MHZ       = 0x00000160,
        WDTOSCCTRL_FREQSEL_2_9MHZ       = 0x00000180,
        WDTOSCCTRL_FREQSEL_3_1MHZ       = 0x000001A0,
        WDTOSCCTRL_FREQSEL_3_2MHZ       = 0x000001C0,
        WDTOSCCTRL_FREQSEL_3_4MHZ       = 0x000001E0,
        WDTOSCCTRL_FREQSEL_MASK         = 0x000001E0,
    };

    enum IRCCTRL : uint32_t {
        IRCCTRL_MASK                    = 0x000000FF,
    };

    enum RESETSTAT : uint32_t {
        RESETSTAT_POR_MASK              = 0x00000001, // POR reset status
        RESETSTAT_EXTRST_MASK           = 0x00000002, // Status of the external reset pin
        RESETSTAT_WDT_MASK              = 0x00000004, // Status of the watchdog reset
        RESETSTAT_BOD_MASK              = 0x00000008, // Status of the brown-out detect reset
        RESETSTAT_SYSRST_MASK           = 0x00000010, // Status of the software system reset
        RESETSTAT_MASK                  = 0x00000010,
    };

    enum PLLCLKSEL : uint32_t {
        PLLCLKSEL_SOURCE_INTERNALOSC    = 0x00000000,
        PLLCLKSEL_SOURCE_MAINOSC        = 0x00000001,
        PLLCLKSEL_SOURCE_MASK           = 0x00000002,
    };

    enum PLLCLKUEN : uint32_t {
        PLLCLKUEN_DISABLE               = 0x00000000,
        PLLCLKUEN_UPDATE                = 0x00000001,
        PLLCLKUEN_MASK                  = 0x00000001,
    };

    enum MAINCLKSEL : uint32_t {
        MAINCLKSEL_SOURCE_INTERNALOSC   = 0x00000000, // Use IRC oscillator for main clock source
        MAINCLKSEL_SOURCE_INPUTCLOCK    = 0x00000001, // Use Input clock to system PLL for main clock source
        MAINCLKSEL_SOURCE_WDTOSC        = 0x00000002, // Use watchdog oscillator for main clock source
        MAINCLKSEL_SOURCE_SYSPLLCLKOUT  = 0x00000003, // Use system PLL clock out for main clock source
        MAINCLKSEL_MASK                 = 0x00000003,
    };

    enum MAINCLKUEN : uint32_t {
        MAINCLKUEN_DISABLE              = 0x00000000,
        MAINCLKUEN_UPDATE               = 0x00000001,
        MAINCLKUEN_MASK                 = 0x00000001,
    };

    enum SYSAHBCLKDIV : uint32_t {
        SYSAHBCLKDIV_DISABLE            = 0x00000000, // 0 will shut the system clock down completely
        SYSAHBCLKDIV_DIV1               = 0x00000001, // 1, 2 or 4 are the most common values
        SYSAHBCLKDIV_DIV2               = 0x00000002,
        SYSAHBCLKDIV_DIV4               = 0x00000004,
        SYSAHBCLKDIV_MASK               = 0x000000FF, // AHB clock divider can be from 0 to 255
    };

    enum SYSAHBCLKCTRL : uint32_t {
        SYSAHBCLKCTRL_SYS               = 0x00000001, // Enables clock for AHB and APB bridges, FCLK, HCLK, SysCon and PMU
        SYSAHBCLKCTRL_SYS_MASK          = 0x00000001,
        SYSAHBCLKCTRL_ROM               = 0x00000002, // Enables clock for ROM
        SYSAHBCLKCTRL_ROM_MASK          = 0x00000002,
        SYSAHBCLKCTRL_RAM               = 0x00000004, // Enables clock for SRAM
        SYSAHBCLKCTRL_RAM_MASK          = 0x00000004,
        SYSAHBCLKCTRL_FLASHREG          = 0x00000008, // Enables clock for flash register interface
        SYSAHBCLKCTRL_FLASHREG_MASK     = 0x00000008,
        SYSAHBCLKCTRL_FLASHARRAY        = 0x00000010, // Enables clock for flash array interface
        SYSAHBCLKCTRL_FLASHARRAY_MASK   = 0x00000010,
        SYSAHBCLKCTRL_I2C               = 0x00000020, // Enables clock for I2C
        SYSAHBCLKCTRL_I2C_MASK          = 0x00000020,
        SYSAHBCLKCTRL_GPIO              = 0x00000040, // Enables clock for GPIO
        SYSAHBCLKCTRL_GPIO_MASK         = 0x00000040,
        SYSAHBCLKCTRL_CT16B0            = 0x00000080, // Enables clock for 16-bit counter/timer 0
        SYSAHBCLKCTRL_CT16B0_MASK       = 0x00000080,
        SYSAHBCLKCTRL_CT16B1            = 0x00000100, // Enables clock for 16-bit counter/timer 1
        SYSAHBCLKCTRL_CT16B1_MASK       = 0x00000100,
        SYSAHBCLKCTRL_CT32B0            = 0x00000200, // Enables clock for 32-bit counter/timer 0
        SYSAHBCLKCTRL_CT32B0_MASK       = 0x00000200,
        SYSAHBCLKCTRL_CT32B1            = 0x00000400, // Enables clock for 32-bit counter/timer 1
        SYSAHBCLKCTRL_CT32B1_MASK       = 0x00000400,
        SYSAHBCLKCTRL_SSP0              = 0x00000800, // Enables clock for SSP0
        SYSAHBCLKCTRL_SSP0_MASK         = 0x00000800,
        SYSAHBCLKCTRL_UART              = 0x00001000, // Enables clock for UART.  UART pins must be configured
        SYSAHBCLKCTRL_UART_MASK         = 0x00001000, // in the IOCON block before the UART clock can be enabled.
        SYSAHBCLKCTRL_ADC               = 0x00002000, // Enables clock for ADC
        SYSAHBCLKCTRL_ADC_MASK          = 0x00002000,
        SYSAHBCLKCTRL_WDT               = 0x00008000, // Enables clock for watchdog timer
        SYSAHBCLKCTRL_WDT_MASK          = 0x00008000,
        SYSAHBCLKCTRL_IOCON             = 0x00010000, // Enables clock for IO configuration block
        SYSAHBCLKCTRL_IOCON_MASK        = 0x00010000,
        SYSAHBCLKCTRL_CAN               = 0x00020000, // Enables clock for CAN
        SYSAHBCLKCTRL_CAN_MASK          = 0x00020000,
        SYSAHBCLKCTRL_SSP1              = 0x00040000, // Enables clock for SSP1
        SYSAHBCLKCTRL_SSP1_MASK         = 0x00040000,
        SYSAHBCLKCTRL_ALL_MASK          = 0x0007FFFF,
    };

    enum SSPCLKDIV : uint32_t {
        SSPCLKDIV_DISABLE               = 0x00000000,
        SSPCLKDIV_DIV1                  = 0x00000001, // Divide SSP0 clock by 1 (can be set from 1..255)
        SSPCLKDIV_DIV2                  = 0x00000002,
        SSPCLKDIV_DIV3                  = 0x00000003,
        SSPCLKDIV_DIV4                  = 0x00000004,
        SSPCLKDIV_DIV5                  = 0x00000005,
        SSPCLKDIV_DIV6                  = 0x00000006,
        SSPCLKDIV_DIV10                 = 0x0000000A,
        SSPCLKDIV_MASK                  = 0x000000FF,
    };

    enum UARTCLKDIV : uint32_t {
        UARTCLKDIV_DISABLE              = 0x00000000,
        UARTCLKDIV_DIV1                 = 0x00000001, // Divide UART clock by 1 (can be set from 1..255)
        UARTCLKDIV_DIV2                 = 0x00000002,
        UARTCLKDIV_DIV4                 = 0x00000004,
        UARTCLKDIV_MASK                 = 0x000000FF,
    };

    enum WDTCLKSEL : uint32_t {
        WDTCLKSEL_SOURCE_INTERNALOSC    = 0x00000000, // Use the internal oscillator
        WDTCLKSEL_SOURCE_INPUTCLOCK     = 0x00000001, // Use the main clock
        WDTCLKSEL_SOURCE_WATCHDOGOSC    = 0x00000002, // Use the watchdog oscillator
        WDTCLKSEL_MASK                  = 0x00000003,
    };

    enum WDTCLKUEN : uint32_t {
        WDTCLKUEN_DISABLE               = 0x00000000,
        WDTCLKUEN_UPDATE                = 0x00000001,
        WDTCLKUEN_MASK                  = 0x00000001,
    };

    enum WDTCLKDIV : uint32_t {
        WDTCLKDIV_DISABLE               = 0x00000000,
        WDTCLKDIV_DIV1                  = 0x00000001, // Divide clock by 1 (can be set from 1..255)
        WDTCLKDIV_MASK                  = 0x000000FF,
    };

    enum CLKOUTCLKSEL : uint32_t {
        CLKOUTCLKSEL_SOURCE_USBPLLOUT   = 0x00000000, // USB PLL output
        CLKOUTCLKSEL_SOURCE_INPUTCLOCK  = 0x00000001, // Use the main clock
        CLKOUTCLKSEL_SOURCE_WATCHDOGOSC = 0x00000002, // Use the watchdog oscillator
        CLKOUTCLKSEL_MASK               = 0x00000003,
    };

    enum CLKOUTCLKUEN : uint32_t {
        CLKOUTCLKUEN_DISABLE            = 0x00000000,
        CLKOUTCLKUEN_UPDATE             = 0x00000001,
        CLKOUTCLKUEN_MASK               = 0x00000001,
    };

    enum CLKOUTCLKDIV : uint32_t {
        CLKOUTCLKDIV_DISABLE            = 0x00000000,
        CLKOUTCLKDIV_DIV1               = 0x00000001, // Divide clock by 1 (can be set from 1..255)
        CLKOUTCLKDIV_MASK               = 0x000000FF,
    };

    enum PIOPORCAP0 : uint32_t {
        PIOPORCAP0_PIO0_0               = 0x00000001,
        PIOPORCAP0_PIO0_0_MASK          = 0x00000001,
        PIOPORCAP0_PIO0_1               = 0x00000002,
        PIOPORCAP0_PIO0_1_MASK          = 0x00000002,
        PIOPORCAP0_PIO0_2               = 0x00000004,
        PIOPORCAP0_PIO0_2_MASK          = 0x00000004,
        PIOPORCAP0_PIO0_3               = 0x00000008,
        PIOPORCAP0_PIO0_3_MASK          = 0x00000008,
        PIOPORCAP0_PIO0_4               = 0x00000010,
        PIOPORCAP0_PIO0_4_MASK          = 0x00000010,
        PIOPORCAP0_PIO0_5               = 0x00000020,
        PIOPORCAP0_PIO0_5_MASK          = 0x00000020,
        PIOPORCAP0_PIO0_6               = 0x00000040,
        PIOPORCAP0_PIO0_6_MASK          = 0x00000040,
        PIOPORCAP0_PIO0_7               = 0x00000080,
        PIOPORCAP0_PIO0_7_MASK          = 0x00000080,
        PIOPORCAP0_PIO0_8               = 0x00000100,
        PIOPORCAP0_PIO0_8_MASK          = 0x00000100,
        PIOPORCAP0_PIO0_9               = 0x00000200,
        PIOPORCAP0_PIO0_9_MASK          = 0x00000200,
        PIOPORCAP0_PIO0_10              = 0x00000400,
        PIOPORCAP0_PIO0_10_MASK         = 0x00000400,
        PIOPORCAP0_PIO0_11              = 0x00000800,
        PIOPORCAP0_PIO0_11_MASK         = 0x00000800,
        PIOPORCAP0_PIO1_0               = 0x00001000,
        PIOPORCAP0_PIO1_0_MASK          = 0x00001000,
        PIOPORCAP0_PIO1_1               = 0x00002000,
        PIOPORCAP0_PIO1_1_MASK          = 0x00002000,
        PIOPORCAP0_PIO1_2               = 0x00004000,
        PIOPORCAP0_PIO1_2_MASK          = 0x00004000,
        PIOPORCAP0_PIO1_3               = 0x00008000,
        PIOPORCAP0_PIO1_3_MASK          = 0x00008000,
        PIOPORCAP0_PIO1_4               = 0x00010000,
        PIOPORCAP0_PIO1_4_MASK          = 0x00010000,
        PIOPORCAP0_PIO1_5               = 0x00020000,
        PIOPORCAP0_PIO1_5_MASK          = 0x00020000,
        PIOPORCAP0_PIO1_6               = 0x00040000,
        PIOPORCAP0_PIO1_6_MASK          = 0x00040000,
        PIOPORCAP0_PIO1_7               = 0x00080000,
        PIOPORCAP0_PIO1_7_MASK          = 0x00080000,
        PIOPORCAP0_PIO1_8               = 0x00100000,
        PIOPORCAP0_PIO1_8_MASK          = 0x00100000,
        PIOPORCAP0_PIO1_9               = 0x00200000,
        PIOPORCAP0_PIO1_9_MASK          = 0x00200000,
        PIOPORCAP0_PIO1_10              = 0x00400000,
        PIOPORCAP0_PIO1_10_MASK         = 0x00400000,
        PIOPORCAP0_PIO1_11              = 0x00800000,
        PIOPORCAP0_PIO1_11_MASK         = 0x00800000,
        PIOPORCAP0_PIO2_0               = 0x01000000,
        PIOPORCAP0_PIO2_0_MASK          = 0x01000000,
        PIOPORCAP0_PIO2_1               = 0x02000000,
        PIOPORCAP0_PIO2_1_MASK          = 0x02000000,
        PIOPORCAP0_PIO2_2               = 0x04000000,
        PIOPORCAP0_PIO2_2_MASK          = 0x04000000,
        PIOPORCAP0_PIO2_3               = 0x08000000,
        PIOPORCAP0_PIO2_3_MASK          = 0x08000000,
        PIOPORCAP0_PIO2_4               = 0x10000000,
        PIOPORCAP0_PIO2_4_MASK          = 0x10000000,
        PIOPORCAP0_PIO2_5               = 0x20000000,
        PIOPORCAP0_PIO2_5_MASK          = 0x20000000,
        PIOPORCAP0_PIO2_6               = 0x40000000,
        PIOPORCAP0_PIO2_6_MASK          = 0x40000000,
        PIOPORCAP0_PIO2_7               = 0x80000000,
        PIOPORCAP0_PIO2_7_MASK          = 0x80000000,
    };

    enum PIOPORCAP1 : uint32_t {
        PIOPORCAP1_PIO2_8               = 0x00000001,
        PIOPORCAP1_PIO2_8_MASK          = 0x00000001,
        PIOPORCAP1_PIO2_9               = 0x00000002,
        PIOPORCAP1_PIO2_9_MASK          = 0x00000002,
        PIOPORCAP1_PIO2_10              = 0x00000004,
        PIOPORCAP1_PIO2_10_MASK         = 0x00000004,
        PIOPORCAP1_PIO2_11              = 0x00000008,
        PIOPORCAP1_PIO2_11_MASK         = 0x00000008,
        PIOPORCAP1_PIO3_0               = 0x00000010,
        PIOPORCAP1_PIO3_0_MASK          = 0x00000010,
        PIOPORCAP1_PIO3_1               = 0x00000020,
        PIOPORCAP1_PIO3_1_MASK          = 0x00000020,
        PIOPORCAP1_PIO3_2               = 0x00000040,
        PIOPORCAP1_PIO3_2_MASK          = 0x00000040,
        PIOPORCAP1_PIO3_3               = 0x00000080,
        PIOPORCAP1_PIO3_3_MASK          = 0x00000080,
        PIOPORCAP1_PIO3_4               = 0x00000100,
        PIOPORCAP1_PIO3_4_MASK          = 0x00000100,
        PIOPORCAP1_PIO3_5               = 0x00000200,
        PIOPORCAP1_PIO3_5_MASK          = 0x00000200,
    };

    enum BODCTRL : uint32_t {
        BODCTRL_RSTLEVEL_1_46V_1_63V    = 0x00000000,
        BODCTRL_RSTLEVEL_2_06V_2_15V    = 0x00000001,
        BODCTRL_RSTLEVEL_2_35V_2_43V    = 0x00000002,
        BODCTRL_RSTLEVEL_2_63V_2_71V    = 0x00000003,
        BODCTRL_RSTLEVEL_MASK           = 0x00000003,
        BODCTRL_INTLEVEL_1_65V_1_80V    = 0x00000000,
        BODCTRL_INTLEVEL_2_22V_2_35V    = 0x00000004,
        BODCTRL_INTLEVEL_2_52V_2_66V    = 0x00000008,
        BODCTRL_INTLEVEL_2_80V_2_90V    = 0x0000000C,
        BODCTRL_INTLEVEL_MASK           = 0x0000000C,
        BODCTRL_RSTENABLE_DISABLE       = 0x00000000,
        BODCTRL_RSTENABLE_ENABLE        = 0x00000010,
        BODCTRL_RSTENABLE_MASK          = 0x00000010,
    };

    enum SYSTICKCCAL : uint32_t {
        SYSTICKCCAL_MASK                = 0x03FFFFFF, // Undefined as of v0.10 of the LPC1114 User Manual
    };

    enum STARTAPRP0 : uint32_t {
        STARTAPRP0_APRPIO0_0            = 0x00000001,
        STARTAPRP0_APRPIO0_0_MASK       = 0x00000001,
        STARTAPRP0_APRPIO0_1            = 0x00000002,
        STARTAPRP0_APRPIO0_1_MASK       = 0x00000002,
        STARTAPRP0_APRPIO0_2            = 0x00000004,
        STARTAPRP0_APRPIO0_2_MASK       = 0x00000004,
        STARTAPRP0_APRPIO0_3            = 0x00000008,
        STARTAPRP0_APRPIO0_3_MASK       = 0x00000008,
        STARTAPRP0_APRPIO0_4            = 0x00000010,
        STARTAPRP0_APRPIO0_4_MASK       = 0x00000010,
        STARTAPRP0_APRPIO0_5            = 0x00000020,
        STARTAPRP0_APRPIO0_5_MASK       = 0x00000020,
        STARTAPRP0_APRPIO0_6            = 0x00000040,
        STARTAPRP0_APRPIO0_6_MASK       = 0x00000040,
        STARTAPRP0_APRPIO0_7            = 0x00000080,
        STARTAPRP0_APRPIO0_7_MASK       = 0x00000080,
        STARTAPRP0_APRPIO0_8            = 0x00000100,
        STARTAPRP0_APRPIO0_8_MASK       = 0x00000100,
        STARTAPRP0_APRPIO0_9            = 0x00000200,
        STARTAPRP0_APRPIO0_9_MASK       = 0x00000200,
        STARTAPRP0_APRPIO0_10           = 0x00000400,
        STARTAPRP0_APRPIO0_10_MASK      = 0x00000400,
        STARTAPRP0_APRPIO0_11           = 0x00000800,
        STARTAPRP0_APRPIO0_11_MASK      = 0x00000800,
        STARTAPRP0_APRPIO1_0            = 0x00001000,
        STARTAPRP0_APRPIO1_0_MASK       = 0x00001000,
        STARTAPRP0_MASK                 = 0xFFFFFFFF,
    };

    enum STARTERP0 : uint32_t {
        STARTERP0_ERPIO0_0              = 0x00000001,
        STARTERP0_ERPIO0_0_MASK         = 0x00000001,
        STARTERP0_ERPIO0_1              = 0x00000002,
        STARTERP0_ERPIO0_1_MASK         = 0x00000002,
        STARTERP0_ERPIO0_2              = 0x00000004,
        STARTERP0_ERPIO0_2_MASK         = 0x00000004,
        STARTERP0_ERPIO0_3              = 0x00000008,
        STARTERP0_ERPIO0_3_MASK         = 0x00000008,
        STARTERP0_ERPIO0_4              = 0x00000010,
        STARTERP0_ERPIO0_4_MASK         = 0x00000010,
        STARTERP0_ERPIO0_5              = 0x00000020,
        STARTERP0_ERPIO0_5_MASK         = 0x00000020,
        STARTERP0_ERPIO0_6              = 0x00000040,
        STARTERP0_ERPIO0_6_MASK         = 0x00000040,
        STARTERP0_ERPIO0_7              = 0x00000080,
        STARTERP0_ERPIO0_7_MASK         = 0x00000080,
        STARTERP0_ERPIO0_8              = 0x00000100,
        STARTERP0_ERPIO0_8_MASK         = 0x00000100,
        STARTERP0_ERPIO0_9              = 0x00000200,
        STARTERP0_ERPIO0_9_MASK         = 0x00000200,
        STARTERP0_ERPIO0_10             = 0x00000400,
        STARTERP0_ERPIO0_10_MASK        = 0x00000400,
        STARTERP0_ERPIO0_11             = 0x00000800,
        STARTERP0_ERPIO0_11_MASK        = 0x00000800,
        STARTERP0_ERPIO1_0              = 0x00001000,
        STARTERP0_ERPIO1_0_MASK         = 0x00001000,
        STARTERP0_MASK                  = 0xFFFFFFFF,
    };

    enum STARTRSRP0CLR : uint32_t {
        STARTRSRP0CLR_RSRPIO0_0         = 0x00000001,
        STARTRSRP0CLR_RSRPIO0_0_MASK    = 0x00000001,
        STARTRSRP0CLR_RSRPIO0_1         = 0x00000002,
        STARTRSRP0CLR_RSRPIO0_1_MASK    = 0x00000002,
        STARTRSRP0CLR_RSRPIO0_2         = 0x00000004,
        STARTRSRP0CLR_RSRPIO0_2_MASK    = 0x00000004,
        STARTRSRP0CLR_RSRPIO0_3         = 0x00000008,
        STARTRSRP0CLR_RSRPIO0_3_MASK    = 0x00000008,
        STARTRSRP0CLR_RSRPIO0_4         = 0x00000010,
        STARTRSRP0CLR_RSRPIO0_4_MASK    = 0x00000010,
        STARTRSRP0CLR_RSRPIO0_5         = 0x00000020,
        STARTRSRP0CLR_RSRPIO0_5_MASK    = 0x00000020,
        STARTRSRP0CLR_RSRPIO0_6         = 0x00000040,
        STARTRSRP0CLR_RSRPIO0_6_MASK    = 0x00000040,
        STARTRSRP0CLR_RSRPIO0_7         = 0x00000080,
        STARTRSRP0CLR_RSRPIO0_7_MASK    = 0x00000080,
        STARTRSRP0CLR_RSRPIO0_8         = 0x00000100,
        STARTRSRP0CLR_RSRPIO0_8_MASK    = 0x00000100,
        STARTRSRP0CLR_RSRPIO0_9         = 0x00000200,
        STARTRSRP0CLR_RSRPIO0_9_MASK    = 0x00000200,
        STARTRSRP0CLR_RSRPIO0_10        = 0x00000400,
        STARTRSRP0CLR_RSRPIO0_10_MASK   = 0x00000400,
        STARTRSRP0CLR_RSRPIO0_11        = 0x00000800,
        STARTRSRP0CLR_RSRPIO0_11_MASK   = 0x00000800,
        STARTRSRP0CLR_RSRPIO1_0         = 0x00001000,
        STARTRSRP0CLR_RSRPIO1_0_MASK    = 0x00001000,
        STARTRSRP0CLR_MASK              = 0xFFFFFFFF,
    };

    enum STARTSPR0 : uint32_t {
        STARTSPR0_SRPIO0_0              = 0x00000001,
        STARTSPR0_SRPIO0_0_MASK         = 0x00000001,
        STARTSPR0_SRPIO0_1              = 0x00000002,
        STARTSPR0_SRPIO0_1_MASK         = 0x00000002,
        STARTSPR0_SRPIO0_2              = 0x00000004,
        STARTSPR0_SRPIO0_2_MASK         = 0x00000004,
        STARTSPR0_SRPIO0_3              = 0x00000008,
        STARTSPR0_SRPIO0_3_MASK         = 0x00000008,
        STARTSPR0_SRPIO0_4              = 0x00000010,
        STARTSPR0_SRPIO0_4_MASK         = 0x00000010,
        STARTSPR0_SRPIO0_5              = 0x00000020,
        STARTSPR0_SRPIO0_5_MASK         = 0x00000020,
        STARTSPR0_SRPIO0_6              = 0x00000040,
        STARTSPR0_SRPIO0_6_MASK         = 0x00000040,
        STARTSPR0_SRPIO0_7              = 0x00000080,
        STARTSPR0_SRPIO0_7_MASK         = 0x00000080,
        STARTSPR0_SRPIO0_8              = 0x00000100,
        STARTSPR0_SRPIO0_8_MASK         = 0x00000100,
        STARTSPR0_SRPIO0_9              = 0x00000200,
        STARTSPR0_SRPIO0_9_MASK         = 0x00000200,
        STARTSPR0_SRPIO0_10             = 0x00000400,
        STARTSPR0_SRPIO0_10_MASK        = 0x00000400,
        STARTSPR0_SRPIO0_11             = 0x00000800,
        STARTSPR0_SRPIO0_11_MASK        = 0x00000800,
        STARTSPR0_SRPIO1_0              = 0x00001000,
        STARTSPR0_SRPIO1_0_MASK         = 0x00001000,
        STARTSPR0_MASK                  = 0xFFFFFFFF,
    };

    enum PDSLEEPCFG : uint32_t {
        PDSLEEPCFG_BOD_ON_WDOSC_ON      = 0x000018B7,
        PDSLEEPCFG_BOD_ON_WDOSC_OFF     = 0x000018F7,
        PDSLEEPCFG_BOD_OFF_WDOSC_ON     = 0x000018BF,
        PDSLEEPCFG_BOD_OFF_WDOSC_OFF    = 0x000018FF,
    };

    enum PDAWAKECFG : uint32_t {
        PDAWAKECFGIRCOUT_PD             = 0x00000001,
        PDAWAKECFGIRCOUT_PD_MASK        = 0x00000001,
        PDAWAKECFGIRC_PD                = 0x00000002,
        PDAWAKECFGIRC_PD_MASK           = 0x00000002,
        PDAWAKECFGFLASH_PD              = 0x00000004,
        PDAWAKECFGFLASH_PD_MASK         = 0x00000004,
        PDAWAKECFGBOD_PD                = 0x00000008,
        PDAWAKECFGBOD_PD_MASK           = 0x00000008,
        PDAWAKECFGADC_PD                = 0x00000010,
        PDAWAKECFGADC_PD_MASK           = 0x00000010,
        PDAWAKECFGSYSOSC_PD             = 0x00000020,
        PDAWAKECFGSYSOSC_PD_MASK        = 0x00000020,
        PDAWAKECFGWDTOSC_PD             = 0x00000040,
        PDAWAKECFGWDTOSC_PD_MASK        = 0x00000040,
        PDAWAKECFGSYSPLL_PD             = 0x00000080,
        PDAWAKECFGSYSPLL_PD_MASK        = 0x00000080,
    };

    enum PDRUNCFG : uint32_t {
        PDRUNCFG_IRCOUT                 = 0x00000001, // IRC oscillator output power-down
        PDRUNCFG_IRCOUT_MASK            = 0x00000001,
        PDRUNCFG_IRC                    = 0x00000002, // IRC oscillator power-down
        PDRUNCFG_IRC_MASK               = 0x00000002,
        PDRUNCFG_FLASH                  = 0x00000004, // Flash power-down
        PDRUNCFG_FLASH_MASK             = 0x00000004,
        PDRUNCFG_BOD                    = 0x00000008, // Brown-out detector power-down
        PDRUNCFG_BOD_MASK               = 0x00000008,
        PDRUNCFG_ADC                    = 0x00000010, // ADC power-down
        PDRUNCFG_ADC_MASK               = 0x00000010,
        PDRUNCFG_SYSOSC                 = 0x00000020, // System oscillator power-down
        PDRUNCFG_SYSOSC_MASK            = 0x00000020,
        PDRUNCFG_WDTOSC                 = 0x00000040, // Watchdog oscillator power-down
        PDRUNCFG_WDTOSC_MASK            = 0x00000040,
        PDRUNCFG_SYSPLL                 = 0x00000080, // System PLL power-down
        PDRUNCFG_SYSPLL_MASK            = 0x00000080,
    };

    enum DEVICEID : uint32_t {
        DEVICEID_LPC1111_101            = 0x041E502B,
        DEVICEID_LPC1111_102            = 0x2516902B,
        DEVICEID_LPC1111_201            = 0x0416502B,
        DEVICEID_LPC1111_202            = 0x2516D02B,
        DEVICEID_LPC1112_101            = 0x042D502B,
        DEVICEID_LPC1112_102            = 0x2524D02B,
        DEVICEID_LPC1112_201            = 0x0425502B,
        DEVICEID_LPC1112_202            = 0x2524902B,
        DEVICEID_LPC1113_201            = 0x0434502B,
        DEVICEID_LPC1113_202            = 0x2532902B,
        DEVICEID_LPC1113_301            = 0x0434102B,
        DEVICEID_LPC1113_302            = 0x2532102B,
        DEVICEID_LPC1114_201            = 0x0444502B,
        DEVICEID_LPC1114_202            = 0x2540902B,
        DEVICEID_LPC1114_301            = 0x0444102B,
        DEVICEID_LPC1114_302            = 0x2540102B,
        DEVICEID_LPC11C12_301           = 0x1421102B,
        DEVICEID_LPC11C14_301           = 0x1440102B,
        DEVICEID_LPC11C22_301           = 0x1431102B,
        DEVICEID_LPC11C24_301           = 0X1430102B,
    };

    enum CPUID : uint32_t {
        CPUID_REVISION_MASK             = 0x0000000F, // Revision Code
        CPUID_PARTNO_MASK               = 0x0000FFF0, // Part Number
        CPUID_CONSTANT_MASK             = 0x000F0000, // Constant
        CPUID_VARIANT_MASK              = 0x00F00000, // Variant
        CPUID_IMPLEMENTER_MASK          = 0xFF000000, // Implementer
    };

    enum ICSR : uint32_t {
        ICSR_NMIPENDSET_MASK            = 0x80000000,
        ICSR_NMIPENDSET                 = 0x80000000,
        ICSR_PENDSVSET_MASK             = 0x10000000,
        ICSR_PENDSVSET                  = 0x10000000,
        ICSR_PENDSVCLR_MASK             = 0x08000000,
        ICSR_PENDSVCLR                  = 0x08000000,
        ICSR_PENDSTSET_MASK             = 0x04000000,
        ICSR_PENDSTSET                  = 0x04000000,
        ICSR_PENDSTCLR_MASK             = 0x02000000,
        ICSR_PENDSTCLR                  = 0x02000000,
        ICSR_ISRPREEMPT_MASK            = 0x00800000,
        ICSR_ISRPREEMPT                 = 0x00800000,
        ICSR_ISRPENDING_MASK            = 0x00400000,
        ICSR_ISRPENDING                 = 0x00400000,
        ICSR_VECTPENDING_MASK           = 0x001FF000,
        ICSR_VECTACTIVE_MASK            = 0x000001FF,
    };

    enum AIRCR : uint32_t {
        AIRCR_VECTKEY_VALUE             = 0x05FA0000, // Vect key needs to be set to 05FA for reset to work
        AIRCR_VECTKEY_MASK              = 0xFFFF0000,
        AIRCR_ENDIANESS                 = 0x00008000, // Read Endianness (1=Big, 0=Little)
        AIRCR_ENDIANESS_MASK            = 0x00008000,
        AIRCR_SYSRESETREQ               = 0x00000004, // Request system reset
        AIRCR_SYSRESETREQ_MASK          = 0x00000004,
        AIRCR_VECTCLRACTIVE             = 0x00000002, // Used to prevent accidental reset
        AIRCR_VECTCLRACTIVE_MASK        = 0x00000002,
    };

    enum SCR : uint32_t {
        SCR_SLEEPONEXIT_MASK            = 0x00000002, // Enable sleep on exit
        SCR_SLEEPONEXIT                 = 0x00000002,
        SCR_SLEEPDEEP_MASK              = 0x00000004,
        SCR_SLEEPDEEP                   = 0x00000004, // Enable deep sleep
        SCR_SEVONPEND_MASK              = 0x00000010, // Wake up from WFE is new int is pended regardless of priority
        SCR_SEVONPEND                   = 0x00000010,
    };

    constexpr Syscon(uint32_t clock, uint32_t reset = 0) :
        _clock_bit(clock),
        _reset_mask(reset)
    {}

    void reset() const __always_inline
    {
        if (_reset_mask != 0) {
            LPC_SYSCON->PRESETCTRL &= ~_reset_mask;
            LPC_SYSCON->PRESETCTRL |= _reset_mask;
        }
    }

    void clock(bool enable) const __always_inline
    {
        if (enable) {
            LPC_SYSCON->SYSAHBCLKCTRL |= _clock_bit;
        } else {
            LPC_SYSCON->SYSAHBCLKCTRL &= ~_clock_bit;
        }
    }

    static void     init_48MHz();           // run CPU & fabric @ 48MHz

    static void     set_uart_prescale(unsigned idiv) { LPC_SYSCON->UARTCLKDIV = idiv; }
    static void     set_ssp0_prescale(unsigned idiv) { LPC_SYSCON->SSP0CLKDIV = idiv; }
    static void     set_ssp1_prescale(unsigned idiv) { LPC_SYSCON->SSP1CLKDIV = idiv; }

private:
    const unsigned  _clock_bit;
    const unsigned  _reset_mask;
};

#define SYSCON_SYS      Syscon(Syscon::SYSAHBCLKCTRL_SYS)
#define SYSCON_ROM      Syscon(Syscon::SYSAHBCLKCTRL_ROM)
#define SYSCON_RAM      Syscon(Syscon::SYSAHBCLKCTRL_RAM)
#define SYSCON_FLASHREG Syscon(Syscon::SYSAHBCLKCTRL_FLASHREG)
#define SYSCON_FLASH    Syscon(Syscon::SYSAHBCLKCTRL_FLASHARRAY)
#define SYSCON_I2C      Syscon(Syscon::SYSAHBCLKCTRL_I2C, Syscon::PRESETCTRL_I2C_MASK)
#define SYSCON_GPIO     Syscon(Syscon::SYSAHBCLKCTRL_GPIO)
#define SYSCON_CT16B0   Syscon(Syscon::SYSAHBCLKCTRL_CT16B0)
#define SYSCON_CT16B1   Syscon(Syscon::SYSAHBCLKCTRL_CT16B1)
#define SYSCON_CT32B0   Syscon(Syscon::SYSAHBCLKCTRL_CT32B0)
#define SYSCON_CT32B1   Syscon(Syscon::SYSAHBCLKCTRL_CT32B1)
#define SYSCON_SSP0     Syscon(Syscon::SYSAHBCLKCTRL_SSP0, Syscon::PRESETCTRL_SSP0_MASK)
#define SYSCON_UART     Syscon(Syscon::SYSAHBCLKCTRL_UART)
#define SYSCON_ADC      Syscon(Syscon::SYSAHBCLKCTRL_ADC)
#define SYSCON_WDT      Syscon(Syscon::SYSAHBCLKCTRL_WDT)
#define SYSCON_IOCON    Syscon(Syscon::SYSAHBCLKCTRL_IOCON)
#define SYSCON_CAN      Syscon(Syscon::SYSAHBCLKCTRL_CAN, Syscon::PRESETCTRL_CAN_MASK)
#define SYSCON_SSP1     Syscon(Syscon::SYSAHBCLKCTRL_SSP1, Syscon::PRESETCTRL_SSP1_MASK)
