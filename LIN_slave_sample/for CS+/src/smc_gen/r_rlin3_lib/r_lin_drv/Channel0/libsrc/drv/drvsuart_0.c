/* -------------------------------------------------------------------- */
/* Copyright(c) 2009-2025 Renesas Electronics Corporation               */
/* -------------------------------------------------------------------- */
/* File name: drvsuart.c                                                */
/* Version:   0.09                                                      */
/* Date:      2025.08.04                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.04.08  Fixed RLIN3 reset method.                       */
/*   V0.03: 2013.08.22  The bit definitions in LEST were deleted.       */
/*                      The order of u1sUnknownErrors was fixed.        */
/*                      [LDrvSUart_vogInitAll,LDrvSUart_vogRefreshReg]  */
/*                       PIM set operation was added.                   */
/*                      [LDrvSUart_u1gEnableNormal]                     */
/*                       The operation of FixedBR was modified as       */
/*                       if LIN bus was dominant level at setting FTS,  */
/*                       clock supplying for RLIN3 is turned off and on,*/
/*                       return afterward.                              */
/*                      [LDrvSUart_vosInitNormalRegs]                   */
/*                       Explicit cast for constant value was added.    */
/*                      [LDrvSUart_vosInitWakeupRegs]                   */
/*                       Explicit cast for constant value was added.    */
/*                       Header comment was modified.                   */
/*                      [LDrvSUart_vogVecTx]                            */
/*                       LST cleaning operation was fixed.              */
/*                       (zero reserved bits were considered)           */
/*                      [LDrvSUart_vogVecRx]                            */
/*                       LST cleaning operation was fixed.              */
/*                       (zero reserved bits were considered)           */
/*                       DisableEdge was added in WAKEUP state.         */
/*                       ERR checking at header reception in RECDATA    */
/*                       added.                                         */
/*                       The turning off and on of RLIN3 clock in SLEEP */
/*                       state was changed to RLIN3 reset.              */
/*                      [LDrvSUart_vogVecStatus]                        */
/*                       DisableEdge was added at not UnknownError in   */
/*                       WAKEUP state.                                  */
/*   V0.04: 2013.12.08  [LDrvSUart_vogVecRx]                            */
/*                       The checking of an error detected at same      */
/*                       timing with response reception completion was  */
/*                       added.                                         */
/*   V0.05: 2015.02.02  Processing corresponding to the same            */
/*                      restriction was deleted by lifting              */
/*                      restrictions of FTS bit set timing of RLIN3.    */
/*                      [Constant]                                      */
/*                      - LDrvSUart_u1sUnknownErrors[]                  */
/*                        - Contents in the table ware corrected.       */
/*                      [Function]                                      */
/*                      - LDrvSUart_u1gEnableNormal                     */
/*                        - Local variable was deleted.                 */
/*                        - The processing divided into Auto-BR mode    */
/*                          and Fixed-BR mode was deleted, and only LIN */
/*                          communication starting (FTS=1) state        */
/*                          setting processing to LTRC register was     */
/*                          left.                                       */
/*                      - LDrvSUart_vogSendResponse                     */
/*                        - Debugging port output processing was added. */
/*                      - LDrvSUart_vogVecTx                            */
/*                        - Comment was corrected.                      */
/*                      - LDrvSUart_vogVecTx                            */
/*                        - Debugging port output processing was added. */
/*                        - Comment was corrected.                      */
/*   V0.06: 2016.06.14  CS+ CC-RL support.                              */
/*   V0.07: 2021.05.31  IAR Compiler, LIN2.1, RL78/F2x support          */
/*   V0.08: 2024.08.30  Modified for RL78/F23,F24.                      */
/*   V0.09: 2025.08.04  Header Rx operation in response is modified.    */
/*                      (RespErr is set at least 1 data has been Rx)    */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Includes before pragma declarations                                  */
/* -------------------------------------------------------------------- */
#include "../dev/device.h"
  #ifdef __CCRL__
#include "../dev/section.h"
  #endif /* __CCRL__ */

/* -------------------------------------------------------------------- */
/* Pragma declaration                                                   */
/* -------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)

 #ifdef __CCRL__
#pragma interrupt   LDrvSUart_vogVecTx_0        (vect=INTLIN0TRM)   /* Interrupt handler(LIN transmission completion) */
#pragma interrupt   LDrvSUart_vogVecRx_0        (vect=INTLIN0RVC)   /* Interrupt handler(LIN reception complete)      */
#pragma interrupt   LDrvSUart_vogVecStatus_0    (vect=INTLIN0STA)   /* Interrupt handler(LIN error detection)         */
 #endif /* __CCRL__ */

  #ifdef __IAR_SYSTEMS_ICC__
#define INTLINnTRM_vect                     INTLIN0TRM_vect
#define INTLINnRVC_vect                     INTLIN0RVC_vect
#define INTLINnSTA_vect                     INTLIN0STA_vect
  #endif

#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)

 #ifdef __CCRL__
#pragma interrupt   LDrvSUart_vogVecTx_1        (vect=INTLIN1TRM)   /* Interrupt handler(LIN transmission completion) */
#pragma interrupt   LDrvSUart_vogVecRx_1        (vect=INTLIN1RVC)   /* Interrupt handler(LIN reception complete)      */
#pragma interrupt   LDrvSUart_vogVecStatus_1    (vect=INTLIN1STA)   /* Interrupt handler(LIN error detection)         */
 #endif /* __CCRL__ */

  #ifdef __IAR_SYSTEMS_ICC__
#define INTLINnTRM_vect                     INTLIN1TRM_vect
#define INTLINnSTA_vect                     INTLIN1STA_vect
#define INTLINnRVC_vect                     INTLIN1RVC_vect
  #endif

#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../com/common.h"
#include "../conf/confslin.h"
#include "../conf/confslin_opt.h"
#include "./drvscom.h"
#include "./drvsext.h"
#include "./drvstimer.h"
#include "./drvsuart.h"
#include "../lcore/lcorescom.h"
#include "../lcore/lcoreshead.h"
#include "../lcore/lcoressleep.h"
#include "../lcore/lcoresstat.h"
#include "../lcore/lcoresresp.h"
#include "../lcore/lcoresmes.h"

/* ------------------------------------------------------------------------- */
/* The addition of LIN channel number to the function name and variable name */
/* ------------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define LDrvSUart_u1sTransitRLIN3Mode       LDrvSUart_u1sTransitRLIN3Mode_0
#define LDrvSUart_vosInitNormalRegs         LDrvSUart_vosInitNormalRegs_0
#define LDrvSUart_vosInitWakeupRegs         LDrvSUart_vosInitWakeupRegs_0
#define LDrvSUart_u1sUnknownErrors          LDrvSUart_u1sUnknownErrors_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define LDrvSUart_u1sTransitRLIN3Mode       LDrvSUart_u1sTransitRLIN3Mode_1
#define LDrvSUart_vosInitNormalRegs         LDrvSUart_vosInitNormalRegs_1
#define LDrvSUart_vosInitWakeupRegs         LDrvSUart_vosInitWakeupRegs_1
#define LDrvSUart_u1sUnknownErrors          LDrvSUart_u1sUnknownErrors_1
#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */

