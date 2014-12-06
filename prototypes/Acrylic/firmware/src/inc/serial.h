#ifndef SERIAL
#define SERIAL

void uart_init(unsigned int baudrate);
void uart_tx(unsigned char c);
void uart_tx_str(char* addr);
void uart_flush(void);
unsigned char uart_read_buff(void);
char uart_available(void);

#endif
