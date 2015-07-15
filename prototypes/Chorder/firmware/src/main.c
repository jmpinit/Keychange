#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "util.h"
#include "serial.h"
#include "kb.h"
#include "keyseq.h"

#define NUM_KEYS        8
#define DEBOUNCE        10
#define CHORD_TIMEOUT   10

volatile long millis = 0;

node* sequences[256];

uint8_t seq_buffer[8];
uint8_t seq_len = 0;

void reset_lookup() {
    for (int i = 0; i < MAX_SEQ_LEN; i++)
        seq_buffer[i] = 0;
    seq_len = 0;
}

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

    // into HID mode
    /*uart_tx_str("$$$");
    _delay_ms(50);
    uart_tx_str("S~,0\n");
    _delay_ms(50);
    uart_tx_str("R,1\n");
    _delay_ms(500);*/

    kb_report_data release = { .scan_1 = 0 };
    kb_report_data letter_a = { .scan_1 = 4 };
    kb_report_data letter_b = { .scan_1 = 5 };
    kb_report_data letter_c = { .scan_1 = 6 };
    kb_report_data letter_d = { .scan_1 = 7 };
    kb_report_data letter_e = { .scan_1 = 8 };
    kb_report_data letter_f = { .scan_1 = 9 };

    kb_report_data reports[6] = {
        letter_a,
        letter_b,
        letter_c,
        letter_d,
        letter_e,
        letter_f
    };

    node report_a = { .data = 0, .next = NULL };
    node report_b = { .data = 1, .next = NULL };
    node report_c = { .data = 2, .next = NULL };
    node report_d = { .data = 3, .next = NULL };
    node report_e = { .data = 4, .next = NULL };
    node report_f = { .data = 5, .next = NULL };

    node seq_single_key = {
        .data = 0x80,
        .next = (struct node*)&report_a
    };

    node seq_single_chord = {
        .data = 0x3,
        .next = (struct node*)&report_b
    };

    node seq_arpeggio_3 = {
        .data = 0x10,
        .next = (struct node*)&report_c
    };

    node seq_arpeggio_2 = {
        .data = 0x01,
        .next = (struct node*)&seq_arpeggio_3
    };

    node seq_arpeggio = {
        .data = 0x40,
        .next = (struct node*)&seq_arpeggio_2
    };

    node seq_arpeggio_chord_3 = {
        .data = 0x14,
        .next = (struct node*)&report_d
    };

    node seq_arpeggio_chord_2 = {
        .data = 0x01,
        .next = (struct node*)&seq_arpeggio_chord_3
    };

    node seq_arpeggio_chord = {
        .data = 0x40,
        .next = (struct node*)&seq_arpeggio_chord_2
    };

    sequences[0] = &seq_single_key;
    sequences[1] = &seq_single_chord;
    sequences[2] = &seq_arpeggio;
    sequences[3] = &seq_arpeggio_chord;

    _delay_ms(50);

    forever {
        static bool waiting = true;
        static unsigned long time_evt_start = 0;
        static unsigned int last = 0;
        static uint8_t chord = 0; // accumulates keypresses
        uint8_t keys = PINC;


        if (keys != last) {
            unsigned long time_now = millis;

            if (waiting) {
                time_evt_start = time_now;
                waiting = false;
            }

            if (time_now - time_evt_start < CHORD_TIMEOUT) {
                // accumulate
                chord |= keys;
            } else {
                if (chord == 0x55) {
                    // special cancel chord
                    reset_lookup();
                } else if (chord != 0) {
                    seq_buffer[seq_len++] = chord;

                    if (seq_len < MAX_SEQ_LEN) {
                        int report_i = lookup_report(sequences, seq_buffer, seq_len);

                        if (report_i >= 0) {
                            send_kb_report(&reports[report_i]);
                            _delay_ms(50);
                            send_kb_report(&release);
                            reset_lookup();
                        } else if (report_i == SEQ_INVALID) {
                            uart_tx('z');
                            reset_lookup();
                        } 
                    } else {
                        // sequence is too long, reset
                        reset_lookup();
                    }
                }

                // reset for next event
                chord = 0;
                time_evt_start = 0;
                waiting = true;
            }
            /*// which keys changed?
            unsigned int diff = keys ^ last;

            for(int key=0; key < 8; key++) {
                if((diff & (1 << key)) > 0) {
                    // the key's state changed
                    unsigned long timeNow = millis;

                    if(timeNow - bounceTimes[key] > DEBOUNCE) {
                        bounceTimes[key] = timeNow;
                        // the key is not bouncing

                        if((keys & (1 << key)) > 0)
                        uart_tx(KEY_F1 + 1 + key);
                    }
                }
            }*/

            /*send_kb_report(&reports[0]);
            _delay_ms(50);
            send_kb_report(&release);*/
        }

        last = keys;
    }

    return 0;
}