/* Register names, bit names */
#define LDRVSUART_u1sLIOS_1INT      (0x00U)     /* Use LIN interrupt                                                              */
#define LDRVSUART_u1sLIOS_3INT      (0x10U)     /* Use LIN transmission completion, LIN reception completion, LIN error interrupt */
#define LDRVSUART_u1sFTCIE_DISABLE  (0x00U)     /* Disable the response and wake-up transmission completion interrupt             */
#define LDRVSUART_u1sFTCIE_ENABLE   (0x01U)     /* Enable the response and wake-up transmission completion interrupt              */
#define LDRVSUART_u1sFRCIE_DISABLE  (0x00U)     /* Disable the response and wake-up reception complete interrupt                  */
#define LDRVSUART_u1sFRCIE_ENABLE   (0x02U)     /* Enable the response and wake-up reception complete interrupt                   */
#define LDRVSUART_u1sERRIE_DISABLE  (0x00U)     /* Disable the error detection interrupt                                          */
#define LDRVSUART_u1sERRIE_ENABLE   (0x04U)     /* Enable the error detection interrupt                                           */
#define LDRVSUART_u1sSHIE_DISABLE   (0x00U)     /* Disable the header reception complete interrupt                                */
#define LDRVSUART_u1sSHIE_ENABLE    (0x08U)     /* Enable the header reception complete interrupt                                 */
#define LDRVSUART_u1sLSS_LASTDATA   (0x00U)     /* Next transmit and receive data type:Final data                                 */
#define LDRVSUART_u1sLSS_NOLASTDATA (0x80U)     /* Next transmit and receive data type:Non-final data(Non-use)                    */
#define LDRVSUART_u1sLTRC_STARTCOMM (0x01U)     /* Start LIN communication(FTS=1)                                                 */
#define LDRVSUART_u1sLCUC_RESET     (0x00U)     /* RLIN3 mode:Reset mode                                                          */
#define LDRVSUART_u1sLCUC_WAKEUP    (0x01U)     /* RLIN3 mode:wake-up mode                                                        */
#define LDRVSUART_u1sLCUC_LINNORMAL (0x03U)     /* RLIN3 mode:Normal LIN communication mode                                       */
#define LDRVSUART_u1sRCDS_SUBSCRIBE (0x00U)     /* Response and wake-up reception                                                 */
#define LDRVSUART_u1sRCDS_PUBLISH   (0x10U)     /* Response and wake-up transmission                                              */
#define LDRVSUART_u1sLMD_SLAVEAUTO  (0x02U)     /* LIN slave mode(Auto baud rate)                                                 */
#define LDRVSUART_u1sLMD_SLAVEFIXED (0x03U)     /* LIN slave mode(Fixed baud rate)                                                */
#define LDRVSUART_u1sLCS_CLASSIC    (0x00U)     /* Classic checksum                                                               */
#define LDRVSUART_u1sLCS_ENHANCED   (0x20U)     /* Enhanced checksum                                                              */
#define LDRVSUART_u1sLST_CLRMASK    (0xC3U)     /* Valid bits at clearing LST.                                                    */
#define LDRVSUART_u1sLWBR_LPRS(x)   (((x) & (u1)0x0EU) >> (u1)1)    /* Baurate prescaler value.                                   */
#define LDRVSUART_u1sLDFC_RFD(x)    ((x) & (u1)0x0FU)               /* Response data length.                                      */

/* Register names, bit names(RLIN3 Status register) */
#define LDRVSUART_u1sHTRC_DETECT    (0x80U)     /* Header reception completion                          */
#define LDRVSUART_u1sD1RC_DETECT    (0x40U)     /* 1st data of response has been received.              */
#define LDRVSUART_u1sERR_DETECT     (0x08U)     /* Error detection(the OR value of the bit in the LEST) */
#define LDRVSUART_u1sFRC_DETECT     (0x02U)     /* Response and wake-up reception completion            */
#define LDRVSUART_u1sFTC_DETECT     (0x01U)     /* Response and wake-up transmission completion         */


/* -------------------------------------------------------------------- */
/* Function and variable prototype declaration                          */
/* -------------------------------------------------------------------- */
static u1 LDrvSUart_u1sTransitRLIN3Mode( u1 );
static void LDrvSUart_vosInitNormalRegs( void );
static void LDrvSUart_vosInitWakeupRegs( void );


