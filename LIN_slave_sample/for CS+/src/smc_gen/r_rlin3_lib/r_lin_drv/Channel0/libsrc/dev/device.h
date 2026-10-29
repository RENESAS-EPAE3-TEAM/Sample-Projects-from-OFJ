/* -------------------------------------------------------------------- */
/* Copyright(c) 2009-2024 Renesas Electronics Corporation               */
/* -------------------------------------------------------------------- */
/* File name: device.h                                                  */
/* Version:   0.05                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.08.05  Macros for TMPR010,TMPR110 were fixed.          */
/*                      PIM macros (naming and setting) were added.     */
/*   V0.03: 2016.06.13  CS+ CC-RL support.                              */
/*   V0.04: 2021.05.31  IAR Compiler, RL78/F2x support                  */
/*   V0.05: 2024.08.30  Modified for RL78/F23,F24.                      */
/* -------------------------------------------------------------------- */
#ifndef H_DEVICE
#define H_DEVICE


/* -------------------------------------------------------------------- */
/* Device file                                                          */
/* -------------------------------------------------------------------- */
#ifdef __CCRL__
#include "../../liblin2/iodefine.h"
#endif /* __CCRL__ */

#ifdef __IAR_SYSTEMS_ICC__
#include "ior7f124fpj.h"
#include "ior7f124fpj_ext.h"
#include "intrinsics.h"
#endif /* __IAR_SYSTEMS_ICC__ */

/* -------------------------------------------------------------------- */
/* Including header files                                               */
/* -------------------------------------------------------------------- */
#include "../conf/confslin_opt.h"

#ifdef RLIN3_CFG_USE_SMC_DEFINITION
  #if (defined __LIN_CH0_P1__) && (RLIN3_CFG_USE_LIN_CH0 != 1)
    #undef __LIN_CH0_P1__

    #if (RLIN3_CFG_USE_LIN_CH0 == 2)
      #define __LIN_CH0_P4__
    #endif

  #endif /* (defined __LIN_CH0_P1__) && (RLIN3_CFG_USE_LIN_CH0 != 1) */

  #if (defined __LIN_CH1_P1__) && (RLIN3_CFG_USE_LIN_CH1 != 1)
    #undef __LIN_CH1_P1__

    #if (RLIN3_CFG_USE_LIN_CH1 == 2)
      #define __LIN_CH1_P10__
    #elif (RLIN3_CFG_USE_LIN_CH1 == 3)
      #define __LIN_CH1_P12__
    #endif

  #endif /* (defined __LIN_CH1_P1__) && (RLIN3_CFG_USE_LIN_CH1 != 1) */

#endif /* RLIN3_CFG_USE_SMC_DEFINITION */

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */


#if (defined __LIN_RL78_F23_F24__) 
/* -------------------------------------------------------------------- */
/* Macro definition                                                     */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/*                                                                      */
/* Macro definition:Register name                                       */
/*                                                                      */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* LIN common channel                                                   */
/* -------------------------------------------------------------------- */
/* (RLIN3) */
/* As switching register access by channel is processed by LCHSEL,  */
/* switching by register name is not necessary.                     */
#define DEV_u1gLWBR         LWBR0
#define DEV_u1gLBRP0        LBRP00
#define DEV_u1gLBRP1        LBRP01
#define DEV_u1gLMD          LMD0
#define DEV_u1gLBFC         LBFC0
#define DEV_u1gLSC          LSC0
#define DEV_u1gLWUP         LWUP0
#define DEV_u1gLIE          LIE0
#define DEV_u1gLEDE         LEDE0
#define DEV_u1gLCUC         LCUC0
#define DEV_u1gLTRC         LTRC0
#define DEV_u1gLMST         LMST0       /* (read only) */
#define DEV_u1gLST          LST0
#define DEV_u1gLEST         LEST0
#define DEV_u1gLDFC         LDFC0
#define DEV_u1gLIDB         LIDB0
#define DEV_u1gLCBR         LCBR0
#define DEV_u1gLDB1         LDB01

