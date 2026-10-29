/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: lcoresresp.h                                              */
/* Version:   0.04                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.08.05  CH adding macro for SetErrorStatus was added.   */
/*   V0.03: 2021.05.31  RL78/F2x support                                */
/*   V0.04: 2024.08.30  Modified for RL78/F23,F24.                      */
/* -------------------------------------------------------------------- */
#ifndef H_LCORESRESP
#define H_LCORESRESP

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
/* Abstraction channel macro for symbol */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define LCoreSResp_vogInitAll               LCoreSResp_vogInitAll_0
#define LCoreSResp_u1gStartResponse         LCoreSResp_u1gStartResponse_0
#define LCoreSResp_vogFinishRecResp         LCoreSResp_vogFinishRecResp_0
#define LCoreSResp_vogFinishSendResp        LCoreSResp_vogFinishSendResp_0
#define LCoreSResp_vogSetErrorStatus        LCoreSResp_vogSetErrorStatus_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define LCoreSResp_vogInitAll               LCoreSResp_vogInitAll_1
#define LCoreSResp_u1gStartResponse         LCoreSResp_u1gStartResponse_1
#define LCoreSResp_vogFinishRecResp         LCoreSResp_vogFinishRecResp_1
#define LCoreSResp_vogFinishSendResp        LCoreSResp_vogFinishSendResp_1
#define LCoreSResp_vogSetErrorStatus        LCoreSResp_vogSetErrorStatus_1
#endif /* __LIN_CH1__ */

/* For the result of response processing */
#define LCORESRESP_u1gSEND      ((u1)0x01U) /* Response direction:Transmission */
#define LCORESRESP_u1gRECEIVE   ((u1)0x02U) /* Response direction:Reception    */
#define LCORESRESP_u1gIGNORE    ((u1)0x03U) /* Response direction:Ignore       */

/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */
void LCoreSResp_vogInitAll( void );
u1   LCoreSResp_u1gStartResponse( u1 );     /* Processing of starting response             */
void LCoreSResp_vogFinishRecResp( void );   /* Processing of response reception success    */
void LCoreSResp_vogFinishSendResp( void );  /* Processing of response transmission success */
void LCoreSResp_vogSetErrorStatus( void );  /* Set the response error                      */


/* -------------------------------------------------------------------- */
/* Public constants                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public variables                                                     */
/* -------------------------------------------------------------------- */

#endif /* H_LCORESRESP */

/* End of files */
