#include <i2c.h>
#include <interrupt.h>
#include <syscon.h>
#include <pin.h>

#include "config.h"

I2C     I2C0;

I2C::State
I2C::transfer(uint8_t slave,
              const uint8_t *writeBuffer,
              uint8_t writeLength,
              uint8_t *readBuffer,
              uint8_t readLength)
{
    // claim ownership of the interface
    bool expected = false;
    if (!_busy.compare_exchange_strong(expected, true)) {
        return ERROR;
    }

    _state = IDLE;
    _slave = slave;
    
    _writeBuffer = etl::const_array_view<unsigned char>(writeBuffer, writeLength);
    _writeIter = _writeBuffer.begin();

    _readBuffer = etl::array_view<unsigned char>(readBuffer, readLength);
    _readIter = _readBuffer.begin();

    // take block out of reset
    SYSCON_I2C.reset();
    // enable clock
    SYSCON_I2C.clock(true);

    // init pins
    P0_4_SCL.configure(Pin::I2CStandard | Pin::OpenDrain);
    P0_5_SDA.configure(Pin::I2CStandard | Pin::OpenDrain);

    // do block setup
    LPC_I2C->CONCLR = CONCLR_AAC |
                      CONCLR_SIC |
                      CONCLR_STAC |
                      CONCLR_I2ENC;

    /* SCLH and SCLL = I2C PCLK High/Low cycles for I2C clock and
      determine the data rate/duty cycle for I2C:

       I2CBitFrequency = I2CPCLK / (I2CSCLH + I2CSCLL)

       Standard Mode   (100KHz) = CFG_CPU_CCLK / 200000
       Fast Mode       (400KHz) = CFG_CPU_CCLK / 800000
       Fast- Mode Plus (1MHz)   = CFG_CPU_CCLK / 2000000       */

    LPC_I2C->SCLL = CONFIG_CPU_FREQUENCY / 200000;
    LPC_I2C->SCLH = CONFIG_CPU_FREQUENCY / 200000;

    // enable interrupt
    I2C_IRQ.enable();

    // start the transfer
    LPC_I2C->CONSET = CONSET_I2EN;

    if (!start()) {
        stop();
        goto done;
    }

    // wait for a non-terminal state
    while (_state < TERMINAL_STATE) {
        // WFI?
        // timeout?
    }

    // relinquish the controller
done:
    auto result = _state;

    SYSCON_I2C.clock(false);
    I2C_IRQ.disable();
    _busy.store(false);

    return result;
}

bool
I2C::start()
{
    LPC_I2C->CONSET = CONSET_STA;

    // make sure it starts -
    unsigned timeout = 0x1000000;

    while (_state == IDLE) {
        if (--timeout == 0) {
            _state = ERROR;
            return false;
        }
    }

    return true;
}

I2C::State
I2C::writeRegister(uint8_t slave, uint8_t address, uint8_t value)
{
    uint8_t buffer[] = { address, value };

    return transfer(slave, buffer, sizeof(buffer));
}

I2C::State
I2C::readRegister(uint8_t slave, uint8_t address, uint8_t &value)
{
    return transfer(slave, &address, 1, &value, 1);
}

bool
I2C::stop()
{
    LPC_I2C->CONSET = CONSET_STO;
    LPC_I2C->CONCLR = CONCLR_SIC;

    // wait for stop bit to clear
    unsigned timeout = 0x1000000;

    while (LPC_I2C->CONSET & CONSET_STO) {
        if (--timeout == 0) {
            _state = ERROR;
            return false;
        }
    }

    return true;
}