/* (PORT) */
#define DEV_u1gPIOR4        PIOR4
#define DEV_u1gPIOR9        PIOR9

/* (TAU) */
#if (CONFSLIN_OPT_u1gTMUNIT_CFG == CONFSLIN_OPT_u1gTMUNIT_UNIT0)
#define DEV_u2gINTVLTPS    TPS0

 #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#define DEV_u2gINTVLTMR    TMR00
#define DEV_u2gINTVLTDR    TDR00
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#define DEV_u2gINTVLTMR    TMR01
#define DEV_u2gINTVLTDR    TDR01
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#define DEV_u2gINTVLTMR    TMR02
#define DEV_u2gINTVLTDR    TDR02
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#define DEV_u2gINTVLTMR    TMR03
#define DEV_u2gINTVLTDR    TDR03
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#define DEV_u2gINTVLTMR    TMR04
#define DEV_u2gINTVLTDR    TDR04
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#define DEV_u2gINTVLTMR    TMR05
#define DEV_u2gINTVLTDR    TDR05
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#define DEV_u2gINTVLTMR    TMR06
#define DEV_u2gINTVLTDR    TDR06
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#define DEV_u2gINTVLTMR    TMR07
#define DEV_u2gINTVLTDR    TDR07
 #endif /* CONFSLIN_OPT_u1gTMCH_CFG */

#elif (CONFSLIN_OPT_u1gTMUNIT_CFG == CONFSLIN_OPT_u1gTMUNIT_UNIT1)
#define DEV_u2gINTVLTPS    TPS1

 #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#define DEV_u2gINTVLTMR    TMR10
#define DEV_u2gINTVLTDR    TDR10
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#define DEV_u2gINTVLTMR    TMR11
#define DEV_u2gINTVLTDR    TDR11
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#define DEV_u2gINTVLTMR    TMR12
#define DEV_u2gINTVLTDR    TDR12
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#define DEV_u2gINTVLTMR    TMR13
#define DEV_u2gINTVLTDR    TDR13
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#define DEV_u2gINTVLTMR    TMR14
#define DEV_u2gINTVLTDR    TDR14
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#define DEV_u2gINTVLTMR    TMR15
#define DEV_u2gINTVLTDR    TDR15
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#define DEV_u2gINTVLTMR    TMR16
#define DEV_u2gINTVLTDR    TDR16
 #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#define DEV_u2gINTVLTMR    TMR17
#define DEV_u2gINTVLTDR    TDR17
 #endif /* CONFSLIN_OPT_u1gTMCH_CFG */
#endif /* CONFSLIN_OPT_u1gTMUNIT_CFG */


/* -------------------------------------------------------------------- */
/*                                                                      */
/* Macro definition:Bit Name                                            */
/*                                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Use LIN channel 0                                                    */
/* -------------------------------------------------------------------- */
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
/* (RLIN3) */
#define DEV_b1gLINEN        PER2_bit.no2
#define DEV_b1gLINMCK       LINCKSEL_bit.no0
#define DEV_b1gLINMCKE      LINCKSEL_bit.no4

  #ifdef __LIN_RL78_F23_F24__
#define DEV_b1gLTIF         IF0H_bit.no7        /* LIN0TRMIF    */
#define DEV_b1gLRIF         IF1L_bit.no0        /* LIN0RVCIF    */
#define DEV_b1gLSIF         IF1L_bit.no1        /* LIN0STAIF    */
#define DEV_b1gLTMK         MK0H_bit.no7        /* LIN0TRMMK    */
#define DEV_b1gLRMK         MK1L_bit.no0        /* LIN0RVCMK    */
#define DEV_b1gLSMK         MK1L_bit.no1        /* LIN0STAMK    */
#define DEV_b1gLTPR0        PR00H_bit.no7       /* LIN0TRMPR0   */
#define DEV_b1gLTPR1        PR10H_bit.no7       /* LIN0TRMPR1   */
#define DEV_b1gLRPR0        PR01L_bit.no0       /* LIN0RVCPR0   */
#define DEV_b1gLRPR1        PR11L_bit.no0       /* LIN0RVCPR1   */
#define DEV_b1gLSPR0        PR01L_bit.no1       /* LIN0STAPR0   */
#define DEV_b1gLSPR1        PR11L_bit.no1       /* LIN0STAPR1   */
  #endif /* __LIN_RL78_F23_F24__ */

