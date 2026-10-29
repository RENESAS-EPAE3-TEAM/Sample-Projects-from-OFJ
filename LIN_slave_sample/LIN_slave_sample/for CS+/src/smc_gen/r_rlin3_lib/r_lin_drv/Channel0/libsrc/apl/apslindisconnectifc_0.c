/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2021 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: apslindisconnectifc.c                                     */
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
#include "../lcore/lcorescom.h"

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
/* Description : Disconnect LIN interface                               */
/* Parameters  : Nothing                                                */
/* Return value: APSLIN_u1gSUCCESS = success                            */
/*               APSLIN_u1gFAIL    = failure                            */
/*                                (state transition of RLIN3 times out) */
/* Misc.       : This function is an alias of LIN API l_ifc_disconnect. */
/* -------------------------------------------------------------------- */
u1
ApSLin_u1gDisconnectIfc(void)
{
    u1 u1tIrq, u1tResult;


    u1tResult = APSLIN_u1gSUCCESS;

    u1tIrq =  ConfSLin_vogCallDisableIrq();
    /*-----------------------*/
    /*   Disable interrupt   */
    /*                       */

                                        /* Disconnected from the bus    */
    if( LCoreSCom_u1gDisconnect() == LCORESCOM_u1gERROR_TRTIMEOUT ) {

        /* Disconnecting is carried out when state transition of RLIN3 times out,             */
        /* but in order to notify the application abnormal, the return value is checked. */
        u1tResult = APSLIN_u1gFAIL;
    }


    /*                       */
    /*   Enable interrupt    */
    /*-----------------------*/
    ConfSLin_vogCallRestoreIrq( u1tIrq );


    return u1tResult;
}
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif

/* End of files */
