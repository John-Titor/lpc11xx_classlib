
extern "C" void main();

#include <syscon.h>
#include <pin.h>
#include <uart.h>
#include <i2c.h>
#include <timer.h>
#include <ssp.h>
#include <interrupt.h>

void
callback(void)
{
}

void
main0()
{
    Timebase(0).configure();

    // SSP
    P0_9_MOSI0.configure();
    SSP0.configure(2400000, 8, 0);
    uint8_t c = 0x55;
    SSP0.transfer(&c, nullptr, 1);

    // I2C
    I2C0.writeRegister(0x22, 1, 1);
    uint8_t reg;
    I2C0.readRegister(0x22, 1, reg);

    // mess around with a GPIO
    P0_2.configure(Gpio::Output, Pin::PushPull);

    P0_2.clear();
    P0_2.set();
    P0_2 << 1 << false << true << 0;
    P0_2.toggle();
    P0_2.set(0);
    P0_2.clear();
    P0_2 = false;
    P0_2 = !P0_2;

    // print some stuff
    P1_7_TXD.configure();
    P1_6_RXD.configure();
    UART.configure(115200);

    UART << 'X' << "test string";
    for (;;) {
        UART.send('A');
        P0_2.toggle();
    }
}
