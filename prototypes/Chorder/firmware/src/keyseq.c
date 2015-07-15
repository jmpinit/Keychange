#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "keyseq.h"

int seq_match(node* current, uint8_t* keys, int i, int len) {
    if (current == NULL) {
        return SEQ_INVALID;
    } else if (len == 0) {
        return SEQ_PARTIAL;
    } else {
        if (current->next == NULL) {
            // must be a report
            return current->data;
        } else {
            if (i < len) {
                if (keys[i] == current->data) {
                    // keep descending
                    return seq_match((node*)current->next, keys, i+1, len);
                } else {
                    // typed and stored sequences don't match 
                    return SEQ_INVALID;
                }
            } else {
                return SEQ_PARTIAL;
            }
        }
    }
}

int lookup_report(node** sequences, int seq_count, uint8_t* keys, int len) {
    if (len == 0)
        return SEQ_PARTIAL;
    
    bool partial = false;

    for (int i = 0; i < seq_count; i++) {
        int res = seq_match(sequences[i], keys, 0, len);

        if (res >= 0) {
            return res;
        } else if (res == SEQ_PARTIAL) {
            partial = true;
        }
    }

    return partial? SEQ_PARTIAL : SEQ_INVALID;
}
