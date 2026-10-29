/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: lcoreshead.c                                              */
/* Version:   0.05                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.08.22  [LCoreSHead_vogWaitHdrFromDis]                  */
/*                       Operation was changed that if LIN bus is domi- */
/*                       nant level at setting FTS, this function       */
/*                       returns with enabling rising edge detection.   */
/*   V0.03: 2015.02.02  Processing corresponding to the same            */
/*                      restriction was deleted by lifting              */
/*                      restrictions of FTS bit set timing of RLIN3.    */
/*                      [Public function]                               */
/*                      - LCoreSHead_vogInitAll                         */
/*                        - The processing divided into Auto-BR mode    */
/*                          and Fixed-BR mode was deleted, and          */
/*                          processing is unified.                      */
/*                        - Comment was corrected.                      */
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
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../dev/device.h"
#include "../com/common.h"
#include "../conf/confslin.h"
#include "../drv/drvscom.h"
#include "../drv/drvsext.h"
#include "../drv/drvssafe.h"
#include "../drv/drvstimer.h"
#include "../drv/drvsuart.h"
#include "./lcorescom.h"
#include "./lcoreshead.h"
#include "./lcoresresp.h"
#include "./lcoresstat.h"

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
/* Description : Initialize the header module                           */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : No process                                             */
/* -------------------------------------------------------------------- */
void
LCoreSHead_vogInitAll(void)
{
    return;
}


/* ---------------------------------------------------------------------*/
/* Description : Start waiting header from the state of Disable UART    */
/* Parameters  : Nothing                                                */
/* Return value: Process result                                         */
/*               OK = success                                           */
/*             LCORESHEAD_u1gERROR_TRTIMEOUT = State transition timeout */
/* Misc.       : This function must be processed on condition that      */
/*               UART module is Disable. If the state transition        */
/*               times out, disconnected from the bus.                  */
/* -------------------------------------------------------------------- */
u1
LCoreSHead_u1gWaitHdrFromDis(void)
{
    u1 u1tTrResult;
    
    
    LDrvSSafe_vogRefreshReg();


    /* Because the state of UART module is Disable,  */
    /* clearing status is not necessary.             */

    /* It makes a UART module transfer to normal communication mode. */
    if( LDrvSUart_u1gEnableNormal() == LDRVSUART_u1gERROR_TRTIMEOUT ) {

        /* When RLIN3 was timeed out in state transition, it's release from a bus. */
        (void)LCoreSCom_u1gDisconnect();
        LCoreSStat_vogSetErrorCode( (u2)LCORESSTAT_u2gERRCD_TRTONORM );

        u1tTrResult = (u1)LCORESHEAD_u1gERROR_TRTIMEOUT;
    }
    else {
        
        /* Only when RLIN3 state transition succeeded, the driver state is changed. */
        LDrvSTimer_vogStartTimeout( ConfSLin_u4gBUSTIMEOUT );
        LCoreSCom_u1gFsm = (u1)LCORESCOM_u1gFSMWAITHEADER;

        u1tTrResult = (u1)OK;
    }


    return u1tTrResult;
}


/* -------------------------------------------------------------------- */
/* Description : Waiting header from an error condition                 */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : For the processing of error,                           */
/*               OFF -> ON the clock supply to the UART driver.         */
/*               If the state transition of the UART module times out,  */
/*               disconnect driver from the bus(The internal function). */
/* -------------------------------------------------------------------- */
void
LCoreSHead_vogWaitHdrFromErr( u2 u2tErrorCode )
{

    LCoreSStat_vogSetErrorCode( u2tErrorCode );
    (void)LCoreSCom_u1gDisconnect();
    LCoreSCom_vogConnect();
    
    /* Start detecting bus activity */
    LDrvSExt_vogEnaBusActDtctNoInt();

    /* If the state transition of UART module times out in the following function, */
    /* disconnected from the bus.                                                  */
    (void)LCoreSHead_u1gWaitHdrFromDis();

    
    return;
}


/* -------------------------------------------------------------------- */
/* Description : Start waiting header without State transition of       */
/*               the UART module                                        */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : UART module must be running in normal mode.            */
/* -------------------------------------------------------------------- */
void
LCoreSHead_vogWaitHeader(void)
{

    /* Configuration of each module */
    LDrvSSafe_vogRefreshReg();
    LDrvSUart_vogClearStatus();
    LDrvSTimer_vogStartTimeout( ConfSLin_u4gBUSTIMEOUT );


    /* Configuration of state of the driver */
    LCoreSCom_u1gFsm = (u1)LCORESCOM_u1gFSMWAITHEADER;


    return;
}


/* -------------------------------------------------------------------- */
/* Internal functions                                                   */
/* -------------------------------------------------------------------- */

#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif

/* End of files */
