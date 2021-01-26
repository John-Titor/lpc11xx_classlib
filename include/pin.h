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

#include <sys/cdefs.h>

#include <LPC11xx.h>

class Pin
{
public:
    enum Modifier : uint32_t {

        // MODE
        NoPull      = (0U << 3),
        PullDown    = (1U << 3),
        PullUp      = (2U << 3),
        Repeater    = (3U << 3),

        // HYS
        Hysteresis  = (1U << 5),

        // ADMODE
        Analog      = (0U << 7),
        Digital     = (1U << 7),

        // I2CMODE
        I2CStandard = (0U << 8),
        I2CNone     = (1U << 8),
        I2CFastPlus = (2U << 8),

        // OD
        PushPull    = (0U << 10),
        OpenDrain   = (1U << 10),
    };

private:
    __IO uint32_t        *const _iocon_reg;
    const uint32_t      _iocon_val;
    __IO uint32_t        *const _loc_reg;
    const uint32_t      _loc_val;


public:
    constexpr Pin(__IO uint32_t *iocon_reg,
                  uint32_t iocon_val,
                  __IO uint32_t *loc_reg = 0,
                  uint32_t loc_val = 0) :
        _iocon_reg(iocon_reg),
        _iocon_val(iocon_val),
        _loc_reg(loc_reg),
        _loc_val(loc_val)
    {}

    Pin &configure(uint32_t modifier = 0) {
        if (_loc_reg) {
            *_loc_reg = _loc_val;
        }

        *_iocon_reg = _iocon_val | modifier;
        return *this;
    }
};

class Gpio : public Pin
{
public:
    enum Direction_t {
        Input,
        Output
    };

private:
    LPC_GPIO_TypeDef     *const _port;
    const unsigned      _pin_number;
    const unsigned      _pin_mask;

public:
    constexpr Gpio(unsigned port_number,
                   unsigned pin_number,
                   __IO uint32_t *iocon_reg,
                   uint32_t iocon_val) :
        Pin(iocon_reg, iocon_val),
        _port((port_number == 0) ? LPC_GPIO0 :
              (port_number == 1) ? LPC_GPIO1 :
              (port_number == 2) ? LPC_GPIO2 : LPC_GPIO3),
        _pin_number(pin_number),
        _pin_mask(1U << _pin_number)
    {}

    Gpio &configure(Direction_t direction, uint32_t modifier = 0) {
        if (direction == Output)
        {
            _port->DIR |= _pin_mask;
        } else
        {
            _port->DIR &= ~_pin_mask;
        }

        Pin::configure(modifier);
        return *this;
    }

    bool        get()    const { return _port->MASKED_ACCESS[1 << _pin_number] != 0; }
    void        clear()  const { _port->MASKED_ACCESS[1 << _pin_number] = 0; }
    void        set()    const { _port->MASKED_ACCESS[1 << _pin_number] = _pin_mask; }
    void        toggle() const { _port->MASKED_ACCESS[1 << _pin_number] ^= _pin_mask; }

    operator    bool()   const { return get(); }
    const Gpio &operator    =(bool from) const { set(from); return *this; }

    __always_inline void set(bool value) const
    {
        if (value) {
            set();
        } else {
            clear();
        }
    }

    __always_inline const Gpio &operator << (int i) const
    {
        set(i != 0);
        return * this;
    }

    __always_inline const Gpio &operator << (bool s) const
    {
        set(s);
        return * this;
    }

    __always_inline const Gpio &operator=(const Gpio &from) const
    {
        set(from.get());
        return *this;
    }

    // XXX TODO interrupts

};

#define P0_0_nRESET         Pin(&LPC_IOCON->RESET_PIO0_0, 0 | Pin::PULL_UP)
#define P0_0                Gpio(0, 0, &LPC_IOCON->RESET_PIO0_0, 1)

#define P0_1                Gpio(0, 1, &LPC_IOCON->PIO0_1, 0)
#define P0_1_CLKOUT         Pin(&LPC_IOCON->PIO0_1, 1)
#define P0_1_CT32B0_MAT2    Pin(&LPC_IOCON->PIO0_1, 2)

#define P0_2                Gpio(0, 2, &LPC_IOCON->PIO0_2, 0)
#define P0_2_SSEL0          Pin(&LPC_IOCON->PIO0_2, 1)
#define P0_2_CT16B0_CAP0    Pin(&LPC_IOCON->PIO0_2, 2, &LPC_IOCON->CT16B0_CAP0_LOC, 0)

