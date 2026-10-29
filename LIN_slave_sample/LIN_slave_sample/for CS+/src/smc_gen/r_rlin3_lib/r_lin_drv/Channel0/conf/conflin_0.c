/* -------------------------------------------------------------------- */
/* Copyright(c) Renesas Electronics Corporation                         */
/* File name: conflin.c                                                 */
/* Version:   1.09                                                      */
/* ---------------------------------------------------------------------*/

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V1.09: 2024.08.30  Modify RL78/F23,F24                             */
/*   V1.08: 2021.07.28  Add statements about RL78/F23,F24 RLIN3         */
/*   V1.07: 2015.07.28  Add statements about RL78/F15 RLIN3             */
/*   V1.06: 2013.05.30  Modify file including statement to add channel. */
/*   V1.05: 2013.04.24  Add statements about RL78/F13,F14 RLIN3         */
/*   V1.04: 2011.12.27  Add statements about RL78/F12 UARTF             */
/*   V1.03: 2009.11.17  Add statements about 78K0R/Fx3 UARTF            */
/*   V1.02: 2006. 7. 7  Add statements about 78K0/Fx UART1              */
/*   V1.01: 2006. 4. 6  Modify statement, #elif -> #ifdef               */
/*   V1.00: 2005.12.19  New                                             */
/* -------------------------------------------------------------------- */

#include "conflin_0.h"


/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */

/* [CONFLIN_u1sMESSAGENUM: Message number]                          */
/*   The size of ConfLin_stgMESINFO is set here.                    */
#define CONFLIN_u1sMESSAGENUM       (5)         /* Message number                 */

/* [CONFLIN_u1sSIGNALNUM: Signal number]                            */
/*   The size of ConfLin_stgSIGNAL is set here.                     */
#define CONFLIN_u1sSIGNALNUM        (36)        /* Signal number                  */

/* [CONFLIN_u1sEVENTMESSAGENUM: Event triggered frame number]       */
/*   The size of ConfLin_stgEVENTINFO is set here.                  */
#define CONFLIN_u1sEVENTMESSAGENUM  (0)         /* Event triggered message number */

/* [CONFLIN_u2sPERICLOCK: Peripheral hardware clock(LSB:10kHz)]     */
/*   Peripheral hardware clock is set in the 10kHz here.            */
/*                                                                  */
/*   [ex.  In the case of 10MHz]                                    */
/*     #define CONFLIN_u2sPERICLOCK        (1000)                   */
#define CONFLIN_u2sPERICLOCK        (4000)      /* Peripheral hardware clock [LSB:10KHz] */

/* [CONFLIN_u2sBAUDRATE: Baud rate[bps]]                            */
/*   Baud rate is set here.                                         */
/*   You can select setting value in 0, 9600, 19200 only.           */
/*   0 can be specified only a library                              */
/*   with an automatic baud rate detection option                   */
#define CONFLIN_u2sBAUDRATE         (0)

/* [CONFLIN_u1sBUSTIMEOUT: Bus time out(LSB:100ms)]                 */
/*   Bus time out is set in the 100ms here.                         */
/*   But if 0 is set, time out turns to noneffective.               */
/*                                                                  */
/*   [ex.  In the case of 10s]                                      */
/*     #define CONFLIN_u1sBUSTIMEOUT     (100)                      */
#define CONFLIN_u1sBUSTIMEOUT       (40)        /* Bus timeout [LSB:100ms]        */
                                                /* If 0 is set, time out turns to noneffective. */
#if (defined __LIN_RL78_F23_F24__)

/* [CONFLIN_u1sWakeupMaxRetry: Wakeup retry count(LSB 3[times])     */
/*   Specify the number of wake-up retries between 1 and 80.        */
/*   This retry setting value x 3 retries are performed.            */
/*                                                                  */
/*   [ex.  For 9 times]                                             */
/*     #define CONFLIN_u1sWAKEUPMAXRETRY  ( 3)                      */
#define CONFLIN_u1sWAKEUPMAXRETRY   (0)         /* Wakeup retry number [LSB:100ms] */
#endif /* __LIN_RL78_F23_F24__ */

