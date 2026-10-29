/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: lcoresmes.h                                               */
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
#ifndef H_LCORESMES
#define H_LCORESMES

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
#define LCORESMES_u1gUPDATE     ((u1)0x01U)         /* Update           */
#define LCORESMES_u1gNONUPDATE  ((u1)0x00U)         /* No update        */

/* --------------------------------------------------------------------- */
/* The addition of LIN channel number in function name and variable name */
/* --------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define LCoreSMes_vogInitAll                LCoreSMes_vogInitAll_0
#define LCoreSMes_u1gGetSigPid              LCoreSMes_u1gGetSigPid_0
#define LCoreSMes_vogClrSigFlag             LCoreSMes_vogClrSigFlag_0
#define LCoreSMes_vogSetSigFlag             LCoreSMes_vogSetSigFlag_0
#define LCoreSMes_u1gReadSigFlag            LCoreSMes_u1gReadSigFlag_0
#define LCoreSMes_pu1gGetSigMesAddr         LCoreSMes_pu1gGetSigMesAddr_0
#define LCoreSMes_u1gStartResponse          LCoreSMes_u1gStartResponse_0
#define LCoreSMes_u1gFinishRecResp          LCoreSMes_u1gFinishRecResp_0
#define LCoreSMes_u1gFinishSendResp         LCoreSMes_u1gFinishSendResp_0
#define LCoreSMes_u1gIsCurEvent             LCoreSMes_u1gIsCurEvent_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define LCoreSMes_vogInitAll                LCoreSMes_vogInitAll_1
#define LCoreSMes_u1gGetSigPid              LCoreSMes_u1gGetSigPid_1
#define LCoreSMes_vogClrSigFlag             LCoreSMes_vogClrSigFlag_1
#define LCoreSMes_vogSetSigFlag             LCoreSMes_vogSetSigFlag_1
#define LCoreSMes_u1gReadSigFlag            LCoreSMes_u1gReadSigFlag_1
#define LCoreSMes_pu1gGetSigMesAddr         LCoreSMes_pu1gGetSigMesAddr_1
#define LCoreSMes_u1gStartResponse          LCoreSMes_u1gStartResponse_1
#define LCoreSMes_u1gFinishRecResp          LCoreSMes_u1gFinishRecResp_1
#define LCoreSMes_u1gFinishSendResp         LCoreSMes_u1gFinishSendResp_1
#define LCoreSMes_u1gIsCurEvent             LCoreSMes_u1gIsCurEvent_1
#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */
void LCoreSMes_vogInitAll( void );      /* Module initialization                                              */
u1   LCoreSMes_u1gGetSigPid( u1 );      /* Return the PID of the frame signal belongs                         */
void LCoreSMes_vogClrSigFlag( u1 );     /* Clear signal update flag                                           */
void LCoreSMes_vogSetSigFlag( u1 );     /* Set signal update flag                                             */
u1   LCoreSMes_u1gReadSigFlag( u1 );    /* Get signal update flag                                             */
u1*  LCoreSMes_pu1gGetSigMesAddr( u1 ); /* Return the address of the message buffer the signal belongs        */
u1   LCoreSMes_u1gStartResponse( u1 );  /* Processing of starting response                                    */
u1   LCoreSMes_u1gFinishRecResp( u1 );  /* Processing of response received success                            */
u1   LCoreSMes_u1gFinishSendResp( u1 ); /* Processing of response transmission success                        */
u1   LCoreSMes_u1gIsCurEvent( void );   /* Return whether the current frame of communicating is event trigger */

/* -------------------------------------------------------------------- */
/* Public constants                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public variables                                                     */
/* -------------------------------------------------------------------- */

#endif /* H_LCORESMES */

/* End of files */
