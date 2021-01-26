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

#pragma once

#include <LPC11xx.h>
#include "syscon.h"

class SSP
{
public:
    constexpr SSP(unsigned index) :
        _reg((index == 0) ? * LPC_SSP0 : * LPC_SSP1),
        _syscon((index == 0) ? SYSCON_SSP0 : SYSCON_SSP1)
    {}

    SSP         &configure(unsigned rate, unsigned nbits, unsigned mode);
    void        transfer(uint8_t *source, uint8_t *destination, unsigned count);
    void        transfer(uint16_t *source, uint16_t *destination, unsigned count);

private:
    LPC_SSP_TypeDef     &_reg;
    Syscon              _syscon;

    enum CR0 : uint32_t {
        CR0_DSS_MASK        = 0x0000000F, // Data size select
        CR0_DSS_4BIT        = 0x00000003,
        CR0_DSS_5BIT        = 0x00000004,
        CR0_DSS_6BIT        = 0x00000005,
        CR0_DSS_7BIT        = 0x00000006,
        CR0_DSS_8BIT        = 0x00000007,
        CR0_DSS_9BIT        = 0x00000008,
        CR0_DSS_10BIT       = 0x00000009,
        CR0_DSS_11BIT       = 0x0000000A,
        CR0_DSS_12BIT       = 0x0000000B,
        CR0_DSS_13BIT       = 0x0000000C,
        CR0_DSS_14BIT       = 0x0000000D,
        CR0_DSS_15BIT       = 0x0000000E,
        CR0_DSS_16BIT       = 0x0000000F,
        CR0_FRF_MASK        = 0x00000030, // Frame format
        CR0_FRF_SPI         = 0x00000000,
        CR0_FRF_TI          = 0x00000010,
        CR0_FRF_MWIRE       = 0x00000020,
        CR0_CPOL_MASK       = 0x00000040, // Clock out polarity
        CR0_CPOL_LOW        = 0x00000000,
        CR0_CPOL_HIGH       = 0x00000040,
        CR0_CPHA_MASK       = 0x00000080, // Clock out phase
        CR0_CPHA_FIRST      = 0x00000000,
        CR0_CPHA_SECOND     = 0x00000080,

        CR0_SCR_MASK        = 0x0000FF00, // Serial clock rate
        CR0_SCR_1           = 0x00000100,
        CR0_SCR_2           = 0x00000200,
        CR0_SCR_3           = 0x00000300,
        CR0_SCR_4           = 0x00000400,
        CR0_SCR_5           = 0x00000500,
        CR0_SCR_6           = 0x00000600,
        CR0_SCR_7           = 0x00000700,
        CR0_SCR_8           = 0x00000800,
        CR0_SCR_9           = 0x00000900,
        CR0_SCR_10          = 0x00000A00,
        CR0_SCR_11          = 0x00000B00,
        CR0_SCR_12          = 0x00000C00,
        CR0_SCR_13          = 0x00000D00,
        CR0_SCR_14          = 0x00000E00,
        CR0_SCR_15          = 0x00000F00,
        CR0_SCR_16          = 0x00001000,
    };

    enum CR1 : uint32_t {
        CR1_LBM_MASK        = 0x00000001, // Loop back mode
        CR1_LBM_NORMAL      = 0x00000000,
        CR1_LBM_INVERTED    = 0x00000001, // MISO/MOSI are reversed
        CR1_SSE_MASK        = 0x00000002, // SSP enable
        CR1_SSE_DISABLED    = 0x00000000,
        CR1_SSE_ENABLED     = 0x00000002,
        CR1_MS_MASK         = 0x00000004, // Master/Slave Mode
        CR1_MS_MASTER       = 0x00000000,
        CR1_MS_SLAVE        = 0x00000004,
        CR1_SOD_MASK        = 0x00000008, // Slave output disable
    };

