#include <stdint.h>

#include "serial.h"
#include "kb.h"

void send_kb_report(kb_report_data* data) {
    uart_tx(RAW_START);
    uart_tx(9); // length
    uart_tx(1); // kb descriptor
    uart_tx(data->modifier);
    uart_tx(0); // ?? magic

    uart_tx(data->scan_1);
    uart_tx(data->scan_2);
    uart_tx(data->scan_3);
    uart_tx(data->scan_4);
    uart_tx(data->scan_5);
    uart_tx(data->scan_6);
}