/* (INTP) */
#define DEV_b1gISC          ISC_bit.no2
#define DEV_b1gEGP          EGP1_bit.no3
#define DEV_b1gEGN          EGN1_bit.no3
#define DEV_b1gPMK          MK2L_bit.no3
#define DEV_b1gPIF          IF2L_bit.no3
#define DEV_b1gPPR0         PR02L_bit.no3       /* PPR011       */
#define DEV_b1gPPR1         PR12L_bit.no3       /* PPR111       */

/* (PORT) */
  #ifdef __LIN_CH0_P1__
#define DEV_b1gTXD          P1_bit.no3
#define DEV_b1gRXD          P1_bit.no4
#define DEV_b1gTXD_MODE     PM1_bit.no3
#define DEV_b1gRXD_MODE     PM1_bit.no4
#define DEV_b1gTXD_POM      POM1_bit.no3
#define DEV_b1gRXD_PIM      PIM1_bit.no4
#define DEV_b1gRXD_PU       PU1_bit.no4
#define DEV_b1gRXD_PITHL    PITHL1_bit.no4
  #endif /* __LIN_CH0_P1__ */

  #ifdef __LIN_CH0_P4__
#define DEV_b1gTXD          P4_bit.no2
#define DEV_b1gRXD          P4_bit.no3
#define DEV_b1gTXD_MODE     PM4_bit.no2
#define DEV_b1gRXD_MODE     PM4_bit.no3
#define DEV_b1gRXD_PU       PU4_bit.no3
#define DEV_b1gRXD_PITHL    PITHL4_bit.no3
  #endif /* __LIN_CH0_P4__ */


#endif /* __LIN_CH0__ */


/* -------------------------------------------------------------------- */
/* Use LIN channel 1                                                    */
/* -------------------------------------------------------------------- */
#if (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
/* (RLIN3) */
#define DEV_b1gLINEN        PER2_bit.no3
#define DEV_b1gLINMCK       LINCKSEL_bit.no1
#define DEV_b1gLINMCKE      LINCKSEL_bit.no5

  #ifdef __LIN_RL78_F23_F24__
#define DEV_b1gLTIF         IF3L_bit.no1        /* LIN1TRMIF    */
#define DEV_b1gLRIF         IF3L_bit.no2        /* LIN1RVCIF    */
#define DEV_b1gLSIF         IF3L_bit.no3        /* LIN1STAIF    */
#define DEV_b1gLTMK         MK3L_bit.no1        /* LIN1TRMMK    */
#define DEV_b1gLRMK         MK3L_bit.no2        /* LIN1RVCMK    */
#define DEV_b1gLSMK         MK3L_bit.no3        /* LIN1STAMK    */
#define DEV_b1gLTPR0        PR03L_bit.no1       /* LIN1TRMPR0   */
#define DEV_b1gLTPR1        PR13L_bit.no1       /* LIN1TRMPR1   */
#define DEV_b1gLRPR0        PR03L_bit.no2       /* LIN1RVCPR0   */
#define DEV_b1gLRPR1        PR13L_bit.no2       /* LIN1RVCPR1   */
#define DEV_b1gLSPR0        PR03L_bit.no3       /* LIN1STAPR0   */
#define DEV_b1gLSPR1        PR13L_bit.no3       /* LIN1STAPR1   */
  #endif /* __LIN_RL78_F23_F24__ */

/* (INTP) */
#define DEV_b1gISC          ISC_bit.no3
#define DEV_b1gEGP          EGP1_bit.no4
#define DEV_b1gEGN          EGN1_bit.no4

  #ifdef __LIN_RL78_F23_F24__
