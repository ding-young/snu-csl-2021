# 4190.308 Computer Architecture (Fall 2021)

# Project #2: FP10 (10-bit Floating Point) Representation

### Due: 11:59PM, October 17 (Sunday)

## Introduction

The goal of this project is to get familiar with the IEEE 754 floating-point standard by implementing a 10-bit floating-point representation (`fp10` for short).

## Problem specification

`fp10` is a 10-bit floating-point representation that follows the IEEE 754 standard for floating-point arithmetic. The overall structure of the `fp10` representation is shown below. The MSB (Most Significant Bit) is used as a sign bit (`S`). The next five bits are used for exponent (`E`) with a bias value of 15. The last four bits are used for the fractional part (`F`). The rules for normalized / denormalized values and the representation of zero, infinity, and NaN are all the same as in the IEEE standard. For rounding, we use the **round-to-even** scheme.

```
bit 9  8  7  6  5  4  3  2  1  0
    S  E  E  E  E  E  F  F  F  F  
    ^  +-----+-----+  +---+----+
    |        |            |                       
Sign bit     |            Fraction (4 bits)
         Exponent (5 bits)                   
```

Note that the smallest positive number in the `fp10` format is 0.0001 x 2<sup>-14</sup> and the largest positive number is 1.1111 x 2<sup>15</sup>.

In C, we use a 16-bit unsigned short integer to store the `fp10` representation. Hence, the new type `fp10` is defined as follows.

```
typedef unsigned short int fp10;
```

Among the 16-bit short integer, we use the lower 10 bits to store the `fp10` representation. The remaining upper 6 bits have the same value as the sign bit. The following shows the actual format in C:

```
bit 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
    S  S  S  S  S  S  S  E  E  E  E  E  F  F  F  F
    +--------+--------+  +-----+-----+  +---+----+
             |                 |            |                       
         Sign bits             |         Fraction (4 bits)
                        Exponent (5 bits)                   
```

Your task is to implement the following four C functions that convert `int` or `float` type values to the `fp10` format and vice versa.

```
fp10 int_fp10(int n);               // convert int -> fp10
int fp10_int(fp10 x);               // convert fp10 -> int
fp10 float_fp10(float f);           // convert float -> fp10
float fp10_float(fp10 x);           // convert fp10 -> float
```

## Implementation details

### int_fp10() (20 points)

- Because `fp10` has only four bits of fraction, you should use the round-to-even rounding mode when necessary.
- The value 0 in `int` should be converted to the plus zero (+0.0) in `fp10`.
- Any `int`-type value that exceeds the range of the `fp10` representation (after rounding) should be converted to the infinity in `fp10` (+∞ or -∞ depending on the sign).

### fp10_int() (20 points)

- Any fractional part is dropped when `fp10`-type values are converted to integers. This is same as rounding into the integer using the round-toward-zero mode.
- Both of -0.0 and +0.0 in `fp10` should be converted to the value 0 in `int`.
- Convert +∞ and -∞ in `fp10` to the smallest number in `int` (i.e., 0x80000000).
- +NaN and -NaN in `fp10` are also converted to the smallest number in `int` (i.e., 0x80000000).
- You can safely assume that the upper 6 bits of the `fp10`-type input are always same as its sign bit.

### float_fp10() (40 points)

- Not all the `float`-type values can be represented in the `fp10` format. When necessary, you should use the round-to-even rounding mode.
- Any floating-point value that exceeds the range of the `fp10` representation (after rounding) should be converted to the infinity in `fp10` (+∞ or -∞ depending on the sign).
- +∞ and -∞ in `float` should be converted to the corresponding +∞ and -∞ in `fp10`, respectively.
- +NaN and -NaN in `float` should be converted to the corresponding +NaN and -NaN in `fp10`, respectively. You can use any of allowed bit patterns for NaN.

### fp10_float() (20 points)

- The `fp10` type is a subset of the `float` type. Hence, all the values in `fp10` can be converted to the `float` format without any error.
- Again, +∞ and -∞ in `fp10` should be converted to the corresponding +∞ and -∞ in `float`, respectively. +NaN and -NaN in `fp10` should be converted to the corresponding +NaN and -NaN in `float`, respectively. You can use any of allowed bit patterns for NaN.
- You can safely assume that the upper 6 bits of the `fp10`-type input are always same as its sign bit.

## Skeleton code

We provide you with the skeleton code for this project. It can be download from Github at https://github.com/snu-csl/ca-pa2/. To download and build the skeleton code, please follow these steps:

```
$ git clone https://github.com/snu-csl/ca-pa2.git
$ cd ca-pa2
$ make
gcc -g -O2 -Wall   -c -o pa2.o pa2.c
gcc -g -O2 -Wall   -c -o pa2-test.o pa2-test.c
gcc -o pa2 pa2.o pa2-test.o
```

The result of a sample run looks like this:

