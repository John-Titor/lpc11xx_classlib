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

#include <config.h>
//#include <sys/cdefs.h>
#include <LPC11xx.h>
#include <etl/string.h>

#include "interrupt.h"
#include "syscon.h"

class UART
{
public:
    constexpr UART(bool polled = false) :
        _polled(polled),
        _irq(UART_IRQ)
    {}

    const UART &configure(unsigned rate) const;

    __always_inline void send(uint8_t c) const
    {
        if (_polled) {
            while (!(LPC_UART->LSR & LSR_TEMT)) {}

            LPC_UART->THR = c;
        } else {
            async_send(c);
        }
    }

    void send(char c)                   const { send((uint8_t)c); }
    void send(const char *s)            const { while (*s != '\0') send(*s++); }
    void send(const etl::istring &s)    const { for (auto c : s) send(c); }

    template<typename T> const UART &operator << (T c) const { send(c); return *this; }

    bool recv(uint8_t &c) const;
    bool recv(etl::istring &s) const;

    bool recv_available() const;
    bool send_space() const;

private:
    friend void UART_Handler(void);

    enum IER : uint32_t {
        IER_RBR_Interrupt_MASK                  = 0x00000001, // Enables the received data available interrupt
        IER_RBR_Interrupt_Enabled               = 0x00000001,
        IER_RBR_Interrupt_Disabled              = 0x00000000,
        IER_THRE_Interrupt_MASK                 = 0x00000002, // Enables the THRE interrupt
        IER_THRE_Interrupt_Enabled              = 0x00000002,
        IER_THRE_Interrupt_Disabled             = 0x00000000,
        IER_RLS_Interrupt_MASK                  = 0x00000004, // Enables the Rx line status interrupt
        IER_RLS_Interrupt_Enabled               = 0x00000004,
        IER_RLS_Interrupt_Disabled              = 0x00000000,
        IER_ABEOIntEn_MASK                      = 0x00000100, // End of auto-baud interrupt
        IER_ABEOIntEn_Enabled                   = 0x00000100,
        IER_ABEOIntEn_Disabled                  = 0x00000000,
        IER_ABTOIntEn_MASK                      = 0x00000200, // Auto-baud timeout interrupt
        IER_ABTOIntEn_Enabled                   = 0x00000200,
        IER_ABTOIntEn_Disabled                  = 0x00000000,
    };

    enum IIR : uint32_t {
        IIR_IntStatus_MASK                      = 0x00000001, // Interrupt status
        IIR_IntStatus_InterruptPending          = 0x00000000,
        IIR_IntStatus_NoInterruptPending        = 0x00000001,
        IIR_IntId_MASK                          = 0x0000000E, // Interrupt identification
        IIR_IntId_RLS                           = 0x00000006, // Receive line status
        IIR_IntId_RDA                           = 0x00000004, // Receive data available
        IIR_IntId_CTI                           = 0x0000000C, // Character time-out indicator
        IIR_IntId_THRE                          = 0x00000002, // THRE interrupt
        IIR_IntId_MODEM                         = 0x00000000, // Modem interrupt
        IIR_FIFO_Enable_MASK                    = 0x000000C0,
        IIR_ABEOInt_MASK                        = 0x00000100, // End of auto-baud interrupt
        IIR_ABEOInt                             = 0x00000100,
        IIR_ABTOInt_MASK                        = 0x00000200, // Auto-baud time-out interrupt
        IIR_ABTOInt                             = 0x00000200,
    };

    enum FCR : uint32_t {
        FCR_FIFO_Enable_MASK                    = 0x00000001, // UART FIFOs enabled/disabled
        FCR_FIFO_Enabled                        = 0x00000001,
        FCR_FIFO_Disabled                       = 0x00000000,
        FCR_Rx_FIFO_Reset_MASK                  = 0x00000002,
        FCR_Rx_FIFO_Reset                       = 0x00000002, // Clear Rx FIFO
        FCR_Tx_FIFO_Reset_MASK                  = 0x00000004,
        FCR_Tx_FIFO_Reset                       = 0x00000004, // Clear Tx FIFO
        FCR_Rx_Trigger_Level_Select_MASK        = 0x000000C0, // Chars written before before interrupt
        FCR_Rx_Trigger_Level_Select_1Char       = 0x00000000,
        FCR_Rx_Trigger_Level_Select_4Char       = 0x00000040,
        FCR_Rx_Trigger_Level_Select_8Char       = 0x00000080,
        FCR_Rx_Trigger_Level_Select_12Char      = 0x000000C0,
    };