/* -------------------------------------------------------------------- */
/* Constant declaration                                                 */
/* -------------------------------------------------------------------- */
/*---------------------------------------------------------------------------------------------------------------*/
/* UNKNOWN error table by State (LEST value)                                                                     */
/* If INTLINSTA occurs, the error correspond to the following table restarts the RLIN3 as UNKNOWN error.         */
/*                                                                                                               */
/* (Policy)                                                                                                      */
/* Do not reset the RLIN3 when an error occurs due to the header reception of communication(Non-UNKNOWN).        */
/* Preparation of response error in WAITHEADER:                                                                  */
/*  It is possible in case receiving an irrelevant header, and receiving the header immediately before LNRR set. */
/*  When the header is received during WAKEUP, exit VecRx in WAIEHEADER state,                                   */
/*  so there can be no response preparation errors during WAKEUP.                                                */
/* Preparation of response error in response transmission or reception:                                          */
/*  It is not able to occur because the driver state is changed to SENDDATA/RECDATA after setting FTS.           */
/*---------------------------------------------------------------------------------------------------------------*/
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_variable_attributes = @ "LSCNST"
#endif
static const u1 LDrvSUart_u1sUnknownErrors[] = 
{
     /* RPER  | IPER  | CSER  | SFER  | FER   | TER   |  -    | BER   | */
     /* 0x80  | 0x40  | 0x20  | 0x10  | 0x08  | 0x04  |       | 0x01  | */
     /*-----------------------------------------------------------------*/
      ( 0x00U ),                                                            /* 0: (not used)    */
      ( 0x80U | 0x40U | 0x20U | 0x10U | 0x08U | 0x04U         | 0x01U ),    /* 1: DORMANT       */
      (                 0x20U                 | 0x04U         | 0x01U ),    /* 2: WAITHEADER    */
      ( 0x80U | 0x40U         | 0x10U         | 0x04U         | 0x01U ),    /* 3: RECDATA       */
      ( 0x80U | 0x40U | 0x20U | 0x10U | 0x08U | 0x04U                 ),    /* 4: SENDDATA      */
      ( 0x80U         | 0x20U                 | 0x04U         | 0x01U ),    /* 5: WAKEUP        */
      ( 0x80U | 0x40U | 0x20U | 0x10U | 0x08U | 0x04U         | 0x01U ),    /* 6: SENDWAKEUP    */
      ( 0x80U | 0x40U | 0x20U | 0x10U | 0x08U | 0x04U         | 0x01U )     /* 7: SLEEP         */
};
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_variable_attributes =
#endif


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
/* Description : Initialize the UART driver                             */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : This function is processed before the clock supply to  */
/*               LIN macro, so initialization of the SFR of LIN         */
/*               macro is not processed here.                           */
/* -------------------------------------------------------------------- */
void
LDrvSUart_vogInitAll( void )
{

    /* Stop the clock supply to LIN macro */
    /* (Previous state of LIN macro is unknown, so do not reset.)  */
    LDrvSUart_vogPowerOff();
    
    /* Set the clock source of LIN communication */
#if (CONFSLIN_OPT_u1gLINMCK_CFG == CONFSLIN_OPT_u1gLINMCK_FCLK)
    DEV_b1gLINMCK = 0;
#else
    DEV_b1gLINMCK = 1;
#endif

    /* Mask LIN interrupt */
    DEV_b1gLTMK = 1;
    DEV_b1gLRMK = 1;
    DEV_b1gLSMK = 1;

    /* Clear LIN interrupt source */
    DEV_b1gLTIF = 0;
    DEV_b1gLRIF = 0;
    DEV_b1gLSIF = 0;
    
    /* Set LIN interrupt priority */
    DEV_b1gLTPR0 =  ((u1)CONFSLIN_OPT_u1gINTLINTRMPR_CFG & (u1)0x01U);
    DEV_b1gLTPR1 = (((u1)CONFSLIN_OPT_u1gINTLINTRMPR_CFG & (u1)0x02U) >> 1);
    DEV_b1gLRPR0 =  ((u1)CONFSLIN_OPT_u1gINTLINRVCPR_CFG & (u1)0x01U);
    DEV_b1gLRPR1 = (((u1)CONFSLIN_OPT_u1gINTLINRVCPR_CFG & (u1)0x02U) >> 1);
    DEV_b1gLSPR0 =  ((u1)CONFSLIN_OPT_u1gINTLINSTAPR_CFG & (u1)0x01U);
    DEV_b1gLSPR1 = (((u1)CONFSLIN_OPT_u1gINTLINSTAPR_CFG & (u1)0x02U) >> 1);
    
    /* Initialize the port configuration for LIN */
    DEV_vogSET_PIOR4();
    DEV_vogSET_PIOR9();

    DEV_vogSET_TXD_PMC();
    DEV_vogSET_RXD_PMC();

    DEV_b1gRXD_PU   = 1;
    DEV_b1gTXD      = 1;

    DEV_vogSET_POM();
    DEV_vogSET_PIM();

    DEV_b1gTXD_MODE = 0;
    DEV_b1gRXD_MODE = 1;


    /* Initialize the port for debugging */
    DEV_vogDBGPORT_A_INIT();
    DEV_vogDBGPORT_B_INIT();
    DEV_vogDBGPORT_C_INIT();


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Refresh the UART driver register                       */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LDrvSUart_vogRefreshReg( void )
{

    /* Refresh setting LIN interrupt priority */
    DEV_b1gLTPR0 =  ((u1)CONFSLIN_OPT_u1gINTLINTRMPR_CFG & (u1)0x01U);
    DEV_b1gLTPR1 = (((u1)CONFSLIN_OPT_u1gINTLINTRMPR_CFG & (u1)0x02U) >> 1);
    DEV_b1gLRPR0 =  ((u1)CONFSLIN_OPT_u1gINTLINRVCPR_CFG & (u1)0x01U);
    DEV_b1gLRPR1 = (((u1)CONFSLIN_OPT_u1gINTLINRVCPR_CFG & (u1)0x02U) >> 1);
    DEV_b1gLSPR0 =  ((u1)CONFSLIN_OPT_u1gINTLINSTAPR_CFG & (u1)0x01U);
    DEV_b1gLSPR1 = (((u1)CONFSLIN_OPT_u1gINTLINSTAPR_CFG & (u1)0x02U) >> 1);


    /* Refresh the port settings for the LIN */
    DEV_vogSET_PIOR4();
    DEV_vogSET_PIOR9();

    DEV_vogSET_TXD_PMC();
    DEV_vogSET_RXD_PMC();

    DEV_b1gRXD_PU   = 1;
    DEV_b1gTXD      = 1;

    DEV_vogSET_POM();
    DEV_vogSET_PIM();

    DEV_b1gTXD_MODE = 0;
    DEV_b1gRXD_MODE = 1;

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Start the clock supply to LIN macro.                  */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LDrvSUart_vogPowerOn( void )
{
    if( DEV_b1gLINEN == (u1)0 ) {
        DEV_b1gLINEN = 1;               /* Start the clock supply to LIN */
    }
    
    if( DEV_b1gLINMCKE == (u1)0 ) {
        DEV_b1gLINMCKE = 1;             /* Start the clock supply to LIN engine */
    }


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Stop the clock supply to LIN macro.                    */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LDrvSUart_vogPowerOff( void )
{
    if( DEV_b1gLINMCKE == (u1)1 ) {
        DEV_b1gLINMCKE = 0;             /* Stop the clock supply to LIN */
    }
    
    if( DEV_b1gLINEN == (u1)1 ) {
        DEV_b1gLINEN = 0;               /* Stop the clock supply to LIN engine */
    }
    

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Transit LIN macro to the reset mode.                   */
/* Parameters  : Nothing                                                */
/* Return value: OK : success                                           */
/*              LDRVSUART_u1gERROR_TRTIMEOUT : State transition timeout */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LDrvSUart_u1gDisable( void )
{
    u1 u1tResult;
    

    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();
    
    /* Change RLIN3 mode(->Reset Mode) */
    u1tResult = LDrvSUart_u1sTransitRLIN3Mode( (u1)LDRVSUART_u1sLCUC_RESET );

    /* Even if RLIN3 state transition has timed out,          */
    /* disable interrupts since previous state is unknown.    */

    /* Clear LIN interrupt source */
    DEV_b1gLTIF = 0;
    DEV_b1gLRIF = 0;
    DEV_b1gLSIF = 0;

    /* Mask LIN interrupt */
    DEV_b1gLTMK = 1;
    DEV_b1gLRMK = 1;
    DEV_b1gLSMK = 1;


    return u1tResult;
}


/* -------------------------------------------------------------------- */
/* Description : Transit to the normal LIN communication mode LIN macro.*/
/* Parameters  : Nothing                                                */
/* Return value: OK : success                                           */
/*              LDRVSUART_u1gERROR_TRTIMEOUT : State transition timeout */
/* Misc.       : Call when RLIN3 is in reset mode.                      */
/*               Call when interrupt can't occur.                       */
/* -------------------------------------------------------------------- */
u1
LDrvSUart_u1gEnableNormal( void )
{
    u1 u1tResult;

    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();

    /* Set RLIN3 register */
    LDrvSUart_vosInitNormalRegs();

    /* Transition state of the RLIN3(->Normal LIN communication mode) */
    u1tResult = LDrvSUart_u1sTransitRLIN3Mode( (u1)LDRVSUART_u1sLCUC_LINNORMAL );
    
    if( u1tResult == (u1)OK ) {

        /* Clear LIN interrupt source */
        DEV_b1gLTIF = 0;
        DEV_b1gLRIF = 0;
        DEV_b1gLSIF = 0;

        /* Mask LIN interrupt */
        DEV_b1gLTMK = 0;
        DEV_b1gLRMK = 0;
        DEV_b1gLSMK = 0;

        /* RCDS(transmit and receive directions) is not set because it is waiting to receive header. */

        /* Start waiting for header reception */
        /* (Other bits don't need to be set because LTRC is 0x00 at RLIN3 reset mode) */
        DEV_u1gLTRC = (u1)LDRVSUART_u1sLTRC_STARTCOMM;
    }


    return u1tResult;
}


/* -------------------------------------------------------------------- */
/* Description : Transit to wake-up pulse transmit mode LIN macro.      */
/* Parameters  : Nothing                                                */
/* Return value: OK : success                                           */
/*              LDRVSUART_u1gERROR_TRTIMEOUT : State transition timeout */
/* Misc.       : This function must be processed during RLIN3           */
/*               reset mode.                                            */
/* -------------------------------------------------------------------- */
u1
LDrvSUart_u1gEnableWakeupTx( void )
{
    u1 u1tResult;

    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();

    /* Set RLIN3 register */
    LDrvSUart_vosInitWakeupRegs();

    /* Transition state of the RLIN3(->Wake-up mode) */
    u1tResult = LDrvSUart_u1sTransitRLIN3Mode( (u1)LDRVSUART_u1sLCUC_WAKEUP );
    
    /* If the transition state has timed out, the interrupt is the same as the reset mode. */
    
    if( u1tResult == (u1)OK ) {

        /* Clear LIN interrupt source */
        DEV_b1gLTIF = 0;
        DEV_b1gLRIF = 0;
        DEV_b1gLSIF = 0;

        /* Mask LIN interrupt */
        DEV_b1gLTMK = 0;
        DEV_b1gLRMK = 1;
        DEV_b1gLSMK = 0;
        
        /* Set the communication direction to transmition (Clear bits other than the RCDS) */
        DEV_u1gLDFC = (u1)LDRVSUART_u1sRCDS_PUBLISH;
        
        /* Start sending wake-up pulse */
        /* (Because LTRC is set to 0x00 at reset mode, other bits are no need to set) */
        DEV_u1gLTRC = (u1)LDRVSUART_u1sLTRC_STARTCOMM;
    }

    
    return u1tResult;
}


#if (CONFSLIN_OPT_u1gBUSWKUP_CFG == CONFSLIN_OPT_u1gBUSWKUP_WIDTH)
/* -------------------------------------------------------------------- */
/* Description : Transit to wake-up pulse receive mode LIN macro.       */
/* Parameters  : Nothing                                                */
/* Return value: OK : success                                           */
/*              LDRVSUART_u1gERROR_TRTIMEOUT : State transition timeout */
/* Misc.       : This function must be processed during RLIN3           */
/*               reset mode.                                            */
/* -------------------------------------------------------------------- */
u1
LDrvSUart_u1gEnableWakeupRx( void )
{
    u1 u1tResult;

    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();

    /* Set RLIN3 register */
    LDrvSUart_vosInitWakeupRegs();

    /* Transition state of the RLIN3(->Wake-up mode) */
    u1tResult = LDrvSUart_u1sTransitRLIN3Mode( (u1)LDRVSUART_u1sLCUC_WAKEUP );
    
    /* If the transition state has timed out, the interrupt is the same as the reset mode. */

    if( u1tResult == (u1)OK ) {

        /* Clear LIN interrupt source */
        DEV_b1gLTIF = 0;
        DEV_b1gLRIF = 0;
        DEV_b1gLSIF = 0;

        /* Mask LIN interrupt */
        DEV_b1gLTMK = 1;
        DEV_b1gLRMK = 0;
        DEV_b1gLSMK = 1;
        
        /* Set the communication direction to reception (Clear bits other than the RCDS) */
        DEV_u1gLDFC = (u1)LDRVSUART_u1sRCDS_SUBSCRIBE;
        
        /* Start Reception wake-up pulse */
        /* (Because LTRC become a 0x00 at reset mode, other bits is no need to set) */
        DEV_u1gLTRC = (u1)LDRVSUART_u1sLTRC_STARTCOMM;
    }

    
    return u1tResult;
}
#endif /* CONFSLIN_OPT_u1gBUSWKUP_CFG */


/* -------------------------------------------------------------------- */
/* Description : Set no response for LIN macro                          */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : This function must be processed during other than      */
/*               RLIN3 reset mode.                                      */
/* -------------------------------------------------------------------- */
void
LDrvSUart_vogSetNonResp( void )
{

    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();

    /* Set no response */
    DEV_u1gLTRC = 0x04U;    /* When setting LNRR, writing 0x04 is specified. */
    

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Start transmission of the response.                    */
/* Parameters  : u1tSize    = Data size[byte] (Except checksum)         */
/*             : u1tSumType = Checksum type                             */
/*               CONFSLIN_u1gCLASSICSUM   : Classic checksum            */
/*               CONFSLIN_u1gENHANCEDSUM  : Enhanced checksum           */
/* Return value: Nothing                                                */
/* Misc.       : This function must be processed during RLIN3 is        */
/*               normally LIN communicating.                            */
/* -------------------------------------------------------------------- */
void
LDrvSUart_vogSendResponse( u1 u1tSize, u1 u1tSumType )
{

    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();

    /* Argument check */
    if(((u1tSize >= (u1)1) && (u1tSize <= (u1)8)) &&
       ((u1tSumType == (u1)CONFSLIN_u1gCLASSICSUM) || (u1tSumType == (u1)CONFSLIN_u1gENHANCEDSUM))) {

        /* Start transmission of the response */
        /* (In RLIN3, the header is never crushed even if the length of the response space is set to 0.) */
        if( u1tSumType == (u1)CONFSLIN_u1gCLASSICSUM ) {
            DEV_u1gLDFC = ((u1)LDRVSUART_u1sLSS_LASTDATA | (u1)LDRVSUART_u1sLCS_CLASSIC  | 
                           (u1)LDRVSUART_u1sRCDS_PUBLISH | u1tSize);
        }
        else {
            DEV_u1gLDFC = ((u1)LDRVSUART_u1sLSS_LASTDATA | (u1)LDRVSUART_u1sLCS_ENHANCED | 
                           (u1)LDRVSUART_u1sRCDS_PUBLISH | u1tSize);
        }

        DEV_vogDBGPORT_C_HI();


        /* (Supplement) */
        /* It is also possible to save the bit position of the register in such CONFSLIN_u1gCLASSICSUM, */
        /* but it is not relevant because conflin.h depends on the driver layer.                        */

        DEV_u1gLTRC = 0x02U;    /* When setting RTS, writing 0x02 is specified. */


        DEV_vogDBGPORT_C_LO();
    }


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Start reception of the response.                       */
/* Parameters  : u1tSize    = Data size[byte] (Except checksum)         */
/*             : u1tSumType = Checksum type                             */
/*               CONFSLIN_u1gCLASSICSUM   : Classic checksum            */
/*               CONFSLIN_u1gENHANCEDSUM  : Enhanced checksum           */
/* Return value: Nothing                                                */
/* Misc.       : This function must be processed during RLIN3 is        */
/*               normally LIN communicating.                            */
/* -------------------------------------------------------------------- */
void
LDrvSUart_vogRecvResponse( u1 u1tSize, u1 u1tSumType )
{

    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();


    /* Argument check */
    if(((u1tSize >= (u1)1) && (u1tSize <= (u1)8)) &&
       ((u1tSumType == (u1)CONFSLIN_u1gCLASSICSUM) || (u1tSumType == (u1)CONFSLIN_u1gENHANCEDSUM))) {

        /* Start reception of the responseStart receiving response */
        if( u1tSumType == (u1)CONFSLIN_u1gCLASSICSUM ) {
            DEV_u1gLDFC = ((u1)LDRVSUART_u1sLSS_LASTDATA   | (u1)LDRVSUART_u1sLCS_CLASSIC  | 
                           (u1)LDRVSUART_u1sRCDS_SUBSCRIBE | u1tSize);
        }
        else {
            DEV_u1gLDFC = ((u1)LDRVSUART_u1sLSS_LASTDATA   | (u1)LDRVSUART_u1sLCS_ENHANCED | 
                           (u1)LDRVSUART_u1sRCDS_SUBSCRIBE | u1tSize);
        }

        /* (Supplement) */
        /* It is also possible to save the bit position of the register in such CONFSLIN_u1gCLASSICSUM, */
        /* but it is not relevant because conflin.h depends on the driver layer.                        */

        DEV_u1gLTRC = 0x02U;    /* When setting RTS, writing 0x02 is specified. */
    }


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Set the UART buffer register                           */
/* Parameters  : pu1tSrc    = source pointer for copy                   */
/*               u1tSize    = copy size                                 */
/* Return value: Nothing                                                */
/* Misc.       : This function must be processed during RTS=0 of RLIN3. */
/* -------------------------------------------------------------------- */
void
LDrvSUart_vogSetBuf( const u1* pu1tSrc, u1 u1tSize )
{
    u1 *pu1tMesBuf;

    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();

    /* Argument check */
    if((pu1tSrc != (u1*)0) && 
       (u1tSize >= (u1)1) && (u1tSize <= (u1)8)) {
        
        pu1tMesBuf = (u1*)&(DEV_u1gLDB1);
        
        /* Copy process sequences in the driver use the generic routine. */
        LDrvSCom_vogCopyBuf( pu1tMesBuf, pu1tSrc, u1tSize );
    }


    return;
}

/* -------------------------------------------------------------------- */
/* Description : Get the data from UART buffer register                 */
/* Parameters  : pu1tDst    = destination pointer for copy              */
/*               u1tSize    = copy size                                 */
/* Return value: Nothing                                                */
/* Misc.       : This function must be processed during RTS=0 of RLIN3. */
/* -------------------------------------------------------------------- */
void
LDrvSUart_vogGetBuf( u1* pu1tDst, u1 u1tSize )
{
    u1 *pu1tMesBuf;

    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();

    /* Argument check */
    if((pu1tDst != (u1*)0) && 
       (u1tSize >= (u1)1) && (u1tSize <= (u1)8)) {
        
        pu1tMesBuf = (u1*)&(DEV_u1gLDB1);
        
        /* Copy process sequences in the driver use the generic routine. */
        LDrvSCom_vogCopyBuf( pu1tDst, pu1tMesBuf, u1tSize );
    }

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Clear status of LIN macro and error status             */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : This function must be processed during both other than */
/*               reset mode of RLIN3 and FTS=0.                         */
/* -------------------------------------------------------------------- */
void
LDrvSUart_vogClearStatus( void )
{

    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();
    
    DEV_u1gLST  = 0x00U;
    DEV_u1gLEST = 0x00U;

    
    return;
}


/* -------------------------------------------------------------------- */
/* Description : Return whether 1st response data has been received.    */
/* Parameters  : Nothing                                                */
/* Return value: TRUE  : 1st data has been received.                    */
/*               FALSE : Not received yet.                              */
/* Misc.       : This function must be called after starting response   */
/*               Rx(after LDrvSUart_vogRecvResponse).                   */
/* -------------------------------------------------------------------- */
u1
LDrvSUart_u1gIsRx1stRespData( void )
{
    u1 u1tResult;

    if( ( DEV_u1gLST & LDRVSUART_u1sD1RC_DETECT ) != (u1)0 ) {
        u1tResult = (u1)TRUE;
    }
    else {
        u1tResult = (u1)FALSE;
    }

    return u1tResult;
}


/* -------------------------------------------------------------------- */
/* Description : Return max response time in ms.                        */
/* Parameters  : Nothing                                                */
/* Return value: Max response time in ms.                               */
/* Misc.       : This function is used for detection of response        */
/*               time-out.                                              */
/* -------------------------------------------------------------------- */
u4
LDrvSUart_u4gGetRespTimeMaxMs( void )
{
    u1 u1tLPRS;
    u2 u2tLBRP;
    u1 u1tRespDataNum;
    u4 u4tRespMaxMs;

    u1tLPRS         = (u1)LDRVSUART_u1sLWBR_LPRS( DEV_u1gLWBR );
    u2tLBRP         = (((u2)DEV_u1gLBRP1 << (u1)8) | (u2)DEV_u1gLBRP0);
    u1tRespDataNum  = (u1)LDRVSUART_u1sLDFC_RFD( DEV_u1gLDFC );

    u4tRespMaxMs = ((((u4)1 << u1tLPRS) * 
                     ((u4)u2tLBRP + (u4)1) * 
                     (u4)(((u1)CONFSLIN_OPT_u1gNSPB_NORM_CFG >> (u1)4) + (u1)1) *
                     ((u4)u1tRespDataNum + (u4)1) *
                     (u4)14) / ((u4)ConfSLin_u2gPERICLOCK * (u4)10)) + (u4)1;

    return u4tRespMaxMs;
}


/* -------------------------------------------------------------------- */
/* Internal functions                                                   */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Description : RLIN3 mode transition                                  */
/* Parameters  : u1tMode = RLIN3 mode transition destination            */
/*              LDRVSUART_u1sLCUC_RESET     : Reset mode                */
/*              LDRVSUART_u1sLCUC_WAKEUP    : Wake-up mode              */
/*              LDRVSUART_u1sLCUC_LINNORMAL : Normal LIN communication  */
/*              mode                                                    */
/* Return value: OK : success                                           */
/*             LDRVSUART_u1gERROR_TRTIMEOUT : State transition timeout  */
/* Misc.       : Select LIN channel before this function.               */
/* -------------------------------------------------------------------- */
static u1
LDrvSUart_u1sTransitRLIN3Mode( u1 u1tMode )
{
    u4 u4tWaitCount;
    u1 u1tResult;
    u1 u1tLMSTExp;

    u1tResult = (u1)OK;

    /* Set RLIN3 mode */
    u1tMode &= 3u;
    DEV_u1gLCUC = u1tMode;
    u1tLMSTExp = DEV_u1gLCUC;   /* Get the expected value of LMST (the value of bit 1 depends on LTRC.FTS) */
    
    /* Wait for mode transition */
    u4tWaitCount = 0;

    while( DEV_u1gLMST != u1tLMSTExp ) {
        u4tWaitCount++;
        if( u4tWaitCount >= (u4)CONFSLIN_OPT_u4gTRWTCNTMAX_CFG ) {

            /* When a timeout is occured, error is recovered in LIN core layer */
            u1tResult = (u1)LDRVSUART_u1gERROR_TRTIMEOUT;

            break;
        }
    }

    return u1tResult;
}


/* -------------------------------------------------------------------- */
/* Description : Set RLIN3 register(for normal LIN communication)       */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : This function must be processed during RLIN3           */
/*               reset mode.                                            */
/*               Select LIN channel before this function.               */
/* -------------------------------------------------------------------- */
static void
LDrvSUart_vosInitNormalRegs( void )
{
    u2 u2tLBRP;
    u1 u1tLMD;
    u1 u1tLPRS;


#ifdef __LIN_HW_AUTO_BR__

    /*------------------------*/
    /*  Auto baud rate mode   */

    u1tLMD  = (u1)LDRVSUART_u1sLMD_SLAVEAUTO;
    u2tLBRP = 0; /* (Set to 0 just in case) */

    /* LPRS value is fixed as prescaler clock is between 8[MHz] and 12[MHz] */
    if( ConfSLin_u2gPERICLOCK <= (u2)1200 ) {
        u1tLPRS = (u1)CONFSLIN_OPT_u1gLPRS_NODIV;
    }
    else if( ConfSLin_u2gPERICLOCK <= (u2)2400 ) {
        u1tLPRS = (u1)CONFSLIN_OPT_u1gLPRS_DIV2;
    }
    else {
        u1tLPRS = (u1)CONFSLIN_OPT_u1gLPRS_DIV2P2;
    }
    
#else /* __LIN_HW_AUTO_BR__ */

    /*------------------------*/
    /*  Fixed baud rate mode  */

    u1tLMD  = (u1)LDRVSUART_u1sLMD_SLAVEFIXED;
    u1tLPRS = (u1)CONFSLIN_OPT_u1gLPRS_NORM_CFG;

    /* LBRP = [Peripheral H/W clock] / ([Prescaler divisor] x [Sampling number of bits] x [Baud rate]) */
    /*          (LSB:10[KHz])                                                       (LSB:1[bps]) */
    u2tLBRP = (u2)(((u4)ConfSLin_u2gPERICLOCK * (u4)10000) /
                   ((u4)((u4)1 << (u1tLPRS >> 1)) *
                    (u4)(((u1)CONFSLIN_OPT_u1gNSPB_NORM_CFG >> 4) + (u1)1) *
                    (u4)ConfSLin_u2gBAUDRATENORM)) - (u2)1;
    
#endif /* __LIN_HW_AUTO_BR__ */


    DEV_u1gLWBR  = (u1tLPRS | (u1)CONFSLIN_OPT_u1gNSPB_NORM_CFG);
    DEV_u1gLBRP0 = (u1)u2tLBRP;
    DEV_u1gLBRP1 = (u1)(u2tLBRP >> 8);
    DEV_u1gLMD   = ((u1)CONFSLIN_OPT_u1gLRDNFS_NORM_CFG | (u1)LDRVSUART_u1sLIOS_3INT | u1tLMD);
    DEV_u1gLBFC  =  (u1)CONFSLIN_OPT_u1gBLT_CFG;
    DEV_u1gLSC   = ((u1)CONFSLIN_OPT_u1gIBS_CFG | (u1)CONFSLIN_OPT_u1gRS_CFG);
    DEV_u1gLWUP  =  (u1)CONFSLIN_OPT_u1gWUTL_CFG;
    DEV_u1gLIE   = ((u1)LDRVSUART_u1sSHIE_ENABLE  | (u1)LDRVSUART_u1sERRIE_ENABLE | 
                    (u1)LDRVSUART_u1sFRCIE_ENABLE | (u1)LDRVSUART_u1sFTCIE_ENABLE);
    DEV_u1gLEDE  = ((u1)CONFSLIN_OPT_u1gLTES_CFG  | (u1)CONFSLIN_OPT_u1gIPERE_CFG | (u1)CONFSLIN_OPT_u1gSFERE_CFG |
                    (u1)CONFSLIN_OPT_u1gFERE_CFG  | (u1)CONFSLIN_OPT_u1gTER_CFG   | (u1)CONFSLIN_OPT_u1gBERE_CFG);

    return;
}


/* --------------------------------------------------------------------- */
/* Description : Set RLIN3 register(for wake-up communication)           */
/* Parameters  : Nothing                                                 */
/* Return value: Nothing                                                 */
/* Misc.       : This function must be processed during RLIN3            */
/*               reset mode.                                             */
/*               Select LIN channel before this function.                */
/*               For the flexibility of the clock prescaler,             */
/*               use fixed baud rate mode.                               */
/*               Same interrupts of Tx/Rx are used in order to be less   */
/*               code size.                                              */
/* --------------------------------------------------------------------- */
static void
LDrvSUart_vosInitWakeupRegs( void )
{
    u2 u2tLBRP;


    /* LBRP = [Peripheral H/W clock] / ([Prescaler divisor] x [Sampling number of bits] x [Baud rate]) */
    /*          (LSB:10[KHz])                                                       (LSB:1[bps]) */
    u2tLBRP = (u2)(((u4)ConfSLin_u2gPERICLOCK * (u4)10000) /
                    ((u4)((u4)1 << ((u1)CONFSLIN_OPT_u1gLPRS_WKUP_CFG >> 1)) *
                     (u4)(((u1)CONFSLIN_OPT_u1gNSPB_WKUP_CFG >> 4) + (u1)1) *
                     (u4)CONFSLIN_OPT_u2gWKUPBR_CFG)) - (u2)1;


    DEV_u1gLWBR  = ((u1)CONFSLIN_OPT_u1gLPRS_WKUP_CFG | (u1)CONFSLIN_OPT_u1gNSPB_WKUP_CFG);
    DEV_u1gLBRP0 = (u1)u2tLBRP;
    DEV_u1gLBRP1 = (u1)(u2tLBRP >> 8);
    DEV_u1gLMD   = ((u1)CONFSLIN_OPT_u1gLRDNFS_WKUP_CFG | (u1)LDRVSUART_u1sLIOS_3INT | (u1)LDRVSUART_u1sLMD_SLAVEFIXED);
    DEV_u1gLBFC  =  (u1)CONFSLIN_OPT_u1gBLT_CFG;
    DEV_u1gLSC   = ((u1)CONFSLIN_OPT_u1gIBS_CFG | (u1)CONFSLIN_OPT_u1gRS_CFG);
    DEV_u1gLWUP  =  (u1)CONFSLIN_OPT_u1gWUTL_CFG;
    DEV_u1gLIE   = ((u1)LDRVSUART_u1sSHIE_DISABLE | (u1)LDRVSUART_u1sERRIE_ENABLE | 
                    (u1)LDRVSUART_u1sFRCIE_ENABLE | (u1)LDRVSUART_u1sFTCIE_ENABLE);
    DEV_u1gLEDE  = ((u1)CONFSLIN_OPT_u1gLTES_FRAMETO | (u1)CONFSLIN_OPT_u1gIPERE_DISABLE | (u1)CONFSLIN_OPT_u1gSFERE_DISABLE |
                    (u1)CONFSLIN_OPT_u1gFERE_DISABLE | (u1)CONFSLIN_OPT_u1gTER_DISABLE   | (u1)CONFSLIN_OPT_u1gBERE_CFG);

    return;
}




/* -------------------------------------------------------------------- */
/* Interrupt handler                                                    */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Description : Interrupt handler for RLIN3 transmission success       */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : This handler is processed when INTLINnTRM occured.     */
/*               Multiple interrupts are disabled.                      */
/* -------------------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#pragma vector = INTLINnTRM_vect
#endif
INTERRUPT void
LDrvSUart_vogVecTx( void )
{
    u1 u1tLSTTmp;


    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();

    /* Drop the flag in preparation for the next interrupt occurs */
    u1tLSTTmp  = DEV_u1gLST;
    DEV_u1gLST = ((u1)(~u1tLSTTmp) & (u1)LDRVSUART_u1sLST_CLRMASK);

    /* Start the timer of bus time-out monitor */
    LDrvSTimer_vogStartTimeout( ConfSLin_u4gBUSTIMEOUT );

    /* Branch by the state of driver */
    switch( LCoreSCom_u1gFsm ) {
    case (u1)LCORESCOM_u1gFSMSENDDATA:
    {
        if( (u1tLSTTmp & (u1)LDRVSUART_u1sFTC_DETECT) != (u1)0 ) {

            /*-----------------------------------------------------*/
            /* Response transmission complete and error undetected */

            /* Transit the state of RLIN3 to waiting for header. */
            /* (OFF -> ON of the clock supply is not necessary because there is no state transition of RLIN3) */

            LCoreSHead_vogWaitHeader();
            LCoreSResp_vogFinishSendResp();
        }
        else {

            /*-----------------------------------------------------*/
            /*  Response transmission incomplete or error detected */

            /* UNKNOWN error handling */
            LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_TRMUNKNOWN );
        }
        
        break;
    }

    case (u1)LCORESCOM_u1gFSMSENDWAKEUP:
    {
        if( (u1tLSTTmp & (u1)LDRVSUART_u1sFTC_DETECT) != (u1)0 ) {

            /*-------------------------------------*/
            /* Wake-up pulse transmission complete */

            LCoreSStat_vogSetStatusBit( LCORESSTAT_u1gSTAT_BUSACTIVITY );
                                                    /* Bus Activity Flag Set          */

            LCoreSSleep_vogWakeup();
        }
        else {

            /*---------------------------------------*/
            /* Wake-up pulse transmission incomplete */

            /* UNKNOWN error handling */
            LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_TRMUNKNOWN );
        }
        
        break;
    }
    
    default:
    {
        /* Arrive here in the following situations. */
        /*  - LCORESCOM_u1gFSMDORMANT               */
        /*  - LCORESCOM_u1gFSMWAITHEADER            */
        /*  - LCORESCOM_u1gFSMRECDATA               */
        /*  - LCORESCOM_u1gFSMWAKEUP                */
        /*  - LCORESCOM_u1gFSMSLEEP                 */
        /*  - Unknown                               */
        
        /* UNKNOWN error handling */
        LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_TRMUNKNOWN );

        break;
    }
    } /* switch */


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Interrupt handler for RLIN3 reception success          */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : This handler is processed when INTLINnTRM occured.     */
/*               Multiple interrupts are disabled.                      */
/* -------------------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#pragma vector = INTLINnRVC_vect
#endif
INTERRUPT void
LDrvSUart_vogVecRx( void )
{
    u1 u1tLSTTmp;
    u1 u1tLIDBTmp;


    DEV_vogDBGPORT_B_HI();


    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();


    /* Drop the flag in preparation for the next interrupt occurs */
    u1tLSTTmp  = DEV_u1gLST;
    DEV_u1gLST = ((u1)(~u1tLSTTmp) & (u1)LDRVSUART_u1sLST_CLRMASK);


    u1tLIDBTmp = DEV_u1gLIDB;


    /* Start the timer of bus time-out monitor */
    /* (Even if FSMWAKEUP, transit state to waiting for header(bus time-out monitor)) */
    LDrvSTimer_vogStartTimeout( ConfSLin_u4gBUSTIMEOUT );


    /* Branch by the state of driver */
    switch( LCoreSCom_u1gFsm ) {
    case (u1)LCORESCOM_u1gFSMWAITHEADER:
    case (u1)LCORESCOM_u1gFSMSENDDATA:  /* (Consideration to receiving a new header in the response transmission) */
    case (u1)LCORESCOM_u1gFSMWAKEUP:
    {

        if( (u1tLSTTmp & (u1)LDRVSUART_u1sHTRC_DETECT) != (u1)0 ) {
            
            /* If an error and the header receive completion occurs at the same time, */
            /* do nothing here and let VecStatus to process.                          */
            /* (There is possibility at timeout error) */
            if( (u1tLSTTmp & (u1)LDRVSUART_u1sERR_DETECT) == (u1)0 ) {

                /*-------------------------*/
                /* Receive header complete */

                /* Start the response process */
                /* (Transit state to RECDATA or SENDDATA is also done in the function) */
                if( LCoreSResp_u1gStartResponse( u1tLIDBTmp ) == (u1)LCORESRESP_u1gIGNORE ) {
                    LCoreSHead_vogWaitHeader();
                }
            }
        }
        else {
        
            /*-----------------------------*/
            /* Receive response incomplete */

            /* UNKNOWN error handling */
            LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_RVCUNKNOWN );
        }

        break;
    }

    case (u1)LCORESCOM_u1gFSMRECDATA:
    {
        if( (u1tLSTTmp & (u1)LDRVSUART_u1sFRC_DETECT) != (u1)0 ) {

            /* If an error and the reception receive completion occurs at the same time, */
            /* do nothing here and let VecStatus to process.                             */
            /* (There is possibility at timeout error) */
            if( (u1tLSTTmp & (u1)LDRVSUART_u1sERR_DETECT) == (u1)0 ) {

                /*---------------------------*/
                /* Receive response complete */

                LCoreSHead_vogWaitHeader();    /* Because there is a possibility that transit to SLEEP in FinishRecResp(),*/
                                               /* transit state to WAITHEADER once.                                       */
                LCoreSResp_vogFinishRecResp(); /* Analyse the received data(According to the data, transit to SLEEP)      */
            }
        }
        else {
            
            /*-----------------------------*/
            /* Receive response incomplete */
            
            if( (u1tLSTTmp & (u1)LDRVSUART_u1sHTRC_DETECT) != (u1)0 ) {
                
                /* If an error and the header receive completion occurs at the same time, */
                /* do nothing here and let VecStatus to process.                          */
                /* (There is possibility at timeout error) */
                if( (u1tLSTTmp & (u1)LDRVSUART_u1sERR_DETECT) == (u1)0 ) {

                    /*----------------------------------------------------------*/
                    /* Receive response incomplete and Receive header complete */

                    /* Start the response process */
                    /* (Transit state to RECDATA or SENDDATA is also done in the function) */
                    if( LCoreSResp_u1gStartResponse( u1tLIDBTmp ) == (u1)LCORESRESP_u1gIGNORE ) {
                        LCoreSHead_vogWaitHeader();
                    }
                }
            }
            else {

                /*-------------------------------------------------------------*/
                /* Incomplete response received and Receive header incomplete */

                /* UNKNOWN error handling */
                LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_RVCUNKNOWN );
            }
        }
        
        break;
    }
    
#if ( CONFSLIN_OPT_u1gBUSWKUP_CFG == CONFSLIN_OPT_u1gBUSWKUP_WIDTH )
    case (u1)LCORESCOM_u1gFSMSLEEP:
    {
        if( u1tLSTTmp & (u1)LDRVSUART_u1sFRC_DETECT ) {

            /*--------------------------------*/
            /* Receive wake-up pulse complete */

            ConfSLin_vogCallWakeupIfc( LIN_CHANNEL );   /* Wake-up call-out */

            /* Turning clock on and off after reset isn't needed because this transition is from wake up */
            if( LDrvSUart_u1gDisable() == (u1)LDRVSUART_u1gERROR_TRTIMEOUT ) {

                /* RLIN3 state transition timed out */
                (void)LCoreSCom_u1gDisconnect();
                LCoreSStat_vogSetErrorCode( (u2)LCORESSTAT_u2gERRCD_TRTORESET );
            }
            else {

                if( LCoreSHead_u1gWaitHdrFromDis() == (u1)LDRVSUART_u1gERROR_TRTIMEOUT ) {

                    /* RLIN3 state transition timed out */
                    (void)LCoreSCom_u1gDisconnect();
                    LCoreSStat_vogSetErrorCode( (u2)LCORESSTAT_u2gERRCD_TRTONORM );
                }
            }
        }
        else {

            /*----------------------------------*/
            /* Receive wake-up pulse incomplete */

            /* UNKNOWN error handling */
            LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_RVCUNKNOWN );
        }
        
        break;
    }
#endif /* CONFSLIN_OPT_u1gBUSWKUP_CFG */
        
    default:
    {
        /* Arrive here in the following situations.       */
        /*  - LCORESCOM_u1gFSMDORMANT                     */
        /*  - LCORESCOM_u1gFSMSENDWAKEUP                  */
        /*  - LCORESCOM_u1gSLEEP(Configuration dependent) */
        /*  - Unknown                                     */
        
        /* UNKNOWN error handling */
        LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_RVCUNKNOWN );

        break;
    }
    } /* switch */


    DEV_vogDBGPORT_B_LO();


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Error detection interrupt handler                      */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : This handler is processed when INTLINnSTA occured.     */
/*               Multiple interrupts are disabled.                      */
/* -------------------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#pragma vector = INTLINnSTA_vect
#endif
INTERRUPT void
LDrvSUart_vogVecStatus(void)
{
    u1 u1tLESTTmp;


    /* Set the number of LIN channel */
    DEV_vogENABLE_LIN_CH();

    /* Drop the status flag in preparation for the next interrupt occurs  */
    /* (When you write an inverted value, status interrupt will not occur */
    /* if another error occurs before the write(LST.ERR does not drop)).  */
    /*  Therefore cleared to 0.)                                          */
    u1tLESTTmp  = DEV_u1gLEST;
    DEV_u1gLEST = (u1)0x00U;

    /* Start the timer of bus time-out monitor */
    LDrvSTimer_vogStartTimeout( ConfSLin_u4gBUSTIMEOUT );


    /* Is driver state outside the Unknown error table ? */
    if( (LCoreSCom_u1gFsm < (u1)LCORESCOM_u1gFSMDORMANT) ||
        (LCoreSCom_u1gFsm > (u1)LCORESCOM_u1gFSMSLEEP) ) {
        
        /*-------------------*/
        /* Unknown state     */

        LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_STAUNKNOWN | (u2)u1tLESTTmp );

    }
    else if( (u1tLESTTmp & LDrvSUart_u1sUnknownErrors[LCoreSCom_u1gFsm]) != (u1)0 ) {
        
        /*-------------------*/
        /* UNKNOWN error     */

        /* Set the Response Error during sending and receiving response */
        if( (LCoreSCom_u1gFsm == (u1)LCORESCOM_u1gFSMRECDATA) ||
            (LCoreSCom_u1gFsm == (u1)LCORESCOM_u1gFSMSENDDATA) ) {

            LCoreSResp_vogSetErrorStatus();

        }

        /* UNKNOWN error handling */
        LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_STAUNKNOWN | (u2)u1tLESTTmp );

    }
    else {

        /*----------------------*/
        /* Except UNKNOWN error */

        switch( LCoreSCom_u1gFsm ) {
        case (u1)LCORESCOM_u1gFSMWAITHEADER:
        case (u1)LCORESCOM_u1gFSMWAKEUP:
        {
            /* Determine that there was a conflict with another header during receiving header */
            /* (Do not reset to use the new header) */
            LCoreSHead_vogWaitHeader();
            
            break;
        }

        case (u1)LCORESCOM_u1gFSMRECDATA:
        case (u1)LCORESCOM_u1gFSMSENDDATA:
        {
            if( LCoreSMes_u1gIsCurEvent() == (u1)FALSE ) {
            
                /* Set response error, if it is not sending or receiving event-triggered frames */
                LCoreSResp_vogSetErrorStatus();
            }
            
            /* Determine that there was a conflict with a new header in the response sending or receiving */
            /* (Do not reset to use the new header) */
            LCoreSHead_vogWaitHeader();
            
            break;
        }

        default:
        {
            /* Arrive here in the following situations. */
            /*  - LCORESCOM_u1gFSMDORMANT               */
            /*  - LCORESCOM_u1gFSMSLEEP                 */
            /*  - LCORESCOM_u1gFSMSENDWAKEUP            */

            /* UNKNOWN error handling(As Fail-safe) */
            LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_STAUNKNOWN | (u2)u1tLESTTmp );
            
            break;
        }
        } /* switch */
    }


    return;
}
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif


/* End of files */
