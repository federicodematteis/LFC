
/*******************************************************************************
*
* File global.h
*
* Global parameters and arrays
*
*******************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

#define M 1
#define W 1

#define D 1
#define E 1
#define G 1


#define lambda 0
#define A 1
#define B 0.25
#define P 0

#define N 64
#define M_sweep 10000000

#if defined MAIN_PROGRAM
  #define EXTERN
#else
  #define EXTERN extern
#endif

EXTERN double xx[N];

EXTERN double yy[N];

#undef EXTERN

#endif