    enum MCR : uint32_t {
        MCR_DTR_Control_MASK                    = 0x00000001, // Source for modem output pin DTR
        MCR_DTR_Control                         = 0x00000001,
        MCR_RTS_Control_MASK                    = 0x00000002, // Source for modem output pin RTS
        MCR_RTS_Control                         = 0x00000002,
        MCR_Loopback_Mode_Select_MASK           = 0x00000010, // Diagnostic loopback mode
        MCR_Loopback_Mode_Select_Enabled        = 0x00000010,
        MCR_Loopback_Mode_Select_Disabled       = 0x00000000,
        MCR_RTSen_MASK                          = 0x00000040, // Disable auto-rts flow control
        MCR_RTSen_Enabled                       = 0x00000040,
        MCR_RTSen_Disabled                      = 0x00000000,
        MCR_CTSen_MASK                          = 0x00000080, // Disable auto-cts flow control
        MCR_CTSen_Enabled                       = 0x00000080,
        MCR_CTSen_Disabled                      = 0x00000000,
    };

    enum LCR : uint32_t {
        LCR_Word_Length_Select_MASK             = 0x00000003, // Word Length Selector
        LCR_Word_Length_Select_5Chars           = 0x00000000,
        LCR_Word_Length_Select_6Chars           = 0x00000001,
        LCR_Word_Length_Select_7Chars           = 0x00000002,
        LCR_Word_Length_Select_8Chars           = 0x00000003,
        LCR_Stop_Bit_Select_MASK                = 0x00000004, // Stop bit select
        LCR_Stop_Bit_Select_1Bits               = 0x00000000,
        LCR_Stop_Bit_Select_2Bits               = 0x00000004,
        LCR_Parity_Enable_MASK                  = 0x00000008, // Parity enable
        LCR_Parity_Enabled                      = 0x00000008,
        LCR_Parity_Disabled                     = 0x00000000,
        LCR_Parity_Select_MASK                  = 0x00000030, // Parity select
        LCR_Parity_Select_OddParity             = 0x00000000,
        LCR_Parity_Select_EvenParity            = 0x00000010,
        LCR_Parity_Select_Forced1               = 0x00000020,
        LCR_Parity_Select_Forced0               = 0x00000030,
        LCR_Break_Control_MASK                  = 0x00000040, // Break transmission control
        LCR_Break_Control_Enabled               = 0x00000040,
        LCR_Break_Control_Disabled              = 0x00000000,
        LCR_Divisor_Latch_Access_MASK           = 0x00000080, // Divisor latch access
        LCR_Divisor_Latch_Access_Enabled        = 0x00000080,
        LCR_Divisor_Latch_Access_Disabled       = 0x00000000,
    };

    enum LSR : uint32_t {
        LSR_RDR_MASK                            = 0x00000001, // Receiver data ready
        LSR_RDR_EMPTY                           = 0x00000000, // RBR   is empty
        LSR_RDR_DATA                            = 0x00000001, // RBR   contains valid data
        LSR_OE_MASK                             = 0x00000002, // Overrun error
        LSR_OE                                  = 0x00000002,
        LSR_PE_MASK                             = 0x00000004, // Parity error
        LSR_PE                                  = 0x00000004,
        LSR_FE_MASK                             = 0x00000008, // Framing error
        LSR_FE                                  = 0x00000008,
        LSR_BI_MASK                             = 0x00000010, // Break interrupt
        LSR_BI                                  = 0x00000010,
        LSR_THRE_MASK                           = 0x00000020, // Transmitter holding register empty
        LSR_THRE                                = 0x00000020,
        LSR_TEMT_MASK                           = 0x00000040, // Transmitter empty
        LSR_TEMT                                = 0x00000040,
        LSR_RXFE_MASK                           = 0x00000080, // Error in Rx FIFO
        LSR_RXFE                                = 0x00000080,
    };

