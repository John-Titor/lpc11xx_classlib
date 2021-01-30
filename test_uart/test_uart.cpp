
#include <syscon.h>
#include <pin.h>
#include <uart.h>
#include <i2c.h>
#include <timer.h>
#include <ssp.h>
#include <interrupt.h>
#include <debug.h>

#define LED1        P1_11
#define LED2        P1_10

void
main()
{
    // LED GPIOs
    LED1.configure(Gpio::Output, Pin::PushPull);
    LED2.configure(Gpio::Output, Pin::PushPull);
    LED1.set();
    LED2.set();

    // UART
    P1_7_TXD.configure();
    P1_6_RXD.configure();
    UART0.configure(115200);
    UART0 << "loopback test start\n";

    unsigned i = 0;
    uint64_t t, r;
    for (;;) {
        uint8_t c;

        if (UART0.recv(c)) {
            UART0.send(c);
            i++;
            t = Timebase.time();
        } else {
            if (((Timebase.time() - t) > 1000000) 
                && (t != r)) {
                debug("%d", i);
                r = t;
            }
        }
        if (i == 1000) {
            LED1.clear();
        }
    }
}
