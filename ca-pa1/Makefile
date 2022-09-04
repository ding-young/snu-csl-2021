#---------------------------------------------------------------
#
#  4190.308 Computer Architecture (Fall 2021)
#
#  Project #1: Run-Length Encoding
#
#  September 14, 2021
#
#  Jaehoon Shim (mattjs@snu.ac.kr)
#  Ikjoon Son (ikjoon.son@snu.ac.kr)
#  Seongyeop Jeong (seongyeop.jeong@snu.ac.kr)
#  Systems Software & Architecture Laboratory
#  Dept. of Computer Science and Engineering
#  Seoul National University
#
#---------------------------------------------------------------

TARGET	= pa1
SRCS	= pa1.c pa1-main.c 
CC		= gcc
CFLAGS	= -g -O2 -Wall
OBJS	= $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
		$(CC) -o $@ $^

clean:
		$(RM) $(TARGET) $(OBJS)
