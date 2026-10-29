/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2025 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: lcoresmes.c                                               */
/* Version:   0.05                                                      */
/* Date:      2025.08.04                                                */
/* Author:    A.Kondo, M.hirano                                          */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2015.02.02  Corresponded to RISRA-C                         */
/*   V0.03: 2021.05.31  IAR Compiler support                            */
/*   V0.04: 2024.08.30  Modified for RL78/F23,F24.                      */
/*   V0.05: 2025.08.04  Modified signal update macro for event.         */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Includes before pragma declarations                                  */
/* -------------------------------------------------------------------- */
  #ifdef __CCRL__
#include "../dev/section.h"
  #endif /* __CCRL__ */

/* --------------------------------------------------------------------- */
/* The addition of LIN channel number in function name and variable name */
/* --------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define LCoreSMes_u1sSearchAndSetCur        LCoreSMes_u1sSearchAndSetCur_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define LCoreSMes_u1sSearchAndSetCur        LCoreSMes_u1sSearchAndSetCur_1
#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../com/common.h"
#include "../conf/confslin.h"
#include "../drv/drvscom.h"
#include "../drv/drvsuart.h"
#include "./lcorescom.h"
#include "./lcoresmes.h"
#include "./lcoresresp.h"   /* Only to use a response direction value   */
#include "./lcoresstat.h"
#include "../dev/device.h"

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */
#ifdef __LIN_SIGNAL_32__
typedef u4  LCoreSMes_tpSigNum;
#else
typedef u2  LCoreSMes_tpSigNum;
#endif

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
/* Macro function to assimilate the 16 and 32 signal version */
#ifdef __LIN_SIGNAL_32__
#define LCORESMES_tpsGET_SIGNUM( u1tSig )        (LCoreSMes_tpSigNum)(ConfSLin_stgSIGNAL[u1tSig].u2SigNum)
#define LCORESMES_u1sGET_SIGMESNO( u1tSig )      (u1)(LCORESMES_tpsGET_SIGNUM(u1tSig) >> 8)
#define LCORESMES_u1sGET_SIGFLAG( tptSigNum )    (u1)((u1)( ConfSLin_u4gMesSig[(tptSigNum) >> 8] >> ((u2)0x001FU & (tptSigNum))) & (u1)0x01U)
#define LCORESMES_vosCLR_SIGFLAG( tptSigNum )    { ConfSLin_u4gMesSig[(tptSigNum) >> 8]    &= ~((u4)0x00000001U << ((u2)0x001FU & (tptSigNum))); }
#define LCORESMES_vosSET_SIGFLAG( tptSigNum )    { ConfSLin_u4gMesSig[(tptSigNum) >> 8]    |=  ((u4)0x00000001U << ((u2)0x001FU & (tptSigNum))); }
#define LCORESMES_vosSET_SIGSUBFLAG( tptSigNum ) { ConfSLin_u4gMesSubSig[(tptSigNum) >> 8] |=  ((u4)0x00000001U << ((u2)0x001FU & (tptSigNum))); }
#define LCORESMES_vosSET_MESFLAG( u1tMesNo )     { ConfSLin_u4gMesSig[u1tMesNo]          = (u4)0xFFFFFFFFU; }
#define LCORESMES_vosCLR_MESFLAG( u1tMesNo )     { ConfSLin_u4gMesSig[u1tMesNo]          = (u4)0x00000000U; }
#define LCORESMES_vosCLR_MESSUBFLAG( u1tMesNo )  { ConfSLin_u4gMesSubSig[u1tMesNo]       = (u4)0x00000000U; }
#define LCORESMES_u1sISSET_MESFLAG( u1tMesNo )   (u1)((ConfSLin_u4gMesSig[u1tMesNo])    != (u4)0x00000000U)
#define LCORESMES_vosCOPY_MESFLAG( u1tMesNo )    { ConfSLin_u4gMesSig[u1tMesNo]          = ConfSLin_u4gMesSubSig[u1tMesNo]; }
#else
#define LCORESMES_tpsGET_SIGNUM( u1tSig )        (LCoreSMes_tpSigNum)(ConfSLin_stgSIGNAL[u1tSig].u1SigNum)
#define LCORESMES_u1sGET_SIGMESNO( u1tSig )      (u1)(LCORESMES_tpsGET_SIGNUM(u1tSig) >> 4)
#define LCORESMES_u1sGET_SIGFLAG( tptSigNum )    (u1)((u1)( ConfSLin_u2gMesSig[(tptSigNum) >> 4] >> ((u1)0x0FU & (tptSigNum))) & (u1)0x01U)
#define LCORESMES_vosCLR_SIGFLAG( tptSigNum )    { ConfSLin_u2gMesSig[(tptSigNum) >> 4]    &= ~((u2)0x0001U << ((u1)0x0FU & (tptSigNum))); }
#define LCORESMES_vosSET_SIGFLAG( tptSigNum )    { ConfSLin_u2gMesSig[(tptSigNum) >> 4]    |=  ((u2)0x0001U << ((u1)0x0FU & (tptSigNum))); }
#define LCORESMES_vosSET_SIGSUBFLAG( tptSigNum ) { ConfSLin_u2gMesSubSig[(tptSigNum) >> 4] |=  ((u2)0x0001U << ((u1)0x0FU & (tptSigNum))); }
#define LCORESMES_vosSET_MESFLAG( u1tMesNo )     { ConfSLin_u2gMesSig[u1tMesNo]          = (u2)0xFFFFU; }
#define LCORESMES_vosCLR_MESFLAG( u1tMesNo )     { ConfSLin_u2gMesSig[u1tMesNo]          = (u2)0x0000U; }
#define LCORESMES_vosCLR_MESSUBFLAG( u1tMesNo )  { ConfSLin_u2gMesSubSig[u1tMesNo]       = (u2)0x0000U; }
#define LCORESMES_u1sISSET_MESFLAG( u1tMesNo )   (u1)((ConfSLin_u2gMesSig[u1tMesNo])    != (u2)0x0000U)
#define LCORESMES_vosCOPY_MESFLAG( u1tMesNo )    { ConfSLin_u2gMesSig[u1tMesNo]          = ConfSLin_u2gMesSubSig[u1tMesNo]; }
#endif