#define DEV_b1gPMK          MK3L_bit.no0
#define DEV_b1gPIF          IF3L_bit.no0
#define DEV_b1gPPR0         PR03L_bit.no0       /* PPR012       */
#define DEV_b1gPPR1         PR13L_bit.no0       /* PPR112       */
  #endif /* __LIN_RL78_F23_F24__ */

/* (PORT) */
  #ifdef __LIN_CH1_P1__
#define DEV_b1gTXD          P1_bit.no0
#define DEV_b1gRXD          P1_bit.no1
#define DEV_b1gTXD_MODE     PM1_bit.no0
#define DEV_b1gRXD_MODE     PM1_bit.no1
#define DEV_b1gTXD_POM      POM1_bit.no0
#define DEV_b1gRXD_PIM      PIM1_bit.no1
#define DEV_b1gRXD_PU       PU1_bit.no1
#define DEV_b1gRXD_PITHL    PITHL1_bit.no1
  #endif /* __LIN_CH1_P1__ */

  #ifdef __LIN_CH1_P10__
#define DEV_b1gTXD          P10_bit.no6
#define DEV_b1gRXD          P10_bit.no7
#define DEV_b1gTXD_MODE     PM10_bit.no6
#define DEV_b1gRXD_MODE     PM10_bit.no7
#define DEV_b1gRXD_PU       PU10_bit.no7
#define DEV_b1gRXD_PITHL    PITHL10_bit.no7
  #endif /* __LIN_CH1_P10__ */

  #ifdef __LIN_CH1_P12__
#define DEV_b1gTXD          P12_bit.no0
#define DEV_b1gRXD          P12_bit.no5
#define DEV_b1gTXD_MODE     PM12_bit.no0
#define DEV_b1gRXD_MODE     PM12_bit.no5
#define DEV_b1gTXD_MODE_CTL PMC12_bit.no0
#define DEV_b1gRXD_MODE_CTL PMC12_bit.no5
#define DEV_b1gTXD_POM      POM12_bit.no0
#define DEV_b1gRXD_PIM      PIM12_bit.no5
#define DEV_b1gRXD_PU       PU12_bit.no5
#define DEV_b1gRXD_PITHL    PITHL12_bit.no5
  #endif /* __LIN_CH1_P12__ */

#endif /* __LIN_CH1__ */


/* -------------------------------------------------------------------- */
/* LIN common channel                                                   */
/* -------------------------------------------------------------------- */
/* (TAU) */
#if (CONFSLIN_OPT_u1gTMUNIT_CFG == CONFSLIN_OPT_u1gTMUNIT_UNIT0)
#define DEV_b1gINTVLTAUEN   PER0_bit.no0

  #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#define DEV_b1gINTVLTE      TE0L_bit.no0
