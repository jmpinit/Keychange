#include <stdio.h>
#include "minunit.h"

#include "../keyseq.c"

int tests_run = 0;

static char* test_root_null() {
    int res = seq_match(NULL, NULL, 0, 1);
    mu_assert("error, res != SEQ_INVALID", res == SEQ_INVALID);
    return 0;
}

static char* test_len_zero() {
    node leaf = { .data = 1, .next = NULL };
    node root = { .data = 5, .next = &leaf };
    uint8_t buff[1] = { 5 };

    int res = seq_match(&root, buff, 0, 0);

    mu_assert("error, res != SEQ_PARTIAL", res == SEQ_PARTIAL);
    return 0;
}

static char* test_seq_len_one() {
    uint8_t report_i = 31;

    node leaf = { .data = report_i, .next = NULL };
    node root = { .data = 5, .next = &leaf };
    uint8_t buff[1] = { 5 };

    int res = seq_match(&root, buff, 0, 1);

    mu_assert("error, res != (report index)", res == report_i);
    return 0;
}

static char* test_seq_len_two() {
    uint8_t report_i = 14;
    uint8_t buff[2] = { 5, 2 };

    node leaf   = { .data = report_i, .next = NULL };
    node branch = { .data = buff[1], .next = &leaf };
    node root   = { .data = buff[0], .next = &branch };

    int res = seq_match(&root, buff, 0, sizeof(buff));

    mu_assert("error, res != (report index)", res == report_i);
    return 0;
}

static char* test_partial() {
    uint8_t report_i = 14;
    uint8_t buff[2] = { 5, 2 };

    node leaf       = { .data = report_i, .next = NULL };
    node branch2    = { .data = 3, .next = &leaf };
    node branch1    = { .data = buff[1], .next = &branch2 };
    node root       = { .data = buff[0], .next = &branch1 };

    int res = seq_match(&root, buff, 0, sizeof(buff));

    mu_assert("error, res != SEQ_PARTIAL", res == SEQ_PARTIAL);
    return 0;
}

static char* test_mismatch() {
    uint8_t report_i = 14;
    uint8_t buff[2] = { 5, 2 };

    node leaf   = { .data = report_i, .next = NULL };
    node branch = { .data = buff[1] + 1, .next = &leaf };
    node root   = { .data = buff[0], .next = &branch };

    int res = seq_match(&root, buff, 0, sizeof(buff));

    mu_assert("error, res != SEQ_INVALID", res == SEQ_INVALID);
    return 0;
}

static char* test_lookup() {
    node* sequences[3];

    uint8_t report1 = 13;
    uint8_t report2 = 37;
    uint8_t report3 = 164;
    uint8_t buff[2] = { 5, 2 };

    { // fully mismatching
        node leaf   = { .data = report1, .next = NULL };
        node branch = { .data = buff[1] + 1, .next = &leaf };
        node root   = { .data = buff[0] - 1, .next = &branch };

        sequences[0] = &root;
    }

    { // mismatch on second
        node leaf   = { .data = report2, .next = NULL };
        node branch = { .data = buff[1] + 1, .next = &leaf };
        node root   = { .data = buff[0], .next = &branch };

        sequences[1] = &root;
    }

    { // matches
        node leaf   = { .data = report3, .next = NULL };
        node branch = { .data = buff[1], .next = &leaf };
        node root   = { .data = buff[0], .next = &branch };

        sequences[2] = &root;
    }

    int res = lookup_report(sequences, sizeof(sequences), buff, sizeof(buff));

    mu_assert("error, res != report3", res == report3);

    return 0;
}

static char* all_tests() {
    mu_run_test(test_root_null);
    mu_run_test(test_len_zero);
    mu_run_test(test_seq_len_one);
    mu_run_test(test_seq_len_two);
    mu_run_test(test_partial);
    mu_run_test(test_mismatch);
    mu_run_test(test_lookup);
    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
