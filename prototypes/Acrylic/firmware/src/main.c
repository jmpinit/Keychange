#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdbool.h>

#include "inc/serial.h"

#define NUM_KEYS 8
#define DEBOUNCE 10

unsigned long bounceTimes[NUM_KEYS];
volatile long millis = 0;

ISR(TIMER1_COMPA_vect) {
    millis++;
    TCNT1 = 0;
}

int main(void) {
    MCUCSR |= 1 << JTD; // disable JTAG
    uart_init(0);

    // debounce timer
    OCR1A = 1;
    TCCR1B |= (1 << CS12) | (1 << CS10); // prescale 1024 
    TIMSK |= 1 << OCIE1A; // compare interrupt
    sei();

    while(1) {
        static unsigned int last = 0;
        unsigned int keys = PINC;

        if(keys != last) {
            // which keys changed?
            unsigned int diff = keys ^ last;

            for(int key=0; key < 8; key++) {
                if((diff & (1 << key)) > 0) {
                    // the key's state changed
                    unsigned long timeNow = millis;

                    if(timeNow - bounceTimes[key] > DEBOUNCE) {
                        bounceTimes[key] = timeNow;
                        // the key is not bouncing

                        if((keys & (1 << key)) > 0)
                        uart_tx('a' + key);
                    }
                }
            }
        }

        last = keys;
    }

    return 0;
}
