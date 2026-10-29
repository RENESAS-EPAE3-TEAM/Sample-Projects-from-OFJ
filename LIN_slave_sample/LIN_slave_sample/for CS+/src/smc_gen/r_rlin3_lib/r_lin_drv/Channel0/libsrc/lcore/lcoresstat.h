/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: lcoresstat.h                                              */
/* Version:   0.03                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2021.05.31  LIN2.1, RL78/F2x support                        */
/*   V0.03: 2024.08.30  Modified for RL78/F23,F24.                      */
/* -------------------------------------------------------------------- */
#ifndef H_LCORESSTAT
#define H_LCORESSTAT

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
/* Bits in the return value of l_ifc_read_status */
#define LCORESSTAT_u1gSTAT_BUSACTIVITY  (0x10U)     /* Bus activity               */

/* The definition of the error code */
#define LCORESSTAT_u2gERRCD_NONE        (0x0000U)   /* Normal status              */
#define LCORESSTAT_u2gERRCD_TRMUNKNOWN  (0x0100U)   /* UART UnknownErr(TRM)       */
#define LCORESSTAT_u2gERRCD_RVCUNKNOWN  (0x0200U)   /* UART UnknownErr(RVC)       */
#define LCORESSTAT_u2gERRCD_STAUNKNOWN  (0x0300U)   /* UART UnknownErr(STA)       */
#define LCORESSTAT_u2gERRCD_TRTONORM    (0x0400U)   /* TO for shift to UART Norm  */
#define LCORESSTAT_u2gERRCD_TRTOWKUPTX  (0x0500U)   /* TO for shift to UART Wkup  */
#define LCORESSTAT_u2gERRCD_TRTOWKUPRX  (0x0600U)   /* TO for shift to UART Wkup  */
#define LCORESSTAT_u2gERRCD_TRTORESET   (0x0700U)   /* TO for shift to UART Reset */
#define LCORESSTAT_u2gERRCD_RESPTIMEOUT (0x0800U)   /* Response time-out          */
#define LCORESSTAT_u2gERRCD_SLPTIMEOUT  (0x0900U)   /* Time-out during Sleep      */
#define LCORESSTAT_u2gERRCD_TMUNKNOWN   (0x0A00U)   /* Timer UnknownErr           */
#define LCORESSTAT_u2gERRCD_FAILSAFE    (0xFF00U)   /* For fail-safe              */

/* Channel abstraction macro for symbol */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define LCoreSStat_vogInitAll           LCoreSStat_vogInitAll_0
#define LCoreSStat_vogSetSuccess        LCoreSStat_vogSetSuccess_0
#define LCoreSStat_u2gGetStatus         LCoreSStat_u2gGetStatus_0
#define LCoreSStat_u2gGetErrorCode      LCoreSStat_u2gGetErrorCode_0
#define LCoreSStat_vogSetErrorCode      LCoreSStat_vogSetErrorCode_0
#define LCoreSStat_vogSetSleepFlag      LCoreSStat_vogSetSleepFlag_0
#define LCoreSStat_u1gGetErrorCount     LCoreSStat_u1gGetErrorCount_0
#define LCoreSStat_vogSetErrorStatus    LCoreSStat_vogSetErrorStatus_0
#define LCoreSStat_vogSetStatusBit      LCoreSStat_vogSetStatusBit_0
#define LCoreSStat_u1gGetRespErr        LCoreSStat_u1gGetRespErr_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define LCoreSStat_vogInitAll           LCoreSStat_vogInitAll_1
#define LCoreSStat_vogSetSuccess        LCoreSStat_vogSetSuccess_1
#define LCoreSStat_u2gGetStatus         LCoreSStat_u2gGetStatus_1
#define LCoreSStat_u2gGetErrorCode      LCoreSStat_u2gGetErrorCode_1
#define LCoreSStat_vogSetErrorCode      LCoreSStat_vogSetErrorCode_1
#define LCoreSStat_vogSetSleepFlag      LCoreSStat_vogSetSleepFlag_1
#define LCoreSStat_u1gGetErrorCount     LCoreSStat_u1gGetErrorCount_1
#define LCoreSStat_vogSetErrorStatus    LCoreSStat_vogSetErrorStatus_1
#define LCoreSStat_vogSetStatusBit      LCoreSStat_vogSetStatusBit_1
#define LCoreSStat_u1gGetRespErr        LCoreSStat_u1gGetRespErr_1
#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */
void LCoreSStat_vogInitAll( void );       /* Initialize status module               */
void LCoreSStat_vogSetSuccess( u1 );      /* Set transmit or receive status success */
u2   LCoreSStat_u2gGetStatus( void );     /* Get LIN status                         */
u2   LCoreSStat_u2gGetErrorCode( void );  /* Get error code                         */
void LCoreSStat_vogSetErrorCode( u2 );    /* Set error code                         */
void LCoreSStat_vogSetSleepFlag( void );  /* Update goto sleep flag                 */
u1   LCoreSStat_u1gGetErrorCount( void ); /* Get error count                        */
void LCoreSStat_vogSetErrorStatus( u1 );  /* Set error status                       */
void LCoreSStat_vogSetStatusBit( u1 );    /* Specified bit set in LIN status        */
u1   LCoreSStat_u1gGetRespErr( void );    /* Get response_error                     */

/* -------------------------------------------------------------------- */
/* Public constants                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public variables                                                     */
/* -------------------------------------------------------------------- */

#endif /* H_LCORESSTAT */

/* End of files */