#define DEV_b1gINTVLTS      TS0L_bit.no0
#define DEV_b1gINTVLTT      TT0L_bit.no0
#define DEV_b1gINTVLTMMK    TMMK00
#define DEV_b1gINTVLTMIF    TMIF00
#define DEV_b1gINTVLTMPR0   PR01L_bit.no4       /* TMPR000      */
#define DEV_b1gINTVLTMPR1   PR11L_bit.no4       /* TMPR100      */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#define DEV_b1gINTVLTE      TE0L_bit.no1
#define DEV_b1gINTVLTS      TS0L_bit.no1
#define DEV_b1gINTVLTT      TT0L_bit.no1
#define DEV_b1gINTVLTMMK    TMMK01
#define DEV_b1gINTVLTMIF    TMIF01
#define DEV_b1gINTVLTMPR0   PR01L_bit.no5       /* TMPR001      */
#define DEV_b1gINTVLTMPR1   PR11L_bit.no5       /* TMPR101      */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#define DEV_b1gINTVLTE      TE0L_bit.no2
#define DEV_b1gINTVLTS      TS0L_bit.no2
#define DEV_b1gINTVLTT      TT0L_bit.no2
#define DEV_b1gINTVLTMMK    TMMK02
#define DEV_b1gINTVLTMIF    TMIF02
#define DEV_b1gINTVLTMPR0   PR01L_bit.no6       /* TMPR002      */
#define DEV_b1gINTVLTMPR1   PR11L_bit.no6       /* TMPR102      */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#define DEV_b1gINTVLTE      TE0L_bit.no3
#define DEV_b1gINTVLTS      TS0L_bit.no3
#define DEV_b1gINTVLTT      TT0L_bit.no3
#define DEV_b1gINTVLTMMK    TMMK03
#define DEV_b1gINTVLTMIF    TMIF03
#define DEV_b1gINTVLTMPR0   PR01L_bit.no7       /* TMPR003      */
#define DEV_b1gINTVLTMPR1   PR11L_bit.no7       /* TMPR103      */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#define DEV_b1gINTVLTE      TE0L_bit.no4
#define DEV_b1gINTVLTS      TS0L_bit.no4
#define DEV_b1gINTVLTT      TT0L_bit.no4
#define DEV_b1gINTVLTMMK    TMMK04
#define DEV_b1gINTVLTMIF    TMIF04
#define DEV_b1gINTVLTMPR0   PR01H_bit.no7       /* TMPR004      */
#define DEV_b1gINTVLTMPR1   PR11H_bit.no7       /* TMPR104      */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#define DEV_b1gINTVLTE      TE0L_bit.no5
#define DEV_b1gINTVLTS      TS0L_bit.no5
#define DEV_b1gINTVLTT      TT0L_bit.no5
#define DEV_b1gINTVLTMMK    TMMK05
#define DEV_b1gINTVLTMIF    TMIF05
#define DEV_b1gINTVLTMPR0   PR02L_bit.no0       /* TMPR005      */
#define DEV_b1gINTVLTMPR1   PR12L_bit.no0       /* TMPR105      */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#define DEV_b1gINTVLTE      TE0L_bit.no6
#define DEV_b1gINTVLTS      TS0L_bit.no6
#define DEV_b1gINTVLTT      TT0L_bit.no6
#define DEV_b1gINTVLTMMK    TMMK06
#define DEV_b1gINTVLTMIF    TMIF06
#define DEV_b1gINTVLTMPR0   PR02L_bit.no1       /* TMPR006      */
#define DEV_b1gINTVLTMPR1   PR12L_bit.no1       /* TMPR106      */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#define DEV_b1gINTVLTE      TE0L_bit.no7
#define DEV_b1gINTVLTS      TS0L_bit.no7
#define DEV_b1gINTVLTT      TT0L_bit.no7
#define DEV_b1gINTVLTMMK    TMMK07
#define DEV_b1gINTVLTMIF    TMIF07
#define DEV_b1gINTVLTMPR0   PR02L_bit.no2       /* TMPR007      */
#define DEV_b1gINTVLTMPR1   PR12L_bit.no2       /* TMPR107      */
  #endif /* CONFSLIN_OPT_u1gTMCH_CFG */


#elif (CONFSLIN_OPT_u1gTMUNIT_CFG == CONFSLIN_OPT_u1gTMUNIT_UNIT1)
#define DEV_b1gINTVLTAUEN   PER0_bit.no1

  #if   (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH0)
#define DEV_b1gINTVLTE      TE1L_bit.no0
#define DEV_b1gINTVLTS      TS1L_bit.no0
#define DEV_b1gINTVLTT      TT1L_bit.no0
#define DEV_b1gINTVLTMMK    TMMK10
#define DEV_b1gINTVLTMIF    TMIF10
  #ifdef __LIN_RL78_F23_F24__
#define DEV_b1gINTVLTMPR0   PR02H_bit.no3       /* TMPR010      */
#define DEV_b1gINTVLTMPR1   PR12H_bit.no3       /* TMPR110      */
  #endif /* __LIN_RL78_F23_F24__ */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH1)
