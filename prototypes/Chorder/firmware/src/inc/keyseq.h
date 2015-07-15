#ifndef H_KEYSEQ
#define H_KEYSEQ

#define MAX_SEQUENCES   256
#define MAX_SEQ_LEN     8

#define SEQ_INVALID (-1)
#define SEQ_PARTIAL (-2)

typedef struct node node;

struct node {
    uint8_t data; // data is report index if next is NULL, key state otherwise
    node* next;
};

int seq_match(node* current, uint8_t* keys, int i, int len);
int lookup_report(node** sequences, int seq_count, uint8_t* keys, int len);

#endif
