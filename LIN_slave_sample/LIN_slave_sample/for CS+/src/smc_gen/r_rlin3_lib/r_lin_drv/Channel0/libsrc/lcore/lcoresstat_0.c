/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: lcoresstat.c                                              */
/* Version:   0.03                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2021.05.31  IAR Compiler support                            */
/*   V0.03: 2024.08.30  Modified for RL78/F23,F24.                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Includes before pragma declarations                                  */
/* -------------------------------------------------------------------- */
  #ifdef __CCRL__
#include "../dev/section.h"
  #endif /* __CCRL__ */

/* -------------------------------------------------------------------- */
/* Channel abstraction macro for static symbols and interrupt functions */
/* -------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define LCoreSStat_u2sStatus        LCoreSStat_u2sStatus_0
#define LCoreSStat_u2sErrorCode     LCoreSStat_u2sErrorCode_0
#define LCoreSStat_u1sErrorCount    LCoreSStat_u1sErrorCount_0
#define LCoreSStat_u1sRespErr       LCoreSStat_u1sRespErr_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define LCoreSStat_u2sStatus        LCoreSStat_u2sStatus_1
#define LCoreSStat_u2sErrorCode     LCoreSStat_u2sErrorCode_1
#define LCoreSStat_u1sErrorCount    LCoreSStat_u1sErrorCount_1
#define LCoreSStat_u1sRespErr       LCoreSStat_u1sRespErr_1
#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../com/common.h"
#include "../conf/confslin.h"
#include "./lcorescom.h"
#include "./lcoresstat.h"
#include "./lcoresmes.h"
#include "../drv/drvsext.h"

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
#define LCORESSTAT_u2sERRORINRESPONSE   (0x0001U)   /* The mask value for the LIN status (bit0) */
#define LCORESSTAT_u2sSUCCESSTRANSFER   (0x0002U)   /* The mask value for the LIN status (bit1) */
#define LCORESSTAT_u2sOVERRUN           (0x0004U)   /* The mask value for the LIN status (bit2) */
#define LCORESSTAT_u2sGOTOSLEEP         (0x0008U)   /* The mask value for the LIN status (bit3) */

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
static u2 LCoreSStat_u2sStatus;                     /* LIN status                         */
static u2 LCoreSStat_u2sErrorCode;                  /* Error code (not public)            */
static u1 LCoreSStat_u1sErrorCount;                 /* Error counter [times] (not public) */
static u1 LCoreSStat_u1sRespErr;                    /* Response_error                     */
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
/* Description : Initialize the status module                           */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LCoreSStat_vogInitAll(void)
{
    LCoreSStat_u2sStatus     = (u2)0;
    LCoreSStat_u2sErrorCode  = (u2)0;
    LCoreSStat_u1sErrorCount = (u1)0;
    LCoreSStat_u1sRespErr    = (u1)0;

    return;
}

