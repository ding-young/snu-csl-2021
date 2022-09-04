#----------------------------------------------------------------
#
#  4190.308 Computer Architecture (Fall 2021)
#
#  Project #3: Image Convolution in RISC-V Assembly
#
#  October 25, 2021
#
#  Jaehoon Shim (mattjs@snu.ac.kr)
#  Ikjoon Son (ikjoon.son@snu.ac.kr)
#  Seongyeop Jeong (seongyeop.jeong@snu.ac.kr)
#  Systems Software & Architecture Laboratory
#  Dept. of Computer Science and Engineering
#  Seoul National University
#
#----------------------------------------------------------------

PREFIX		= riscv32-unknown-elf-
CC		= $(PREFIX)gcc
CXX		= $(PREFIX)g++
AS		= $(PREFIX)as
OBJDUMP		= $(PREFIX)objdump

PYRISC		= ../pyrisc/sim/snurisc.py      # <-- Change this line
PYRISCOPT	= -l 1

INCDIR		=
LIBDIR		=
LIBS		=

CFLAGS		= -Og -march=rv32i -mabi=ilp32 -static
ASLFAGS		= -march=rv32i -mabi=ilp32 -static
LDFLAGS		= -T./link.ld -nostdlib -nostartfiles
OBJDFLAGS	= -D --section=.text --section=.data

TARGET		= bmpconv
ASRCS		= bmpconv-main.s bmpconv.s bmpconv-test.s
OBJS		= $(ASRCS:.s=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBDIR) $(LIBS)

.s.o:
	$(CC) -c $(CFLAGS) $(INCDIR) $< -o $@

#.c.s:
#	$(CC) $(CFLAGS) $(INCDIR) -S $< -o $@

objdump: $(TARGET)
	$(OBJDUMP) $(OBJDFLAGS) $(TARGET) > $(TARGET).objdump

run: $(TARGET)
	$(PYRISC) $(PYRISCOPT) $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).objdump $(OBJS) *~ a.out