/*--------------------------------------------------------------------------------------*/
/* [ConfLin_stgMESINFO: Message information]                                            */
/*                                                                                      */
/*   Message information on slave node is defined here.                                 */
/*   Following description explains how to set each member of the structure             */
/*   which is an element of this array.                                                 */
/*                                                                                      */
/* [Structure ConfLin_stMesInfo]                                                        */
/*                                                                                      */
/*   - u2 u2MesId (Message ID)                                                          */
/*     This member can be set arbitrarily in the range 0x0000-0xffff.                   */
/*     But this mustn't correspond with any other message IDs.                          */
/*     RL78/F23,F24 is a LIN2.1 compliant product.                                      */
/*     There is no message ID.                                                          */
/*                                                                                      */
/*   - u1 u1Dir (Direction of message)                                                  */
/*     A value of the two below must be set as this member.                             */
/*       CONFLIN_u1gPUBLISH    publish                                                  */
/*       CONFLIN_u1gSUBSCRIBE  subscribe                                                */
/*                                                                                      */
/*   - u1 u1ByteSize (Message size [byte])                                              */
/*     Message size is set here.                                                        */
/*     The range of this value is 1-8 byte.                                             */
/*                                                                                      */
/*   - u1 u1InitPid (Initial PID)                                                       */
/*     This member is default PID.                                                      */
/*     PID can be changed by ld_assign_frame_id on master node.                         */
/*                                                                                      */
/*   - u1 u1SumType (Checksum type)                                                     */
/*     A value of the two method of calculation below must be set as this member.       */
/*       CONFLIN_u1gCLASSICSUM    calculate checksum with only data                     */
/*       CONFLIN_u1gENHANCEDSUM   calculate checksum with PID and data                  */
/*                                                                                      */
/*   - u1 u1InitData[8] (Initial data)                                                  */
/*     Initial data is set here.                                                        */
/*     Byte size of used data is the same as u1ByteSize.                                */
/*                                                                                      */
/* [Note]                                                                               */
/*   You can register and delete the messages as you like.                              */
/*   The Maximum number of registration is 16.                                          */
/*                                                                                      */
/*   - Deletion of a message                                                            */
/*     Delete the element of array related to the message you don't need.               */
/*                                                                                      */
/*   - Registration of a message                                                        */
/*     Register the element of array related to the message you need.                   */
/*                                                                                      */
/*   Increasing and decreasing the number of message, set CONFLIN_u1sMESSAGENUM         */
/*   of macro definition section as the number of element of this array.                */
/*--------------------------------------------------------------------------------------*/
const ConfLin_stMesInfo ConfLin_stgMESINFO[CONFLIN_u1sMESSAGENUM] = /* Message information  */
{
    /* Message buffer 0 */
    {
        CONFLIN_u1gPUBLISH,                         /* Publish                           */
        (u1)1,                                      /* Message size (byte)               */
        0x80,                                       /* Initial PID                       */
        CONFLIN_u1gENHANCEDSUM,                     /* Checksum type                     */
        { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 } /* Initial data (set as 8-byte data) */
    },

    /* Message buffer 1 */
    {
        CONFLIN_u1gSUBSCRIBE,                       /* Subscribe                         */
        (u1)1,                                      /* Message size (byte)               */
        0xC1,                                       /* Initial PID                       */
        CONFLIN_u1gENHANCEDSUM,                     /* Checksum type                     */
        { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 } /* Initial data (set as 8-byte data) */
    },

    /* Message buffer 2 */
    {
        CONFLIN_u1gPUBLISH,                         /* Publish                           */
        (u1)2,                                      /* Message size (byte)               */
        0x42,                                       /* Initial PID                       */
        CONFLIN_u1gENHANCEDSUM,                     /* Checksum type                     */
        { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 } /* Initial data (set as 8-byte data) */
    },

    /* Message buffer 3 */
    {
        CONFLIN_u1gSUBSCRIBE,                       /* Subscribe                         */
        (u1)8,                                      /* Message size (byte)               */
        0x03,                                       /* Initial PID                       */
        CONFLIN_u1gENHANCEDSUM,                     /* Checksum type                     */
        { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 } /* Initial data (set as 8-byte data) */
    },

    /* Message buffer 4 */
    {
        CONFLIN_u1gPUBLISH,                         /* Publish                           */
        (u1)1,                                      /* Message size (byte)               */
        0x6F,                                       /* Initial PID                       */
        CONFLIN_u1gENHANCEDSUM,                     /* Checksum type                     */
        { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 } /* Initial data (set as 8-byte data) */
    },
};

