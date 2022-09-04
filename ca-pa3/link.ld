/****************************************************************
 *
 *  4190.308 Computer Architecture (Fall 2021)
 *
 *  Project #3: Image Convolution in RISC-V Assembly
 *
 *  October 25, 2021
 *
 *  Jaehoon Shim (mattjs@snu.ac.kr)
 *  Ikjoon Son (ikjoon.son@snu.ac.kr)
 *  Seongyeop Jeong (seongyeop.jeong@snu.ac.kr)
 *  Systems Software & Architecture Laboratory
 *  Dept. of Computer Science and Engineering
 *  Seoul National University
 *
 ****************************************************************/


OUTPUT_ARCH( "riscv" )
ENTRY(_start)

SECTIONS
{
  . = 0x80000000;
  .text.init : { *(.text.init) }
  . = ALIGN(0x1000);
  .tohost : { *(.tohost) }
  . = ALIGN(0x1000);
  .text : { *(.text) }
  . = 0x80010000;
  .data : { *(.data) }
  .bss : { *(.bss) }
  _end = .;
}
