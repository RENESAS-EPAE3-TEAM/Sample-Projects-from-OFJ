/* -------------------------------------------------------------------- */
/* Copyright(c) Renesas Electronics Corporation                         */
/* File name: conflin.h                                                 */
/* Version:   1.06                                                      */
/* ---------------------------------------------------------------------*/

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V1.06: 2024.08.30  Modify RL78/F23,F24                             */
/*   V1.05: 2021.07.28  Add parameters of RL78/F23,F24 RLIN3            */
/*   V1.04: 2015.07.28  Add parameters of RL78/F15 RLIN3                */
/*   V1.03: 2014.03.31  Add macro definition area for application       */
/*   V1.02: 2013.04.24  Add parameters of RL78/F13,F14 RLIN3            */
/*   V1.01: 2012.03.28  Add parameters of RL78/F12 UARTF and change     */
/*                      copyright                                       */
/*   V1.00: 2005.12.19  New                                             */
/* -------------------------------------------------------------------- */
#ifndef H_CONFLIN
#define H_CONFLIN

/* ******************************************************************** */
/* [Add macro definitions for LIN application] (from here)              */
/*                                                                      */
/* [Example]                                                            */
/*   #define __LIN_CH0__                                                */
/*                                                                      */
/* [Note]                                                               */
/*   Definition contents are different by each devices.                 */
/*   For more detail, see "compile option" in readme.txt .              */
/* ******************************************************************** */
#define __LIN_RL78_F23_F24__
#define __LIN_CH0__
#define __LIN_SIGNAL_32__

/* ******************************************************************** */
/* [Add macro definitions for LIN application] (to here)                */
/* ******************************************************************** */

/*!!!!!!!!!!! The following values are defined by user !!!!!!!!!!!!!!!!!*/
/* Index of the array, ConfLin_stgSignal, are named as signal name.     */
/*   (This array is defined in conflin.c)                               */
#define SIG1    (0)
#define SIG2    (1)
#define SIG3    (2)
#define SIG4    (3)
#define SIG5    (4)
#define SIG6    (5)
#define SIG7    (6)
#define SIG8    (7)
#define SIG9    (8)
#define SIG10   (9)
#define SIG11   (10)
#define SIG12   (11)
#define SIG13   (12)
#define SIG14   (13)
#define SIG15   (14)
#define SIG16   (15)
#define SIG17   (16)
#define SIG18   (17)
#define SIG19   (18)
#define SIG20   (19)
#define SIG21   (20)
#define SIG22   (21)
#define SIG23   (22)
#define SIG24   (23)
#define SIG25   (24)
#define SIG26   (25)
#define SIG27   (26)
#define SIG28   (27)
#define SIG29   (28)
#define SIG30   (29)
#define SIG31   (30)
#define SIG32   (31)
#define SIG33   (32)
#define SIG34   (33)
#define SIG35   (34)
#define SIG_Err (35)

/*!!!!! Frame name. This section is filled by LIN Configurator. !!!!!!!!*/
#define Frame_A   (0)
#define Frame_B   (1)
#define Frame_C   (2)
#define Frame_D   (3)
#define Frame_Err (4)

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/


/* ******************************************************************** */
/* [Not user setting]                                                   */
/* ******************************************************************** */
/* LIN channel number */
#if defined(__LIN_CH0__)
  #define LIN_CHANNEL0      0

#elif defined(__LIN_CH1__)
  #define LIN_CHANNEL0      1

#endif


/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

typedef unsigned char  u1;
typedef unsigned short u2;
typedef   signed short s2;
typedef unsigned long  u4;

#define l_bool          u1
#define l_u8            u1
#define l_u16           u2
#define l_irqmask       u1
#define l_signal_handle u1
#define l_flag_handle   u1
#define l_ifc_handle    u1

#define ConfLin_stMesInfo               ConfSLin_stMesInfo
#define ConfLin_stSignal                ConfSLin_stSignal
#define ConfLin_stNode                  ConfSLin_stNode
#define ConfLin_stEventInfo             ConfSLin_stEventInfo
#define ConfLin_stMesBuf                ConfSLin_stMesBuf

typedef struct                                       /* Message information                         */
{
#if (!defined __LIN_RL78_F23_F24__)
    u2 u2MesId;                                      /* Message ID                                  */
#endif /* !__LIN_RL78_F23_F24__ */
    u1 u1Dir;                                        /* 0 is publish, and 1 is subscribe            */
    u1 u1ByteSize;                                   /* Message size                                */
    u1 u1InitPid;                                    /* Initial PID                                 */
    u1 u1SumType;                                    /* Checksum (0 is classic, and 1 is enhanced)  */
    u1 u1InitData[8];                                /* Initial data                                */
} ConfLin_stMesInfo;


