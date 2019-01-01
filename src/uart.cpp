// Copyright (c) 2018 Michael Smith, All Rights Reserved
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

#include "uart.h"

UART &
UART::configure(unsigned rate)
{
    Syscon::set_uart_prescale(1);           // start UART clock & set 1:1 divisor
    LPC_UART->IER = 0;                      // disable interrupts
    LPC_UART->FCR = (FCR_FIFO_Enabled |     // FIFO must be enabled
                     FCR_Rx_FIFO_Reset |
                     FCR_Tx_FIFO_Reset |
                     FCR_Rx_Trigger_Level_Select_1Char);
    LPC_UART->MCR = 0;
    LPC_UART->LCR = (LCR_Word_Length_Select_8Chars |
                     LCR_Stop_Bit_Select_1Bits);
    set_divisors(rate);
    LPC_UART->ACR = 0;
    LPC_UART->TER = TER_TXEN_Enabled;

    return *this;
}

// fractional divider logic from LPCOpen 2.00a
void
UART::set_divisors(uint32_t rate)
{
    uint32_t dval, mval;
    uint32_t dl;
    uint32_t rate16 = 16U * rate;

    /* The fractional is calculated as (PCLK  % (16 * Baudrate)) / (16 * Baudrate)
     * Let's make it to be the ratio DivVal / MulVal
     */
    dval = Syscon::PCLK_FREQ % rate16;

   /* The PCLK / (16 * Baudrate) is fractional
    * => dval = pclk % rate16
    * mval = rate16;
    * now normalize the ratio
    * dval / mval = 1 / new_mval
    * new_mval = mval / dval
    * new_dval = 1
    */
    if (dval > 0) {
        mval = rate16 / dval;
        dval = 1;

        /* In case mval still bigger then 4 bits
        * no adjustment require
        */
        if (mval > 12) {
            dval = 0;
        }
    } else {
        mval = 0;
    }
    dval &= 0xf;
    mval &= 0xf;
    dl = Syscon::PCLK_FREQ / (rate16 + rate16 * dval / mval);

    LPC_UART->LCR |= LCR_Divisor_Latch_Access_Enabled;
    LPC_UART->DLL = dl & 0xff;
    LPC_UART->DLM = dl >> 8;
    LPC_UART->LCR &= ~LCR_Divisor_Latch_Access_Enabled;
    LPC_UART->FDR = (mval << 4) | dval;
}
