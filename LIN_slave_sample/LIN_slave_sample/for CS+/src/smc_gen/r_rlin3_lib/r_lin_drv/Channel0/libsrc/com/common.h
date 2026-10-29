/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2021 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: Common header file for CubeSuite+                         */
/* Version:   0.02                                                      */
/* Date:      2021.05.31                                                */
/* Author:    A.Kondo, K.Ogata                                          */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2021.05.31  IAR Compiler support                            */
/* -------------------------------------------------------------------- */
#ifndef H_COMMON
#define H_COMMON

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */
typedef signed char     s1;
typedef signed short    s2;
typedef signed long     s4;
typedef unsigned char   u1;
typedef unsigned short  u2;
typedef unsigned long   u4;
typedef float           f4;

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
/* Maximum and minimum value */
#define s1gMIN  ((s1)0x80U)
#define s1gMAX  ((s1)0x7fU)
#define u1gMAX  ((u1)0xffU)
#define s2gMIN  ((s2)0x8000U)
#define s2gMAX  ((s2)0x7fffU)
#define u2gMAX  ((u2)0xffffU)
#define s4gMIN  ((s4)0x80000000U)
#define s4gMAX  ((s4)0x7fffffffU)
#define u4gMAX  ((u4)0xffffffffU)

/* Return value of function */
#define ON      1
#define OFF     0
#define OK      0
#define ERROR   (-1)
#define TRUE    1
#define FALSE   0


/* Interrupt function prefix */
#ifdef __CCRL__
#define  INTERRUPT
#endif /* __CCRL__ */

#ifdef __IAR_SYSTEMS_ICC__
#define  INTERRUPT __interrupt
#endif /* __IAR_SYSTEMS_ICC__ */

#endif /* H_COMMON */

/* End of file */