typedef struct                                       /* Signal information                          */
{
    u1 u1OffsetByte;                                 /* Byte offset                                 */
    u1 u1OffsetBit;                                  /* Bit offset in the starting byte of signal   */
    u1 u1BitSize;                                    /* Bit size                                    */
#ifdef __LIN_SIGNAL_32__
    u2 u2SigNum;                                     /* Signal number                               */
#else
    u1 u1SigNum;                                     /* Signal number                               */
#endif /* __LIN_SIGNAL_32__ */
} ConfLin_stSignal;


typedef struct                                       /* Node information                            */
{
    u1 u1InitNad;                                    /* Initial NAD                                 */
    u2 u2SupId;                                      /* Supplier ID                                 */
    u2 u2FunId;                                      /* Function ID                                 */
    u1 u1Variant;                                    /* Variant                                     */
    u1 u1ErrRes;                                     /* Signal number of Resonse_Error              */
} ConfLin_stNode;


typedef struct                                       /* Event triggered frame information           */
{
#if (!defined __LIN_RL78_F23_F24__)
    u2 u2MesId;                                      /* Message ID                                  */
#endif /* !__LIN_RL78_F23_F24__ */
    u1 u1InitEventPid;                               /* Initial PID                                 */
    u1 u1MesNum;                                     /* Associated message buffer number            */
} ConfLin_stEventInfo;


typedef struct                                       /* Message buffer                              */
{
    u1 u1DataBuf[8];                                 /* Data buffer for transmission and reception  */
} ConfLin_stMesBuf;


/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */

/* l_ifc_read_status        */
#define LD_MASK_LAST_FRAME_PID      ((u2)0xFF00)
#define LD_MASK_SLEEP               ((u2)0x0008)
#define LD_MASK_OVERRUN             ((u2)0x0004)
#define LD_MASK_SUCCESSFUL_TRANSFER ((u2)0x0002)
#define LD_MASK_ERROR_IN_RESPONSE   ((u2)0x0001)
#define LD_MASK_INIT                ((u2)0x0000)

/* l_bool */
#define L_SUCCESS ((u1)0x00)
#define L_FAIL    ((u1)0xFF)

#define CONFLIN_u1gPUBLISH     ((u1)0x00)       /* Publish   */
#define CONFLIN_u1gSUBSCRIBE   ((u1)0x01)       /* Subscribe */

#define CONFLIN_u1gCLASSICSUM  ((u1)0x00)       /* Classic checksum  */
#define CONFLIN_u1gENHANCEDSUM ((u1)0x01)       /* Enhanced checksum */


/* -------------------------------------------------------------------- */
/* Convert function names with internal coding rule                     */
/* to external interface names                                          */
/* -------------------------------------------------------------------- */

#define l_sys_init                      ApSLin_u1gInitSys

#define l_bool_rd                       ApLin_u1gRead8bitsSig
#define l_u8_rd                         ApLin_u1gRead8bitsSig
#define l_u16_rd                        ApLin_u2gRead16bitsSig
#define l_bool_wr                       ApLin_vogWrite8bitsSig
#define l_u8_wr                         ApLin_vogWrite8bitsSig
#define l_u16_wr                        ApLin_vogWrite16bitsSig
#define l_bytes_rd                      ApLin_vogReadBytesSig
#define l_bytes_wr                      ApLin_vogWriteBytesSig
#define l_flg_tst                       ApLin_u1gTestFlag
#define l_flg_clr                       ApLin_vogClearFlag

#define l_sys_irq_disable               ConfSLin_vogCallDisableIrq
#define l_sys_irq_restore               ConfSLin_vogCallRestoreIrq
#define l_sys_call_sleep                ConfSLin_vogCallSleepIfc
#define l_sys_call_wake_up              ConfSLin_vogCallWakeupIfc

#define l_ifc_init(ch)                  ApSLin_u1gInitIfc_DUMMY(ch)
#define ApSLin_u1gInitIfc_DUMMY(ch)     ApSLin_u1gInitIfc_##ch()
#define l_ifc_wake_up(ch)               ApSLin_vogWakeupIfc_DUMMY(ch)
#define ApSLin_vogWakeupIfc_DUMMY(ch)   ApSLin_vogWakeupIfc_##ch()
#define l_ifc_read_status(ch)           ApSLin_u2gReadStatus_DUMMY(ch)
#define ApSLin_u2gReadStatus_DUMMY(ch)  ApSLin_u2gReadStatus_##ch()


/* -------------------------------------------------------------------- */
/* Channel abstraction macros for symbols                               */
/* -------------------------------------------------------------------- */