#define P0_3                Gpio(0, 3, &LPC_IOCON->PIO0_3, 0)

#define P0_4                Gpio(0, 4, &LPC_IOCON->PIO0_4, 0 | Pin::I2CModeNone)
#define P0_4_SCL            Pin(&LPC_IOCON->PIO0_4, 1)

#define P0_5                Gpio(0, 5, &LPC_IOCON->PIO0_5, 0 | Pin::I2CModeNone)
#define P0_5_SDA            Pin(&LPC_IOCON->PIO0_5, 1)

#define P0_6                Gpio(0, 6, &LPC_IOCON->PIO0_6, 0)
#define P0_6_SCK0_0_6       Pin(&LPC_IOCON->PIO0_6, 2, &LPC_IOCON->SCK_LOC, 2)

#define P0_7                Gpio(0, 7, &LPC_IOCON->PIO0_7, 0)
#define P0_7_nCTS           Pin(&LPC_IOCON->PIO0_7, 1)

#define P0_8                Gpio(0, 8, &LPC_IOCON->PIO0_8, 0)
#define P0_8_MISO0          Pin(&LPC_IOCON->PIO0_8, 1)
#define P0_8_CT16B0_MAT0    Pin(&LPC_IOCON->PIO0_8, 2)

#define P0_9                Gpio(0, 9, &LPC_IOCON->PIO0_9, 0)
#define P0_9_MOSI0          Pin(&LPC_IOCON->PIO0_9, 1)
#define P0_9_CT16B0_MAT1    Pin(&LPC_IOCON->PIO0_9, 2)

#define P0_10               Gpio(0, 10, &LPC_IOCON->SWCLK_PIO0_10, 1)
#define P0_10_SWCLK         Pin(&LPC_IOCON->SWCLK_PIO0_10, 0)
#define P0_10_SCK0          Pin(&LPC_IOCON->SWCLK_PIO0_10, 2, &LPC_IOCON->SCK_LOC, 0)
#define P0_10_CT16B0_MAT2   Pin(&LPC_IOCON->SWCLK_PIO0_10, 3)

#define P0_11               Gpio(0, 11, &LPC_IOCON->R_PIO0_11, 1 | Pin::Digital)
#define P0_11_AD0           Pin(&LPC_IOCON->R_PIO0_11, 2 | Pin::Analog)
#define P0_11_CT32B0_MAT3   Pin(&LPC_IOCON->R_PIO0_11, 3 | Pin::Digital)

#define P1_0                Gpio(1, 0, &LPC_IOCON->PIO1_0, 1 | Pin::Digital)
#define P1_0_AD1            Pin(&LPC_IOCON->PIO1_0, 2 | Pin::Analog)
#define P1_0_CT32B1_CAP0    Pin(&LPC_IOCON->PIO1_0, 3 | Pin::Digital)

#define P1_1                Gpio(1, 1, &LPC_IOCON->PIO1_1, 1 | Pin::Digital)
#define P1_1_AD2            Pin(&LPC_IOCON->PIO1_1, 2 | Pin::Analog)
#define P1_1_CT32B1_MAT0    Pin(&LPC_IOCON->PIO1_1, 3 | Pin::Digital)

#define P1_2                Gpio(1, 2, &LPC_IOCON->PIO1_2, 1 | Pin::Digital)
#define P1_2_AD3            Pin(&LPC_IOCON->PIO1_2, 2 | Pin::Analog)
#define P1_2_CT32B1_MAT1    Pin(&LPC_IOCON->PIO1_2, 3 | Pin::Digital)

#define P1_3                Gpio(1, 3, &LPC_IOCON->SWDIO_PIO1_3, 1 | Pin::Digital)
#define P1_3_SWDIO          Pin(&LPC_IOCON->SWDIO_PIO1_3, 0 | Pin::Digital)
#define P1_3_AD4            Pin(&LPC_IOCON->SWDIO_PIO1_3, 2 | Pin::Analog)
#define P1_3_CT32B1_MAT2    Pin(&LPC_IOCON->SWDIO_PIO1_3, 3 | Pin::Digital)