/* --------------------------------------------------------------------------- */
/* Description : Processing at the end of normal transmit and receive response */
/* Parameters  : u1tPid : PID                                                  */
/* Return value: Nothing                                                       */
/* Misc.       : This function is prohibited to run from application.          */
/* --------------------------------------------------------------------------- */
void
LCoreSStat_vogSetSuccess( u1 u1tPid )
{

    /* If the previous status is set untransmitted, set overrun */
    if( (LCoreSStat_u2sStatus & ((u2)0xFFFFU ^ (u2)LCORESSTAT_u1gSTAT_BUSACTIVITY)) != (u2)0x0000U ) {
        LCoreSStat_u2sStatus |= (u2)LCORESSTAT_u2sOVERRUN;
    }


    /* Set PID and Successful Transfer */
    LCoreSStat_u2sStatus = ( (LCoreSStat_u2sStatus & (u2)0x00FFU) | (u2)((u2)u1tPid << 8) );
    LCoreSStat_u2sStatus |= (u2)LCORESSTAT_u2sSUCCESSTRANSFER;


    /* If the response transmitted includes error flag, clear the response error flag */
    /* ( Determination of the direction of communication is not necessary, because response error flag is always sent) */
    if( u1tPid == LCoreSMes_u1gGetSigPid( ConfSLin_stgNODE.u1ErrRes ) ) {
        LCoreSStat_u1sRespErr = (u1)0x00U;
    }


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Get the LIN status                                     */
/* Parameters  : Nothing                                                */
/* Return value: LIN status                                             */
/* Misc.       : This process has the period of suspending interrupts   */
/* -------------------------------------------------------------------- */
u2
LCoreSStat_u2gGetStatus(void)
{
    u2 u2tRet;
    u1 u1tIrq;
    u1 u1tStat;


    u1tIrq =  ConfSLin_vogCallDisableIrq();
    /*-----------------------*/
    /*   Interrupt pending   */
    /*                       */


    u1tStat  = LDrvSExt_u1gReadBusStatus();                     /* Read external interrupt request flag */

    if ( u1tStat != (u1)LDRVSEXT_u1gBUSACTIVE_FALSE ) {         /* Bus level change or rising edge      */
        LCoreSStat_u2sStatus    = LCoreSStat_u2sStatus | (u2)LCORESSTAT_u1gSTAT_BUSACTIVITY;
    }



    u2tRet = LCoreSStat_u2sStatus;
    LCoreSStat_u2sStatus = (u2)0x0000U;


    /*                       */
    /*   Interrupt return    */
    /*-----------------------*/
    ConfSLin_vogCallRestoreIrq(u1tIrq);


    return u2tRet;
}

/* -------------------------------------------------------------------- */
/* Description : Get the error code                                     */
/* Parameters  : Nothing                                                */
/* Return value: Error code                                             */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u2
LCoreSStat_u2gGetErrorCode(void)
{
    return LCoreSStat_u2sErrorCode;
}


/* -------------------------------------------------------------------- */
/* Description : Set the error code                                     */
/* Parameters  : u2tCode = error code                                   */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LCoreSStat_vogSetErrorCode( u2 u2tCode )
{
    LCoreSStat_u2sErrorCode = u2tCode;

    if( LCoreSStat_u1sErrorCount < u1gMAX )
    {
        LCoreSStat_u1sErrorCount++;                  /* Increment the error counter */
    }
    return;
}


/* -------------------------------------------------------------------- */
/* Description : Update the goto sleep flag                             */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LCoreSStat_vogSetSleepFlag(void)
{
    LCoreSStat_u2sStatus |= (u2)LCORESSTAT_u2sGOTOSLEEP; /* Set the goto sleep flag */

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Get the error count                                    */
/* Parameters  : Nothing                                                */
/* Return value: Error count                                            */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LCoreSStat_u1gGetErrorCount(void)
{
    return LCoreSStat_u1sErrorCount;
}

/* -------------------------------------------------------------------- */
/* Description : Set the error status                                   */
/* Parameters  : u1tPid : PID                                             */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LCoreSStat_vogSetErrorStatus( u1 u1tPid )
{
    if( (LCoreSStat_u2sStatus & ((u2)0xFFFFU ^ (u2)LCORESSTAT_u1gSTAT_BUSACTIVITY)) != (u2)0x0000U ) {
                                                                /* Check if the status is updated before */
        LCoreSStat_u2sStatus |= (u2)LCORESSTAT_u2sOVERRUN;      /* Set the overrun bit */
    }

    LCoreSStat_u2sStatus |= (u2)LCORESSTAT_u2sERRORINRESPONSE;  /* Set the error in response bit */

    LCoreSStat_u2sStatus = ( (LCoreSStat_u2sStatus & (u2)0x00FFU) | (u2)((u2)u1tPid << 8) );  /* Set the current PID as the last frame identifier */

    LCoreSStat_u1sRespErr = (u1)0x01U;                          /* Set the response error bit */

    return;
}

/* -------------------------------------------------------------------- */
/* Description : Specified bit set in LCoreSStat_u2sStatus              */
/* Parameters  : u1tSetBit                                              */
/*               LCORESSTAT_u1gSTAT_BUSACTIVITY = Set the               */
/*                                                Bus activity bit      */
/* Return value: Nothing                                                */
/* Misc.       : This process has the period of suspending interrupts   */
/* -------------------------------------------------------------------- */
void
LCoreSStat_vogSetStatusBit( u1 u1tSetBit )
{
    u1 u1tIrq;


    u1tIrq = ConfSLin_vogCallDisableIrq();
    /*-----------------------*/
    /*   Interrupt pending   */
    /*                       */


    LCoreSStat_u2sStatus    = LCoreSStat_u2sStatus | (u2)u1tSetBit;


    /*                       */
    /*   Interrupt return    */
    /*-----------------------*/
    ConfSLin_vogCallRestoreIrq( u1tIrq );

    
    return;
}


/* -------------------------------------------------------------------- */
/* Description : Get the response_error                                 */
/* Parameters  : Nothing                                                */
/* Return value: Response_error=TRUE(0x01),FALSE(0x00)                  */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LCoreSStat_u1gGetRespErr( void )
{
    return LCoreSStat_u1sRespErr;
}

/* -------------------------------------------------------------------- */
/* Internal functions                                                   */
/* -------------------------------------------------------------------- */

#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif


/* End of files */