/*--------------------------------------------------------------------------------------*/
/* [ConfLin_stgSIGNAL: Signal definition]                                               */
/*                                                                                      */
/*   Signal information on slave node is defined here.                                  */
/*   Definition of multiple signals at arbitrary positions of one frame is possible,    */
/*   but definition with overlapping of several signals or definition of                */
/*   scalar signals whose area is over more than 2 byte boundary is impossible.         */
/*   If you define a byte array, it's necessary to take care to define it from          */
/*   byte boundary. And its size must be multiples of 8.                                */
/*                                                                                      */
/*   Following description explains how to set each member of the structure             */
/*   which is an element of this array.                                                 */
/*                                                                                      */
/* [Structure ConfLin_stSignal]                                                         */
/*                                                                                      */
/*   - u1 u1OffsetByte (Byte offset)                                                    */
/*     Start location of the signal is set here.                                        */
/*     The range of this value is 0-7 byte.                                             */
/*                                                                                      */
/*   - u1 u1OffsetBit (Bit offset)                                                      */
/*     Start bit in start byte of the signal is set here.                               */
/*     The range of this value is 0-7.                                                  */
/*     If you the signal is byte array, 0 should be set into this value.                */
/*                                                                                      */
/*   - u1 u1BitSize (Signal size)                                                       */
/*     Bit size is set here.                                                            */
/*     The range of this value is 1-16, 24, 32, 40, 48, 56, 64.                         */
/*                                                                                      */
/*   - u2 u2SigNum (Signal number) / selected 32 signals by compile option.             */
/*     Message number of upper 8bits and signal number of lower 8bits is set here.      */
/*     (This signal number is numbered from 0.)                                         */
/*                                                                                      */
/*   - u1 u1SigNum (Signal number) / not selected 32 signals by compile option.         */
/*     Message number of upper 4bits and signal number of lower 4bits is set here.      */
/*     (This signal number is numbered from 0.)                                         */
/*                                                                                      */
/* [Note]                                                                               */
/*   You can register and delete the signals as you like.                               */
/*   The Maximum number of registration in each frame is 32 or 16.                      */
/*                                                                                      */
/*   - Deletion of a signal                                                             */
/*     Delete the element of array related to the signal you don't need.                */
/*                                                                                      */
/*   - Registration of a signal                                                         */
/*     Register the element of array related to the signal you need.                    */
/*                                                                                      */
/*   Increasing and decreasing the number of message, set CONFLIN_u1sSIGNALNUM          */
/*   of macro definition section as the number of element of this array,                */
/*   and define macro of signal name which means the index of this array in conflin.h.  */
/*--------------------------------------------------------------------------------------*/
const ConfLin_stSignal    ConfLin_stgSIGNAL[CONFLIN_u1sSIGNALNUM] = /* Signal definition    */
{
    /* Signal name SIG1 */
    {
        (u1)0,                                      /* Byte offset       */
        (u1)0,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0000                                  /* Signal number     */
    #else
        (u1)0x00                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG2 */
    {
        (u1)0,                                      /* Byte offset       */
        (u1)0,                                      /* Bit offset        */
        (u1)8,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0100                                  /* Signal number     */
    #else
        (u1)0x10                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG3 */
    {
        (u1)0,                                      /* Byte offset       */
        (u1)1,                                      /* Bit offset        */
        (u1)14,                                     /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0200                                  /* Signal number     */
    #else
        (u1)0x20                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG4 */
    {
        (u1)0,                                      /* Byte offset       */
        (u1)0,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0300                                  /* Signal number     */
    #else
        (u1)0x30                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG5 */
    {
        (u1)0,                                      /* Byte offset       */
        (u1)2,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0301                                  /* Signal number     */
    #else
        (u1)0x31                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG6 */
    {
        (u1)0,                                      /* Byte offset       */
        (u1)4,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0302                                  /* Signal number     */
    #else
        (u1)0x32                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG7 */
    {
        (u1)0,                                      /* Byte offset       */
        (u1)6,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0303                                  /* Signal number     */
    #else
        (u1)0x33                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG8 */
    {
        (u1)1,                                      /* Byte offset       */
        (u1)0,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0304                                  /* Signal number     */
    #else
        (u1)0x34                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG9 */
    {
        (u1)1,                                      /* Byte offset       */
        (u1)2,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0305                                  /* Signal number     */
    #else
        (u1)0x35                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG10 */
    {
        (u1)1,                                      /* Byte offset       */
        (u1)4,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0306                                  /* Signal number     */
    #else
        (u1)0x36                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG11 */
    {
        (u1)1,                                      /* Byte offset       */
        (u1)6,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0307                                  /* Signal number     */
    #else
        (u1)0x37                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG12 */
    {
        (u1)2,                                      /* Byte offset       */
        (u1)0,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0308                                  /* Signal number     */
    #else
        (u1)0x38                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG13 */
    {
        (u1)2,                                      /* Byte offset       */
        (u1)2,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0309                                  /* Signal number     */
    #else
        (u1)0x39                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG14 */
    {
        (u1)2,                                      /* Byte offset       */
        (u1)4,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x030A                                  /* Signal number     */
    #else
        (u1)0x3A                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG15 */
    {
        (u1)2,                                      /* Byte offset       */
        (u1)6,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x030B                                  /* Signal number     */
    #else
        (u1)0x3B                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG16 */
    {
        (u1)3,                                      /* Byte offset       */
        (u1)0,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x030C                                  /* Signal number     */
    #else
        (u1)0x3C                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG17 */
    {
        (u1)3,                                      /* Byte offset       */
        (u1)2,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x030D                                  /* Signal number     */
    #else
        (u1)0x3D                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG18 */
    {
        (u1)3,                                      /* Byte offset       */
        (u1)4,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x030E                                  /* Signal number     */
    #else
        (u1)0x3E                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG19 */
    {
        (u1)3,                                      /* Byte offset       */
        (u1)6,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x030F                                  /* Signal number     */
    #else
        (u1)0x3F                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG20 */
    {
        (u1)4,                                      /* Byte offset       */
        (u1)0,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0310                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG21 */
    {
        (u1)4,                                      /* Byte offset       */
        (u1)2,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0311                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG22 */
    {
        (u1)4,                                      /* Byte offset       */
        (u1)4,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0312                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG23 */
    {
        (u1)4,                                      /* Byte offset       */
        (u1)6,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0313                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG24 */
    {
        (u1)5,                                      /* Byte offset       */
        (u1)0,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0314                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG25 */
    {
        (u1)5,                                      /* Byte offset       */
        (u1)2,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0315                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG26 */
    {
        (u1)5,                                      /* Byte offset       */
        (u1)4,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0316                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG27 */
    {
        (u1)5,                                      /* Byte offset       */
        (u1)6,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0317                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG28 */
    {
        (u1)6,                                      /* Byte offset       */
        (u1)0,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0318                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG29 */
    {
        (u1)6,                                      /* Byte offset       */
        (u1)2,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0319                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG30 */
    {
        (u1)6,                                      /* Byte offset       */
        (u1)4,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x031A                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG31 */
    {
        (u1)6,                                      /* Byte offset       */
        (u1)6,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x031B                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG32 */
    {
        (u1)7,                                      /* Byte offset       */
        (u1)0,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x031C                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG33 */
    {
        (u1)7,                                      /* Byte offset       */
        (u1)2,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x031D                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG34 */
    {
        (u1)7,                                      /* Byte offset       */
        (u1)4,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x031E                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG35 */
    {
        (u1)7,                                      /* Byte offset       */
        (u1)6,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x031F                                  /* Signal number     */
    #else
        (u1)0xFF                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },

    /* Signal name SIG_Err */
    {
        (u1)0,                                      /* Byte offset       */
        (u1)0,                                      /* Bit offset        */
        (u1)1,                                      /* Signal size (bit) */
    #ifdef __LIN_SIGNAL_32__
        (u2)0x0400                                  /* Signal number     */
    #else
        (u1)0x40                                    /* Signal number     */
    #endif /* __LIN_SIGNAL_32__ */
    },
};

/*--------------------------------------------------------------------------------------*/
/* [ConfLin_stgEVENTINFO: Event triggered frame information]                            */
/*                                                                                      */
/*   Event triggered frame information on slave node is defined here.                   */
/*   Following description explains how to set each member of the structure             */
/*   which is an element of this array.                                                 */
/*                                                                                      */
/* [Structure ConfLin_stgEventInfo]                                                     */
/*                                                                                      */
/*   - u2 u2MesId (Message ID)                                                          */
/*     This member can be set arbitrarily in the range 0x0000-0xffff.                   */
/*     But this mustn't correspond with any other message IDs.                          */
/*     RL78/F23,F24 is a LIN2.1 compliant product.                                      */
/*     There is no message ID.                                                          */
/*                                                                                      */
/*   - u1 u1InitEventPid (Initial PID)                                                  */
/*     This member is default PID.                                                      */
/*     This PID can be changed by ld_assign_frame_id on master node.                    */
/*                                                                                      */
/*   - u1 u1MesNum(Associated message buffer number)                                    */
/*     Associated message buffer number is set here.                                    */
/*                                                                                      */
/* [Note]                                                                               */
/*   You can register and delete the event triggered frame as you like.                 */
/*   The Maximum number of registration is 16.                                          */
/*                                                                                      */
/*   - Deletion of a event triggered frame information                                  */
/*     Delete the element of array related to the event triggered frame you don't need. */
/*                                                                                      */
/*   - Registration of a event triggered frame                                          */
/*     Register the element of array related to the event triggered frame you need.     */
/*                                                                                      */
/*   Increasing and decreasing the number of event triggered frame,                     */
/*   set CONFLIN_u1sMESSAGENUM of macro definition section                              */
/*   as the number of element of this array.                                            */
/*                                                                                      */
/* [Sample of registration]                                                             */
/*    In the case of assignment of FRM_UNCOND1, 2 to event triggered frame.             */
/* const ConfLin_stEventInfo ConfLin_stgEVENTINFO[CONFLIN_u1sEVENTMESSAGENUM] =         */
/* {                                                                                    */
/*     {                                                                                */
/*         (u2)0x0010,                                                                  */
/*         (u1)0x50,                                                                    */
/*         (u1)FRM_UNCOND01                                                             */
/*     },                                                                               */
/*     {                                                                                */
/*         (u2)0x0010,                                                                  */
/*         (u1)0x50,                                                                    */
/*         (u1)FRM_UNCOND02                                                             */
/*     },                                                                               */
/* };                                                                                   */
/*--------------------------------------------------------------------------------------*/
/* When the number of event triggered frames is zero, dummy array is defined. */
#if  CONFLIN_u1sEVENTMESSAGENUM == 0
const ConfLin_stEventInfo ConfLin_stgEVENTINFO[1]; 
#else

const ConfLin_stEventInfo ConfLin_stgEVENTINFO[CONFLIN_u1sEVENTMESSAGENUM]; /* Event information (DUMMY definition) */

#endif

/*--------------------------------------------------------------------------------------*/
/* [ConfLin_stgNODE: Node information]                                                  */
/*                                                                                      */
/*   Slave node information is defined here.                                            */
/*   Following description explains how to set each member of the structure.            */
/* [Structure ConfLin_stNode]                                                           */
/*                                                                                      */
/*   - u1 u1InitNad (Node address)                                                      */
/*     NAD is set here.                                                                 */
/*                                                                                      */
/*   - u2 u2SupId(Supplier ID)                                                          */
/*     Supplier ID is set here.                                                         */
/*                                                                                      */
/*   - u2 u2FunId(Function ID)                                                          */
/*     Function ID is set here.                                                         */
/*                                                                                      */
/*   - u1 u1Variant(Variant ID)                                                         */
/*     Variant ID is set here.                                                          */
/*                                                                                      */
/*   - u1 u1ErrRes (Index of Response_Error signal)                                     */
/*     Response_Error signal index used in network error is set here.                   */
/*     As this index, signal index defined in ConfLin_stgSignal is used.                */
/*--------------------------------------------------------------------------------------*/
const ConfLin_stNode ConfLin_stgNODE =              /* Node definition          */
{
    0x01,                                           /* Initial NAD          */
    0x0000,                                         /* Supplier ID          */
    0x0000,                                         /* Function ID          */
    0x00,                                           /* Variant              */
    SIG_Err                                         /* Network error signal */
};


/************************************************************************/
/* The following is generally out of user setting                       */
/************************************************************************/

#define CONFLIN_u2sTIMERBASE       ((u2)0xFFFF)     /* The CR value of timer(No need for setting generally) */

#if (defined __LIN_KX__)            || \
    (defined __LIN_FX_UART60__)     || \
    (defined __LIN_FX_L__)
#define CONFLIN_u1sTIMERPRESCALER ((u1)4)
#define CONFLIN_u1sCAPTUREBITSIZE ((u1)2)
#define CONFLIN_f4sBITSTIMECOUNT (CONFLIN_u1sCAPTUREBITSIZE * CONFLIN_u2sPERICLOCK * 10000.0 / CONFLIN_u1sTIMERPRESCALER / CONFLIN_u2sBAUDRATE)
#endif /* __LIN_KX__, __LIN_FX_UART60__, __LIN_FX_L__ */

#ifdef __LIN_FX_UART61__
#define CONFLIN_u1sTIMERPRESCALER ((u1)16)
#define CONFLIN_u1sCAPTUREBITSIZE ((u1)8)
#define CONFLIN_f4sBITSTIMECOUNT (CONFLIN_u1sCAPTUREBITSIZE * CONFLIN_u2sPERICLOCK * 10000.0 / CONFLIN_u1sTIMERPRESCALER / CONFLIN_u2sBAUDRATE)
#endif /* __LIN_FX_UART61__ */

#if (defined __LIN_78K0R_FX3__)     || \
    (defined __LIN_RL78_F12__)
#define CONFLIN_u1sTIMERPRESCALER ((u1)4)
#endif /* __LIN_78K0R_FX3__,__LIN_RL78_F12__ */

#if (defined __LIN_FX3_UARTD0__)    || \
    (defined __LIN_FX3_UARTD1__)    || \
    (defined __LIN_FX3_UARTD2__)    || \
    (defined __LIN_FX3_UARTD3_P4__) || \
    (defined __LIN_FX3_UARTD3_P8__) || \
    (defined __LIN_FX3_UARTD4__)    || \
    (defined __LIN_FX3_UARTD5__)    || \
    (defined __LIN_FX3_UARTD6__)    || \
    (defined __LIN_FX3_UARTD7__)
#define CONFLIN_u1sTIMERPRESCALER ((u1)8)
#define CONFLIN_u1sCAPTUREBITSIZE ((u1)4)
#define CONFLIN_f4sBITSTIMECOUNT (CONFLIN_u1sCAPTUREBITSIZE * CONFLIN_u2sPERICLOCK * 10000.0 / CONFLIN_u1sTIMERPRESCALER / CONFLIN_u2sBAUDRATE)
#endif /* __LIN_FX3_UARTDn__ */


/* -------------------------------------------------------------------- */
/* Public constants                                                     */
/* -------------------------------------------------------------------- */

#if (!defined __LIN_RL78_F13_F14__) && \
    (!defined __LIN_RL78_F15__)     && \
    (!defined __LIN_RL78_F23_F24__)
const u2 ConfLin_u2gTIMEBASE     = CONFLIN_u2sTIMERBASE;
#endif /* __LIN_RL78_F13_F14__, __LIN_RL78_F15__, !__LIN_RL78_F23_F24__ */

/*----------------------------------------------------------------------*/
/* Response space:                                                      */
/*   You can set a wait time between header field and response field.   */
/*   Process time is added to actual wait time.                         */
/*   The setting value follows the following calculation formula.       */
/*   (xxRESSPACEBASE + (65536 * (xxRESSPACETIME - 1)))                  */
/*                                           / (Fx/xxTIMERPRESCALER)[s] */
/*   Note. If ConfLin_u2gRESSPACETIME = 0, the wait time is zero.       */
/*----------------------------------------------------------------------*/
#if (defined __LIN_KX__)            || \
    (defined __LIN_FX_UART60__)     || \
    (defined __LIN_FX_UART61__)     || \
    (defined __LIN_FX_L__)          || \
    (defined __LIN_78K0R_FX3__)     || \
    (defined __LIN_RL78_F12__)
const u2 ConfLin_u2gRESSPACETIME = (u2)0;
const u2 ConfLin_u2gRESSPACEBASE = (u2)1;           /* Setting range is 0x1-0xFFFF  */
#endif /* __LIN_KX__, __LIN_FX_UART60__, __LIN_FX_UART61__, __LIN_FX_L__, __LIN_78K0R_FX3__,__LIN_RL78_F12__ */

#if (defined __LIN_FX3_UARTD0__)    || \
    (defined __LIN_FX3_UARTD1__)    || \
    (defined __LIN_FX3_UARTD2__)    || \
    (defined __LIN_FX3_UARTD3_P4__) || \
    (defined __LIN_FX3_UARTD3_P8__) || \
    (defined __LIN_FX3_UARTD4__)    || \
    (defined __LIN_FX3_UARTD5__)    || \
    (defined __LIN_FX3_UARTD6__)    || \
    (defined __LIN_FX3_UARTD7__)
const u2 ConfLin_u2gRESSPACETIME = (u2)1;                                                              /* Set up within 0x1-0xFFFF */
const u2 ConfLin_u2gRESSPACEBASE = (u2)(((((u4)53 * (u4)CONFLIN_u2sPERICLOCK) + (u4)799) / (u4)800) % (u4)65536);   /* About 53 us */
#endif /* __LIN_FX3_UARTDn__ */

/*----------------------------------------------------------------------*/
/* Time out:                                                            */
/*   xxxBUSTIMEOUT =                                                    */
/*          [BUSTIMEOUT*(Fx*10000/xxTIMERPRESCALER) / 65536 * 1.14 + 2] */
/*   Considering oscillation error, which is 14%,                       */
/*   the value is multiplied by 1.14.                                   */
/*   Additionally, considering omission of fraction made by division    */
/*   and one counter reset, 2 is added to the value.                    */
/*   Note. If ConfLin_u2gBUSTIMEOUT = 0, the bus off isn't detected.    */
/*----------------------------------------------------------------------*/
#if (defined __LIN_RL78_F13_F14__)  || \
    (defined __LIN_RL78_F15__)      || \
    (defined __LIN_RL78_F23_F24__)
 #if ( CONFLIN_u1sBUSTIMEOUT == 0 )
const u4 ConfSLin_u4gBUSTIMEOUT = (u4)0;
 #else
const u4 ConfSLin_u4gBUSTIMEOUT = (u4)CONFLIN_u1sBUSTIMEOUT * (u4)114;
 #endif
#else /* __LIN_RL78_F13_F14__, __LIN_RL78_F15__, __LIN_RL78_F23_F24__ */
 #if ( CONFLIN_u1sBUSTIMEOUT == 0 )
const u2 ConfLin_u2gBUSTIMEOUT = 0;
 #else
const u2 ConfLin_u2gBUSTIMEOUT = ((u2)((((u4)CONFLIN_u1sBUSTIMEOUT * ((u4)CONFLIN_u2sPERICLOCK / (u4)CONFLIN_u1sTIMERPRESCALER) * (u4)1140)
                                         / ((u4)CONFLIN_u2sTIMERBASE + (u4)1)) + (u4)2));
 #endif /* CONFLIN_u1sBUSTIMEOUT */
#endif /* __LIN_RL78_F13_F14__, __LIN_RL78_F15__, __LIN_RL78_F23_F24__ */

/*----------------------------------------------------------------------*/
/* Setting for baud rate generator                                      */
/*   Following peripheral hardware clock and baud rate value,           */
/*   setting values for the registers related to baud rate are selected.*/
/*   Note. In the case of 78K0R/Fx3, setting value to the register      */
/*         related to baud rate is the value at 9600bps@24MHz.          */
/*----------------------------------------------------------------------*/
#if (defined __LIN_RL78_F13_F14__)  || \
    (defined __LIN_RL78_F15__)      || \
    (defined __LIN_RL78_F23_F24__)
const u2 ConfSLin_u2gPERICLOCK      = (u2)CONFLIN_u2sPERICLOCK;
const u2 ConfSLin_u2gTMPERICLOCK    = (u2)CONFLIN_u2sPERICLOCK; /* If timer peripheral clock isn't same as for LIN, change here. */
const u2 ConfSLin_u2gBAUDRATENORM   = (u2)CONFLIN_u2sBAUDRATE;
#endif /* __LIN_RL78_F13_F14__, __LIN_RL78_F15__, __LIN_RL78_F23_F24__ */

#ifdef __LIN_RL78_F12__
 #if ( CONFLIN_u2sBAUDRATE == 0 )
  #if ( CONFLIN_u2sPERICLOCK < 1600 )              /* Setting for using automatic baud rate detection option */
    #define ConfSLin_u2gUFCTL1INIT_PRS ((u2)0)
    #define ConfSLin_u2gUFCTL1INIT_BRS ((u2)625)
  #elif ( CONFLIN_u2sPERICLOCK < 3200 )
    #define ConfSLin_u2gUFCTL1INIT_PRS ((u2)1)
    #define ConfSLin_u2gUFCTL1INIT_BRS ((u2)625)
  #else
    #define ConfSLin_u2gUFCTL1INIT_PRS ((u2)2)
    #define ConfSLin_u2gUFCTL1INIT_BRS ((u2)417)
  #endif
 #else /* CONFLIN_u2sBAUDRATE == 0 */
  #error CONFLIN_u2sBAUDRATE must be 0 in RL78/F12.
 #endif /* CONFLIN_u2sBAUDRATE == 0 */
const u2 ConfLin_u2gUFCTL1INIT = ((u2)(ConfSLin_u2gUFCTL1INIT_PRS << 13)) | ConfSLin_u2gUFCTL1INIT_BRS;
#endif /* __LIN_RL78_F12__ */

#ifdef __LIN_78K0R_FX3__
 #if ( CONFLIN_u2sBAUDRATE == 0 )                 /* Setting for using automatic baud rate detection option */
  #if ( CONFLIN_u2sPERICLOCK < 1600 )
    #define ConfSLin_u2gUFCTL1INIT_PRS ((u2)0)
    #define ConfSLin_u2gUFCTL1INIT_BRS ((u2)625)
  #else
    #define ConfSLin_u2gUFCTL1INIT_PRS ((u2)1)
    #define ConfSLin_u2gUFCTL1INIT_BRS ((u2)625)
  #endif
 #else
  #define ConfSLin_u2gUFCTL1INIT_PRS ((u2)1)
  #define ConfSLin_u2gUFCTL1INIT_BRS ((u2)625)
#endif /* CONFLIN_u2sBAUDRATE */
const u2 ConfLin_u2gUFCTL1INIT = ((u2)(ConfSLin_u2gUFCTL1INIT_PRS << 13)) | ConfSLin_u2gUFCTL1INIT_BRS;
#endif /* __LIN_78K0R_FX3__ */

#if (defined __LIN_KX__)            || \
    (defined __LIN_FX_UART60__)     || \
    (defined __LIN_FX_UART61__)     || \
    (defined __LIN_FX_L__)          || \
    (defined __LIN_FX3_UARTD0__)    || \
    (defined __LIN_FX3_UARTD1__)    || \
    (defined __LIN_FX3_UARTD2__)    || \
    (defined __LIN_FX3_UARTD3_P4__) || \
    (defined __LIN_FX3_UARTD3_P8__) || \
    (defined __LIN_FX3_UARTD4__)    || \
    (defined __LIN_FX3_UARTD5__)    || \
    (defined __LIN_FX3_UARTD6__)    || \
    (defined __LIN_FX3_UARTD7__)
/* Peripheral hardware clock: 16MHz, Baud rate: 9600bps */
const u1 ConfLin_u1gCKSRINIT     = (u1)0x02;       /* Baud rate selection 1  */
const u1 ConfLin_u1gBRGCINIT     = (u1)0xD0;       /* Baud rate selection 2  */
#endif /* __LIN_KX__, __LIN_FX_UART60__, __LIN_FX_UART61__, __LIN_FX_L__, __LIN_FX3_UARTDn__ */

/*----------------------------------------------------------------------*/
/* Setting of error threshold for the function checking error of clock  */
/*   Maximum and minimum value of error tolerance for the function      */
/*   checking error of clock are set depending on                       */
/*   peripheral hardware clock and baud rate. (+-14%)                   */
/*  ConfLin_u2gSynchCntMin: Minimum value of timer counting             */
/*                          bit width to be measured                    */
/*  ConfLin_u2gSynchCntMax: Maximum value of timer counting             */
/*                          bit width to be measured                    */
/*  Note. This setting is only using by the library with the option     */
/*        enabling the function detecting baud rate automatically.      */
/*----------------------------------------------------------------------*/
#if (defined __LIN_KX__)            || \
    (defined __LIN_FX_UART60__)     || \
    (defined __LIN_FX_UART61__)     || \
    (defined __LIN_FX_L__)          || \
    (defined __LIN_FX3_UARTD0__)    || \
    (defined __LIN_FX3_UARTD1__)    || \
    (defined __LIN_FX3_UARTD2__)    || \
    (defined __LIN_FX3_UARTD3_P4__) || \
    (defined __LIN_FX3_UARTD3_P8__) || \
    (defined __LIN_FX3_UARTD4__)    || \
    (defined __LIN_FX3_UARTD5__)    || \
    (defined __LIN_FX3_UARTD6__)    || \
    (defined __LIN_FX3_UARTD7__)
 #if ( CONFLIN_u2sBAUDRATE != 0 )
    const u2 ConfLin_u2gSynchCntMin = (u2)(CONFLIN_f4sBITSTIMECOUNT * 0.86);
    const u2 ConfLin_u2gSynchCntMax = (u2)(CONFLIN_f4sBITSTIMECOUNT * 1.14);
 #endif /* CONFLIN_u2sBAUDRATE */
#endif /* __LIN_KX__, __LIN_FX_UART60__, __LIN_FX_UART61__, __LIN_FX_L__, __LIN_FX3_UARTDn__ */

/*----------------------------------------------------------------------*/
/* Retry time interval of wake up transmission:                         */
/*  ConfLin_u2gCRWAKEUP  :The residue of total timer count to base      */
/*                        timer cycle. That timer count is based on     */
/*                        retry time interval of wake up transmission.  */
/*  ConfLin_u2gTIMEWAKEUP:The quotient in the expression that timer     */
/*                        count is divided by timer cycle + 1.          */
/*  ConfLin_u2gCRWAKEUPAF3  :The residue of total timer count to base   */
/*                           timer cycle. That timer count is based on  */
/*                           third retry time interval of wake up       */
/*                           transmission.                              */
/*  ConfLin_u2gTIMEWAKEUPAF3:The quotient in the expression that timer  */
/*                           count is divided by timer cycle + 1. That  */
/*                           timer count is based on third retry time   */
/*                           interval of wake up transmission.          */
/*   The setting value follows the following calculation formula.       */
/*    (xxCRWAKEUPxx + xxTIMEWAKEUPxx * 65536) * (4/Fx or 16/Fx)[s]      */
/*  In default setting, WAKEUP = 150ms, and WAKEUPAF3 = 1.5s            */
/*----------------------------------------------------------------------*/
#if (!defined __LIN_RL78_F13_F14__)  && \
    (!defined __LIN_RL78_F15__)      && \
    (!defined __LIN_RL78_F23_F24__)
const u2 ConfLin_u2gCRWAKEUP     = (u2)(((u4)1710 * (u4)CONFLIN_u2sPERICLOCK / (u4)CONFLIN_u1sTIMERPRESCALER) % (u4)65536);               /* 0-0xFFFF */
const u2 ConfLin_u2gTIMEWAKEUP   = (u2)((((u4)1710 * (u4)CONFLIN_u2sPERICLOCK / (u4)CONFLIN_u1sTIMERPRESCALER) / (u4)65536) + (u4)1);     /* 1-0xFFFF: Setting 0 is prohibited */
const u2 ConfLin_u2gCRWAKEUPAF3   = (u2)(((u4)17100 * (u4)CONFLIN_u2sPERICLOCK / (u4)CONFLIN_u1sTIMERPRESCALER) % (u4)65536);             /* 0-0xFFFF */
const u2 ConfLin_u2gTIMEWAKEUPAF3 = (u2)((((u4)17100 * (u4)CONFLIN_u2sPERICLOCK / (u4)CONFLIN_u1sTIMERPRESCALER) / (u4)65536 ) + (u4)1);  /* 1-0xFFFF: Setting 0 is prohibited */
#endif /* __LIN_RL78_F13_F14__, __LIN_RL78_F15__, !__LIN_RL78_F23_F24__ */

const u1 ConfLin_u1gMESNUM       = CONFLIN_u1sMESSAGENUM;
const u1 ConfLin_u1gSIGNUM       = CONFLIN_u1sSIGNALNUM;
const u1 ConfLin_u1gEVENTNUM     = CONFLIN_u1sEVENTMESSAGENUM;
 #if (defined __LIN_RL78_F23_F24__)
#if ( CONFLIN_u1sWAKEUPMAXRETRY > 80 )
 #error [conflin.c] Set the value from 0 to 80.
#endif
const u1 ConfLin_u1gWKMAX        = (CONFLIN_u1sWAKEUPMAXRETRY + (u1)1) * (u1)3;
 #endif /* __LIN_RL78_F23_F24__ */
#if (!defined __LIN_RL78_F13_F14__)  && \
    (!defined __LIN_RL78_F15__)      && \
    (!defined __LIN_RL78_F23_F24__)
const u1 ConfLin_u1gAPITMPMESNO  = CONFLIN_u1sMESSAGENUM;
const u1 ConfLin_u1gCORETMPMESNO = CONFLIN_u1sMESSAGENUM + 1;
#endif /* __LIN_RL78_F13_F14__, __LIN_RL78_F15__, !__LIN_RL78_F23_F24__ */


/* -------------------------------------------------------------------- */
/* Public variables                                                     */
/* -------------------------------------------------------------------- */
ConfLin_stMesBuf ConfLin_stgMesBuf[CONFLIN_u1sMESSAGENUM + 2];

#if ( CONFLIN_u1sEVENTMESSAGENUM == 0 )
u1 ConfLin_u1gEventPid[1];
#else
u1 ConfLin_u1gEventPid[CONFLIN_u1sEVENTMESSAGENUM];
#endif

u1 ConfLin_u1gMesPid[CONFLIN_u1sMESSAGENUM];
u1 ConfLin_u1gMesNoTbl[CONFLIN_u1sMESSAGENUM + 2];  /* Two temporary domains for concurrent access evasion are added   */
#ifdef __LIN_SIGNAL_32__
u4 ConfLin_u4gMesSig[CONFLIN_u1sMESSAGENUM];
u4 ConfLin_u4gMesSubSig[CONFLIN_u1sMESSAGENUM];
#else
u2 ConfLin_u2gMesSig[CONFLIN_u1sMESSAGENUM];
u2 ConfLin_u2gMesSubSig[CONFLIN_u1sMESSAGENUM];
#endif /* __LIN_SIGNAL_32__ */


/* End of files */
