//---------------------------------------------------------------
//
//  4190.308 Computer Architecture (Fall 2021)
//
//  Project #1: Run-Length Encoding
//
//  September 14, 2021
//
//  Jaehoon Shim (mattjs@snu.ac.kr)
//  Ikjoon Son (ikjoon.son@snu.ac.kr)
//  Seongyeop Jeong (seongyeop.jeong@snu.ac.kr)
//  Systems Software & Architecture Laboratory
//  Dept. of Computer Science and Engineering
//  Seoul National University
//
//---------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "pa1.h"


void print_ans(const char* const buf, const int buflen)
{
    for (int i = 0; i < buflen; i++) {
        printf("0x%x, ", (unsigned char) buf[i]);
    }
    printf("\n");
}

void print_buffer(const char* const buf, const int buflen, const int offset)
{
    int cnt = 0;
    for (int i = 0; i < buflen; i++) {
        for (int j = 7; j >= 0; j--) {
            int bit = (buf[i] >> j) & 1;
            printf("%d", bit);
            if (++cnt >= offset) {
                printf(" ");
                cnt = 0;
            }
        }
    }
    printf("\n");
}

int test_encoding(const int num)
{
    char dst[LEN_DST + LEN_GUARD];
    *(unsigned long*)(dst + LEN_DST) = GUARD_WORD;

    int input_len = 0;
    if (tc[num].dtype == STRING)
        input_len = strlen(tc[num].input);
    else
        input_len = tc[num].input_len;

    printf("-------- TEST #%d Encoding\n", num);

    int len = encode(tc[num].input, input_len, dst, LEN_DST);

    printf("[Input] length (bytes): %d\n", input_len);
    print_buffer(tc[num].input, input_len, 8);

    printf("[Encode] length (bytes): %d\n", len);
    print_buffer(dst, len, 3);

    printf("[Answer] length (bytes): %d \n", tc[num].ans_len);
    print_buffer(tc[num].ans, tc[num].ans_len, 3);

    if (*(unsigned long *)(dst + LEN_DST) != GUARD_WORD)
        return 1;       // memory corrupted
    else if (len != tc[num].ans_len)
        return 2;       // invalid length
    else if (memcmp(dst, tc[num].ans, tc[num].ans_len) != 0)
        return 3;       // wrong output

    return 0;
}

int test_decoding(const int num)
{
    char dst[LEN_DST + LEN_GUARD];
    *(unsigned long*)(dst + LEN_DST) = GUARD_WORD;

    int input_len = 0;
    if (tc[num].dtype == STRING)
        input_len = strlen(tc[num].input);
    else
        input_len = tc[num].input_len;

    printf("-------- TEST #%d Decoding\n", num);

    int len = decode(tc[num].ans, tc[num].ans_len, dst, LEN_DST);

    printf("[Decode] length (bytes): %d\n", len);
    print_buffer(dst, len, 8);

    if (*(unsigned long *)(dst + LEN_DST) != GUARD_WORD)
        return 1;       // memory corrupted
    else if (len != input_len)
        return 2;       // invalid length
    else if (memcmp(dst, tc[num].input, input_len) != 0)
        return 3;       // wrong output

    return 0;
}

int test_routine(const int num)
{
    int ret = test_encoding(num);

    if (!ret)
        printf("-------- ENCODING CORRECT!\n\n");
    else
        printf("-------- ENCODING WRONG! %d\n\n", ret);

    ret = test_decoding(num);

    if (!ret)
        printf("-------- DECODING CORRECT!\n\n");
    else
        printf("-------- DECODING WRONG! %d\n\n", ret);

    return !!ret;
}

int main(void)
{
    int ret = 0;

    for (int i = 0; i < (sizeof tc / sizeof(testcase)); i++) {
        ret += test_routine(i);
    }
    return ret;
}
