// Copyright (c) 2018, 2021 Michael Smith, All Rights Reserved
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

#include <etl/queue_spsc_atomic.h>

#include "uart.h"

namespace
{
etl::queue_spsc_atomic<uint8_t,
    CONFIG_UART_TX_BUFFER,
    etl::memory_model::MEMORY_MODEL_SMALL> tx_queue;

etl::queue_spsc_atomic<uint8_t,
    CONFIG_UART_RX_BUFFER,
    etl::memory_model::MEMORY_MODEL_SMALL> rx_queue;
};

const UART &
UART::configure(unsigned rate) const
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
    LPC_UART->IER = IER_RBR_Interrupt_Enabled;  // enable RX interrupts
    _irq.enable();

    return *this;
}

// fractional divider logic from LPCOpen 2.00a
void
UART::set_divisors(uint32_t rate) const
{
    // divisor calculations from lpcopen_v2_00a
    uint32_t rate16 = 16U * rate;
    uint32_t dval = Syscon::PCLK_FREQ % rate16;
    uint32_t mval = 0;

    if (dval > 0) {
        mval = rate16 / dval;
        dval = 1;

        if (mval > 12) {
            dval = 0;
        }
    }

    dval &= 0xf;
    mval &= 0xf;
    uint32_t dl = Syscon::PCLK_FREQ / (rate16 + rate16 * dval / mval);

    LPC_UART->LCR |= LCR_Divisor_Latch_Access_Enabled;
    LPC_UART->DLL = dl & 0xff;
    LPC_UART->DLM = dl >> 8;
    LPC_UART->LCR &= ~LCR_Divisor_Latch_Access_Enabled;
    LPC_UART->FDR = (mval << 4) | dval;
}

void
UART::async_send(uint8_t c) const
{
    while (!tx_queue.push(c)) {
    }

    // If the transmit interrupt is disabled, the transmit path
    // is idle. Try to get a byte from the queue and if we succeed,
    // stuff it into the THR & re-enable the interrupt.
    if ((LPC_UART->IER & IER_THRE_Interrupt_MASK) == IER_THRE_Interrupt_Disabled) {
        if (tx_queue.pop(c)) {
            LPC_UART->THR = c;
            LPC_UART->IER |= IER_THRE_Interrupt_Enabled;
        }
    }
}

bool
UART::recv(uint8_t &c) const
{
    return rx_queue.pop(c);
}

bool
UART::recv_available() const
{
    return !rx_queue.empty();
}

bool
UART::send_space() const
{
    return !tx_queue.full();
}

void
UART::interrupt(void) const
{
    // receive any available bytes
    while (LPC_UART->LSR & LSR_RDR_DATA) {
        // we're going to drop bytes here; if we wanted
        // to implement flow control we'd want to check
        // the rx_queue and mask the interrupt...
        rx_queue.push(LPC_UART->RBR);
    }

    // send any available bytes we have space for
    while (LPC_UART->LSR & LSR_THRE) {
        uint8_t c;

        if (tx_queue.pop(c)) {
            LPC_UART->THR = c;
        } else {
            // we've run out of data to send, mask the interrupt
            LPC_UART->IER &= ~IER_THRE_Interrupt_Enabled;
            break;
        }
    }
}

void
UART_Handler(void)
{
    UART0.interrupt();
}
