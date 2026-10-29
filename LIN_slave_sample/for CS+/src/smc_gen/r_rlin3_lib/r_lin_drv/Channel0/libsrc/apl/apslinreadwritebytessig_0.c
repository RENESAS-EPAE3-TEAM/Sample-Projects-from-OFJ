/* -------------------------------------------------------------------- */
/* Copyright(c) 2009 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: apslinreadwritebytessig.c                                 */
/* Version:   0.04                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.08.05  Comment of u1sCheckByteArray was fixed.         */
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
/* Channel abstraction macro for static symbols and interrupt functions */
/* -------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)        /* Use LIN channel 0 */
#define ApSLin_u1sCheckByteArray    ApSLin_u1sCheckByteArray_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)       /* Use LIN channel 1 */
#define ApSLin_u1sCheckByteArray    ApSLin_u1sCheckByteArray_1
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
#include "../drv/drvscom.h"
#include "../lcore/lcorescom.h"
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
static u1 ApSLin_u1sCheckByteArray( u1, u1, const ConfSLin_stSignal* ); /* Check a byte array */

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
/* Description : Read a byte array                                      */
/* Parameters  : u1tSigName = signal name                               */
/*               u1tStart = read byte index                             */
/*               u1tCount = read byte size                              */
/*               pu1tData = pointer to read value                       */
/* Return value: Nothing                                                */
/* Misc.       : This function is an alias of LIN API l_bytes_rd.       */
/*               This process has the period of suspending interrupts.  */
/* -------------------------------------------------------------------- */
void
ApSLin_vogReadBytesSig( u1 u1tSigName, u1 u1tStart, u1 u1tCount, u1* const pu1tData )
{
    u1 u1tIrq;
    u1 *pu1tMesData;
    const ConfSLin_stSignal *psttSignal;


    if( u1tSigName < ConfSLin_u1gSIGNUM ) {
        
        psttSignal = &(ConfSLin_stgSIGNAL[u1tSigName]);

        if( ApSLin_u1sCheckByteArray( u1tStart, u1tCount, psttSignal ) == (u1)OK ) {

            /* Set the start address to the pointer */
            pu1tMesData = LCoreSMes_pu1gGetSigMesAddr( u1tSigName );
            pu1tMesData = &(pu1tMesData[(psttSignal -> u1OffsetByte) + u1tStart]);


            u1tIrq =  ConfSLin_vogCallDisableIrq();
            /*-----------------------*/
            /*   Disable interrupt   */
            /*                       */


            /* Copy data( pu1tMesData -> pu1tData ) */
            LDrvSCom_vogCopyBuf( pu1tData, pu1tMesData, u1tCount );


            /*                       */
            /*   Enable interrupt    */
            /*-----------------------*/
            ConfSLin_vogCallRestoreIrq( u1tIrq );

        }
    }


    return;
}

/* -------------------------------------------------------------------- */
/* Description : Write a byte array                                     */
/* Parameters  : u1tSigName = signal name                               */
/*               u1tStart = write byte index                            */
/*               u1tCount = write byte size                             */
/*               pu1tData = pointer to source data                      */
/* Return value: Nothing                                                */
/* Misc.       : This function is an alias of LIN API l_bytes_wr.       */
/*               This process has the period of suspending interrupts.  */
/* -------------------------------------------------------------------- */
void
ApSLin_vogWriteBytesSig( u1 u1tSigName, u1 u1tStart, u1 u1tCount, const u1* const pu1tData )
{
    u1 u1tIrq;
    u1 *pu1tMesData;
    const ConfSLin_stSignal *psttSignal;


    if( u1tSigName < ConfSLin_u1gSIGNUM ) {

        psttSignal = &(ConfSLin_stgSIGNAL[u1tSigName]);

        if( ApSLin_u1sCheckByteArray( u1tStart, u1tCount, psttSignal ) == (u1)OK ) {

            /* Set the start pointer reading  */
            pu1tMesData = LCoreSMes_pu1gGetSigMesAddr( u1tSigName );
            pu1tMesData = &(pu1tMesData[(psttSignal -> u1OffsetByte) + u1tStart]);


            u1tIrq =  ConfSLin_vogCallDisableIrq();
            /*-----------------------*/
            /*   Interrupt pending   */
            /*                       */


            /* Copy data( pu1tData -> pu1tMesData ) */
            LDrvSCom_vogCopyBuf( pu1tMesData, pu1tData, u1tCount );

            /* Set signal update flag */
            LCoreSMes_vogSetSigFlag( u1tSigName );


            /*                       */
            /*   Interrupt return    */
            /*-----------------------*/
            ConfSLin_vogCallRestoreIrq( u1tIrq );
        }
    }


    return;
}

/* -------------------------------------------------------------------- */
/* Internal functions                                                   */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Description : Check a byte array                                     */
/* Parameters  : u1tStart   = byte index                                */
/*               u1tCount   = byte size                                 */
/*               psttSignal = pointer to signal                         */
/* Return value: Process result                                         */
/*               OK    = OK                                             */
/*               ERROR = parameter error                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
static u1
ApSLin_u1sCheckByteArray( u1 u1tStart, u1 u1tCount, const ConfSLin_stSignal *psttSignal )
{
    u1 u1tResult;


    if( ((u1tStart <= (u1)7) && (u1tCount <= (u1)8))
     && ((u1tStart + u1tCount) <= ((psttSignal -> u1BitSize) >> (u1)3)) /* Check the parameter by the byte               */
     && (((psttSignal -> u1OffsetBit) & (u1)0x07U) == (u1)0x00U)        /* Check whether the offset is on byte boundary  */
     && (((psttSignal -> u1BitSize)   & (u1)0x07U) == (u1)0x00U) )      /* Check whether the bit-size is multiplies of 8 */
    {
        u1tResult = (u1)OK;
    }
    else
    {
        u1tResult = (u1)ERROR;
    }

    return u1tResult;
}
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif


/* End of files */
