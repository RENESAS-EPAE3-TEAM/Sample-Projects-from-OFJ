/* -------------------------------------------------------------------- */
/* Copyright(c) 2009-2024 Renesas Electronics Corporation               */
/* -------------------------------------------------------------------- */
/* File name: drvsext.c                                                 */
/* Version:   0.07                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.05.17  Interrupt name INTPx was changed to INTLINnWUP. */
/*   V0.03: 2013.08.22  Name of LDrvSExt_vogEnaNegEdgeNoInt was changed.*/
/*                      LDrvSExt_vogEnablePosEdge was added.            */
/*                      Name of LDrvSExt_vogEnableNegEdge was changed.  */
/*                      Added operation into LDrvSExt_vogEnableNegEdge. */
/*                      Added operation into LDrvSExt_vogDisableEdge.   */
/*                      Added operation into LDrvSExt_vogVecExt only    */
/*                      FixedBR mode. (If LIN bus was dominant level    */
/*                      when FTS was set, FTS setting is done at rise   */
/*                      edge)                                           */
/*                      The compiled timing of LDrvSExt_vogVecExt was   */
/*                      changed as edge wake-up or FixedBR mode.        */
/*   V0.04: 2015.02.02  Processing corresponding to the same            */
/*                      restriction was deleted by lifting              */
/*                      restrictions of FTS bit set timing of RLIN3.    */
/*                      [#pragma]                                       */
/*                        - Condition was corrected.                    */
/*                      [Public function]                               */
/*                      - LDrvSExt_vogEnableEdgeNoInt                   */
/*                        - Function name was corrected.                */
/*                      - LDrvSExt_vogEnablePosEdge                     */
/*                        - Function was deleted.                       */
/*                      - LDrvSExt_vogEnableEdge                        */
/*                        - Name and processing of the function were    */
/*                          corrected.                                  */
/*                      - LDrvSExt_vogDisableEdge                       */
/*                        - Processing was corrected.                   */
/*                      - LDrvSExt_vogVecEdge                           */
/*                        - Processing was corrected.                   */
/*   V0.05: 2016.06.13  CS+ CC-RL support.                              */
/*   V0.06: 2021.05.31  IAR Compiler, LIN2.1, RL78/F2x support          */
/*   V0.07: 2024.08.30  Modified for RL78/F23,F24.                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Includes before pragma declarations                                  */
/* -------------------------------------------------------------------- */
#include "../dev/device.h"
  #ifdef __CCRL__
#include "../dev/section.h"
  #endif /* __CCRL__ */

/* -------------------------------------------------------------------- */
/* Including header files (must be defined before pragma)               */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Channel abstraction macro for static symbols and interrupt functions */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Pragma declaration                                                   */
/* -------------------------------------------------------------------- */
#if ( CONFSLIN_OPT_u1gBUSWKUP_CFG == CONFSLIN_OPT_u1gBUSWKUP_EDGE )

 #if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
  #ifdef __CCRL__
#pragma interrupt   LDrvSExt_vogVecEdge_0   (vect=INTLIN0WUP)
  #endif /* __CCRL__ */
  #ifdef __IAR_SYSTEMS_ICC__
#define INTLINnWUP_vect                     INTLIN0WUP_vect
  #endif
 #endif /* __LIN_CH0__ */

 #if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
  #ifdef __CCRL__
#pragma interrupt   LDrvSExt_vogVecEdge_1   (vect=INTLIN1WUP)
  #endif /* __CCRL__ */
  #ifdef __IAR_SYSTEMS_ICC__
#define INTLINnWUP_vect                     INTLIN1WUP_vect
  #endif
 #endif /* __LIN_CH1__ */

#endif /* CONFSLIN_OPT_u1gBUSWKUP_CFG */

