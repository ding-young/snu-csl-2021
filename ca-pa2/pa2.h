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

#ifndef _PA2_H_
#define _PA2_H_

typedef unsigned short int fp10;

fp10 int_fp10(int n);		// convert int -> fp10
int fp10_int(fp10 x);		// convert fp10 -> int
fp10 float_fp10(float f);	// convert float -> fp10
float fp10_float(fp10 x);	// convert fp10 -> float

#endif /* _PA2_H_ */
