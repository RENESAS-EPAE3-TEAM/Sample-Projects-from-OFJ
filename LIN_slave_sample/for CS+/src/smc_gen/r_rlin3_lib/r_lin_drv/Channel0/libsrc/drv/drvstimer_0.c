/* -------------------------------------------------------------------- */
/* Copyright(c) 2009-2025 Renesas Electronics Corporation               */
/* -------------------------------------------------------------------- */
/* File name: drvstimer.c                                               */
/* Version:   0.08                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.08.22  The operation for WAITHEADERDOM and WAKEUPDOM   */
/*                      was added into LDrvSTimer_vogVecCompare.        */
/*   V0.03: 2015.02.02  Processing corresponding to the same            */
/*                      restriction was deleted by lifting              */
/*                      restrictions of FTS bit set timing of RLIN3.    */
/*                      Macro definition and function name of the       */
/*                      [Public function]                               */
/*                      - LDrvSTimer_vogVecCompare                      */
/*                        - Processing was corrected.                   */
/*   V0.04: 2016.06.14  CS+ CC-RL support.                              */
/*   V0.05: 2021.05.31  IAR Compiler, RL78/F2x support                  */
/*   V0.06: 2024.08.30  Modified for RL78/F23,F24.                      */
/*   V0.07: 2025.07.16  Timeout more than 1 cycle is correctly mesured. */
/*   V0.08: 2025.07.25  Header Rx operation in response is modified.    */
/*                      (RespErr is detected only after Rx 1st data)    */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Includes before pragma declarations                                  */
/* -------------------------------------------------------------------- */
#include "../dev/device.h"
  #ifdef __CCRL__
#include "../dev/section.h"
  #endif /* __CCRL__ */

/* -------------------------------------------------------------------- */
/* Including header files (must be defined before pragma)               */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Channel abstraction macro for static symbols and interrupt functions */
/* -------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define LDrvSTimer_u2sTimeOut               LDrvSTimer_u2sTimeOut_0
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define LDrvSTimer_u2sTimeOut               LDrvSTimer_u2sTimeOut_1
#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Pragma declaration                                                   */
/* -------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
 #if   (CONFSLIN_OPT_u1gTMUNIT_CFG == CONFSLIN_OPT_u1gTMUNIT_UNIT0)

  #ifdef __CCRL__
   #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM00)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM01)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM02)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM03)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM04)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM05)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM06)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM07)
   #endif /* CONFSLIN_OPT_u1gTMCH_CFG */
  #endif /* __CCRL__ */


  #ifdef __IAR_SYSTEMS_ICC__
   #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#define INTTMmn_vect      INTTM00_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#define INTTMmn_vect      INTTM01_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#define INTTMmn_vect      INTTM02_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#define INTTMmn_vect      INTTM03_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#define INTTMmn_vect      INTTM04_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#define INTTMmn_vect      INTTM05_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#define INTTMmn_vect      INTTM06_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#define INTTMmn_vect      INTTM07_vect
   #endif /* CONFSLIN_OPT_u1gTMCH_CFG */
  #endif /* __IAR_SYSTEMS_ICC__ */



 #elif (CONFSLIN_OPT_u1gTMUNIT_CFG == CONFSLIN_OPT_u1gTMUNIT_UNIT1)

  #ifdef __CCRL__
   #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM10)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM11)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM12)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM13)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM14)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM15)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM16)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#pragma interrupt   LDrvSTimer_vogVecCompare_0      (vect=INTTM17)
   #endif /* CONFSLIN_OPT_u1gTMCH_CFG */
  #endif /* __CCRL__ */

  #ifdef __IAR_SYSTEMS_ICC__
   #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#define INTTMmn_vect      INTTM10_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#define INTTMmn_vect      INTTM11_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#define INTTMmn_vect      INTTM12_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#define INTTMmn_vect      INTTM13_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#define INTTMmn_vect      INTTM14_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#define INTTMmn_vect      INTTM15_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#define INTTMmn_vect      INTTM16_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#define INTTMmn_vect      INTTM17_vect
   #endif /* CONFSLIN_OPT_u1gTMCH_CFG */
  #endif /* __IAR_SYSTEMS_ICC__ */

 #endif /* CONFSLIN_OPT_u1gTMUNIT_CFG */
