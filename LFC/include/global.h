
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

#define N 128
#define M_sweep 1200000

#if defined MAIN_PROGRAM
  #define EXTERN
#else
  #define EXTERN extern
#endif

EXTERN double xx[N];

#undef EXTERN

#endif

