/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: confslin.h                                                */
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
#ifndef H_CONFSLIN
#define H_CONFSLIN

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */
typedef struct                          /* Message information              */
{
    u1 u1Dir;                           /* 0=Publish, 1=Subscribe           */
    u1 u1ByteSize;                      /* Message size                     */
    u1 u1InitPid;                       /* Initial PID                      */
    u1 u1SumType;                       /* Checksum (0=classic, 1=enhanced) */
    u1 u1InitData[8];                   /* Initial data                     */

} ConfSLin_stMesInfo;


typedef struct                          /* Signal information               */
{
    u1 u1OffsetByte;                    /* Byte offset                      */
    u1 u1OffsetBit;                     /* Bit offset                       */
    u1 u1BitSize;                       /* Data size                        */

#ifdef __LIN_SIGNAL_32__
    u2 u2SigNum;                        /* Signal number                    */
#else
    u1 u1SigNum;                        /* Signal number                    */
#endif /* __LIN_SIGNAL_32__ */

} ConfSLin_stSignal;


typedef struct                          /* Node information                 */
{
    u1 u1InitNad;                       /* Initial NAD                      */
    u2 u2SupId;                         /* Supplier ID                      */
    u2 u2FunId;                         /* Function ID                      */
    u1 u1Variant;                       /* variant                          */
    u1 u1ErrRes;                        /* signal number of Response_Error  */
    
} ConfSLin_stNode;

typedef struct                          /* Event triggered frame information       */
{
    u1 u1InitEventPid;                  /* Initial PID of an Event triggered frame */
    u1 u1MesNum;                        /* Buffer number of an associated message  */

} ConfSLin_stEventInfo;


typedef struct                          /* Message buffer                             */
{
    u1 u1DataBuf[8];                    /* Data buffer for transmission and reception */

} ConfSLin_stMesBuf;

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
/* Channel abstraction macro for symbols */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)    /* Use LIN channel 0 */
#define LIN_CHANNEL ((u1)0x00U)     /* LIN channel number */

#define ConfSLin_u1gMESNUM          ConfSLin_u1gMESNUM_0
#define ConfSLin_u1gSIGNUM          ConfSLin_u1gSIGNUM_0
#define ConfSLin_u1gEVENTNUM        ConfSLin_u1gEVENTNUM_0
#define ConfSLin_u1gWKMAX           ConfSLin_u1gWKMAX_0
#define ConfSLin_u4gBUSTIMEOUT      ConfSLin_u4gBUSTIMEOUT_0
#define ConfSLin_u2gPERICLOCK       ConfSLin_u2gPERICLOCK_0
#define ConfSLin_u2gTMPERICLOCK     ConfSLin_u2gTMPERICLOCK_0
#define ConfSLin_stgMESINFO         ConfSLin_stgMESINFO_0
#define ConfSLin_stgSIGNAL          ConfSLin_stgSIGNAL_0
#define ConfSLin_stgNODE            ConfSLin_stgNODE_0
#define ConfSLin_stgEVENTINFO       ConfSLin_stgEVENTINFO_0
#define ConfSLin_u2gBAUDRATENORM    ConfSLin_u2gBAUDRATENORM_0
#define ConfSLin_stgMesBuf          ConfSLin_stgMesBuf_0
#define ConfSLin_u1gEventPid        ConfSLin_u1gEventPid_0
#define ConfSLin_u1gMesNoTbl        ConfSLin_u1gMesNoTbl_0
#define ConfSLin_u1gMesPid          ConfSLin_u1gMesPid_0

 #ifdef __LIN_SIGNAL_32__
#define ConfSLin_u4gMesSig          ConfSLin_u4gMesSig_0
#define ConfSLin_u4gMesSubSig       ConfSLin_u4gMesSubSig_0
 #else /* __LIN_SIGNAL_32__ */
#define ConfSLin_u2gMesSig          ConfSLin_u2gMesSig_0
#define ConfSLin_u2gMesSubSig       ConfSLin_u2gMesSubSig_0
 #endif /* __LIN_SIGNAL_32__ */

#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)  /* Use LIN channel 1 */
#define LIN_CHANNEL ((u1)0x01U)     /* LIN channel number */

#define ConfSLin_u1gMESNUM          ConfSLin_u1gMESNUM_1
#define ConfSLin_u1gSIGNUM          ConfSLin_u1gSIGNUM_1
#define ConfSLin_u1gEVENTNUM        ConfSLin_u1gEVENTNUM_1
#define ConfSLin_u1gWKMAX           ConfSLin_u1gWKMAX_1
#define ConfSLin_u1gAPITMPMESNO     ConfSLin_u1gAPITMPMESNO_1
#define ConfSLin_u1gCORETMPMESNO    ConfSLin_u1gCORETMPMESNO_1
#define ConfSLin_u4gBUSTIMEOUT      ConfSLin_u4gBUSTIMEOUT_1
#define ConfSLin_u2gPERICLOCK       ConfSLin_u2gPERICLOCK_1
#define ConfSLin_u2gTMPERICLOCK     ConfSLin_u2gTMPERICLOCK_1
#define ConfSLin_stgMESINFO         ConfSLin_stgMESINFO_1
#define ConfSLin_stgSIGNAL          ConfSLin_stgSIGNAL_1
#define ConfSLin_stgNODE            ConfSLin_stgNODE_1
#define ConfSLin_stgEVENTINFO       ConfSLin_stgEVENTINFO_1
#define ConfSLin_u2gBAUDRATENORM    ConfSLin_u2gBAUDRATENORM_1
#define ConfSLin_stgMesBuf          ConfSLin_stgMesBuf_1
#define ConfSLin_u1gEventPid        ConfSLin_u1gEventPid_1
#define ConfSLin_u1gMesNoTbl        ConfSLin_u1gMesNoTbl_1
#define ConfSLin_u1gMesPid          ConfSLin_u1gMesPid_1

 #ifdef __LIN_SIGNAL_32__