#define DEV_b1gINTVLTE      TE1L_bit.no1
#define DEV_b1gINTVLTS      TS1L_bit.no1
#define DEV_b1gINTVLTT      TT1L_bit.no1
#define DEV_b1gINTVLTMMK    TMMK11
#define DEV_b1gINTVLTMIF    TMIF11
  #ifdef __LIN_RL78_F23_F24__
#define DEV_b1gINTVLTMPR0   PR02H_bit.no4       /* TMPR011      */
#define DEV_b1gINTVLTMPR1   PR12H_bit.no4       /* TMPR111      */
  #endif /* __LIN_RL78_F23_F24__ */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH2)
#define DEV_b1gINTVLTE      TE1L_bit.no2
#define DEV_b1gINTVLTS      TS1L_bit.no2
#define DEV_b1gINTVLTT      TT1L_bit.no2
#define DEV_b1gINTVLTMMK    TMMK12
#define DEV_b1gINTVLTMIF    TMIF12
  #ifdef __LIN_RL78_F23_F24__
#define DEV_b1gINTVLTMPR0   PR02H_bit.no5       /* TMPR012      */
#define DEV_b1gINTVLTMPR1   PR12H_bit.no5       /* TMPR112      */
  #endif /* __LIN_RL78_F23_F24__ */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH3)
#define DEV_b1gINTVLTE      TE1L_bit.no3
#define DEV_b1gINTVLTS      TS1L_bit.no3
#define DEV_b1gINTVLTT      TT1L_bit.no3
#define DEV_b1gINTVLTMMK    TMMK13
#define DEV_b1gINTVLTMIF    TMIF13
  #ifdef __LIN_RL78_F23_F24__
#define DEV_b1gINTVLTMPR0   PR02H_bit.no6       /* TMPR013      */
#define DEV_b1gINTVLTMPR1   PR12H_bit.no6       /* TMPR113      */
  #endif /* __LIN_RL78_F23_F24__ */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH4)
#define DEV_b1gINTVLTE      TE1L_bit.no4
#define DEV_b1gINTVLTS      TS1L_bit.no4
#define DEV_b1gINTVLTT      TT1L_bit.no4
#define DEV_b1gINTVLTMMK    TMMK14
#define DEV_b1gINTVLTMIF    TMIF14
  #ifdef __LIN_RL78_F23_F24__
#define DEV_b1gINTVLTMPR0   PR03L_bit.no4       /* TMPR014      */
#define DEV_b1gINTVLTMPR1   PR13L_bit.no4       /* TMPR114      */
  #endif /* __LIN_RL78_F23_F24__ */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH5)
#define DEV_b1gINTVLTE      TE1L_bit.no5
#define DEV_b1gINTVLTS      TS1L_bit.no5
#define DEV_b1gINTVLTT      TT1L_bit.no5
#define DEV_b1gINTVLTMMK    TMMK15
#define DEV_b1gINTVLTMIF    TMIF15
  #ifdef __LIN_RL78_F23_F24__
#define DEV_b1gINTVLTMPR0   PR03L_bit.no5       /* TMPR015      */
#define DEV_b1gINTVLTMPR1   PR13L_bit.no5       /* TMPR115      */
  #endif /* __LIN_RL78_F23_F24__ */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH6)
#define DEV_b1gINTVLTE      TE1L_bit.no6
#define DEV_b1gINTVLTS      TS1L_bit.no6
#define DEV_b1gINTVLTT      TT1L_bit.no6
#define DEV_b1gINTVLTMMK    TMMK16
#define DEV_b1gINTVLTMIF    TMIF16
  #ifdef __LIN_RL78_F23_F24__
#define DEV_b1gINTVLTMPR0   PR03L_bit.no6       /* TMPR016      */
#define DEV_b1gINTVLTMPR1   PR13L_bit.no6       /* TMPR116      */
  #endif /* __LIN_RL78_F23_F24__ */

  #elif (CONFSLIN_OPT_u1gTMCH_CFG == CONFSLIN_OPT_u1gTMCH_CH7)
