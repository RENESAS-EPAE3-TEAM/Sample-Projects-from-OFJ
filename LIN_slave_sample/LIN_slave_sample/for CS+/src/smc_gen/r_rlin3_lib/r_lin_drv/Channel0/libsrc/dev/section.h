/* -------------------------------------------------------------------- */
/* Copyright(c) 2014 - 2021 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: section.h                                                 */
/* Version:   0.03                                                      */
/* Date:      2021.05.31                                                */
/* Author:    A.Kondo, S.Sugawara, K.Ogata                              */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2014.02.28  New                                             */
/*   V0.02: 2017.08.30  CS+ CC-RL support. (RL78/F15 edition base)      */
/*   V0.03: 2021.05.31  CS+ CA78K0R deleted                             */
/* -------------------------------------------------------------------- */
#ifndef H_SECTION
#define H_SECTION

/* -------------------------------------------------------------------- */
/* Section name definitions used by master driver (for CubeSuite+).     */
/* (Change right side if needed)                                        */
/* -------------------------------------------------------------------- */
/* [CC-RL]                                                              */
/*  [ROM]                                                               */
/*   text        : Code area (near).                                    */
/*   textf       : Code area (far).                                     */
/*   const       : Constant data area (near).                           */
/*   constf      : Constant data area (far).                            */
/*                                                                      */
/*  [RAM]                                                               */
/*   bss         : Area for variable data without default (near).       */
/*   bssf        : Area for variable data without default (far).        */
/* -------------------------------------------------------------------- */
#ifdef __CCRL__
#pragma section text        LSCODE
#pragma section const       LSCNST
#pragma section bss         LSDATA
#endif /* __CCRL__ */

#endif /* H_SECTION */

/* end of file */
