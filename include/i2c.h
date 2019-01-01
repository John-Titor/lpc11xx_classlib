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

class I2C
{
public:
    constexpr I2C() {}

    I2C     &configure();

private:
    enum I2CCONSET : uint32_t {
        I2CCONSET_AA_MASK               = 0x00000004,
        I2CCONSET_AA                    = 0x00000004, // Asset acknowlegde flag
        I2CCONSET_SI_MASK               = 0x00000008,
        I2CCONSET_SI                    = 0x00000008, // I2C interrupt flag
        I2CCONSET_STO_MASK              = 0x00000010,
        I2CCONSET_STO                   = 0x00000010, // Stop flag
        I2CCONSET_STA_MASK              = 0x00000020,
        I2CCONSET_STA                   = 0x00000020, // Start flag
        I2CCONSET_I2EN_MASK             = 0x00000040,
        I2CCONSET_I2EN                  = 0x00000040, // I2C interface enable
    };

    enum I2CSTAT : uint32_t {
        I2CSTAT_Status_MASK             = 0x000000F8, // Status information
    };

    enum I2CADR0 : uint32_t {
        I2CADR0_GC_MASK                 = 0x00000001,
        I2CADR0_GC                      = 0x00000001, // General call enable bit
        I2CADR0_Address_MASK            = 0x000000FE, // I2C device address for slave mode
    };

    enum I2CCONCLR : uint32_t {
        I2CCONCLR_AAC_MASK              = 0x00000004, // Assert acknowledge clear bit
        I2CCONCLR_AAC                   = 0x00000004,
        I2CCONCLR_SIC_MASK              = 0x00000008, // I2C interrupt clear bit
        I2CCONCLR_SIC                   = 0x00000008,
        I2CCONCLR_STAC_MASK             = 0x00000020, // Start flag clear bit
        I2CCONCLR_STAC                  = 0x00000020,
        I2CCONCLR_I2ENC_MASK            = 0x00000040, // I2C interface disable bit
        I2CCONCLR_I2ENC                 = 0x00000040,
    };

    enum I2CMMCTRL : uint32_t {
        I2CMMCTRL_MM_ENA_MASK           = 0x00000001, // Monitor mode enable
        I2CMMCTRL_MM_ENA_ENABLED        = 0x00000001,
        I2CMMCTRL_MM_ENA_DISABLED       = 0x00000000,
        I2CMMCTRL_ENA_SCL_MASK          = 0x00000002, // SCL output enable
        I2CMMCTRL_ENA_SCL_HOLDLOW       = 0x00000002,
        I2CMMCTRL_ENA_SCL_FORCEHIGH     = 0x00000000,
        I2CMMCTRL_MATCH_ALL_MASK        = 0x00000008, // Select interrupt register match
        I2CMMCTRL_MATCH_ALL_NORMAL      = 0x00000000,
        I2CMMCTRL_MATCH_ALL_ANYADDRESS  = 0x00000008,
    };

    enum I2CADR1 : uint32_t {
        I2CADR1_GC_MASK                 = 0x00000001, // General call enable bit
        I2CADR1_GC                      = 0x00000001,
        I2CADR1_Address_MASK            = 0x000000FE,
    };

    enum I2CADR2 : uint32_t {
        I2CADR2_GC_MASK                 = 0x00000001, // General call enable bit
        I2CADR2_GC                      = 0x00000001,
        I2CADR2_Address_MASK            = 0x000000FE,
    };

    enum I2CADR3 : uint32_t {
        I2CADR3_GC_MASK                 = 0x00000001, // General call enable bit
        I2CADR3_GC                      = 0x00000001,
        I2CADR3_Address_MASK            = 0x000000FE,
    };

};

#define I2C0        I2C()
