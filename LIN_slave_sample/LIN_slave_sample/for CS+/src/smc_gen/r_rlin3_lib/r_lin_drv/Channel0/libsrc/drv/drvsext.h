/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: drvsext.h                                                 */
/* Version:   0.05                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.08.22  Macro function to add CH for vogRefreshReg was  */
/*                      added.                                          */
/*                      LDrvSExt_vogEnaNegEdgeNoInt was remaned.        */
/*                      LDrvSExt_vogEnablePosEdge was added.            */
/*                      LDrvSExt_vogEnableNegEdge was renamed.          */
/*                      The build timing of LDrvSExt_vogVecEdge was     */
/*                      changed.                                        */
/*   V0.03: 2015.02.02  Processing corresponding to the same            */
/*                      restriction was deleted by lifting              */
/*                      restrictions of FTS bit set timing of RLIN3.    */
/*                      Macro definition and function name of the       */
/*                      public function was corrected.                  */
/*                      - LDrvSExt_vogEnableEdgeNoInt                   */
/*                      - LDrvSExt_vogEnableEdge                        */
/*   V0.04: 2021.05.31  LIN2.1, RL78/F2x support                        */
/*   V0.05: 2024.08.30  Modified for RL78/F23,F24.                      */
/* -------------------------------------------------------------------- */
#ifndef H_DRVSEXT
#define H_DRVSEXT

/* -------------------------------------------------------------------- */
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../conf/confslin_opt.h"

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
/* LDrvMExt_u1gReadBusStatus return value */
#define LDRVSEXT_u1gBUSACTIVE_FALSE         (0)     /* Inactive                                   */
#define LDRVSEXT_u1gBUSACTIVE_TRUE          (1)     /* Active (bus level change or rising edge)   */
#define LDRVSEXT_u1gBUSACTIVE_FALL_EDGE     (2)     /* Active (rising edge)                       */
                                                    /* For judgment on wakeup transmission retry  */


#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define LDrvSExt_vogInitAll             LDrvSExt_vogInitAll_0
#define LDrvSExt_vogRefreshReg          LDrvSExt_vogRefreshReg_0
#define LDrvSExt_vogEnableEdge          LDrvSExt_vogEnableEdge_0
#define LDrvSExt_vogDisableEdge         LDrvSExt_vogDisableEdge_0
#define LDrvSExt_vogEnaBusActDtctNoInt LDrvSExt_vogEnaBusActDtctNoInt_0
#define LDrvSExt_u1gReadBusStatus       LDrvSExt_u1gReadBusStatus_0
#define LDrvSExt_vogVecEdge             LDrvSExt_vogVecEdge_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define LDrvSExt_vogInitAll             LDrvSExt_vogInitAll_1
#define LDrvSExt_vogRefreshReg          LDrvSExt_vogRefreshReg_1
#define LDrvSExt_vogEnableEdge          LDrvSExt_vogEnableEdge_1
#define LDrvSExt_vogDisableEdge         LDrvSExt_vogDisableEdge_1
#define LDrvSExt_vogEnaBusActDtctNoInt LDrvSExt_vogEnaBusActDtctNoInt_1
#define LDrvSExt_u1gReadBusStatus       LDrvSExt_u1gReadBusStatus_1
#define LDrvSExt_vogVecEdge             LDrvSExt_vogVecEdge_1
#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */
void LDrvSExt_vogInitAll( void );             /* Initialize the external interrupt driver                       */
void LDrvSExt_vogRefreshReg( void );          /* Register refresh                                               */
void LDrvSExt_vogDisableEdge( void );         /* Disable external interrupts                                    */
void LDrvSExt_vogEnaBusActDtctNoInt( void );  /* Enable bus active detection (interrupt disabled)               */
u1   LDrvSExt_u1gReadBusStatus( void );       /* Read the external interrupt request flag                       */

#if ( CONFSLIN_OPT_u1gBUSWKUP_CFG == CONFSLIN_OPT_u1gBUSWKUP_EDGE )
void LDrvSExt_vogEnableEdge( void );          /* Enable the falling edge                                        */
INTERRUPT void LDrvSExt_vogVecEdge( void );   /* External edge interrupt interface                              */
#endif

/* -------------------------------------------------------------------- */
/* Public constants                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public variables                                                     */
/* -------------------------------------------------------------------- */

#endif /* H_DRVSEXT */

/* End of files */