#define P1_4                Gpio(1, 4, &LPC_IOCON->PIO1_4, 0 | Pin::Digital)
#define P1_4_AD5            Pin(&LPC_IOCON->PIO1_4, 1 | Pin::Analog)
#define P1_4_CT32B1_MAT3    Pin(&LPC_IOCON->PIO1_4, 2 | Pin::Digital)

#define P1_5                Gpio(1, 5, &LPC_IOCON->PIO1_5, 0)
#define P1_5_nRTS           Pin(&LPC_IOCON->PIO1_5, 1)
#define P1_5_CT32B0_CAP0    Pin(&LPC_IOCON->PIO1_5, 2, &LPC_IOCON->CT32B0_CAP0_LOC, 0)

#define P1_6                Gpio(1, 6, &LPC_IOCON->PIO1_6, 0)
#define P1_6_RXD            Pin(&LPC_IOCON->PIO1_6, 1, &LPC_IOCON->RXD_LOC, 0)
#define P1_6_CT32B0_MAT0    Pin(&LPC_IOCON->PIO1_6, 2)

#define P1_7                Gpio(1, 7, &LPC_IOCON->PIO1_7, 0)
#define P1_7_TXD            Pin(&LPC_IOCON->PIO1_7, 1)
#define P1_7_CT32B0_MAT1    Pin(&LPC_IOCON->PIO1_7, 2)

#define P1_8                Gpio(1, 8, &LPC_IOCON->PIO1_8, 0)
#define P1_8_CT16B1_CAP0    Pin(&LPC_IOCON->PIO1_8, 1)

#define P1_9                Gpio(1, 9, &LPC_IOCON->PIO1_9, 0)
#define P1_9_CT16B1_MAT0    Pin(&LPC_IOCON->PIO1_9, 1)
#define P1_9_MOSI1          in(&LPC_IOCON->PIO1_9, 2, &LPC_IOCON->MOSI1_LOC, 1)

#define P1_10               Gpio(1, 10, &LPC_IOCON->PIO1_10, 0 | Pin::Digital)
#define P1_10_AD6           Pin(&LPC_IOCON->PIO1_10, 1 | Pin::Analog)
#define P1_10_CT16B1_MAT1   Pin(&LPC_IOCON->PIO1_10, 2 | Pin::Digital)
#define P1_10_MISO1         Pin(&LPC_IOCON->PIO1_10, 3 | Pin::Digital, &LPC_IOCON->MIS01_LOC, 1)

#define P1_11               Gpio(1, 11, &LPC_IOCON->PIO1_11, 0 | Pin::Digital)
#define P1_11_AD7           Pin(&LPC_IOCON->PIO1_11, 1 | Pin::Analog)
#define P1_11_CT32B1_CAP1   Pin(&LPC_IOCON->PIO1_11, 2 | Pin::Digital)

#define P2_0                Gpio(2, 0, &LPC_IOCON->PIO2_0, 0)
#define P2_0_nDTR           Pin(&LPC_IOCON->PIO2_0, 1)
#define P2_0_SSEL1          Pin(&LPC_IOCON->PIO2_0, 2, &LPC_IOCON->SSEL1_LOC, 0)

#define P2_1                Gpio(2, 1, &LPC_IOCON->PIO2_1, 0)
#define P2_1_nDSR           Pin(&LPC_IOCON->PIO2_1, 1, &LPC_IOCON->DSR_LOC, 0)
#define P2_1_SCK1           Pin(&LPC_IOCON->PIO2_1, 2, &LPC_IOCON->SCK1_LOC, 0)

#define P2_2                Gpio(2, 2, &LPC_IOCON->PIO2_2, 0)
#define P2_2_nDCD           Pin(&LPC_IOCON->PIO2_2, 1, &LPC_IOCON->DCD_LOC, 0)
#define P2_2_MISO1          Pin(&LPC_IOCON->PIO2_2, 2, &LPC_IOCON->MIS01_LOC, 0)

#define P2_3                Gpio(2, 3, &LPC_IOCON->PIO2_3, 0)
#define P2_3_nRI            Pin(&LPC_IOCON->PIO2_3, 1, &LPC_IOCON->RI_LOC, 0)
#define P2_3_MOSI1          Pin(&LPC_IOCON->PIO2_3, 2, &LPC_IOCON->MOSI1_LOC, 0)

#define P2_4                Gpio(2, 4, &LPC_IOCON->PIO2_4, 0)
#define P2_4_CT16B1_MAT1    Pin(&LPC_IOCON->PIO2_4, 1)
#define P2_4_SSEL1          Pin(&LPC_IOCON->PIO2_4, 2, &LPC_IOCON->SSEL1_LOC, 1)

