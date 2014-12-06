#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "inc/serial.h"

int main(void) {
    MCUCSR |= 1 << JTD; // disable JTAG
    uart_init(0);

    while(1) {
        uint8_t btns = PINC;
        
        if(btns) {
            for(int i=0; i < 8; i++) {
                if(btns & (1 << i))
                    uart_tx('a' + i);
            }

            _delay_ms(10);
        }
    }

    return 0;
}