/* Response type */
#define LCORESMES_u1sNOFRAME                    (0x00U)
#define LCORESMES_u1sNORMALFRAME                (0x01U)
#define LCORESMES_u1sEVENTTRIGGERFRAME          (0x02U)

/* -------------------------------------------------------------------- */
/* Function prototype declaration                                       */
/* -------------------------------------------------------------------- */
static u1   LCoreSMes_u1sSearchAndSetCur( u1 );


/* -------------------------------------------------------------------- */
/* Constant declaration                                                 */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Variable declaration                                                 */
/* -------------------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_variable_attributes = @ "LSDATA"
#endif
static const ConfSLin_stMesInfo *LCoreSMes_pstsCurMesInfo; /* Pointer to the message information of the current communication */
static u1 LCoreSMes_u1sCurMesType;                         /* Message type of the current communication                       */
static u1 LCoreSMes_u1sCurMesNo;                           /* Logical message number of the current                           */
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
/* Description : Initialize the message module                          */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LCoreSMes_vogInitAll( void )
{
    u1 i;


    LCoreSMes_pstsCurMesInfo = (const ConfSLin_stMesInfo*)0;
    LCoreSMes_u1sCurMesType  = (u1)LCORESMES_u1sNOFRAME;
    LCoreSMes_u1sCurMesNo    = (u1)0x00U;       /* Array index of unconditional frame */


    for( i = (u1)0; i < ConfSLin_u1gMESNUM; i++ ) {

        ConfSLin_u1gMesPid[i] = ConfSLin_stgMESINFO[i].u1InitPid;
                                                /* PID initial value setting */
        LCORESMES_vosCLR_MESFLAG( i );
        LCORESMES_vosCLR_MESSUBFLAG( i );
        
        LDrvSCom_vogCopyBuf( &(ConfSLin_stgMesBuf[i].u1DataBuf[0]), 
                             &(ConfSLin_stgMESINFO[i].u1InitData[0]), (u1)8 );
                                                /* Two temporary value is not initialized. */
                                                /* (Data is always set before used)        */
    }


    for( i =(u1)0; i < ConfSLin_u1gEVENTNUM; i++ ) {
        ConfSLin_u1gEventPid[i]= ConfSLin_stgEVENTINFO[i].u1InitEventPid; 
                                                /* Event PID initial value setting */
    }

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Get the signal update flag                             */
/* Parameters  : u1tSigName = signal name                               */
/* Return value: Update status                                          */
/*               0 = No update                                          */
/*               1 = Update                                             */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LCoreSMes_u1gReadSigFlag( u1 u1tSigName )
{
    u1 u1tRet;
    LCoreSMes_tpSigNum tptSigNum;


    /* Get a signal update flag value */
    tptSigNum = LCORESMES_tpsGET_SIGNUM( u1tSigName );
    u1tRet    = LCORESMES_u1sGET_SIGFLAG( tptSigNum );


    return u1tRet;
}



/* -------------------------------------------------------------------- */
/* Description : Clear signal update flag                               */
/* Parameters  : u1tSigName = signal name                               */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LCoreSMes_vogClrSigFlag( u1 u1tSigName )
{
    LCoreSMes_tpSigNum tptSigNum;
    

    /* Clear signal update flag */
    tptSigNum = LCORESMES_tpsGET_SIGNUM( u1tSigName );
    LCORESMES_vosCLR_SIGFLAG( tptSigNum );


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Set signal update flag                                 */
/* Parameters  : u1tSigName : signal name                               */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LCoreSMes_vogSetSigFlag( u1 u1tSigName )
{
    LCoreSMes_tpSigNum tptSigNum;


    /* Set signal update flag(including sub) */
    tptSigNum = LCORESMES_tpsGET_SIGNUM( u1tSigName );
    LCORESMES_vosSET_SIGFLAG( tptSigNum );
    LCORESMES_vosSET_SIGSUBFLAG( tptSigNum );
    

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Return the address of the message buffer               */
/*               the signal belongs.                                    */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1*
LCoreSMes_pu1gGetSigMesAddr( u1 u1tSigName )
{
    u1 u1tSigMesNo;
    
    u1tSigMesNo = LCORESMES_u1sGET_SIGMESNO( u1tSigName );

    return &(ConfSLin_stgMesBuf[u1tSigMesNo].u1DataBuf[0]);

}


/* -------------------------------------------------------------------- */
/* Description : Return the PID of the frame signal belongs             */
/* Parameters  : u1tSig : signal name                                   */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LCoreSMes_u1gGetSigPid( u1 u1tSigName )
{
    u1 u1tSigMesNo;
    
    u1tSigMesNo = LCORESMES_u1sGET_SIGMESNO( u1tSigName );

    return ConfSLin_u1gMesPid[u1tSigMesNo];
}


/* -------------------------------------------------------------------- */
/* Description : Start sending and receiving the response.              */
/* Parameters  : u1tPid = PID                                           */
/* Return value: LCORESRESP_u1gIGNORE  = Send and receive unnecessary   */
/*             LCORESRESP_u1gRECEIVE = Complete the reception process   */
/*             LCORESRESP_u1gSEND   = Complete the transmission process */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LCoreSMes_u1gStartResponse( u1 u1tPid )
{
    u1 u1tRespDir;
    u1 u1tRespSendBuf[8];
    const ConfSLin_stSignal *psttRespErrSignal;


    /*                                                                          */
    /* Search the frame that should be transmitted or received                  */
    /* (If the frame was to be transmitted or received, the following variables */
    /*  are set at the same time)                                               */
    /*  LCoreSMes_u1sCurMesType                                                 */
    /*  LCoreSMes_u1sCurMesNo                                                   */
    /*  LCoreSMes_pstsCurMesInfo                                                */
    /*                                                                          */
    /*  - Do not touch other than transmission and reception                    */
    u1tRespDir = LCoreSMes_u1sSearchAndSetCur( u1tPid );
    
    if( u1tRespDir == (u1)LCORESRESP_u1gRECEIVE ) {
        
        /************************************************************/
        /* Event trigger reception or unconditional frame reception */
        
        LDrvSUart_vogRecvResponse( LCoreSMes_pstsCurMesInfo -> u1ByteSize, 
                                   LCoreSMes_pstsCurMesInfo -> u1SumType );

    }
    else if( u1tRespDir == (u1)LCORESRESP_u1gSEND ) {

        /**************************************************************************/
        /* Event trigger transmission(update) or unconditional frame transmission */

        /* (If the trigger event, unconditional frame No. associated with it is stored in LCoreSMes_u1sCurMesNo) */

        /* For the event trigger and Response Error, copy the data to a local variable once */
        LDrvSCom_vogCopyBuf( &(u1tRespSendBuf[0]), 
                             &(ConfSLin_stgMesBuf[LCoreSMes_u1sCurMesNo].u1DataBuf[0]),
                             LCoreSMes_pstsCurMesInfo -> u1ByteSize );


        /* If unconditional frame containing the Response Error, rewrite the only Response Error bit part */
        /* (Limit at configurator that Response Error is not to be set in the event trigger frame.)       */
        if( u1tPid == LCoreSMes_u1gGetSigPid( ConfSLin_stgNODE.u1ErrRes ) ) {
            
            psttRespErrSignal = &ConfSLin_stgSIGNAL[ConfSLin_stgNODE.u1ErrRes];
            u1tRespSendBuf[psttRespErrSignal -> u1OffsetByte] &= (u1)~((u1)0x01U << (psttRespErrSignal -> u1OffsetBit));
            u1tRespSendBuf[psttRespErrSignal -> u1OffsetByte] |= (u1)(LCoreSStat_u1gGetRespErr() << (psttRespErrSignal -> u1OffsetBit));
        }


        /* For event-triggered transmission, set the PID to the top data */
        if( LCoreSMes_u1sCurMesType == (u1)LCORESMES_u1sEVENTTRIGGERFRAME ) {
            u1tRespSendBuf[0] = ConfSLin_u1gMesPid[LCoreSMes_u1sCurMesNo];
        }


        /* Run the driver layer transmission processing */
        LDrvSUart_vogSetBuf( &(u1tRespSendBuf[0]), LCoreSMes_pstsCurMesInfo -> u1ByteSize );
        LDrvSUart_vogSendResponse( LCoreSMes_pstsCurMesInfo -> u1ByteSize, LCoreSMes_pstsCurMesInfo -> u1SumType );


        /* Clear sub-message flags(all signal) in currently communication     */
        /* (Update by API during transmission can be checked in the sub-flag) */
        LCORESMES_vosCLR_MESSUBFLAG( LCoreSMes_u1sCurMesNo );

    }
    else {
        
        /* No processing */
    }
   

    return u1tRespDir;
}


/* -------------------------------------------------------------------- */
/* Description : Process of the response received success               */
/* Parameters  : u1tPid = PID                                           */
/* Return value: TRUE  = There is a process of message module           */
/*               FALSE = There is no process of message module          */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LCoreSMes_u1gFinishRecResp( u1 u1tPid )
{
    u1 u1tRecvBuff;
    u1 u1tGetDataFlg;


    /* Step for compiler warning(the argument is not used in this version) */
    (void)u1tPid;

    /* Flag that indicates whether to buffer the received data */
    u1tGetDataFlg = (u1)FALSE;

    
    /* In case of event trigger frame, ignore the frame if the PID of the unconditional frame */
    /* for which they are received in the first byte is not set                               */
    if( LCoreSMes_u1gIsCurEvent() == (u1)TRUE ) {

        LDrvSUart_vogGetBuf( &u1tRecvBuff, 1 );
        if( u1tRecvBuff == ConfSLin_u1gMesPid[LCoreSMes_u1sCurMesNo] ) {
            u1tGetDataFlg = (u1)TRUE;
        }
    }
    else {
        
        /* In case of other than event trigger(unconditional frame), get the buffer unconditionally */
        u1tGetDataFlg = (u1)TRUE;
    }
    
    
    if( u1tGetDataFlg == (u1)TRUE ) {
    
        /* Copy the data in the message buffer from the data buffer of RLIN3 */
        LDrvSUart_vogGetBuf( &(ConfSLin_stgMesBuf[LCoreSMes_u1sCurMesNo].u1DataBuf[0]),
                             LCoreSMes_pstsCurMesInfo -> u1ByteSize );

        /* Set the message update flag(All signal) in currently communication */
        LCORESMES_vosSET_MESFLAG( LCoreSMes_u1sCurMesNo );
    }


    /* Variable initialization */
    LCoreSMes_pstsCurMesInfo = (const ConfSLin_stMesInfo*)0;
    LCoreSMes_u1sCurMesType  = (u1)LCORESMES_u1sNOFRAME;
    LCoreSMes_u1sCurMesNo    = (u1)0x00U;


    return (u1)TRUE;
}


/* -------------------------------------------------------------------- */
/* Description : Process of response transmission success               */
/* Parameters  : u1tPid = PID                                           */
/* Return value: TRUE  = There is a process of message module           */
/*               FALSE = There is no process of message module          */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
u1
LCoreSMes_u1gFinishSendResp( u1 u1tPid )
{

    /* Step for compiler warning(the argument is not used in this version) */
    (void)u1tPid;


    /* Copy the update flag to the sub-update flag */
    /* (In order to recognize the signals that are updated by API during response transmission) */
    LCORESMES_vosCOPY_MESFLAG( LCoreSMes_u1sCurMesNo );
    

    /* Variable initialization */
    LCoreSMes_pstsCurMesInfo = (const ConfSLin_stMesInfo*)0;
    LCoreSMes_u1sCurMesType  = (u1)LCORESMES_u1sNOFRAME;
    LCoreSMes_u1sCurMesNo    = (u1)0x00U;

    
    return (u1)TRUE;
}


/* --------------------------------------------------------------------- */
/* Description : Return whether the current frame of communicating is    */
/*               event trigger.                                          */
/* Parameters  : Nothing                                                 */
/* Return value: TRUE  = Event trigger frame                             */
/*               FALSE = Not event trigger frame                         */
/* Misc.       : Nothing                                                 */
/* --------------------------------------------------------------------- */
u1
LCoreSMes_u1gIsCurEvent( void )
{
    u1 u1tResult;


    if( LCoreSMes_u1sCurMesType == (u1)LCORESMES_u1sEVENTTRIGGERFRAME ) {
        u1tResult = (u1)TRUE;
    }
    else {
        u1tResult = (u1)FALSE;
    }
    
    
    return u1tResult;
}


/* -------------------------------------------------------------------- */
/* Internal functions                                                   */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Description : Check whether the PID is related to itself.            */
/*               At the same time, set the following global variables.  */
/*               LCoreSMes_u1sCurMesType                                */
/*               LCoreSMes_u1sCurMesNo                                  */
/*               LCoreSMes_pstsCurMesInfo                               */
/* Parameters  : u1tPid = PID                                           */
/* Return value: LCORESRESP_u1gIGNORE = Send and receive unnecessary    */
/*             LCORESRESP_u1gRECEIVE = Complete the reception process   */
/*             LCORESRESP_u1gSEND   = Complete the transmission process */
/* Misc.       : processing time depends on the unconditional number of */
/*               frames and Event trigger number.                       */
/* -------------------------------------------------------------------- */
static u1
LCoreSMes_u1sSearchAndSetCur( u1 u1tPid )
{
    u1 u1tMesNo;
    u1 u1tAssocMesNo;
    u1 u1tRespDir;
    const ConfSLin_stMesInfo *psttAssocMesInfo;


    u1tRespDir = (u1)LCORESRESP_u1gIGNORE;

    /* Search unconditional frame */
    for( u1tMesNo = 0;u1tMesNo < ConfSLin_u1gMESNUM;u1tMesNo++ ) {
        if( ConfSLin_u1gMesPid[u1tMesNo] == u1tPid ) {
            
            /* Match -> Set the current transmission and reception frame information */
            LCoreSMes_u1sCurMesType  = (u1)LCORESMES_u1sNORMALFRAME;
            LCoreSMes_u1sCurMesNo    = u1tMesNo;
            LCoreSMes_pstsCurMesInfo = &ConfSLin_stgMESINFO[LCoreSMes_u1sCurMesNo];
            
            if( (LCoreSMes_pstsCurMesInfo -> u1Dir) == (u1)CONFSLIN_u1gPUBLISH ) {
                u1tRespDir = (u1)LCORESRESP_u1gSEND;
            }
            else {
                u1tRespDir = (u1)LCORESRESP_u1gRECEIVE;
            }
            
            break; /* Break if the PID is found */
        }
    }


    if( u1tRespDir == (u1)LCORESRESP_u1gIGNORE ) {

        /* Search for event trigger frame because unconditionally frame was not found. */
        /* (Message No. of unconditional frame associated with the event is set to the current) */
        for( u1tMesNo = 0;u1tMesNo < ConfSLin_u1gEVENTNUM;u1tMesNo++ ) {
            if( ConfSLin_u1gEventPid[u1tMesNo] == u1tPid ) {
                
                /* Unconditional frame number and information associated with the event */
                u1tAssocMesNo    =  ConfSLin_stgEVENTINFO[u1tMesNo].u1MesNum;
                psttAssocMesInfo = &ConfSLin_stgMESINFO[u1tAssocMesNo];
                
                if( (psttAssocMesInfo -> u1Dir) == (u1)CONFSLIN_u1gPUBLISH ) {
                    
                    /* When sending the event, send only when there is a signal update                  */
                    /* (If there is no signal update, it does not break and check the event of next frame) */
                    if( LCORESMES_u1sISSET_MESFLAG( u1tAssocMesNo ) == (u1)TRUE ) {

                        LCoreSMes_u1sCurMesType  = (u1)LCORESMES_u1sEVENTTRIGGERFRAME;
                        LCoreSMes_u1sCurMesNo    = u1tAssocMesNo;
                        LCoreSMes_pstsCurMesInfo = psttAssocMesInfo;

                        u1tRespDir = (u1)LCORESRESP_u1gSEND;
                        break;
                    }
                }
                else {

                    /* When receiving the event, whether start receiving unconditionally and capture is determined when reception is completed */
                    LCoreSMes_u1sCurMesType  = (u1)LCORESMES_u1sEVENTTRIGGERFRAME;
                    LCoreSMes_u1sCurMesNo    = u1tAssocMesNo;
                    LCoreSMes_pstsCurMesInfo = psttAssocMesInfo;

                    u1tRespDir = (u1)LCORESRESP_u1gRECEIVE;
                    break;
                }
            }
        }
    }


    return u1tRespDir;
}

#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif

/* End of files */