#define ConfSLin_u4gMesSig          ConfSLin_u4gMesSig_1
#define ConfSLin_u4gMesSubSig       ConfSLin_u4gMesSubSig_1
 #else /* __LIN_SIGNAL_32__ */
#define ConfSLin_u2gMesSig          ConfSLin_u2gMesSig_1
#define ConfSLin_u2gMesSubSig       ConfSLin_u2gMesSubSig_1
 #endif /* __LIN_SIGNAL_32__ */

#endif /* __LIN_CH1__ */


/*!!!!! Signal list is output by LIN Configurator !!!!!!!!!!!!!!!!!!!!!!!!*/
/*!!!!! Signal list is output by LIN Configurator !!!!!!!!!!!!!!!!!!!!!!!!*/

/*!!!!! Channel name is output by LIN Configurator !!!!!!!!!!!!!!!!!!!!!!*/

/*!!!!! Signal list is output by LIN Configurator !!!!!!!!!!!!!!!!!!!!!!!!*/

/* l_ifc_read_status */
#define LD_MASK_LAST_FRAME_PID      ((u2)0xFF00U)
#define LD_MASK_SLEEP               ((u2)0x0008U)
#define LD_MASK_OVERRUN             ((u2)0x0004U)
#define LD_MASK_SUCCESSFUL_TRANSFER ((u2)0x0002U)
#define LD_MASK_ERROR_IN_RESPONSE   ((u2)0x0001U)
#define LD_MASK_INIT                ((u2)0x0000U)

/* l_bool */
#define L_SUCCESS ((u1)0x00U)
#define L_FAIL    ((u1)0xFFU)

#define CONFSLIN_u1gPUBLISH     ((u1)0x00U)     /* Publish   */
#define CONFSLIN_u1gSUBSCRIBE   ((u1)0x01U)     /* Subscribe */

#define CONFSLIN_u1gCLASSICSUM  ((u1)0x00U)     /* Classic checksum  */
#define CONFSLIN_u1gENHANCEDSUM ((u1)0x01U)     /* Enhanced checksum */

/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */
u1   ConfSLin_vogCallDisableIrq(void);
void ConfSLin_vogCallRestoreIrq(u1 u1tPreIrq);
void ConfSLin_vogCallSleepIfc(u1 u1tIfcName);
void ConfSLin_vogCallWakeupIfc(u1 u1tIfcName);

/* -------------------------------------------------------------------- */
/* Public constants                                                     */
/* -------------------------------------------------------------------- */
extern const u1 ConfSLin_u1gMESNUM;                         /* The number of messages               */
extern const u1 ConfSLin_u1gSIGNUM;                         /* The number of signals                */
extern const u1 ConfSLin_u1gEVENTNUM;                       /* The number of event triggered frames */
extern const u1 ConfSLin_u1gWKMAX;                          /* The number of wakeup retry(LSB 3[times]) */
extern const u1 ConfSLin_u1gAPITMPMESNO;
extern const u1 ConfSLin_u1gCORETMPMESNO;
extern const u2 ConfSLin_u2gTIMEBASE;                       /* Timer base count                     */
extern const u4 ConfSLin_u4gBUSTIMEOUT;                     /* Bus timeout interval                 */

extern const ConfSLin_stMesInfo   ConfSLin_stgMESINFO[];
extern const ConfSLin_stSignal    ConfSLin_stgSIGNAL[];
extern const ConfSLin_stNode      ConfSLin_stgNODE;
extern const ConfSLin_stEventInfo ConfSLin_stgEVENTINFO[];

extern const u2 ConfSLin_u2gPERICLOCK;                      /* Peripheral hardware clock              */
extern const u2 ConfSLin_u2gTMPERICLOCK;                    /* Peripheral hardware clock for timer    */
extern const u2 ConfSLin_u2gBAUDRATENORM;                   /* Baud rate for Normal LIN communication */

/* -------------------------------------------------------------------- */
/* Public variables                                                     */
/* -------------------------------------------------------------------- */
extern       ConfSLin_stMesBuf    ConfSLin_stgMesBuf[];
extern       u1                   ConfSLin_u1gEventPid[];
extern       u1                   ConfSLin_u1gMesNoTbl[];
extern       u1                   ConfSLin_u1gMesPid[];
#ifdef __LIN_SIGNAL_32__
extern       u4                   ConfSLin_u4gMesSig[];
extern       u4                   ConfSLin_u4gMesSubSig[];
#else /* __LIN_SIGNAL_32__ */
extern       u2                   ConfSLin_u2gMesSig[];
extern       u2                   ConfSLin_u2gMesSubSig[];
#endif /* __LIN_SIGNAL_32__ */

#endif /* H_CONFSLIN */

/* End of files */