    enum MSR : uint32_t {
        MSR_Delta_CTS_MASK                      = 0x00000001, // State change of input CTS
        MSR_Delta_CTS                           = 0x00000001,
        MSR_Delta_DSR_MASK                      = 0x00000002, // State change of input DSR
        MSR_Delta_DSR                           = 0x00000002,
        MSR_Trailing_Edge_RI_MASK               = 0x00000004, // Low to high transition of input RI
        MSR_Trailing_Edge_RI                    = 0x00000004,
        MSR_Delta_DCD_MASK                      = 0x00000008, // State change of input DCD
        MSR_Delta_DCD                           = 0x00000008,
        MSR_CTS_MASK                            = 0x00000010, // Clear to send state
        MSR_CTS                                 = 0x00000010,
        MSR_DSR_MASK                            = 0x00000020, // Data set ready state
        MSR_DSR                                 = 0x00000020,
        MSR_RI_MASK                             = 0x00000040, // Ring indicator state
        MSR_RI                                  = 0x00000040,
        MSR_DCD_MASK                            = 0x00000080, // Data carrier detect state
        MSR_DCD                                 = 0x00000080,
    };

    enum ACR : uint32_t {
        ACR_Start_MASK                          = 0x00000001, // Auto-baud start/stop
        ACR_Start                               = 0x00000001,
        ACR_Stop                                = 0x00000000,
        ACR_Mode_MASK                           = 0x00000002, // Auto-baud mode select
        ACR_Mode_Mode1                          = 0x00000000,
        ACR_Mode_Mode2                          = 0x00000002,
        ACR_AutoRestart_MASK                    = 0x00000004,
        ACR_AutoRestart_NoRestart               = 0x00000000,
        ACR_AutoRestart_Restart                 = 0x00000004, // Restart in case of time-out
        ACR_ABEOIntClr_MASK                     = 0x00000100, // End of auto-baud interrupt clear bit
        ACR_ABEOIntClr                          = 0x00000100,
        ACR_ABTOIntClr_MASK                     = 0x00000200, // Auto-baud timeout interrupt clear bit
        ACR_ABTOIntClr                          = 0x00000200,
    };

    enum FDR : uint32_t {
        FDR_DIVADDVAL_MASK                      = 0x0000000F, // Fractional divider: prescaler register
        FDR_MULVAL_MASK                         = 0x000000F0, // Fractional divider: prescaler multiplier
    };

    enum TER : uint32_t {
        TER_TXEN_MASK                           = 0x00000080, // UART transmit enable
        TER_TXEN_Enabled                        = 0x00000080,
        TER_TXEN_Disabled                       = 0x00000000,
    };

    enum RS485CTRL : uint32_t {
        RS485CTRL_NMMEN_MASK                    = 0x00000001, // Normal multi-drop mode
        RS485CTRL_NMMEN                         = 0x00000001,
        RS485CTRL_RXDIS_MASK                    = 0x00000002, // Receiver
        RS485CTRL_RXDIS                         = 0x00000002,
        RS485CTRL_AADEN_MASK                    = 0x00000004, // Auto-address detect
        RS485CTRL_AADEN                         = 0x00000004,
        RS485CTRL_SEL_MASK                      = 0x00000008,
        RS485CTRL_SEL_RTS                       = 0x00000000, // Use RTS for direction control
        RS485CTRL_SEL_DTS                       = 0x00000008, // Use DTS for direction control
        RS485CTRL_DCTRL_MASK                    = 0x00000010, // Enable/Disable auto-direction control
        RS485CTRL_DCTRL_Disabled                = 0x00000000,
        RS485CTRL_DCTRL_Enabled                 = 0x00000010,
        RS485CTRL_OINV_MASK                     = 0x00000020, // Reverse polarity of direction control signal on RTS/DTR pin
        RS485CTRL_OINV_Normal                   = 0x00000000,
        RS485CTRL_OINV_Inverted                 = 0x00000020,
    };

    enum FIFOLVL : uint32_t {
        FIFOLVL_RXFIFOLVL_MASK                  = 0x0000000F,
        FIFOLVL_RXFIFOLVL_Empty                 = 0x00000000,
        FIFOLVL_RXFIFOLVL_Full                  = 0x0000000F,
        FIFOLVL_TXFIFOLVL_MASK                  = 0x00000F00,
        FIFOLVL_TXFIFOLVL_Empty                 = 0x00000000,
        FIFOLVL_TXFIFOLVL_Full                  = 0x00000F00,
    };

    const bool      _polled;
    const Interrupt _irq;

    void            set_divisors(uint32_t rate) const;
    void            async_send(uint8_t c) const;
    void            interrupt(void) const;
};

#define UART0           UART()
#define UART0_POLLED    UART(true)