#endif /* __LIN_CH0__ */

#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
 #if   (CONFSLIN_OPT_u1gTMUNIT_CFG == CONFSLIN_OPT_u1gTMUNIT_UNIT0)


  #ifdef __CCRL__
   #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM00)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM01)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM02)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM03)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM04)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM05)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM06)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM07)
   #endif /* CONFSLIN_OPT_u1gTMCH_CFG */
  #endif /* __CCRL__ */

  #ifdef __IAR_SYSTEMS_ICC__
   #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#define INTTMmn_vect      INTTM00_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#define INTTMmn_vect      INTTM01_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#define INTTMmn_vect      INTTM02_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#define INTTMmn_vect      INTTM03_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#define INTTMmn_vect      INTTM04_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#define INTTMmn_vect      INTTM05_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#define INTTMmn_vect      INTTM06_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#define INTTMmn_vect      INTTM07_vect
   #endif /* CONFSLIN_OPT_u1gTMCH_CFG */
  #endif /* __IAR_SYSTEMS_ICC__ */


 #elif (CONFSLIN_OPT_u1gTMUNIT_CFG == CONFSLIN_OPT_u1gTMUNIT_UNIT1)


  #ifdef __CCRL__
   #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM10)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM11)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM12)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM13)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM14)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM15)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM16)
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#pragma interrupt   LDrvSTimer_vogVecCompare_1      (vect=INTTM17)
   #endif /* CONFSLIN_OPT_u1gTMCH_CFG */
  #endif /* __CCRL__ */

  #ifdef __IAR_SYSTEMS_ICC__
   #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#define INTTMmn_vect      INTTM10_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#define INTTMmn_vect      INTTM11_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#define INTTMmn_vect      INTTM12_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#define INTTMmn_vect      INTTM13_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#define INTTMmn_vect      INTTM14_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#define INTTMmn_vect      INTTM15_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#define INTTMmn_vect      INTTM16_vect
   #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#define INTTMmn_vect      INTTM17_vect
   #endif /* CONFSLIN_OPT_u1gTMCH_CFG */
  #endif /* __IAR_SYSTEMS_ICC__ */

 #endif /* CONFSLIN_OPT_u1gTMUNIT_CFG */
#endif /* __LIN_CH1__ */

/* -------------------------------------------------------------------- */
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../com/common.h"
#include "../conf/confslin.h"
#include "./drvsext.h"
#include "./drvstimer.h"
#include "./drvsuart.h"
#include "../lcore/lcorescom.h"
#include "../lcore/lcoreshead.h"
#include "../lcore/lcoressleep.h"
#include "../lcore/lcoresstat.h"
#include "../lcore/lcoresresp.h"

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
#define LDRVSTIMER_u2sTIMERBASE     (0xFFFFU)
#define LDRVSTIMER_u2sIS_1STCYCLE   (0x8000U)
#define LDRVSTIMER_u2sTMR_MD0       (0x0001U)

/* -------------------------------------------------------------------- */
/* Function prototype declaration                                       */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Constant declaration                                                 */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Variable declaration                                                 */
/* -------------------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_variable_attributes = @ "LSDATA"
#endif
static u2 LDrvSTimer_u2sTimeOut;        /* Time-out count [Time]        */
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
/* Description : Initialize the timer driver                            */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LDrvSTimer_vogInitAll( void )
{

    /* Set the interrupt priority */
    DEV_b1gINTVLTMPR0 =  ((u1)CONFSLIN_OPT_u1gINTTMPR_CFG & (u1)0x01U);
    DEV_b1gINTVLTMPR1 = (((u1)CONFSLIN_OPT_u1gINTTMPR_CFG & (u1)0x02U) >> 1);


    /* (Supplement) */
    /* Considering stopping the TAU clock supply while the driver is sleeping,  */
    /* this function does not access the register of TAU macros.                */
    /* (Because this function is not called upon wake-up)                       */

    
    return;
}


