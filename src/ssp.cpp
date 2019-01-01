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

#include "ssp.h"
#include "syscon.h"

SSP &
SSP::configure(unsigned rate, unsigned nbits, unsigned mode)
{
    Syscon::set_ssp0_prescale(1);
    _syscon.clock(true);
    _syscon.reset();
    _reg.CPSR = CPSR_CPSDVSR_DIV2;
    _reg.CR0 = ((CR0_DSS_4BIT + (nbits - 4)) | 
                CR0_FRF_SPI |
                ((mode == 0) ? CR0_CPOL_LOW | CR0_CPHA_FIRST :
                 (mode == 1) ? CR0_CPOL_LOW | CR0_CPHA_SECOND :
                 (mode == 2) ? CR0_CPOL_HIGH | CR0_CPHA_FIRST : CR0_CPOL_HIGH | CR0_CPHA_SECOND) |
                ((Syscon::PCLK_FREQ / (2 * rate) - 1) << 8));
    _reg.CR1 = 0;
    _reg.CR1 = CR1_SSE_ENABLED | CR1_MS_MASTER;

    return *this;
}

void
SSP::transfer(uint8_t *source, uint8_t *destination, unsigned count)
{
    // missing all sorts of things here, but to start with...
    auto txresid = source ? count : 0;
    auto rxresid = destination ? count : 0;

    while ((txresid > 0) || (rxresid > 0)) {
        if ((txresid > 0) && (_reg.SR & SR_TNF_NOTFULL)) {
            _reg.DR = *source++;
            txresid--;
        }
        if ((rxresid > 0) && (_reg.SR & SR_RNE_NOTEMPTY)) {
            *destination++ = _reg.DR;
            rxresid--;
        }
    }
}

void
SSP::transfer(uint16_t *source, uint16_t *destination, unsigned count)
{
    // missing all sorts of things here, but to start with...
    auto txresid = source ? count : 0;
    auto rxresid = destination ? count : 0;

    while ((txresid > 0) || (rxresid > 0)) {
        if ((txresid > 0) && (_reg.SR & SR_TNF_NOTFULL)) {
            _reg.DR = *source++;
            txresid--;
        }
        if ((rxresid > 0) && (_reg.SR & SR_RNE_NOTEMPTY)) {
            *destination++ = _reg.DR;
            rxresid--;
        }
    }
}