/* -------------------------------------------------------------------- */
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../com/common.h"
#include "../conf/confslin.h"
#include "./drvsext.h"
#include "./drvsuart.h"
#include "../lcore/lcorescom.h"
#include "../lcore/lcoreshead.h"
#include "../lcore/lcoresstat.h"

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
  #ifdef __CCRL__
#define LDrvSExt_vogVecEdge             LDrvSExt_vogVecEdge_0
  #endif /* __CCRL__ */
#endif /* __LIN_CH0_x__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
  #ifdef __CCRL__
#define LDrvSExt_vogVecEdge             LDrvSExt_vogVecEdge_1
  #endif /* __CCRL__ */
#endif /* __LIN_CH1_x__ */

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
static u1 LDrvSExt_u1sLastBusLevel;     /* Bus level at the time of the last ReadBusStatus */
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
/* Description : Initialize the external interrupt driver               */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LDrvSExt_vogInitAll( void )
{

    /* Initialize the value  */
    LDrvSExt_u1sLastBusLevel = (u1)1; /* Set to 1 since the LIN bus is normally high. */

    /* Mask external interrupt */
    DEV_b1gPMK  = 1;

    /* Disable the detection of both edges */
    DEV_b1gEGP  = 0;
    DEV_b1gEGN  = 0;

    /* Clear external interrupt request */
    DEV_b1gPIF  = 0;

    /* Set interrupt priority */
    DEV_b1gPPR0 =  (CONFSLIN_OPT_u1gINTPPR_CFG & 0x01U);
    DEV_b1gPPR1 = ((CONFSLIN_OPT_u1gINTPPR_CFG & 0x02U) >> 1);

    /* Connect input signal from LIN reception pin to an external interrupt. */
    /* (External interrupt pins are not used) */
    DEV_b1gISC  = 1;
    

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Refresh driver registers of the external interrupt     */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LDrvSExt_vogRefreshReg( void )
{

    /* Refresh interrupt priority setting */
    DEV_b1gPPR0 =  (CONFSLIN_OPT_u1gINTPPR_CFG & 0x01U);
    DEV_b1gPPR1 = ((CONFSLIN_OPT_u1gINTPPR_CFG & 0x02U) >> 1);

    /* Refresh the connection settings to the LIN reception pin */
    DEV_b1gISC  = 1;


    return;
}


#if ( CONFSLIN_OPT_u1gBUSWKUP_CFG == CONFSLIN_OPT_u1gBUSWKUP_EDGE )
/* ------------------------------------------------------------------------------------------ */
/* Description : Enable reception of the falling edge                                         */
/* Parameters  : Nothing                                                                      */
/* Return value: Nothing                                                                      */
/* Misc.       : This function is not used if Wake-up method is the dominant width detection. */
/* ------------------------------------------------------------------------------------------ */
void
LDrvSExt_vogEnableEdge( void )
{
    DEV_b1gEGP  = 0; /* Disable the rising edge of the external interrupt */
    DEV_b1gEGN  = 1; /* Enable the falling edge of the external interrupt */
    DEV_b1gPIF  = 0; /* Clear the request of the external interrupt       */
    DEV_b1gPMK  = 0; /* Enable the external interrupt                     */

    return;
}
#endif /* CONFSLIN_OPT_u1gBUSWKUP_CFG */


/* -------------------------------------------------------------------- */
/* Description : Disable external interrupts                            */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LDrvSExt_vogDisableEdge( void )
{
    DEV_b1gPMK  = 1; /* Disable the external interrupt                     */
    DEV_b1gEGN  = 0; /* Disable the falling edge of the external interrupt */
    DEV_b1gEGP  = 0; /* Disable the rising edge of the external interrupt  */
    DEV_b1gPIF  = 0; /* Clear the request of the external interrupt        */

    return;
}

/* -------------------------------------------------------------------- */
/* Description : Both edge detection permitted                          */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Disable interrupts,                                    */
/*               and only enable detection of both edges.               */
/* -------------------------------------------------------------------- */
void
LDrvSExt_vogEnaBusActDtctNoInt( void )
{
    LDrvSExt_u1sLastBusLevel = DEV_b1gRXD;  /* Initialize the previous bus level value */

    DEV_b1gPMK  = 1; /* Disable the external interrupt                     */
    DEV_b1gEGN  = 1; /* Enable the falling edge of the external interrupt  */


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Read the external interrupt request flag               */
/* Parameters  : Nothing                                                */
/* Return value: LDRVSEXT_u1gBUSACTIVE_FALSE     (0): Inactive          */
/*               LDRVSEXT_u1gBUSACTIVE_TRUE      (1):                   */
/*                        Active (with bus level change or rising edge) */
/*               LDRVSEXT_u1gBUSACTIVE_FALL_EDGE (2):                   */
/*                        Active (with rising edge)                     */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LDrvSExt_u1gReadBusStatus( void )
{
    u1 u1tStat;
    u1 u1tBusLevel;

    u1tBusLevel = DEV_b1gRXD;                           /* Get the current bus level                          */

    if ( u1tBusLevel != LDrvSExt_u1sLastBusLevel )
    {
        if( DEV_b1gPIF == (u1)1 )
        {
            u1tStat = LDRVSEXT_u1gBUSACTIVE_FALL_EDGE;  /* bus level change and falling edge is present       */
        }
        else
        {
            u1tStat = LDRVSEXT_u1gBUSACTIVE_TRUE;       /* bus level change and no falling edge is present    */
        }
    }
    else
    {
        if( DEV_b1gPIF == (u1)1 )
        {
            u1tStat = LDRVSEXT_u1gBUSACTIVE_FALL_EDGE;  /* no bus level change and falling edge is present    */
        }
        else
        {
            u1tStat = LDRVSEXT_u1gBUSACTIVE_FALSE;      /* no bus level change and no falling edge is present */
        }
    }

    DEV_b1gPIF  = 0;                                    /* Clear the request of the external interrupt        */
    LDrvSExt_u1sLastBusLevel = u1tBusLevel;             /* Update the previous bus level value                */


    return u1tStat;
}


/* -------------------------------------------------------------------- */
/* Internal functions                                                   */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Interrupt handler                                                    */
/* -------------------------------------------------------------------- */

#if ( CONFSLIN_OPT_u1gBUSWKUP_CFG == CONFSLIN_OPT_u1gBUSWKUP_EDGE )
/* ------------------------------------------------------------------------------------------ */
/* Description : External interrupt interface for edge detection                              */
/* Parameters  : Nothing                                                                      */
/* Return value: Nothing                                                                      */
/* Misc.       : This function is not used if Wake-up method is the dominant width detection. */
/* ------------------------------------------------------------------------------------------ */
#ifdef __IAR_SYSTEMS_ICC__
#pragma vector = INTLINnWUP_vect
#endif
INTERRUPT void
LDrvSExt_vogVecEdge( void )
{

    switch( LCoreSCom_u1gFsm ) {
    case LCORESCOM_u1gFSMSLEEP:
    {
        LCoreSStat_vogSetStatusBit( LCORESSTAT_u1gSTAT_BUSACTIVITY );
                                                    /* Bus Activity Flag Set                      */

        ConfSLin_vogCallWakeupIfc( LIN_CHANNEL );   /* Call the user defined callback             */

        LCoreSCom_vogConnect();                     /* Connect LIN interface                      */

        LDrvSExt_vogEnaBusActDtctNoInt();

        (void)LCoreSHead_u1gWaitHdrFromDis();       /* Set the initialization header with waiting */

        break;
    }

    default:
    {
        /* Because it is not usually possible to come here,  */
        /* a transition is made to wait for the header as a fail-safe */
        LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_FAILSAFE );

        break;
    }
    } /* switch */


    return;
}
#endif /* CONFSLIN_OPT_u1gBUSWKUP_CFG */

#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif

/* End of files */