/* Public functions */
#if defined(__LIN_CH0__)
  #define ApLin_u1gRead8bitsSig         ApSLin_u1gRead8bitsSig_0
  #define ApLin_u2gRead16bitsSig        ApSLin_u2gRead16bitsSig_0
  #define ApLin_vogWrite8bitsSig        ApSLin_vogWrite8bitsSig_0
  #define ApLin_vogWrite16bitsSig       ApSLin_vogWrite16bitsSig_0
  #define ApLin_vogReadBytesSig         ApSLin_vogReadBytesSig_0
  #define ApLin_vogWriteBytesSig        ApSLin_vogWriteBytesSig_0
  #define ApLin_u1gTestFlag             ApSLin_u1gTestFlag_0
  #define ApLin_vogClearFlag            ApSLin_vogClearFlag_0

  #define ApLin_u1gInitIfc              ApSLin_u1gInitIfc_0
  #define ApLin_vogWakeupIfc            ApSLin_vogWakeupIfc_0
  #define ApLin_u2gReadStatus           ApSLin_u2gReadStatus_0

#elif defined(__LIN_CH1__)
  #define ApLin_u1gRead8bitsSig         ApSLin_u1gRead8bitsSig_1
  #define ApLin_u2gRead16bitsSig        ApSLin_u2gRead16bitsSig_1
  #define ApLin_vogWrite8bitsSig        ApSLin_vogWrite8bitsSig_1
  #define ApLin_vogWrite16bitsSig       ApSLin_vogWrite16bitsSig_1
  #define ApLin_vogReadBytesSig         ApSLin_vogReadBytesSig_1
  #define ApLin_vogWriteBytesSig        ApSLin_vogWriteBytesSig_1
  #define ApLin_u1gTestFlag             ApSLin_u1gTestFlag_1
  #define ApLin_vogClearFlag            ApSLin_vogClearFlag_1

  #define ApLin_u1gInitIfc              ApSLin_u1gInitIfc_1
  #define ApLin_vogWakeupIfc            ApSLin_vogWakeupIfc_1
  #define ApLin_u2gReadStatus           ApSLin_u2gReadStatus_1

#endif

/* Public constants */
#if defined(__LIN_CH0__)
  #define ConfLin_u1gMESNUM             ConfSLin_u1gMESNUM_0
  #define ConfLin_u1gSIGNUM             ConfSLin_u1gSIGNUM_0
  #define ConfLin_u1gEVENTNUM           ConfSLin_u1gEVENTNUM_0
  #define ConfLin_u1gWKMAX              ConfSLin_u1gWKMAX_0

  #define ConfSLin_u4gBUSTIMEOUT        ConfSLin_u4gBUSTIMEOUT_0
  #define ConfSLin_u2gPERICLOCK         ConfSLin_u2gPERICLOCK_0
  #define ConfSLin_u2gTMPERICLOCK       ConfSLin_u2gTMPERICLOCK_0
  #define ConfSLin_u2gBAUDRATENORM      ConfSLin_u2gBAUDRATENORM_0

  #define ConfLin_stgMESINFO            ConfSLin_stgMESINFO_0
  #define ConfLin_stgSIGNAL             ConfSLin_stgSIGNAL_0
  #define ConfLin_stgNODE               ConfSLin_stgNODE_0
  #define ConfLin_stgEVENTINFO          ConfSLin_stgEVENTINFO_0

#elif defined(__LIN_CH1__)
  #define ConfLin_u1gMESNUM             ConfSLin_u1gMESNUM_1
  #define ConfLin_u1gSIGNUM             ConfSLin_u1gSIGNUM_1
  #define ConfLin_u1gEVENTNUM           ConfSLin_u1gEVENTNUM_1
  #define ConfLin_u1gWKMAX              ConfSLin_u1gWKMAX_1

  #define ConfSLin_u4gBUSTIMEOUT        ConfSLin_u4gBUSTIMEOUT_1
  #define ConfSLin_u2gPERICLOCK         ConfSLin_u2gPERICLOCK_1
  #define ConfSLin_u2gTMPERICLOCK       ConfSLin_u2gTMPERICLOCK_1
  #define ConfSLin_u2gBAUDRATENORM      ConfSLin_u2gBAUDRATENORM_1

  #define ConfLin_stgMESINFO            ConfSLin_stgMESINFO_1
  #define ConfLin_stgSIGNAL             ConfSLin_stgSIGNAL_1
  #define ConfLin_stgNODE               ConfSLin_stgNODE_1
  #define ConfLin_stgEVENTINFO          ConfSLin_stgEVENTINFO_1

#endif

