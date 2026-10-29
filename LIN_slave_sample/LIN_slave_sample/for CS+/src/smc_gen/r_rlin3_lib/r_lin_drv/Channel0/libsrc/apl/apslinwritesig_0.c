/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: apslinwritesig.c                                          */
/* Version:   0.03                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2021.05.31  IAR Compiler support                            */
/*   V0.03: 2024.08.30  Modified for RL78/F23,F24.                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Includes before pragma declarations                                  */
/* -------------------------------------------------------------------- */
  #ifdef __CCRL__
#include "../dev/section.h"
  #endif /* __CCRL__ */

/* -------------------------------------------------------------------- */
/* Channel abstraction macro for static symbols and interrupt functions */
/* -------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)        /* Use LIN channel 0 */
#define ApSLin_vosWriteSig  ApSLin_vosWriteSig_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)       /* Use LIN channel 1 */
#define ApSLin_vosWriteSig  ApSLin_vosWriteSig_1
#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Pragma declaration                                                   */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../com/common.h"
#include "../conf/confslin.h"
#include "./apslin.h"
#include "../lcore/lcoresmes.h"

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Function prototype declaration                                       */
/* -------------------------------------------------------------------- */
static void ApSLin_vosWriteSig( u1, u2 );      /* Write 2 byte data */

/* -------------------------------------------------------------------- */
/* Constant declaration                                                 */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Variable declaration                                                 */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes = @ "LSCODE"
#endif

/* -------------------------------------------------------------------- */
/* Description : Write a 8 bit signal                                   */
/* Parameters  : u1tSigName = signal name                               */
/*               u1tData    = write data                                */
/* Return value: Nothing                                                */
/* Misc.       : This process has the period of suspending interrupts.  */
/* -------------------------------------------------------------------- */
void
ApSLin_vogWrite8bitsSig( u1 u1tSigName, u1 u1tData )
{
    ApSLin_vosWriteSig( u1tSigName, (u2)u1tData );

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Write a 16 bit signal                                  */
/* Parameters  : u1tSigName = signal name                               */
/*               u2tData    = write data                                */
/* Return value: Nothing                                                */
/* Misc.       : This process has the period of suspending interrupts.  */
/* -------------------------------------------------------------------- */
void
ApSLin_vogWrite16bitsSig(u1 u1tSigName, u2 u2tData)
{
    ApSLin_vosWriteSig( u1tSigName, u2tData );

    return;
}


/* -------------------------------------------------------------------- */
/* Internal functions                                                   */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Description : Write 2 byte                                           */
/* Parameters  : u1tSigName = signal name                               */
/*               u2tData = 2 byte data                                  */
/* Return value: Nothing                                                */
/* Misc.       : This process has the period of suspending interrupts.  */
/*               As this function is only for API, locate here instead  */
/*               LCoreSMes.                                             */
/* -------------------------------------------------------------------- */
static void
ApSLin_vosWriteSig( u1 u1tSigName, u2 u2tData )
{
    u2 u2tStore;
    u2 u2tMask;
    const ConfSLin_stSignal *psttSignal;
    u1 *pu1tMesData;
    u1 u1tIrq;


    if( u1tSigName < ConfSLin_u1gSIGNUM) {
        psttSignal = &ConfSLin_stgSIGNAL[u1tSigName];

        if( psttSignal->u1BitSize <= (u1)16 ) {                         /* Data is not written if u1BitSize is zero */

            u2tMask = ~(u2)( (u2)0xFFFFU << psttSignal->u1BitSize );    /* ex: if bit-size is 4 -> 00001111b */
            u2tData &= u2tMask;                                         /* Mask outside of target area       */
            u2tData <<= psttSignal->u1OffsetBit;                        /* Adjust a offset                   */
            u2tMask = ~(u2)( u2tMask << psttSignal->u1OffsetBit );      /* ex: if offset is 2 -> 11000011b   */

            pu1tMesData = LCoreSMes_pu1gGetSigMesAddr( u1tSigName );
            pu1tMesData = &pu1tMesData[psttSignal -> u1OffsetByte];


            u1tIrq =  ConfSLin_vogCallDisableIrq();
            /*-----------------------*/
            /*   Disable interrupt   */
            /*                       */


            /* Store existing data in the local variable */
            u2tStore = (u2)*pu1tMesData;                                /* Higher byte */
            if( (psttSignal -> u1OffsetByte) < (u1)7 ) {
                u2tStore |= (u2)( (u2)*(pu1tMesData + 1) << 8 );        /* Lower byte  */
            }

            /* Write back(Only target bit) */
            u2tStore = ( u2tStore & u2tMask ) | u2tData;
            
            if( psttSignal->u1OffsetByte < (u1)7 ) {
                *(pu1tMesData + 1) = (u1)(u2tStore >> 8);               /* Higher byte */
            }

            *pu1tMesData = (u1)u2tStore;                                /* Lower byte  */

            /* Set update signal flag */
            LCoreSMes_vogSetSigFlag( u1tSigName );


            /*                       */
            /*   Enable interrupt    */
            /*-----------------------*/
            ConfSLin_vogCallRestoreIrq( u1tIrq );
        }
    }


    return;
}
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif

/* End of files */
