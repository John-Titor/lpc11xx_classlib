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

#include <sys/cdefs.h>

#include <etl/array.h>
#include <etl/array_view.h>
#include <etl/atomic.h>

#include <LPC11xx.h>

extern "C" void I2C_Handler(void);

/// I2C master mode
class I2C
{
public:
    enum State {
        // non-terminal states
        IDLE = 0,
        PENDING,

        // terminal states
        TERMINAL_STATE,
        ACK,
        NACK,
        ERROR
    };

    State                    transfer(uint8_t slave,
                                      const uint8_t *writeBuffer,
                                      uint8_t writeLength,
                                      uint8_t *readBuffer = nullptr,
                                      uint8_t readLength = 0);

    State                   writeRegister(uint8_t slave, uint8_t address, uint8_t value);
    State                   readRegister(uint8_t slave, uint8_t address, uint8_t &value);

    friend void I2C_Handler(void);

private:

    etl::atomic<bool>                               _busy;

    State                                           _state = IDLE;
    uint8_t                                         _slave = 0;
    etl::const_array_view<uint8_t>                  _writeBuffer;
    etl::const_array_view<uint8_t>::const_iterator  _writeIter;
    etl::array_view<uint8_t>                        _readBuffer;
    etl::array_view<uint8_t>::iterator              _readIter;

    void                    transfer();
    bool                    start();
    bool                    stop();
    void                    handleInterrupt();

    enum I2CCONSET : uint32_t {
        CONSET_AA_MASK                  = 0x00000004,
        CONSET_AA                       = 0x00000004, // Asset acknowlegde flag
        CONSET_SI_MASK                  = 0x00000008,
        CONSET_SI                       = 0x00000008, // I2C interrupt flag
        CONSET_STO_MASK                 = 0x00000010,
        CONSET_STO                      = 0x00000010, // Stop flag
        CONSET_STA_MASK                 = 0x00000020,
        CONSET_STA                      = 0x00000020, // Start flag
        CONSET_I2EN_MASK                = 0x00000040,
        CONSET_I2EN                     = 0x00000040, // I2C interface enable
    };

    enum I2CSTAT : uint32_t {
        STAT_Status_MASK                = 0x000000F8, // Status information
    };

    enum I2CADR0 : uint32_t {
        ADR0_GC_MASK                    = 0x00000001,
        ADR0_GC                         = 0x00000001, // General call enable bit
        ADR0_Address_MASK               = 0x000000FE, // I2C device address for slave mode
    };

    enum I2CCONCLR : uint32_t {
        CONCLR_AAC_MASK                 = 0x00000004, // Assert acknowledge clear bit
        CONCLR_AAC                      = 0x00000004,
        CONCLR_SIC_MASK                 = 0x00000008, // I2C interrupt clear bit
        CONCLR_SIC                      = 0x00000008,
        CONCLR_STAC_MASK                = 0x00000020, // Start flag clear bit
        CONCLR_STAC                     = 0x00000020,
        CONCLR_I2ENC_MASK               = 0x00000040, // I2C interface disable bit
        CONCLR_I2ENC                    = 0x00000040,
    };

    enum I2CMMCTRL : uint32_t {
        MMCTRL_MM_ENA_MASK              = 0x00000001, // Monitor mode enable
        MMCTRL_MM_ENA_ENABLED           = 0x00000001,
        MMCTRL_MM_ENA_DISABLED          = 0x00000000,
        MMCTRL_ENA_SCL_MASK             = 0x00000002, // SCL output enable
        MMCTRL_ENA_SCL_HOLDLOW          = 0x00000002,
        MMCTRL_ENA_SCL_FORCEHIGH        = 0x00000000,
        MMCTRL_MATCH_ALL_MASK           = 0x00000008, // Select interrupt register match
        MMCTRL_MATCH_ALL_NORMAL         = 0x00000000,
        MMCTRL_MATCH_ALL_ANYADDRESS     = 0x00000008,
    };

    enum I2CADR1 : uint32_t {
        ADR1_GC_MASK                    = 0x00000001, // General call enable bit
        ADR1_GC                         = 0x00000001,
        ADR1_Address_MASK               = 0x000000FE,
    };

    enum I2CADR2 : uint32_t {
        ADR2_GC_MASK                    = 0x00000001, // General call enable bit
        ADR2_GC                         = 0x00000001,
        ADR2_Address_MASK               = 0x000000FE,
    };

    enum I2CADR3 : uint32_t {
        ADR3_GC_MASK                    = 0x00000001, // General call enable bit
        ADR3_GC                         = 0x00000001,
        ADR3_Address_MASK               = 0x000000FE,
    };

};

extern I2C      I2C0;