#define DEV_b1gINTVLTE      TE1L_bit.no7
#define DEV_b1gINTVLTS      TS1L_bit.no7
#define DEV_b1gINTVLTT      TT1L_bit.no7
#define DEV_b1gINTVLTMMK    TMMK17
#define DEV_b1gINTVLTMIF    TMIF17
  #ifdef __LIN_RL78_F23_F24__
#define DEV_b1gINTVLTMPR0   PR03L_bit.no7       /* TMPR017      */
#define DEV_b1gINTVLTMPR1   PR13L_bit.no7       /* TMPR117      */
  #endif /* __LIN_RL78_F23_F24__ */
  #endif /* CONFSLIN_OPT_u1gTMCH_CFG */


#endif /* CONFSLIN_OPT_u1gTMUNIT_CFG */


/* -------------------------------------------------------------------- */
/*                                                                      */
/* Macro definition:Macro function                                      */
/*                                                                      */
/* -------------------------------------------------------------------- */

/* (Macro function RLIN3 channel selection) */
  #if(defined __LIN_RL78_F23_F24__)
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define DEV_vogENABLE_LIN_CH()  { LCHSEL = (u1)0x00U; }
#endif /* __LIN_CH0__ */

#if   (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define DEV_vogENABLE_LIN_CH()  { LCHSEL = (u1)0x01U; }
#endif /* __LIN_CH1__ */
  #endif /* __LIN_RL78_F2x__ */


/* (POM setting macro function) */
  #ifdef __LIN_RL78_F23_F24__
#if   (defined __LIN_CH0_P1__) || (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P12__)
#define DEV_vogSET_POM()        { DEV_b1gTXD_POM = 0; }
#elif (defined __LIN_CH0_P4__) || (defined __LIN_CH1_P10__)
#define DEV_vogSET_POM()
#endif /* __LIN_CHx__ */
  #endif /* __LIN_RL78_F23_F24__ */


/* (PMC setting macro function) */
#if   (defined __LIN_CH1_P12__)
#define DEV_vogSET_TXD_PMC()        { DEV_b1gTXD_MODE_CTL = 0; }
#define DEV_vogSET_RXD_PMC()        { DEV_b1gRXD_MODE_CTL = 0; }
#elif (defined __LIN_CH0_P4__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH0_P1__) || (defined __LIN_CH1_P1__)
#define DEV_vogSET_TXD_PMC()
#define DEV_vogSET_RXD_PMC()
#endif


/* (PIM setting macro function) */
  #ifdef __LIN_RL78_F23_F24__
#if   (defined __LIN_CH0_P1__) || (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P12__)
#define DEV_vogSET_PIM()        { DEV_b1gRXD_PIM = 0; }
#elif (defined __LIN_CH0_P4__) || (defined __LIN_CH1_P10__)
#define DEV_vogSET_PIM()
#endif /* __LIN_CHx__ */
  #endif /* __LIN_RL78_F23_F24__ */