/* -------------------------------------------------------------------- */
/* Description : Refresh timer driver register                          */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LDrvSTimer_vogRefreshReg( void )
{

    /* Set the interrupt priority */
    DEV_b1gINTVLTMPR0 =  ((u1)CONFSLIN_OPT_u1gINTTMPR_CFG & (u1)0x01U);
    DEV_b1gINTVLTMPR1 = (((u1)CONFSLIN_OPT_u1gINTTMPR_CFG & (u1)0x02U) >> 1);


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Start the timer clock supply and initialize            */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : This function does not start the timer count.          */
/* -------------------------------------------------------------------- */
void
LDrvSTimer_vogPowerOnAndInit( void )
{

    /* (Supplement) */
    /* Because there is a possibility that the user is using the other channels, */
    /* clock control should not be set here.                                     */
    /* This function process only start clock supplying,                         */
    if( DEV_b1gINTVLTAUEN == (u1)0 ) {
        DEV_b1gINTVLTAUEN = 1;
    }


    /* Stop timer */
    LDrvSTimer_vogStopTimeout();
    LDrvSTimer_u2sTimeOut = (u2)0;
    

    return;
}


/* -------------------------------------------------------------------- */
/* Description : Start the interval timer                               */
/* Parameters  : u4TimeoutMs : Interval time(LSB:[ms])                  */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LDrvSTimer_vogStartTimeout( u4 u4TimeoutMs )
{
    u4 u4tCount;
    
    /* (Supplement) */
    /* TAU unit selection is done in LDrvSTimer_vogStopTimeout function. */
    /* Therefore it is not done in this functino. */
    
    LDrvSTimer_vogStopTimeout();
    

    /* (Supplement) */
    /* The timer clock divider value setting and the timer mode setting below are processed in this */
    /* function also as a refreshing.                                                               */
    /* (Because state of the timer is unknown in the execution timing of LDrvSTimer_vogRefreshReg,  */
    /* refreshing the function of 2 above is impossible.)                                           */


    /* Set the timer clock divider value */
#if   (CONFSLIN_OPT_u2gTMCLKSEL_CFG == CONFSLIN_OPT_u2gTMCLKSEL_SEL0)
    DEV_u2gINTVLTPS = (DEV_u2gINTVLTPS & (u2)0xFFF0U) |  (u2)CONFSLIN_OPT_u2gTMCLKDIV_CFG;
#elif (CONFSLIN_OPT_u2gTMCLKSEL_CFG == CONFSLIN_OPT_u2gTMCLKSEL_SEL1)
    DEV_u2gINTVLTPS = (DEV_u2gINTVLTPS & (u2)0xFF0FU) | ((u2)CONFSLIN_OPT_u2gTMCLKDIV_CFG << 4);
#endif

    /* Set the timer mode */
    /* (disable external interruption, single channel, 16-bit timer,          */
    /* software trigger, interval timer, no interruption when timer starting) */

    /* At timer start, if it is 1st cycle, the value of compare register is modified to next value  */
    /* in interrupt handler. Because it is too late at timer expiration(due to after loading from   */
    /* compare register to counter) and in case of after starting count, 1st compare register value */
    /* may not be loaded to counter register.                                                       */
    DEV_u2gINTVLTMR = ((u2)CONFSLIN_OPT_u2gTMCLKSEL_CFG | (u2)LDRVSTIMER_u2sTMR_MD0);


    /* If the set time is 0 [ms], the timer does not start */
    if( u4TimeoutMs > (u4)0 ) {
        
        u4tCount = (((u4)ConfSLin_u2gTMPERICLOCK * (u4)10 * u4TimeoutMs) / 
                    ((u4)1 << (u4)CONFSLIN_OPT_u2gTMCLKDIV_CFG));
        
        
        /* Timer laps */
        /* Interrupt at timer starting is always used because the value of TMR is commonly used not */
        /* depends on laps. Therefore 1st cycle flag(LDRVSTIMER_u2sIS_1STCYCLE) is always set.      */
        if( u4tCount <= (u2)LDRVSTIMER_u2sTIMERBASE ) {
            
            /* (In timer interrupt handler, this value is compared after minus 1) */
            LDrvSTimer_u2sTimeOut = ((u2)1 | (u2)LDRVSTIMER_u2sIS_1STCYCLE);
            DEV_u2gINTVLTDR       = (u2)u4tCount;
        }
        else {
            
            LDrvSTimer_u2sTimeOut = (u2)((u4tCount / ((u4)LDRVSTIMER_u2sTIMERBASE + (u4)1)) + (u4)1) | (u2)LDRVSTIMER_u2sIS_1STCYCLE;
            DEV_u2gINTVLTDR       = (u2)(u4tCount & (u4)LDRVSTIMER_u2sTIMERBASE);
        }


        DEV_b1gINTVLTMIF = 0;       /* Clear interrupt flag of timer.             */
        DEV_b1gINTVLTMMK = 0;       /* Enable timer interrupt.                    */

        DEV_b1gINTVLTS = 1;         /* Start interval timer.                      */
    }


    return;
}


/* -------------------------------------------------------------------- */
/* Description : Stop counting time out                                 */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
void
LDrvSTimer_vogStopTimeout( void )
{

    DEV_b1gINTVLTMMK = 1;           /* Disable the timer interrupt */

    DEV_b1gINTVLTT   = 1;           /* Stop the timer              */


    return;
}


/* -------------------------------------------------------------------- */
/* Internal functions                                                   */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Interrupt handler                                                    */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Description : Timer comparing interrupt interface                    */
/* Parameters  : Nothing                                                */
/* Return value: Nothing                                                */
/* Misc.       : Nothing                                                */
/* -------------------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#pragma vector = INTTMmn_vect
#endif
INTERRUPT void
LDrvSTimer_vogVecCompare( void )
{

    /* In case a flag of the 1st cycle of timer is set. */
    /* (At the timer starting interrupt)                */
    if( (LDrvSTimer_u2sTimeOut & (u2)LDRVSTIMER_u2sIS_1STCYCLE) != (u2)0 )
    {
        /* Set TDR for 2nd cycle of timer.                           */
        /* Set timer base value to operate the same mode as free run timer. */
        DEV_u2gINTVLTDR  = (u2)LDRVSTIMER_u2sTIMERBASE;

        LDrvSTimer_u2sTimeOut &= (u2)~LDRVSTIMER_u2sIS_1STCYCLE;    /* Clear a flag of the 1st cycle of timer. */
    }
    else
    {
        LDrvSTimer_u2sTimeOut--;                /* Decrement timeout count. */

        if( LDrvSTimer_u2sTimeOut == (u2)0 )
        {
            switch( LCoreSCom_u1gFsm ) {
            case LCORESCOM_u1gFSMWAKEUP:
            {
                /* If wake-up pulse is transmitting, retry that. */
                LCoreSSleep_vogRtrySendWakeup();
                
                break;
            }

            case LCORESCOM_u1gFSMWAITHEADER:
            {
                /* Bus timeout */
                LCoreSSleep_vogSleep();

                break;
            }

            case LCORESCOM_u1gFSMRECDATA:
            {
                /* Receive response timeout */
                 LCoreSStat_vogSetErrorCode( LCORESSTAT_u2gERRCD_RESPTIMEOUT );

                 if( LDrvSUart_u1gIsRx1stRespData() == (u1)TRUE ) {

                    /* The response error is set only if slave has received more than 1 response data.  */
                    LCoreSResp_vogSetErrorStatus();
                 }

                 /* Slave status transits to Wait-Header after RLIN3 reset mode because RLIN3 leaves    */
                 /* from response operation status.                                                     */
                 /* (Bus time out will be detected in this Wait Header status.)                         */
                 (void)LDrvSUart_u1gDisable();
                 (void)LCoreSHead_u1gWaitHdrFromDis();

                 break;
            }
            case LCORESCOM_u1gFSMSENDDATA:
            {
                /* Record the error code if the bus times out while waiting on response */
                /* (Time-out is not specified in LIN2.0, but because of the notification to the user) */
                LCoreSStat_vogSetErrorCode( LCORESSTAT_u2gERRCD_RESPTIMEOUT );
                LCoreSResp_vogSetErrorStatus();
                LCoreSSleep_vogSleep();

                break;
            }

            default:
            {
                /* Come here if SENDWAKEUP, SLEEP, DORMANT (As Fail-safe) */
                LCoreSHead_vogWaitHdrFromErr( (u2)LCORESSTAT_u2gERRCD_FAILSAFE );

                break;
            }
            }
        }
    }


    return;
}
#ifdef __IAR_SYSTEMS_ICC__
#pragma default_function_attributes =
#endif

/* End of files */
