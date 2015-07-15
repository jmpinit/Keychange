#ifndef H_KB
#define H_KB

#define RAW_START   0xFD

typedef struct {
    uint8_t modifier;
    uint8_t scan_1;
    uint8_t scan_2;
    uint8_t scan_3;
    uint8_t scan_4;
    uint8_t scan_5;
    uint8_t scan_6;
} kb_report_data;

void send_kb_report(kb_report_data* data);

#endif
