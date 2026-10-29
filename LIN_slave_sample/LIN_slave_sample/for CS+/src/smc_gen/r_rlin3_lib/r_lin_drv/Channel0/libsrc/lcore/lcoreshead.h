/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: lcoreshead.h                                              */
/* Version:   0.03                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2021.05.31  RL78/F2x support                                */
/*   V0.03: 2024.08.30  Modified for RL78/F23,F24.                      */
/* -------------------------------------------------------------------- */
#ifndef H_LCORESHEAD
#define H_LCORESHEAD

/* --------------------------------------------------------------------- */
/* The addition of LIN channel number in function name and variable name */
/* --------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define LCoreSHead_vogInitAll               LCoreSHead_vogInitAll_0
#define LCoreSHead_u1gWaitHdrFromDis        LCoreSHead_u1gWaitHdrFromDis_0
#define LCoreSHead_vogWaitHdrFromErr        LCoreSHead_vogWaitHdrFromErr_0
#define LCoreSHead_vogWaitHeader            LCoreSHead_vogWaitHeader_0
#define LCoreSHead_vogIgnoreResp            LCoreSHead_vogIgnoreResp_0
#define LCoreSHead_vogSetResponse           LCoreSHead_vogSetResponse_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define LCoreSHead_vogInitAll               LCoreSHead_vogInitAll_1
#define LCoreSHead_u1gWaitHdrFromDis        LCoreSHead_u1gWaitHdrFromDis_1
#define LCoreSHead_vogWaitHdrFromErr        LCoreSHead_vogWaitHdrFromErr_1
#define LCoreSHead_vogWaitHeader            LCoreSHead_vogWaitHeader_1
#define LCoreSHead_vogIgnoreResp            LCoreSHead_vogIgnoreResp_1
#define LCoreSHead_vogSetResponse           LCoreSHead_vogSetResponse_1
#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
#define LCORESHEAD_u1gERROR_TRTIMEOUT       ((u1)OK + (u1)1)

/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */
void LCoreSHead_vogInitAll( void );         /* Initialize the header module                                     */
u1   LCoreSHead_u1gWaitHdrFromDis( void );  /* Start waiting header from the state Disable UART module          */
void LCoreSHead_vogWaitHdrFromErr( u2 );    /* Waiting header from an error condition                           */
void LCoreSHead_vogWaitHeader( void );      /* Start waiting header without State transition of the UART module */
void LCoreSHead_vogIgnoreResp( void );      /* Set no response                                                  */
void LCoreSHead_vogSetResponse( u1 );       /* Set the response operation                                       */

/* -------------------------------------------------------------------- */
/* Public constants                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public variables                                                     */
/* -------------------------------------------------------------------- */

#endif /* H_LCORESHEAD */

/* End of files */