/* Public variables */
#if defined(__LIN_CH0__)
  #define ConfLin_stgMesBuf             ConfSLin_stgMesBuf_0
  #define ConfLin_u1gEventPid           ConfSLin_u1gEventPid_0
  #define ConfLin_u1gMesNoTbl           ConfSLin_u1gMesNoTbl_0
  #define ConfLin_u1gMesPid             ConfSLin_u1gMesPid_0

  #ifdef __LIN_SIGNAL_32__
   #define ConfLin_u4gMesSig            ConfSLin_u4gMesSig_0
   #define ConfLin_u4gMesSubSig         ConfSLin_u4gMesSubSig_0
  #else
   #define ConfLin_u2gMesSig            ConfSLin_u2gMesSig_0
   #define ConfLin_u2gMesSubSig         ConfSLin_u2gMesSubSig_0
  #endif /* __LIN_SIGNAL_32__ */

#elif defined(__LIN_CH1__)
  #define ConfLin_stgMesBuf             ConfSLin_stgMesBuf_1
  #define ConfLin_u1gEventPid           ConfSLin_u1gEventPid_1
  #define ConfLin_u1gMesNoTbl           ConfSLin_u1gMesNoTbl_1
  #define ConfLin_u1gMesPid             ConfSLin_u1gMesPid_1

  #ifdef __LIN_SIGNAL_32__
   #define ConfLin_u4gMesSig            ConfSLin_u4gMesSig_1
   #define ConfLin_u4gMesSubSig         ConfSLin_u4gMesSubSig_1
  #else
   #define ConfLin_u2gMesSig            ConfSLin_u2gMesSig_1
   #define ConfLin_u2gMesSubSig         ConfSLin_u2gMesSubSig_1
  #endif /* __LIN_SIGNAL_32__ */

#endif


/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */

/* APIs that do not have channel number as arguments */
u1   ApSLin_u1gInitSys(void);

u1   ApLin_u1gRead8bitsSig(u1);
u2   ApLin_u2gRead16bitsSig(u1);
void ApLin_vogWrite8bitsSig(u1, u1);
void ApLin_vogWrite16bitsSig(u1, u2);
void ApLin_vogReadBytesSig(u1, u1, u1, u1* const);
void ApLin_vogWriteBytesSig(u1, u1, u1, const u1* const);
u1   ApLin_u1gTestFlag(u1);
void ApLin_vogClearFlag(u1);

u1   ConfSLin_vogCallDisableIrq(void);
void ConfSLin_vogCallRestoreIrq(u1);
void ConfSLin_vogCallSleepIfc(u1);
void ConfSLin_vogCallWakeupIfc(u1);


/* APIs that have channel number as arguments */
u1   ApLin_u1gInitIfc(void);                                /* LIN interface initialization  */
void ApLin_vogWakeupIfc(void);                              /* Wakeup                        */
u2   ApLin_u2gReadStatus(void);                             /* Read and clear LIN status     */

/* -------------------------------------------------------------------- */
/* Public constants                                                     */
/* -------------------------------------------------------------------- */

extern const u1 ConfLin_u1gMESNUM;                          /* Message buffer number                */
extern const u1 ConfLin_u1gSIGNUM;                          /* Signal number                        */
extern const u1 ConfLin_u1gEVENTNUM;                        /* Event triggered frame number         */
extern const u1 ConfLin_u1gWKMAX;                           /* Wakeup retry count(LSB 3[times])       */

extern const u4 ConfSLin_u4gBUSTIMEOUT;                     /* Bus timeout time(normally 4sec)      */
extern const u2 ConfSLin_u2gPERICLOCK;                      /* Peripheral H/W clock                 */
extern const u2 ConfSLin_u2gTMPERICLOCK;                    /* Peripheral H/W clock for timer       */
extern const u2 ConfSLin_u2gBAUDRATENORM;                   /* Normal LIN baudrate                  */


extern const ConfLin_stMesInfo   ConfLin_stgMESINFO[];
extern const ConfLin_stSignal    ConfLin_stgSIGNAL[];
extern const ConfLin_stNode      ConfLin_stgNODE;
extern const ConfLin_stEventInfo ConfLin_stgEVENTINFO[];


/* -------------------------------------------------------------------- */
/* Public variables                                                     */
/* -------------------------------------------------------------------- */

extern       ConfLin_stMesBuf    ConfLin_stgMesBuf[];
extern       u1                  ConfLin_u1gEventPid[];
extern       u1                  ConfLin_u1gMesNoTbl[];
extern       u1                  ConfLin_u1gMesPid[];
#ifdef __LIN_SIGNAL_32__
extern       u4                  ConfLin_u4gMesSig[];
extern       u4                  ConfLin_u4gMesSubSig[];
#else
extern       u2                  ConfLin_u2gMesSig[];
extern       u2                  ConfLin_u2gMesSubSig[];
#endif /* __LIN_SIGNAL_32__ */


#endif /* H_CONFLIN */


/* End of files */