```
$ ./pa2
Test 1: Casting from int to fp10
int(00000000 00000000 00000000 00000000) => fp10(00000000 00000001), ans(00000000 00000000), WRONG
int(00000000 00000000 00000000 00000001) => fp10(00000000 00000001), ans(00000000 11110000), WRONG
int(00000000 00000000 00000111 11100100) => fp10(00000000 00000001), ans(00000001 10100000), WRONG
int(11111111 11111111 11111111 10011011) => fp10(00000000 00000001), ans(11111111 01011001), WRONG
int(00000000 00000000 11111000 01000010) => fp10(00000000 00000001), ans(00000001 11101111), WRONG
int(00000000 00000000 11111111 11111111) => fp10(00000000 00000001), ans(00000001 11110000), WRONG

Test 2: Casting from fp10 to int
fp10(11111110 00000000) => int(00000000 00000000 00000000 00000001), ans(00000000 00000000 00000000 00000000), WRONG
fp10(00000001 00111101) => int(00000000 00000000 00000000 00000001), ans(00000000 00000000 00000000 00011101), WRONG
fp10(00000001 00010011) => int(00000000 00000000 00000000 00000001), ans(00000000 00000000 00000000 00000100), WRONG
fp10(11111110 00001111) => int(00000000 00000000 00000000 00000001), ans(00000000 00000000 00000000 00000000), WRONG
fp10(00000001 11110000) => int(00000000 00000000 00000000 00000001), ans(10000000 00000000 00000000 00000000), WRONG
fp10(11111111 11111111) => int(00000000 00000000 00000000 00000001), ans(10000000 00000000 00000000 00000000), WRONG

Test 3: Casting from float to fp10
float(00000000 00000000 00000000 00000001) => fp10(00000000 00000001), ans(00000000 00000000), WRONG
float(00111111 11100000 00000000 00000000) => fp10(00000000 00000001), ans(00000000 11111100), WRONG
float(11100000 00000000 00000000 00000000) => fp10(00000000 00000001), ans(11111111 11110000), WRONG
float(10000000 10110101 10000100 00001100) => fp10(00000000 00000001), ans(11111110 00000000), WRONG
float(00111000 01111000 10000000 00000000) => fp10(00000000 00000001), ans(00000000 00010000), WRONG
float(10000011 11110000 11111111 11111111) => fp10(00000000 00000001), ans(11111110 00000000), WRONG

Test 4: Casting from fp10 to float
fp10(00000000 00000000) => float(00111111 10000000 00000000 00000000), ans(00000000 00000000 00000000 00000000), WRONG
fp10(11111110 00000001) => float(00111111 10000000 00000000 00000000), ans(10110110 10000000 00000000 00000000), WRONG
fp10(00000001 01010101) => float(00111111 10000000 00000000 00000000), ans(01000010 10101000 00000000 00000000), WRONG
fp10(00000001 11110000) => float(00111111 10000000 00000000 00000000), ans(01111111 10000000 00000000 00000000), WRONG
fp10(11111111 11101010) => float(00111111 10000000 00000000 00000000), ans(11000111 01010000 00000000 00000000), WRONG
fp10(00000001 11110001) => float(00111111 10000000 00000000 00000000), ans(01111111 10000000 00000000 00000001), WRONG
```

You are required to complete the `int_fp10()`, `fp10_int()`, `float_fp10()`, and `fp10_float()` functions in the `pa2.c` file.

## Restrictions

- You should not use any array even in the comment lines. Any source file that contains the symbol `[` or `]` will be rejected by the server.
- You are not allowed to use the `long` or `double` data type. Any source file that contains the word `long` or `double` (even in the comment lines) will be rejected.
- Do not include any header file other than `pa2.h` in the `pa2.c` file.
- Your `pa2.c` file should not contain any external library functions including `printf()`. Please remove them before you submit your code to the server.
- Your code should finish within a reasonable time. We will measure the time to perform a certain number of operations. If your code does not finish within a predefined threshold (e.g., 5 sec), it will be killed.
- **The top 15 fastest float_fp10() implementations will receive a 10% extra bonus.**

## Hand in instructions

- Submit only the `pa2.c` file to the submission server.

## Logistics

- You will work on this project alone.
- Only the upload submitted before the deadline will receive the full credit. 25% of the credit will be deducted for every single day delay.
- **You can use up to 4 *slip days* during this semester**. If your submission is delayed by 1 day and if you decided to use 1 slip day, there will be no penalty. In this case, you should explicitly declare the number of slip days you want to use in the QnA board of the submission server after each submission. Saving the slip days for later projects is highly recommended!
- Any attempt to copy others’ work will result in heavy penalty (for both the copier and the originator). Don’t take a risk.

Have fun!

[Jin-Soo Kim](mailto:jinsoo.kim_AT_snu.ac.kr)  
[Systems Software and Architecture Laboratory](http://csl.snu.ac.kr)  
[Dept. of Computer Science and Engineering](http://cse.snu.ac.kr)  
[Seoul National University](http://www.snu.ac.kr)
