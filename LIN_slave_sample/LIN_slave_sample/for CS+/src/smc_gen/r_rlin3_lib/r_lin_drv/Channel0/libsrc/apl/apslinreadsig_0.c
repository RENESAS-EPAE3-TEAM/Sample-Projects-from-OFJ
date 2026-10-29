/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: apslinreadsig.c                                           */
/* Version:   0.04                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.08.05  Comment of u2sReadSig was fixed.                */
/*                      CH addinf macro for u2sReadSig was fixed.       */
/*   V0.03: 2021.05.31  IAR Compiler support                            */
/*   V0.04: 2024.08.30  Modified for RL78/F23,F24.                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Includes before pragma declarations                                  */
/* -------------------------------------------------------------------- */
  #ifdef __CCRL__
#include "../dev/section.h"
  #endif /* __CCRL__ */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define ApSLin_u2sReadSig   ApSLin_u2sReadSig_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define ApSLin_u2sReadSig   ApSLin_u2sReadSig_1
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
static u2 ApSLin_u2sReadSig( u1 );

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
/* Description : Read a 8 bit signal                                    */
/* Parameters  : u1tSigName = signal name                               */
/* Return value: 8 bit signal data                                      */
/*               Return zero if the parameter is invalid.               */
/* Misc.       : This function is an alias of LIN API l_bool_rd,        */
/*               l_u8_rd.                                               */
/* -------------------------------------------------------------------- */
u1
ApSLin_u1gRead8bitsSig( u1 u1tSigName )
{

    return (u1)ApSLin_u2sReadSig( u1tSigName );
}


/* -------------------------------------------------------------------- */
/* Description : Read a 16 bit signal                                   */
/* Parameters  : u1tSigName = signal name                               */
/* Return value: 16 bit signal data                                     */
/*               Return zero if the parameter is invalid.               */
/* Misc.       : This function is an alias of the LIN API l_u16_rd.     */
/* -------------------------------------------------------------------- */
u2
ApSLin_u2gRead16bitsSig( u1 u1tSigName )
{

    return ApSLin_u2sReadSig( u1tSigName );
}


/* -------------------------------------------------------------------- */
/* Internal functions                                                   */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Description : Returns the value of the signal                        */
/* Parameters  : u1tSigName = signal name                               */
/* Return value: 16 bit signal data                                     */
/* Misc.       : As this function is only for API, locate here instead  */
/*               LCoreSMes.                                             */
/*               This process has the period of suspending interrupts.  */
/* -------------------------------------------------------------------- */
static u2
ApSLin_u2sReadSig( u1 u1tSigName )
{
    u1 u1tIrq;
    u2 u2tStore;
    const ConfSLin_stSignal *psttSignal;
    u1 *pu1tMesData;

    u2tStore = (u2)0x0000U;

    if( u1tSigName < ConfSLin_u1gSIGNUM ) {
        psttSignal = &ConfSLin_stgSIGNAL[u1tSigName];

        if( (psttSignal -> u1BitSize) <= (u1)16 ) {
                                                /* Get the pointer to read */

            pu1tMesData = LCoreSMes_pu1gGetSigMesAddr( u1tSigName );
            pu1tMesData = &pu1tMesData[psttSignal -> u1OffsetByte];


            u1tIrq =  ConfSLin_vogCallDisableIrq();
            /*-----------------------*/
            /*   Disable interrupt   */
            /*                       */


            u2tStore = (u2)*pu1tMesData;        /* Read lower byte           */
            if( (psttSignal -> u1OffsetByte) < (u1)7 ) {
                                                /* Read higher byte          */
                u2tStore |= (u2)( (u2)*(pu1tMesData + 1) << 8 );
            }


            /*                       */
            /*   Enable interrupt    */
            /*-----------------------*/
            ConfSLin_vogCallRestoreIrq( u1tIrq );


            u2tStore >>= (psttSignal -> u1OffsetBit);
            u2tStore &= (u2)~(u2)( (u2)0xFFFFU << (psttSignal -> u1BitSize) );
        }
    }

    return u2tStore;
}
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif


/* End of files */
