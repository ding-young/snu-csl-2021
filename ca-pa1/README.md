# 4190.308 Computer Architecture (Fall 2021)
# Project #1: Run-length Encoding
### Due: 11:59PM, September 26 (Sunday)


## Introduction

In this project, you need to perform a variant of run-length encoding to the given array of bytes in memory. The purpose of this project is to make you familiar with the binary representations of strings/integers and the logical operations supported in the C programming language. Another goal is to make your Linux or MacOS development environment ready and to get familiar with our project submission server.


## Background: Run-length Encoding

Run-length encoding (RLE) is a form of lossless data compression in which runs of data (sequences in which the same data value occurs in many consecutive data elements) are stored as a single data value and count, rather than as the original run. This is most efficient on data that contains many such runs, for example simple graphic images such as icons, line drawings, and animations. For files that do not have many runs, RLE could increase the file size. For more details on RLE, please refer to https://en.wikipedia.org/wiki/Run-length_encoding.

## Our Simplified Run-length Encoding

In memory, every data is represented as a sequence of binary digits. Therefore, we just count those runs that consist of all 0's or all 1's. Because we have only two kinds of runs, we do not store the data value. Instead, we repeatedly store the length of the run with all 0's and then that with all 1's. We allocate three bits to represent the length of each run. Our simplified run-length encoding works as follows.

1. First, find a run with all 0's in the input. If the length is less than 8, emit the three bits that correspond to the length of the run.

2. And then find the next run with all 1's in the input. Again, if the length is less than 8, emit the three bits that denote the length of the  run.

3. Basically, step 1 and 2 are repeated until the end of the input.

4. If the length of the run is longer than 7 in step 1 or 2, it should be represented as a series of full runs (i.e., the run with the length 7) except for the last run. For example, the run `00000000` is represented as `0000000` + `0` where there is no run with 1's between them. This leads to the following encoding result. (In this case, please note that any other combinations such as `0000` + `000` are not allowed.)

```
111 000 001 
 ^   ^   ^
 |   |   |   
 |   |   the length of the next run with all 0's (1)
 |   the length of the next run with all 1's (0)
 the length of the first run with all 0's (7)
```



5. If the total number of output bits is not a multiple of 8 after encoding, we pad 0's until it becomes a multiple of 8. This is because a byte is the smallest unit that can be stored in memory. Hence, the output has the following format:

```
| AAA BBB CCC DDD  ...         |  Padding  |
|------------------------------|-----------|
|          (3*n bits)          | (0~7 bits)|
```

Here, `AAA` is the length of the first run with all 0's, `BBB` is the length of the next run with all 1's, and `CCC` and `DDD` are the lengths of the next runs with all 0's and 1's, respectively. You should pad 0's in the last byte so that the toal number of bits becomes a multiple of 8.





Putting it all together, the final encoded value for the 8-bit input `0x00` becomes `0xe0 0x80` as can be seen in the following.


```
Original value: 00000000
Encoded value:  111 000 001 0000000   == 0xe0 0x80
                 ^   ^   ^  ^^^^^^^
                 |   |   |     |
                 |   |   |    padding (to make the output 8*n bits)
                 |   |   the length of the next run with all 0's (1)
                 |   the length of the next run with all 1's (0)
                 the length of the first run with all 0's (7)
```

### Example

Let's see an example. Assume that we want to encode the string `The quick brown fox` using our simplified run-length encoding scheme. Each character in the text string in C is represented as 8-bit unsigned integer according to the [ASCII](https://en.wikipedia.org/wiki/ASCII) standard. The following shows the actual values (in hexadecimal) to represent the given string.

```
T  h  e     q  u  i  c  k   
54 68 65 20 71 75 69 63 6b 20 

b  r  o  w  n     f  o  x    
62 72 6f 77 6e 20 66 6f 78 

```

In memory, the string is stored as binary digits as follows:

```
T        h        e                 
01010100 01101000 01100101 00100000 
q        u        i        c        k        
01110001 01110101 01101001 01100011 01101011 00100000
b        r        o        w        n
01100010 01110010 01101111 01110111 01101110 00100000
f        o        x
01100110 01101111 01111000 
```

Now, we convert each run according to our run-length encoding scheme.

```
T             h           e             []        q
0 1 0 1 0 1 000 11 0 1 0000 11 00 1 0 1 00 1 000000 
=> 001 001 001 001 001 001 011 010 001 001 100 010 010 001 001 001 010 001 110

          u             i             c           k             []        b
111 000 1 0 111 0 1 0 1 0 11 0 1 00 1 0 11 000 11 0 11 0 1 0 11 00 1 000000
=> 011 011 001 001 011 001 001 001 001 001 010 001 001 010 001 001 010 011 010 001 010 001 001 001 010 010 001 110

          r           o           w           n           []        f
11 000 1 00 111 00 1 00 11 0 1111 0 111 0 111 0 11 0 111 000 1 000000
=> 010 011 001 010 011 010 001 010 010 001 100 001 011 001 011 001 010 001 011 011 001 110

          o           x         
11 00 11 00 11 0 1111 0 1111 000 
=> 010 010 010 010 010 001 100 001 100 011
```

The final output will be as follows.

