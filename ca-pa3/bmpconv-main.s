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

	.text
	.align	2

	.globl	_start
_start:
	lui	sp, 0x80018 # 32KB space for result array
	call	main
	ebreak

main:
	addi	sp, sp, -4
	sw	ra, 0(sp)

	# t6:	loop index
	# s2:	indirect pointer to test cases
	# s3:	indirect pointer to answer

	addi	t6, zero, 1
	la	s2, test
	la	s3, ans

loop:
	lui	a4, 0x80018
	lw	a3, 0(s2)
	lw	a2, 12(a3)
	lw	a1, 16(a3)
	addi	a0, a3, 20
	call	bmpconv

cmp_begin:
	# s4:	pointer to result
	# s5:	pointer to correct answer
	# s6:	pointer to end of answer

	lui	s4, 0x80018
	lw	s5, 0(s3)
	lw	s6, 4(s3)

cmp_loop:
	lw	s7, 0(s4)
	lw	s8, 0(s5)
	bne	s7, s8, cmp_fail
	addi	s4, s4, 4
	addi	s5, s5, 4
	bne	s5, s6, cmp_loop

cmp_done:
	addi	t6, t6, 1
	addi	s2, s2, 4
	addi	s3, s3, 4
	lw	s7, 0(s2)
	bne	s7, zero, loop

return:
	mv	t6, zero

	lw	ra, 0(sp)
	addi	sp, sp, 4
	ret

cmp_fail:
	addi	t5, s4, 0
	ebreak
