/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2025 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: drvsuart.h                                                */
/* Version:   0.09                                                      */
/* Date:      2025.09.02                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.08.05  CH adding macro for vogRefreshReg was added.    */
/*   V0.03: 2013.08.22  LDRVSUART_u1gERROR_FTSRXDOM was added.          */
/*   V0.04: 2015.02.02  Processing corresponding to the same            */
/*                      restriction was deleted by lifting              */
/*                      restrictions of FTS bit set timing of RLIN3.    */
/*                      [Macro]                                         */
/*                      - LDRVSUART_u1gERROR_FTSRXDOM was deleted.      */
/*   V0.05: 2021.05.31  RL78/F2x support                                */
/*   V0.06: 2024.08.30  Modified for RL78/F23,F24.                      */
/*   V0.07: 2025.08.04  Added returning max response time function.     */
/*   V0.08: ----.--.--  This version is not used.                       */
/*   V0.09: 2025.09.02  Added macro definition to add CH for 2 funcs.   */
/* -------------------------------------------------------------------- */
#ifndef H_DRVSUART
#define H_DRVSUART

/* -------------------------------------------------------------------- */
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../conf/confslin_opt.h"

/* ------------------------------------------------------------------------- */
/* The addition of LIN channel number to the function name and variable name */
/* ------------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define LDrvSUart_vogInitAll                LDrvSUart_vogInitAll_0
#define LDrvSUart_vogRefreshReg             LDrvSUart_vogRefreshReg_0
#define LDrvSUart_vogPowerOn                LDrvSUart_vogPowerOn_0
#define LDrvSUart_vogPowerOff               LDrvSUart_vogPowerOff_0
#define LDrvSUart_u1gDisable                LDrvSUart_u1gDisable_0
#define LDrvSUart_u1gEnableNormal           LDrvSUart_u1gEnableNormal_0
#define LDrvSUart_u1gEnableWakeupTx         LDrvSUart_u1gEnableWakeupTx_0
#define LDrvSUart_u1gEnableWakeupRx         LDrvSUart_u1gEnableWakeupRx_0
#define LDrvSUart_vogSetNonResp             LDrvSUart_vogSetNonResp_0
#define LDrvSUart_vogSendResponse           LDrvSUart_vogSendResponse_0
#define LDrvSUart_vogRecvResponse           LDrvSUart_vogRecvResponse_0
#define LDrvSUart_vogSetBuf                 LDrvSUart_vogSetBuf_0
#define LDrvSUart_vogGetBuf                 LDrvSUart_vogGetBuf_0
#define LDrvSUart_vogClearStatus            LDrvSUart_vogClearStatus_0
#define LDrvSUart_u1gIsRx1stRespData        LDrvSUart_u1gIsRx1stRespData_0
#define LDrvSUart_u4gGetRespTimeMaxMs       LDrvSUart_u4gGetRespTimeMaxMs_0
#define LDrvSUart_vogVecTx                  LDrvSUart_vogVecTx_0
#define LDrvSUart_vogVecRx                  LDrvSUart_vogVecRx_0
#define LDrvSUart_vogVecStatus              LDrvSUart_vogVecStatus_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define LDrvSUart_vogInitAll                LDrvSUart_vogInitAll_1
#define LDrvSUart_vogRefreshReg             LDrvSUart_vogRefreshReg_1
#define LDrvSUart_vogPowerOn                LDrvSUart_vogPowerOn_1
#define LDrvSUart_vogPowerOff               LDrvSUart_vogPowerOff_1
#define LDrvSUart_u1gDisable                LDrvSUart_u1gDisable_1
#define LDrvSUart_u1gEnableNormal           LDrvSUart_u1gEnableNormal_1
#define LDrvSUart_u1gEnableWakeupTx         LDrvSUart_u1gEnableWakeupTx_1
#define LDrvSUart_u1gEnableWakeupRx         LDrvSUart_u1gEnableWakeupRx_1
#define LDrvSUart_vogSetNonResp             LDrvSUart_vogSetNonResp_1
#define LDrvSUart_vogSendResponse           LDrvSUart_vogSendResponse_1
#define LDrvSUart_vogRecvResponse           LDrvSUart_vogRecvResponse_1
#define LDrvSUart_vogSetBuf                 LDrvSUart_vogSetBuf_1
#define LDrvSUart_vogGetBuf                 LDrvSUart_vogGetBuf_1
#define LDrvSUart_vogClearStatus            LDrvSUart_vogClearStatus_1
#define LDrvSUart_u1gIsRx1stRespData        LDrvSUart_u1gIsRx1stRespData_1
#define LDrvSUart_u4gGetRespTimeMaxMs       LDrvSUart_u4gGetRespTimeMaxMs_1
#define LDrvSUart_vogVecTx                  LDrvSUart_vogVecTx_1
#define LDrvSUart_vogVecRx                  LDrvSUart_vogVecRx_1
#define LDrvSUart_vogVecStatus              LDrvSUart_vogVecStatus_1
#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
/* Error Definition */
#define LDRVSUART_u1gERROR_TRTIMEOUT        ((u1)OK + (u1)1)    /* RLIN3 state transition timed out  */

/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */
void LDrvSUart_vogInitAll( void );              /* Initialize UART driver                 */
void LDrvSUart_vogRefreshReg( void );           /* Refresh the UART driver                */
void LDrvSUart_vogPowerOn( void );              /* Turn ON UART clock                     */
void LDrvSUart_vogPowerOff( void );             /* Turn OFF UART clock                    */
u1   LDrvSUart_u1gDisable( void );              /* Shift to the reset                     */
u1   LDrvSUart_u1gEnableNormal( void );         /* Shift to normal LIN communication mode */
u1   LDrvSUart_u1gEnableWakeupTx( void );       /* Shift to wake up sending               */
void LDrvSUart_vogSetNonResp( void );           /* Set no response                        */
void LDrvSUart_vogSendResponse( u1, u1 );       /* Start sending response                 */
void LDrvSUart_vogRecvResponse( u1, u1 );       /* Start receiving response               */
void LDrvSUart_vogSetBuf( const u1*, u1 );      /* Write message buffer                   */
void LDrvSUart_vogGetBuf( u1*, u1 );            /* Read message buffer                    */
void LDrvSUart_vogClearStatus( void );          /* Clear status                           */
u1   LDrvSUart_u1gIsRx1stRespData( void );      /* Check if 1st resp data Rx.             */
u4   LDrvSUart_u4gGetRespTimeMaxMs( void );     /* Return max response time.              */
INTERRUPT void LDrvSUart_vogVecTx( void );      /* Transmission completion handler        */
INTERRUPT void LDrvSUart_vogVecRx( void );      /* Reception completion handler           */
INTERRUPT void LDrvSUart_vogVecStatus( void );  /* Error detection handler                */

#if (CONFSLIN_OPT_u1gBUSWKUP_CFG == CONFSLIN_OPT_u1gBUSWKUP_WIDTH)
u1   LDrvSUart_u1gEnableWakeupRx( void );       /* Shift to wake up reception */
#endif

/* -------------------------------------------------------------------- */
/* Public constants                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public variables                                                     */
/* -------------------------------------------------------------------- */

#endif /* H_DRVSUART */

/* End of files */