void
I2C::handleInterrupt()
{
    // run the I2C state machine

    auto statReg = LPC_I2C->STAT;

    switch (statReg) {
    case 0x08:
        /*
         * A START condition has been transmitted.
         * Send the slave address with the R bit clear.
         */
        LPC_I2C->DAT = _slave;
        LPC_I2C->CONCLR = (CONCLR_SIC | CONCLR_STAC);
        _state = PENDING;
        break;

    case 0x10:
        /*
         * A repeated START condition has been transmitted.
         * Send the slave address with the R bit set.
         */
        LPC_I2C->DAT = _slave | 1;
        LPC_I2C->CONCLR = (CONCLR_SIC | CONCLR_STAC);
        break;

    case 0x18:
        /*
         * SLA+W has been transmitted; ACK has been received.
         * We now start writing bytes.
         */
        LPC_I2C->DAT = *_writeIter++;
        LPC_I2C->CONCLR = CONCLR_SIC;
        break;

    case 0x20:
        /*
         * SLA+W has been transmitted; NOT ACK has been received.
         * Send a stop condition to terminate the transaction
         * and signal I2CEngine the transaction is aborted.
         */
        LPC_I2C->CONSET = CONSET_STO;
        LPC_I2C->CONCLR = CONCLR_SIC;
        _state = NACK;
        break;

    case 0x28:

        /*
         * Data in I2DAT has been transmitted; ACK has been received.
         * Continue sending more bytes as long as there are bytes to send
         * and after this check if a read transaction should follow.
         */
        if (_writeIter < _writeBuffer.end()) {
            /* Keep writing as long as bytes avail */
            LPC_I2C->DAT = *_writeIter++;
        } else {
            if (_readBuffer.size()) {
                /* Send a Repeated START to initialize a read transaction */
                /* (handled in state 0x10)                                */
                LPC_I2C->CONSET = CONSET_STA;   /* Set Repeated-start flag */
            } else {
                _state = ACK;
                LPC_I2C->CONSET = CONSET_STO;      /* Set Stop flag */
            }
        }

        LPC_I2C->CONCLR = CONCLR_SIC;
        break;

    case 0x30:
        /*
         * Data byte in I2DAT has been transmitted; NOT ACK has been received
         * Send a STOP condition to terminate the transaction and inform the
         * I2CEngine that the transaction failed.
         */
        LPC_I2C->CONSET = CONSET_STO;
        LPC_I2C->CONCLR = CONCLR_SIC;
        _state = NACK;
        break;

    case 0x38:
        /*
         * Arbitration loss in SLA+R/W or Data bytes.
         * This is a fatal condition, the transaction did not complete due
         * to external reasons (e.g. hardware system failure).
         * Inform the I2CEngine of this and cancel the transaction
         * (this is automatically done by the I2C hardware)
         */
        _state = ERROR;
        LPC_I2C->CONCLR = CONCLR_SIC;
        break;

    case 0x40:

        /*
         * SLA+R has been transmitted; ACK has been received.
         * Initialize a read.
         * Since a NOT ACK is sent after reading the last byte,
         * we need to prepare a NOT ACK in case we only read 1 byte.
         */
        if (_readBuffer.size() == 1) {
            /* last (and only) byte: send a NACK after data is received */
            LPC_I2C->CONCLR = CONCLR_AAC;
        } else {
            /* more bytes to follow: send an ACK after data is received */
            LPC_I2C->CONSET = CONSET_AA;
        }

        LPC_I2C->CONCLR = CONCLR_SIC;
        break;

    case 0x48:
        /*
         * SLA+R has been transmitted; NOT ACK has been received.
         * Send a stop condition to terminate the transaction
         * and signal I2CEngine the transaction is aborted.
         */
        LPC_I2C->CONSET = CONSET_STO;
        LPC_I2C->CONCLR = CONCLR_SIC;
        _state = NACK;
        break;

    case 0x50:
        /*
         * Data byte has been received; ACK has been returned.
         * Read the byte and check for more bytes to read.
         * Send a NOT ACK after the last byte is received
         */
        *_readIter++ = LPC_I2C->DAT;

        if (_readIter < _readBuffer.end()) {
            /* lmore bytes to follow: send an ACK after data is received */
            LPC_I2C->CONSET = CONSET_AA;
        } else {
            /* last byte: send a NACK after data is received */
            LPC_I2C->CONCLR = CONCLR_AAC;
        }

        LPC_I2C->CONCLR = CONCLR_SIC;
        break;

    case 0x58:
        /*
         * Data byte has been received; NOT ACK has been returned.
         * This is the last byte to read.
         * Generate a STOP condition and flag the I2CEngine that the
         * transaction is finished.
         */
        *_readIter++ = LPC_I2C->DAT;
        _state = ACK;
        LPC_I2C->CONSET = CONSET_STO;   /* Set Stop flag */
        LPC_I2C->CONCLR = CONCLR_SIC;   /* Clear SI flag */
        break;

    }
}

extern "C"
void
I2C_Handler()
{
    if (I2C0._busy) {
        I2C0.handleInterrupt();
    } else {
        I2C_IRQ.disable();
    }
}
