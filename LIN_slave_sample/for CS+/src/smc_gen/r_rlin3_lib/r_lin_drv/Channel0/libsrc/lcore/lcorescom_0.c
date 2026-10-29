/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2021 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: lcorescom.c                                               */
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
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../com/common.h"
#include "../conf/confslin.h"
#include "../drv/drvsext.h"
#include "../drv/drvssafe.h"
#include "../drv/drvstimer.h"
#include "../drv/drvsuart.h"
#include "../drv/drvsext.h"
#include "./lcorescom.h"
#include "./lcoresdiag.h"
#include "./lcoreshead.h"
#include "./lcoresmes.h"
#include "./lcoressleep.h"
#include "./lcoresstat.h"
#include "./lcoresresp.h"

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
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_variable_attributes = @ "LSDATA"
#endif
u1 LCoreSCom_u1gFsm;            /* State of the driver                  */
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_variable_attributes =
#endif

/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes = @ "LSCODE"
#endif
/* -------------------------------------------------------------------- */
/* Description : Initialize LIN driver                                  */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : This function must be used in the period of            */
/*               suspending interrupts.                                 */
/* -------------------------------------------------------------------- */
void
LCoreSCom_vogInitAll(void)
{

    /* Initialize the LIN core layer */
    LCoreSDiag_vogInitAll();
    LCoreSHead_vogInitAll();
    LCoreSStat_vogInitAll();
    LCoreSResp_vogInitAll();
    LCoreSSleep_vogInitAll();
    LCoreSMes_vogInitAll();

    /* Initialize the driver layer */
    LDrvSUart_vogInitAll();
    LDrvSTimer_vogInitAll();
    LDrvSExt_vogInitAll();
    LDrvSSafe_vogInitAll();

    /* Set the state of the driver */
    LCoreSCom_u1gFsm = (u1)LCORESCOM_u1gFSMDORMANT;

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Connect the LIN interface                              */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : RLIN3 is in reset mode.                                */
/*               This function is processed at the start of waiting for */
/*               the header and wake-up.                                */
/* -------------------------------------------------------------------- */
void
LCoreSCom_vogConnect(void)
{

    /* Driver layer connection process */
    LDrvSExt_vogDisableEdge();
    LDrvSUart_vogPowerOn();
    LDrvSTimer_vogPowerOnAndInit();
    LDrvSTimer_vogStopTimeout();
    

    /* Set the state of the driver */
    LCoreSCom_u1gFsm = (u1)LCORESCOM_u1gFSMDORMANT;


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Disconnect the LIN interface                           */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : This function does not check time-out, since in case   */
/*               time-out occured in transition to reset mode in        */
/*               LDrvSUart_vogDisable, disconnecting is processed       */
/*               as same.                                               */
/* -------------------------------------------------------------------- */
u1
LCoreSCom_u1gDisconnect(void)
{
    u1 u1tTrResult;
    
    
    u1tTrResult = (u1)OK;
    
    LDrvSExt_vogDisableEdge();

    /* Must be reset RLIN3 before PowerOFF */
    if( LDrvSUart_u1gDisable() == (u1)LDRVSUART_u1gERROR_TRTIMEOUT ) {
        LCoreSStat_vogSetErrorCode( (u2)LCORESSTAT_u2gERRCD_TRTORESET );
        
        /* Though transition to RLIN3 reset mode fails,                   */
        /* due to the need to stop the clock supply, continue processing. */
        
        u1tTrResult = (u1)LCORESCOM_u1gERROR_TRTIMEOUT;
    }

    LDrvSUart_vogPowerOff();
    LDrvSTimer_vogStopTimeout();

    /* Set the state of the driver */
    LCoreSCom_u1gFsm = (u1)LCORESCOM_u1gFSMDORMANT;


    /* (Supplement) */
    /* Considering the possibility that the user is using the same timer unit, */
    /* do not stop the timer clock.                                            */


    return u1tTrResult;
}


/* -------------------------------------------------------------------- */
/* Internal functions                                                   */
/* -------------------------------------------------------------------- */

#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif
/* End of files */