#define P2_5                Gpio(2, 5, &LPC_IOCON->PIO2_5, 0)
#define P2_5_CT32B0_MAT0    Pin(&LPC_IOCON->PIO2_5, 1)

#define P2_6                Gpio(2, 6, &LPC_IOCON->PIO2_6, 0)
#define P2_6_CT32B0_MAT1    Pin(&LPC_IOCON->PIO2_6, 1)

#define P2_7                Gpio(2, 7, &LPC_IOCON->PIO2_7, 0)
#define P2_7_CT32B0_MAT2    Pin(&LPC_IOCON->PIO2_7, 1)
#define P2_7_RXD            Pin(&LPC_IOCON->PIO2_7, 2, &LPC_IOCON->RXD_LOC, 1)

#define P2_8                Gpio(2, 8, &LPC_IOCON->PIO2_8, 0)
#define P2_8_CT32B0_MAT3    Pin(&LPC_IOCON->PIO2_8, 1)
#define P2_8_TXD            Pin(&LPC_IOCON->PIO2_8, 2)

#define P2_9                Gpio(2, 9, &LPC_IOCON->PIO2_9, 0)
#define P2_9_CT32B0_CAP0    Pin(&LPC_IOCON->PIO2_9, 1, &LPC_IOCON->CT32B0_CAP0_LOC, 1)

#define P2_10               Gpio(2, 10, &LPC_IOCON->PIO2_10, 0)

#define P2_11               Gpio(2, 11, &LPC_IOCON->PIO2_11, 0)
#define P2_11_SCK0          Pin(&LPC_IOCON->PIO2_11, 1, &LPC_IOCON->SCK_LOC, 1)
#define P2_11_CT32B0_CAP1   Pin(&LPC_IOCON->PIO2_11, 2)

#define P3_0                Gpio(3, 0, &LPC_IOCON->PIO3_0, 0)
#define P3_0_nDTR           Pin(&LPC_IOCON->PIO3_0, 1)
#define P3_0_CT16B0_MAT0    Pin(&LPC_IOCON->PIO3_0, 2)
#define P3_0_TXD            Pin(&LPC_IOCON->PIO3_0, 3)

#define P3_1                Gpio(3, 1, &LPC_IOCON->PIO3_1, 0)
#define P3_1_nDSR           Pin(&LPC_IOCON->PIO3_1, 1, &LPC_IOCON->DSR_LOC, 1)
#define P3_1_CT16B0_MAT1    Pin(&LPC_IOCON->PIO3_1, 2)
#define P3_1_nRXD           Pin(&LPC_IOCON->PIO3_1, 3, &LPC_IOCON->RXD_LOC, 2)

#define P3_2                Gpio(3, 2, &LPC_IOCON->PIO3_2, 0)
#define P3_2_nDCD           Pin(&LPC_IOCON->PIO3_2, 1, &LPC_IOCON->DCD_LOC, 1)
#define P3_2_CT16B0_MAT2    Pin(&LPC_IOCON->PIO3_2, 2)
#define P3_2_SCK1           Pin(&LPC_IOCON->PIO3_2, 3, &LPC_IOCON->SCK1_LOC, 1)

#define P3_3                Gpio(3, 3, &LPC_IOCON->PIO3_3, 0)
#define P3_3_nRI            Pin(&LPC_IOCON->PIO3_3, 1, &LPC_IOCON->RI_LOC, 1)
#define P3_3_CT16B0_CAP0    Pin(&LPC_IOCON->PIO3_3, 2, &LPC_IOCON->CT16B0_CAP0_LOC, 1)

#define P3_4                Gpio(3, 4, &LPC_IOCON->PIO3_4, 0)
#define P3_4_CT16B0_CAP1    Pin(&LPC_IOCON->PIO3_4, 1)
#define P3_4_RXD            Pin(&LPC_IOCON->PIO3_4, 2, &LPC_IOCON->RXD_LOC, 3)

#define P3_5                Gpio(3, 5, &LPC_IOCON->PIO3_5, 0)
#define P3_5_CT16B1_CAP1    Pin(&LPC_IOCON->PIO3_5, 1)
#define P3_5_TXD            Pin(&LPC_IOCON->PIO3_5, 2)
