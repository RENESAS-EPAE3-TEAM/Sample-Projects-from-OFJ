/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2021 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: apslintestflag.c                                          */
/* Version:   0.02                                                      */
/* Date:      2021.05.31                                                */
/* Author:    A.Kondo, K.Ogata                                          */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2021.05.31  IAR Compiler support                            */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Includes before pragma declarations                                  */
/* -------------------------------------------------------------------- */
  #ifdef __CCRL__
#include "../dev/section.h"
  #endif /* __CCRL__ */

/* -------------------------------------------------------------------- */
/* Pragma declaration                                                   */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../com/common.h"
#include "../conf/confslin.h"
#include "./apslin.h"
#include "../lcore/lcoresmes.h"

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Function prototype declaration                                       */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Constant declaration                                                 */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Variable declaration                                                 */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes = @ "LSCODE"
#endif

/* -------------------------------------------------------------------- */
/* Description : Read a signal update flag                              */
/* Parameters  : u1tSigName = signal name                               */
/* Return value: signal update flag                                     */
/*               1 = updated                                            */
/*               0 = not updated                                        */
/* Misc.       : Return zero if the signal name is invalid.             */
/*               This process has the period of suspending interrupts.  */
/* -------------------------------------------------------------------- */
u1
ApSLin_u1gTestFlag( u1 u1tSigName )
{
    u1 u1tRet;
    u1 u1tIrq;

    u1tRet = (u1)0;

    if( u1tSigName < ConfSLin_u1gSIGNUM ) {

        u1tIrq =  ConfSLin_vogCallDisableIrq();
        /*-----------------------*/
        /*   Disable interrupt   */
        /*                       */


        u1tRet = LCoreSMes_u1gReadSigFlag( u1tSigName );


        /*                       */
        /*   Enable interrupt    */
        /*-----------------------*/
        ConfSLin_vogCallRestoreIrq( u1tIrq );

    }


    return u1tRet;
}
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif

/* End of files */
