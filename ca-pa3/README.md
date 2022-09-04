# 4190.308 Computer Architecture (Fall 2021)
# Project #3: Image Convolution in the RISC-V Assembly Language
### Due: 11:59PM, November 14 (Sunday)


## Introduction

In this project, you will implement an image convolution routine using the 32-bit RISC-V (RV32I) assembly language. An image file in the BMP format will be given as an input. The goal of this project is to give you an opportunity to practice the RISC-V assembly programming. In addition, this project introduces various RISC-V tools that help you compile and run your RISC-V programs.

## Backgrounds

### RGB color model

[<img align="right" width="150" src="https://upload.wikimedia.org/wikipedia/commons/c/c2/AdditiveColor.svg?sanitize=true">](https://en.wikipedia.org/wiki/RGB_color_model)

The RGB color model is one of the most common ways to encode color images in the digital world. The RGB color model is based on the theory that all visible colors can be created using the primary additive colors: red, green, and blue. When two or three of them are combined in different amounts, other colors are produced. The RGB color model is important to graphic design as it is used in computer monitors.

### BMP file format

The BMP file format is an image file format used to store digital images, especially on Microsoft Windows operating systems. A BMP file contains a BMP file header, a Bitmap information header, an optional color palette, and an array of bytes that defines the bitmap data. Since the BMP file format has been extended several times, it supports different types of encoding modes. For example, image pixels can be stored with a color depth of 1 (black and white), 4, 8, 16, 24 (true color, 16.7 million colors) or 32 bits per pixel. Images of 8 bits and fewer can be either grayscale or indexed color mode. More details on the BMP file format can be found at http://en.wikipedia.org/wiki/BMP_file_format.

In this project, we will focus only on the __24-bit uncompressed RGB color mode__ with the "Windows V3" bitmap information header. Under this mode, our target image file has the following structure.

```
              +-----------------------------------------+
              |   BMP file header (14 bytes)            |
              +-----------------------------------------+
              |   Bitmap information header (40 bytes)  |
              +-----------------------------------------+
    imgptr -> |   Bitmap data                           |
              |                                         |
              |                                         |
              |                                         |
              +-----------------------------------------+
```

We will provide you with the skeleton code that has only the bitmap data. So you don't have to worry about these headers. 

### Bitmap data format

The bitmap data describes the image, pixel by pixel. Each pixel consists of an 8-bit blue (B) byte, a green (G) byte, and a red (R) byte in that order. __Pixels are stored "upside-down"__ with respect to normal image raster scan order, starting in the lower left corner, going from left to right, and then row by row from the bottom to the top of the image. Note that __the number of bytes occupied by each row should be a multiple of 4__. If that's not the case, the remaining bytes are padded with zeroes. The following figure summarizes the structure of the bitmap data.

![BMP image format](http://csl.snu.ac.kr/courses/4190.308/2021-2/bmpformat.jpg)


### Image convolution

In image processing, a *kernel* is a small matrix used for blurring, sharpening, embossing, edge detection, and more. It is also used in machine learning for *feature extraction*, a technique for determining the most important portions of an image. This is accomplished by performing a *convolution* operation between the kernel and an image.

Convolution is the process of adding each element of the image to its local neighbors, weighted by the kernel. The following example shows the result where the 3x3 kernel `K` is applied to the 5x5 source image `S`.

```
S = [[ 1, 1, 1, 0, 0],   
     [ 0, 1, 1, 1, 1],    K = [[1, 0, 1],      [[4, 3, 4],
     [ 0, 0, 1, 1, 1], *       [0, 1, 0],  =    [2, 4, 3],
     [ 0, 0, 1, 1, 0],         [1, 0, 1]]       [2, 3, 4]]
     [ 0, 1, 1, 0, 0]]     
```

<center><img src=http://csl.snu.ac.kr/courses/4190.308/2021-2/conv.gif alt="Convolution example" width=300></center>

Differently sized kernels containing different patterns of numbers produce different results under convolution. The size of a kernel is arbitrary but we will always use the 3x3 kernel. 

There are several ways to handle the missing values outside of the image boundary. We assume that any pixel in the output image which would require values from beyond the edge is skipped. Hence, for the given `S` = `h` x `w` image, our convolution operation always results in the `D` = `(h-2)` x `(w-2)` output image. 

## Problem specification

Complete the file `bmpconv.s` that implements the function `bmpconv()` in the 32-bit RISC-V assembly language. The prototype of `bmpconv()` is as follows:

```
  void bmpconv (unsigned char *imgptr, int h, int w, 
                unsigned char *k, unsigned char *outptr);
```

The first argument, `imgptr` points to the bitmap data that stores the actual image, pixel by pixel. The next two arguments, `h` and `w`, represent the height and the width of the given image (in pixels), respectively. The argument `k` points to the address of the kernel. The last argument ``outptr`` points to the address of the output image. Note that the pixel data for the input image (indicated by `imgptr`) and the output image (indicated by `outptr`) should follow the same bitmap data format used in the BMP image file. You can assume that a sufficient memory region has been already allocated to store the output image, whose start address is given by `outptr`. You need to perform the convolution operation for each color separately. 

In the assembly code, those arguments, `imgptr`, `h`, `w`, `k`, and `outptr`, are available in the `a0`, `a1`, `a2`, `a3,` and `a4` registers. Because we are using the 32-bit RISC-V simulator, all the registers are 32-bit wide. 

The kernel is always a 3x3 matrix of unsigned characters stored in memory using the row-major ordering (as in the C language). That is, `k == &k[0][0]`, `k+1 == &k[0][1]`, `k+2 == &k[0][2]`, ... and `k+8 == &k[2][2]`. In this project, each element in the kernel is set to one of 0, 1, and -1, in order to avoid the use of the multiplication instruction (Note: The RV32I lacks  multiplication/division instructions).

Let us assume that `S` represents the input image and `D` the output image. For the given `h` x `w` input image, the output image will have the dimension of `(h-2)` x `(w-2)`. In this case, each pixel value in `D[i][j]` is given by:
```
D[i][j] = Sat(S[i][j]*K[0][0]   + S[i][j+1]*K[0][1]   + S[i][j+2]*K[0][2] 
            + S[i+1][j]*K[1][0] + S[i+1][j+1]*K[1][1] + S[i+1][j+2]*K[1][2]
            + S[i+2][j]*K[2][0] + S[i+2][j+1]*K[2][1] + S[i+2][j+2]*K[2][2])
```

The function `Sat(v)` is the saturation arithmetic operation. For the given signed integer `v`, `Sat(v)` returns an 8-bit unsigned integer, where
```
Sat(v) = 0    if v < 0,
Sat(v) = 255  if v > 255,
Sat(v) = v    otherwise
```


## Restrictions

* You are allowed to use only the following registers in the `bmpconv.s` file: `zero (x0)`, `sp`, `ra`, `a0` ~ `a4`, `t0` ~ `t4`. If you are running out of registers, use stack as temporary storage.

* The maximum amount of the space you can use in the stack is limited to 256 bytes. Let `A` be the address indicated by the `sp` register at the beginning of the function `bmpconv()`. The valid stack area you can use is from `A - 256` to `A - 1`. You should always access the stack area using the `sp` register such as `sw a0, 16(sp)`.

* The `lb` and `sb` instructions are not available in the simulator. Therefore, you need to use `lw` and `sw` instructions to access data in memory.

* The padding area in the output image should be set to the value 0, if any.

* Your program should work for any `h` x `w` input image where `h` > 3 and `w` > 3. 

* You are allowed to define any extra functions inside of the `bmpconv.s` file, if necessary.

* Your program should finish within a reasonable time. If your code does not finish within a predefined threshold, it will be terminated.

* __The top 15 fastest `bmpconv()` implementations will receive a 10% extra bonus__. Also, __the next 15 fastest implementations will receive a 5% bonus__. The time will be measured in clock cycles which is identical to the total number of instructions executed.


## Building RISC-V GCC compiler

In order to compile RISC-V assembly programs, you need to build a cross compiler, i.e. the compiler that generates the RISC-V binary code on the x86-64 machine. To build the RISC-V toolchain on your machine (on either Linux or MacOS), please take the following steps. These instructions are also available in the [README.md](https://github.com/snu-csl/pyrisc/blob/master/README.md) file of the [PyRISC toolset](https://github.com/snu-csl/pyrisc).

### 1. Install prerequisite packages first

For Ubuntu, perform the following commands:
```
$ sudo apt-get install autoconf automake autotools-dev curl libmpc-dev
$ sudo apt-get install libmpfr-dev libgmp-dev gawk build-essential bison flex
$ sudo apt-get install texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev
```
If your machine runs MacOS, perform the following command (If you don't have the `brew` utility, you have to install it first -- please refer to https://brew.sh):
```
$ brew install gawk gnu-sed gmp mpfr libmpc isl zlib expat
```

### 2. Download the RISC-V GNU Toolchain from Github

```
$ git clone --recursive https://github.com/riscv/riscv-gnu-toolchain
```

### 3. Configure the RISC-V GNU toolchain

```
$ cd riscv-gnu-toolchain
$ mkdir build
$ cd build
$ ../configure --prefix=/opt/riscv --with-arch=rv32i
```

### 4. Compile and install them.

Note that they are installed in the path given as the prefix, i.e. `/opt/riscv` in this example. (Warning: This step may take some time.)

```
$ sudo make
```

### 5. Add the directory `/opt/riscv/bin` in your `PATH`

```
$ export PATH=/opt/riscv/bin:$PATH
```

`PATH` is an environment variable on Linux, specifying a set of directories where executable programs are located. When a command name is specified by the user, the system searches through `$PATH`, examining each directory from left to right in the list, looking for a filename that matches the command name. 

If you don't want to type the above command every time you log in, put it in your `~/.bashrc` or `~/.bash_aliases` file.


## Skeleton code

We provide you with the skeleton code for this project. It can be downloaded from Github at https://github.com/snu-csl/ca-pa3/.

To download and build the skeleton code, please follow these steps:

```
$ git clone https://github.com/snu-csl/ca-pa3.git
$ cd ca-pa3
$ make
riscv32-unknown-elf-gcc -c -Og -march=rv32i -mabi=ilp32 -static  bmpconv-main.s -o bmpconv-main.o
riscv32-unknown-elf-gcc -c -Og -march=rv32i -mabi=ilp32 -static  bmpconv.s -o bmpconv.o
riscv32-unknown-elf-gcc -c -Og -march=rv32i -mabi=ilp32 -static  bmpconv-test.s -o bmpconv-test.o
riscv32-unknown-elf-gcc -T./link.ld -nostdlib -nostartfiles -o bmpconv bmpconv-main.o bmpconv.o bmpconv-test.o
```

## Running your RISC-V executable file

The executable file generated by `riscv32-unknown-elf-gcc` should be run in the RISC-V machine. In this project, we provide you with a RISC-V instruction set simulator written in Python, called __snurisc__. It is available at the separate Github repository at https://github.com/snu-csl/pyrisc. You can install it by performing the following command. 
```
$ git clone https://github.com/snu-csl/pyrisc
```

To run your RISC-V executable file, you need to modify the `./ca-pa3/Makefile` so that it can find the __snurisc__ simulator. In the `Makefile`, there is a variable called `PYRISC`. By default, it was set to `../pyrisc/sim/snurisc.py`. For example, if you have downloaded PyRISC in `/dir1/dir2/pyrisc`, set `PYRISC` to `/dir1/dir2/pyrisc/sim/snurisc.py`.

```
...

PREFIX      = riscv32-unknown-elf-
CC          = $(PREFIX)gcc
CXX         = $(PREFIX)g++
AS          = $(PREFIX)as
OBJDUMP     = $(PREFIX)objdump

PYRISC      = /dir1/dir2/pyrisc/sim/snurisc.py      # <-- Change this line
PYRISCOPT   = -l 1

INCDIR      =
LIBDIR      =
LIBS        =

...
```

Now you can run `bmpconv`, by performing `make run`. The result of a sample run using the __snurisc__ simulator looks like this:

```
$ make run
/dir1/dir2/pyrisc/sim/snurisc.py   -l 1 bmpconv
Loading file bmpconv
Execution completed
Registers
=========
zero ($0): 0x00000000    ra ($1):   0x800001b8    sp ($2):   0x80017ffc    gp ($3):   0x00000000
tp ($4):   0x00000000    t0 ($5):   0x000005a0    t1 ($6):   0x4c000000    t2 ($7):   0x00000000
s0 ($8):   0x00000000    s1 ($9):   0x00000003    a0 ($10):  0x80012d2c    a1 ($11):  0x00000000
a2 ($12):  0x000000bc    a3 ($13):  0x80017ff0    a4 ($14):  0x8001aa30    a5 ($15):  0x00000000
a6 ($16):  0x00000000    a7 ($17):  0x00000000    s2 ($18):  0x80010008    s3 ($19):  0x80010018
s4 ($20):  0x80018020    s5 ($21):  0x80012f24    s6 ($22):  0x80015934    s7 ($23):  0x80018000
s8 ($24):  0x00000000    s9 ($25):  0x00000000    s10 ($26): 0x00000000    s11 ($27): 0x00000000
t3 ($28):  0x00000000    t4 ($29):  0x00000000    t5 ($30):  0x80018020    t6 ($31):  0x00000003
1945220 instructions executed in 1945220 cycles. CPI = 1.000
Data transfer:    448930 instructions (23.08%)
ALU operation:    1180661 instructions (60.70%)
Control transfer: 315629 instructions (16.23%)
```

If the value of the `t6` (or `x31`) register is nonzero, it means that your program has failed to pass all test cases.  If you failed a test case, it shall stop running and the index of the test case will be stored in `t6`. The memory address of the first incorrect word will be stored in `t5`. For example, if the value of `t6` is equal to 0x00000003, it means that your program passed test 1 and 2, but didn't pass test 3. If the value of `t5` is equal to 0x80018020, it means that your program has failed to match the value of the answer output at the memory address 0x80018020.

Please note that the simulator `snurisc.py` has the ability to show various log information. For example, if you specify the log level 3 (`-l 3`), you can see each instruction executed by the simulator as shown below.

```
/dir1/dir2/pyrisc/sim/snurisc.py   -l 3 bmpconv
Loading file bmpconv
0 0x80000000: lui    sp, 0x80018000
1 0x80000004: jal    ra, 0x8000000c
2 0x8000000c: addi   sp, sp, -4
3 0x80000010: sw     ra, 0(sp)
4 0x80000014: addi   t6, zero, 1
5 0x80000018: auipc  s2, 0x10000
6 0x8000001c: addi   s2, s2, -24
7 0x80000020: auipc  s3, 0x10000
8 0x80000024: addi   s3, s3, -16
9 0x80000028: lui    a4, 0x80018000
10 0x8000002c: lw     a3, 0(s2)
11 0x80000030: lw     a2, 12(a3)
12 0x80000034: lw     a1, 16(a3)
13 0x80000038: addi   a0, a3, 20
14 0x8000003c: jal    ra, 0x80000090
15 0x80000090: jalr   zero, ra, 0
16 0x80000040: lui    s4, 0x80018000
17 0x80000044: lw     s5, 0(s3)
18 0x80000048: lw     s6, 4(s3)
19 0x8000004c: lw     s7, 0(s4)
20 0x80000050: lw     s8, 0(s5)
21 0x80000054: bne    s7, s8, 0x80000088
22 0x80000088: addi   t5, s4, 0
23 0x8000008c: ebreak
Execution completed
...
```

## Hand-in instructions

* Submit only the `bmpconv.s` file to the submission server.

* If your program contains any register names other than the allowed ones, it will be rejected by the server.

* Your program will be rejected if it contains such keywords as `.octa`, `.quad`, `.long`, `.int`, `.word`, `.short`, `.hword`, `.byte`, `.double`, `.single`, `.float`, etc. 



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
