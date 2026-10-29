/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name:   apslinconnectifc.c                                      */
/* Version:     0.05                                                    */
/* Date:        2024.08.30                                              */
/* Author:      A.Kondo, M.hirano                                       */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.08.22  Header comment of u1gConnectIfc was modified.   */
/*   V0.03: 2015.02.02  Processing corresponding to the same            */
/*                      restriction is eliminated by lifting            */
/*                      restrictions of FTS bit set timing of RLIN3.    */
/*                      [Public function]                               */
/*                      - ApSLin_u1gConnectIfc                          */
/*                        - Header comment of misc. was modified.       */
/*   V0.04: 2021.05.31  IAR Compiler support                            */
/*   V0.05: 2024.08.30  Modified for RL78/F23,F24.                      */
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
#include "../lcore/lcoreshead.h"
#include "../drv/drvsext.h"

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
/* Description : Connect LIN interface, and Start waiting header        */
/* Parameters  : Nothing                                                */
/* Return value: APSLIN_u1gSUCCESS = success                            */
/*               APSLIN_u1gFAIL    = failure                            */
/*                               (state transition of RLIN3 times out)  */
/* Misc.       : This function is an alias of LIN API l_ifc_connect.    */
/*               This function does not do anything if the previous     */
/*               state is not DORMANT.                                  */
/* -------------------------------------------------------------------- */
u1
ApSLin_u1gConnectIfc( void )
{
    u1 u1tIrq, u1tResult;


    u1tResult = APSLIN_u1gSUCCESS;

    if( LCoreSCom_u1gFsm == LCORESCOM_u1gFSMDORMANT ) {

        u1tIrq =  ConfSLin_vogCallDisableIrq();
        /*-----------------------*/
        /*   Disable interrupt   */
        /*                       */
    
        LCoreSCom_vogConnect();
        if( LCoreSHead_u1gWaitHdrFromDis() != (u1)OK ) {
            
            /* It is disconnected from the bus already when the state transition is timed out. */
            /* (-> DORMANT state)                                                              */
            /* Do only notify the user.                                                        */
            u1tResult = APSLIN_u1gFAIL;
        }
        else {
            /* Start detecting bus activity. */
            LDrvSExt_vogEnaBusActDtctNoInt();
        }
    
        /*                       */
        /*   Enable interrupt    */
        /*-----------------------*/
        ConfSLin_vogCallRestoreIrq( u1tIrq );
    }


    return u1tResult;
}
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif

/* End of files */