    enum SR : uint32_t {
        SR_TFE_MASK         = 0x00000001, // Transmit FIFO empty
        SR_TFE_EMPTY        = 0x00000001,
        SR_TFE_NOTEMPTY     = 0x00000000,
        SR_TNF_MASK         = 0x00000002, // Transmit FIFO not full
        SR_TNF_NOTFULL      = 0x00000002,
        SR_TNF_FULL         = 0x00000000,
        SR_RNE_MASK         = 0x00000004, // Receive FIFO not empty
        SR_RNE_NOTEMPTY     = 0x00000004,
        SR_RNE_EMPTY        = 0x00000000,
        SR_RFF_MASK         = 0x00000008, // Receive FIFO full
        SR_RFF_FULL         = 0x00000008,
        SR_RFF_NOTFULL      = 0x00000000,
        SR_BSY_MASK         = 0x00000010, // Busy Flag
        SR_BSY_IDLE         = 0x00000000,
        SR_BSY_BUSY         = 0x00000010,
    };

    enum CPSR : uint32_t {
        CPSR_CPSDVSR_MASK   = 0x000000FF,
        CPSR_CPSDVSR_DIV2   = 0x00000002,
        CPSR_CPSDVSR_DIV4   = 0x00000004,
    };

    enum IMSC : uint32_t {
        IMSC_RORIM_MASK     = 0x00000001, // Receive overrun interrupt
        IMSC_RORIM_ENBL     = 0x00000001,
        IMSC_RORIM_DSBL     = 0x00000000,
        IMSC_RTIM_MASK      = 0x00000002, // Receive timeout interrupt
        IMSC_RTIM_ENBL      = 0x00000002,
        IMSC_RTIM_DSBL      = 0x00000000,
        IMSC_RXIM_MASK      = 0x00000004, // Rx FIFO >= 1/2 full interrupt
        IMSC_RXIM_ENBL      = 0x00000004,
        IMSC_RXIM_DSBL      = 0x00000000,
        IMSC_TXIM_MASK      = 0x00000008, // Tx FIFO >= 1/2 empty interrupt
        IMSC_TXIM_ENBL      = 0x00000008,
        IMSC_TXIM_DSBL      = 0x00000000,
    };

    enum RIS : uint32_t {
        RIS_RORRIS_MASK     = 0x00000001, // Frame received while Rx FIFO full
        RIS_RORRIS_RCVD     = 0x00000001,
        RIS_RTRIS_MASK      = 0x00000002, // Rx FIFO not empty no read within timeout
        RIS_RTRIS_NOTEMPTY  = 0x00000002,
        RIS_RXRIS_MASK      = 0x00000004, // Rx FIFO >= half full
        RIS_RXRIS_HALFFULL  = 0x00000004,
        RIS_TXRIS_MASK      = 0x00000008, // Tx FIF0 >= half-empty
        RIS_TXRIS_HALFEMPTY = 0x00000008,
    };

    enum MIS : uint32_t {
        MIS_RORMIS_MASK     = 0x00000001, // Frame received while Rx FIFO full
        MIS_RORMIS_FRMRCVD  = 0x00000001,
        MIS_RTMIS_MASK      = 0x00000002, // Rx FIFO not empty no read withing timeout
        MIS_RTMIS_NOTEMPTY  = 0x00000002,
        MIS_RXMIS_MASK      = 0x00000004, // Rx FIFO >= half full
        MIS_RXMIS_HALFFULL  = 0x00000004,
        MIS_TXMIS_MASK      = 0x00000008, // Tx FIFO >= half-empty
        MIS_TXMIS_HALFEMPTY = 0x00000008,
    };

    enum ICR : uint32_t {
        ICR_RORIC_MASK      = 0x00000001, // Clears RORIC interrupt flag
        ICR_RORIC_CLEAR     = 0x00000001,
        ICR_RTIC_MASK       = 0x00000002, // Clear Rx FIFO not empty/no read flag
        ICR_RTIC_CLEAR      = 0x00000002,
    };
};

#define SSP0    SSP(0)
#define SSP1    SSP(1)