```
T             h           e             []        q
0 1 0 1 0 1 000 11 0 1 0000 11 00 1 0 1 00 1 000000 
=> 001 001 001 001 001 001 011 010 001 001 100 010 010 001 001 001 010 001 110
=> 00100100 10010010 01011010 00100110 00100100 01001001 01000111 0
=> 0x24     0x92     0x5a     0x26     0x24     0x49     0x47     0x36

          u             i             c           k             []        b
111 000 1 0 111 0 1 0 1 0 11 0 1 00 1 0 11 000 11 0 11 0 1 0 11 00 1 000000
=> 011 011 001 001 011 001 001 001 001 001 010 001 001 010 001 001 010 011 010 001 010 001 001 001 010 010 001 110
=> 0110110 01001011 00100100 10010010 10001001 01000100 10100110 10001010 00100100 10100100 01110
           0x4b     0x24     0x92     0x89     0x44     0xa6     0x8a     0x24     0xa4     0x72

          r           o           w           n           []        f
11 000 1 00 111 00 1 00 11 0 1111 0 111 0 111 0 11 0 111 000 1 000000
=> 010 011 001 010 011 010 001 010 010 001 100 001 011 001 011 001 010 001 011 011 001 110
=> 010 01100101 00110100 01010010 00110000 10110010 11001010 00101101 1001110
       0x65     0x34     0x52     0x30     0xb2     0xca     0x2d     0x9c

          o           x           
11 00 11 00 11 0 1111 0 1111 000  
=> 010 010 010 010 010 001 100 001 100 011 
=> 0 10010010 01001000 11000011 00011000 (3 bits padded)
     0x92     0x48     0xc3     0x18     
```


## Problem specification

### 1. encode()

Write the C function named `encode()` that encodes the input binary data using our run-length encoding scheme. The prototype of `encode()` is as follows:

```
int encode(const char* const src, const int srclen, char* const dst, const int dstlen);
```

The first argument `src` points to the memory address of the input data. The length of the input data (in bytes) is specified in the second argument `srclen`. The encoded result should be stored starting from the address pointed to by ``dst``. Finally, the `dstlen` argument indicates the number of bytes allocated for the result by the caller.

The function `encode()` returns the actual length of the output in bytes including the encoded data and padded bits. If the size of the output exceeds the `dstlen`, it should return -1 and the contents of the output is ignored. When `srclen` is zero, `encode()` returns zero. In any case, the contents of the memory outside of the allocated buffer should not be corrupted.

### 2. decode()

Write the C function named `decode()` that decodes the input data encoded with our simplified run-length encoding scheme. The prototype of `decode()` is as follows:

```
int decode(const char* const src, const int srclen, char* const dst, const int dstlen);
```

The first argument `src` points to the memory address of the input encoded data. The length of the input data (in bytes) is specified in the second argument `srclen`. The decoded (original) data should be stored starting from the address pointed to by `dst`. Finally, the `dstlen` argument indicates the number of bytes allocated for the result by the caller.

The function `decode()` returns the actual length of the output in bytes. If the size of the output exceeds the `dstlen`, it should return -1 and the contents of the output is ignored. When `srclen` is zero, `decode()` returns zero. In any case, the contents of the memory outside of the allocated buffer should not be corrupted.


## Skeleton code

We provide you with the skeleton code for this project. It can be downloaded from Github at https://github.com/snu-csl/ca-pa1/. If you don't have the `git` utility, you need to install it first. You can install the `git` utility on Ubuntu by running the following command:
```
$ sudo apt install git
```
For MacOS, install the Xcode command line tools which come with `git`.

To download and build the skeleton code, please follow these steps:

```
$ git clone https://github.com/snu-csl/ca-pa1.git
$ cd ca-pa1
$ make
gcc -g -O2 -Wall   -c -o pa1.o pa1.c
gcc -g -O2 -Wall   -c -o pa1-main.o pa1-main.c
gcc -o pa1 pa1.o pa1-main.o 
```

The result of a sample run looks like this:

```
$ ./pa1
-------- TEST #0 Encoding
[Input] length (bytes): 1
11111111
[Encode] length (bytes): 0

[Answer] length (bytes): 2
000 111 000 001 000 0
-------- ENCODING WRONG! 2

-------- TEST #0 Decoding
[Decode] length (bytes): 0

-------- DECODING WRONG! 2

(... more test cases below ...)
```

## Restrictions

* You are not allowed to use any array even in the comment lines. Any source file that contains the symbol `[` or `]` will be rejected by the server. 

* Do not include any header file in the `pa1.c` file. You are not allowed to use any library functions (including `printf()`) inside the `pa1.c` file. 

* Your solution should finish within a reasonable time. If your code does not finish within a predefined threshold (e.g., 5 sec), it will be killed.


## Hand in instructions

* In order to submit your solution, you need to register an account to the submission server at https://sys.snu.ac.kr
  * You must enter your real name & student ID
  * Wait for an approval from the TA
* Note that the submission server is only accessible inside the SNU campus network. If you want off-campus access to the submission server, please send your IP to the TAs via email (`snucsl.ta` AT `gmail`)
* Upload only the `pa1.c` file to the submission server

## Logistics

* You will work on this project alone.
* Only the upload submitted before the deadline will receive the full credit. 25% of the credit will be deducted for every single day delay.
* __You can use up to 4 _slip days_ during this semester__. If your submission is delayed by 1 day and if you decided to use 1 slip day, there will be no penalty. In this case, you should explicitly declare the number of slip days you want to use in the QnA board of the submission server after each submission. Saving the slip days for later projects is highly recommended!
* Any attempt to copy others' work will result in heavy penalty (for both the copier and the originator). Don't take a risk.

Have fun!

[Jin-Soo Kim](mailto:jinsoo.kim_AT_snu.ac.kr)  
[Systems Software and Architecture Laboratory](http://csl.snu.ac.kr)  
[Dept. of Computer Science and Engineering](http://cse.snu.ac.kr)  
[Seoul National University](http://www.snu.ac.kr)
