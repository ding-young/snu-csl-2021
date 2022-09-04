//---------------------------------------------------------------
//
//  4190.308 Computer Architecture (Fall 2021)
//
//  Project #2: FP10 (10-bit floating point) Representation
//
//  October 5, 2021
//
//  Jaehoon Shim (mattjs@snu.ac.kr)
//  Ikjoon Son (ikjoon.son@snu.ac.kr)
//  Seongyeop Jeong (seongyeop.jeong@snu.ac.kr)
//  Systems Software & Architecture Laboratory
//  Dept. of Computer Science and Engineering
//  Seoul National University
//
//---------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>

#define RED   "\033[0;31m"
#define GREEN "\033[0;32m"
#define CYAN  "\033[0;36m"
#define RESET "\033[0m"

#include "pa2.h"

#define BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BINARY(BYTE)			\
	(BYTE & 0x80 ? '1' : '0'),	\
	(BYTE & 0x40 ? '1' : '0'),	\
	(BYTE & 0x20 ? '1' : '0'),	\
	(BYTE & 0x10 ? '1' : '0'),	\
	(BYTE & 0x08 ? '1' : '0'),	\
	(BYTE & 0x04 ? '1' : '0'),	\
	(BYTE & 0x02 ? '1' : '0'),	\
	(BYTE & 0x01 ? '1' : '0')

#define PRINT_BYTE(BYTE) printf(BINARY_PATTERN, BINARY(BYTE))
#define PRINT(DATATYPE, TYPENAME, NUM)				\
	do {							\
		size_t typesize = sizeof(DATATYPE);		\
		DATATYPE data = NUM;				\
		uint8_t *ptr = (uint8_t *)&data;		\
								\
		printf("%s(", TYPENAME);			\
		PRINT_BYTE(*(ptr + typesize - 1));		\
		for (ssize_t i = typesize - 2; i >= 0; i--) {	\
			printf(" ");				\
			PRINT_BYTE(*(ptr + i));			\
		}						\
		printf(")");					\
	} while (0)

#define CHECK(RES, ANS) printf("%s"RESET, (RES) == (ANS) ? GREEN"CORRECT" : RED"WRONG")
#define COMP(RES, ANS, TYPENAME) comp_##TYPENAME(RES, ANS)

static void comp_int(uint32_t result, uint32_t answer)
{
	CHECK(result, answer);
}

static void comp_fp10(uint16_t result, uint16_t answer)
{
	uint16_t exp = 0x1f0 & result;
	uint16_t frac = 0x0f & result;
	if (exp == 0x1f0 && frac != 0) {
		result &= 0xfff0;
		result++;
	}
	CHECK(result, answer);
}

static void comp_float(uint32_t result, uint32_t answer)
{
	uint32_t exp = 0x7f800000 & result;
	uint32_t frac = 0x7fffff & result;
	if (exp == 0x7f800000 && frac != 0) {
		result &= 0xff800000;
		result++;
	}
	CHECK(result, answer);
}

#define N 6

/* int -> fp10 */
uint32_t test1[N] =	{0x00000000, 0x00000001, 0x000007e4, 0xffffff9b, 0x0000f842, 0x0000ffff};
uint16_t ans1[N] =	{0x0000, 0x00f0, 0x01a0, 0xff59, 0x01ef, 0x01f0};

/* fp10 -> int */
uint16_t test2[N] =	{0xfe00, 0x013d, 0x0113, 0xfe0f, 0x01f0, 0xffff};
uint32_t ans2[N] =	{0x00000000, 0x0000001d, 0x00000004, 0x00000000, 0x80000000, 0x80000000};

/* float -> fp10 */
uint32_t test3[N] =	{0x00000001, 0x3fe00000, 0xe0000000, 0x80b5840c, 0x38788000, 0x83f0ffff};
uint16_t ans3[N] =	{0x0000, 0x00fc, 0xfff0, 0xfe00, 0x0010, 0xfe00};

/* fp10 -> float */
uint16_t test4[N] =	{0x0000, 0xfe01, 0x0155, 0x01f0, 0xffea, 0x01f1};
uint32_t ans4[N] =	{0x00000000, 0xb6800000, 0x42a80000, 0x7f800000, 0xc7500000, 0x7f800001};

int main(void)
{
	printf("\n%sTest 1: Casting from int to fp10%s\n", CYAN, RESET);
	for (int i = 0; i < N; i++) {
		uint16_t result = (uint16_t)int_fp10(test1[i]);

		PRINT(uint32_t, "int", test1[i]);
		printf(" => ");
		PRINT(uint16_t, "fp10", result);
		printf(", ");
		PRINT(uint16_t, "ans", ans1[i]);
		printf(", ");
		COMP(result, ans1[i], fp10);
		printf("\n");
	}

	printf("\n%sTest 2: Casting from fp10 to int%s\n", CYAN, RESET);
	for (int i = 0; i < N; i++) {
		uint32_t result = (uint32_t)fp10_int(test2[i]);

		PRINT(uint16_t, "fp10", test2[i]);
		printf(" => ");
		PRINT(uint32_t, "int", result);
		printf(", ");
		PRINT(uint32_t, "ans", ans2[i]);
		printf(", ");
		COMP(result, ans2[i], int);
		printf("\n");
	}

	printf("\n%sTest 3: Casting from float to fp10%s\n", CYAN, RESET);
	for (int i = 0; i < N; i++) {
		float *p = (float *)&test3[i];
		float f = *p;
		uint16_t result = (uint16_t)float_fp10(f);

		PRINT(uint32_t, "float", test3[i]);
		printf(" => ");
		PRINT(uint16_t, "fp10", result);
		printf(", ");
		PRINT(uint16_t, "ans", ans3[i]);
		printf(", ");
		COMP(result, ans3[i], fp10);
		printf("\n");
	}

	printf("\n%sTest 4: Casting from fp10 to float%s\n", CYAN, RESET);
	for (int i = 0; i < N; i++) {
		float f = fp10_float(test4[i]);
		uint32_t *p = (uint32_t *)&f;
		uint32_t result = *p;

		PRINT(uint16_t, "fp10", test4[i]);
		printf(" => ");
		PRINT(uint32_t, "float", result);
		printf(", ");
		PRINT(uint32_t, "ans", ans4[i]);
		printf(", ");
		COMP(result, ans4[i], float);
		printf("\n");
	}

	printf("\n");
	return 0;
}