/* Macro function of PIOR4/9 setting. */
/* - Function uses "read modify write" because PIOR4/PIOR9 can't be access per 1[bit].     */
/*   Application shouldn't set PIOR4/PIOR9 because it isn't cleared by this driver.        */
/*   The relationship between PIOR44, PIOR45 and PIOR93 at F2x is as follows               */
/*          | 100pin                | 80pin               | 64pin               | 48pin               | 32pin               */
/*   -------+-----------------------+---------------------+---------------------+------------------------------------------ */
/*   PIOR44 | (0)       | (1)       | (0)     | (1)       | (0)     | (1)       | (0)     | (1)       | (0)     | (1)       */
/*   F23    | P13/P14   | P42/P43   | P13/P14 | P42/P43   | P13/P14 | P42/P43   | P13/P14 | P42/P43   | P13/P14 | ----      */
/*   F24    | P13/P14   | P42/P43   | P13/P14 | P42/P43   | P13/P14 | P42/P43   | P13/P14 | P42/P43   | P13/P14 | ----      */
/*   -------+-----------+-----------+---------+-----------+---------+-----------+---------+-----------+---------+---------- */
/*   PIOR93 | (0)                   | (0)                 | (0)                 | (0)                 | (0)                 */
/*   PIOR45 | (0)       | (1)       | (0)     | (1)       | (0)     | (1)       | (0)     | (1)       | (0)     | (1)       */
/*   F24    | P10/P11   | P106/P107 | P10/P11 | ----      | P10/P11 | ----      | P10/P11 | ----      | P10/P11 | ----      */
/*   -------+-----------+-----------+---------+-----------+---------+-----------+---------+-----------+---------+---------- */
/*   PIOR93 | (1)                   | (1)                 | (1)                 | (1)                 | (1)                 */
/*   PIOR45 | (0)       | (1)       | (0)     | (1)       | (0)     | (1)       | (0)     | (1)       | (0)     | (1)       */
/*   F24    | P10/P11   | P120/P125 | P10/P11 | P120/P125 | P10/P11 | P120/P125 | P10/P11 | P120/P125 | P10/P11 | P120/P125 */
/*   -------+-----------+-----------+---------+-----------+---------+-----------+---------+-----------+---------+---------- */
#ifdef __LIN_CH0_P4__
#define DEV_vogSET_PIOR4()   { DEV_u1gPIOR4 = (DEV_u1gPIOR4 | (u1)0x10U); }
#elif (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define DEV_vogSET_PIOR4()   { DEV_u1gPIOR4 = (DEV_u1gPIOR4 | (u1)0x20U); }
#elif (defined __LIN_CH0_P1__) || (defined __LIN_CH1_P1__)
#define DEV_vogSET_PIOR4()
#endif /* __LIN_CHx_x__ */

#ifdef __LIN_CH1_P12__
#define DEV_vogSET_PIOR9()   { DEV_u1gPIOR9 = (DEV_u1gPIOR9 | (u1)0x08U); }
#elif (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__) || (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__)
#define DEV_vogSET_PIOR9()
#endif /* __LIN_CHx_x__ */


/* -------------------------------------------------------------------- */
/*                                                                      */
/* Macro definition:Other                                               */
/*                                                                      */
/* -------------------------------------------------------------------- */

/* Port for debugging (for RL78/F13,F14 target bord) */
#ifdef __DEBUG__
#define DEV_vogDBGPORT_A_INIT() { P0_bit.no0 = 0; PM0_bit.no0 = 0; }
#define DEV_vogDBGPORT_A_HI()   { P0_bit.no0 = 1; }
#define DEV_vogDBGPORT_A_LO()   { P0_bit.no0 = 0; }

#define DEV_vogDBGPORT_B_INIT() { P0_bit.no1 = 0; PM0_bit.no1 = 0; }
#define DEV_vogDBGPORT_B_HI()   { P0_bit.no1 = 1; }
#define DEV_vogDBGPORT_B_LO()   { P0_bit.no1 = 0; }

#define DEV_vogDBGPORT_C_INIT() { P0_bit.no2 = 0; PM0_bit.no2 = 0; }
#define DEV_vogDBGPORT_C_HI()   { P0_bit.no2 = 1; }
#define DEV_vogDBGPORT_C_LO()   { P0_bit.no2 = 0; }

#else /* __DEBUG__ */
#define DEV_vogDBGPORT_A_INIT()
#define DEV_vogDBGPORT_A_HI()
#define DEV_vogDBGPORT_A_LO()

#define DEV_vogDBGPORT_B_INIT()
#define DEV_vogDBGPORT_B_HI()
#define DEV_vogDBGPORT_B_LO()

#define DEV_vogDBGPORT_C_INIT()
#define DEV_vogDBGPORT_C_HI()
#define DEV_vogDBGPORT_C_LO()
#endif /* __DEBUG__ */

#endif /* __LIN_RL78_F23_F24__ */


/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public constants                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public variables                                                     */
/* -------------------------------------------------------------------- */

#endif /* H_DEVICE */

/* End of files */
