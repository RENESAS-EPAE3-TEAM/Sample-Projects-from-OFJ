/* -------------------------------------------------------------------- */
/* Copyright(c) 2016 - 2024 Renesas Electronics Corporation             */
/* -------------------------------------------------------------------- */
/* File name: confslin_opt.h                                            */
/* Version:   0.06                                                      */
/* Date:      2024.08.30                                                */
/* Author:    A.Kondo, M.hirano                                         */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* History                                                              */
/*   V0.01: 2013.01.18  1st creation.                                   */
/*   V0.02: 2013.02.26  Added "LIN communication clock source selection"*/
/*   V0.03: 2013.08.22  The category deviation of configuration was     */
/*                      modified.                                       */
/*   V0.04: 2013.12.06  The maximum value of RLIN3 transition wait      */
/*                      counter was added.                              */
/*   V0.05: 2016.01.21  Using fMX and TER at same time are profibited.  */
/*   V0.06: 2024.08.30  Modified for RL78/F23,F24.                      */
/* -------------------------------------------------------------------- */
#ifndef H_CONFSLIN_OPT
#define H_CONFSLIN_OPT

#include "r_rlin3_config.h"

/* -------------------------------------------------------------------- */
/* Type definition                                                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/*                                                                      */
/* Macro definition                                                     */
/*                                                                      */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* Driver configuration parameters                                      */
/* (Do not change)                                                      */
/* -------------------------------------------------------------------- */
/* Wake-up pulse detection method */
#define CONFSLIN_OPT_u1gBUSWKUP_EDGE        (0x00U)     /* Falling-edge detection   */
#define CONFSLIN_OPT_u1gBUSWKUP_WIDTH       (0x01U)     /* Dominant width detection */

/* Select LIN prescaler clock(Normal communication, Wake up) */
#define CONFSLIN_OPT_u1gLPRS_NODIV          (0x00U)     /* 1/1   */
#define CONFSLIN_OPT_u1gLPRS_DIV2           (0x02U)     /* 1/2   */
#define CONFSLIN_OPT_u1gLPRS_DIV2P2         (0x04U)     /* 1/4   */
#define CONFSLIN_OPT_u1gLPRS_DIV2P3         (0x06U)     /* 1/8   */
#define CONFSLIN_OPT_u1gLPRS_DIV2P4         (0x08U)     /* 1/16  */
#define CONFSLIN_OPT_u1gLPRS_DIV2P5         (0x0AU)     /* 1/32  */
#define CONFSLIN_OPT_u1gLPRS_DIV2P6         (0x0CU)     /* 1/64  */
#define CONFSLIN_OPT_u1gLPRS_DIV2P7         (0x0EU)     /* 1/128 */

/* Select the number of LIN bit sampling(Normal communication, Wake up) */
#define CONFSLIN_OPT_u1gNSPB_4SMPL          (0x30U)     /* For auto baud rate  */
#define CONFSLIN_OPT_u1gNSPB_8SMPL          (0x70U)     /* For auto baud rate  */
#define CONFSLIN_OPT_u1gNSPB_16SMPL         (0xF0U)     /* For fixed baud rate */

/* Select whether to use the data noise filter of LIN reception */
#define CONFSLIN_OPT_u1gLRDNFS_USE          (0x00U)
#define CONFSLIN_OPT_u1gLRDNFS_NOUSE        (0x20U)

/* Select the break detection width (Low) LIN receiver */
#define CONFSLIN_OPT_u1gBLT_SHORT           (0x00U)     /*  9.5 or 10[Tbits]  */
#define CONFSLIN_OPT_u1gBLT_LONG            (0x01U)     /* 10.5 or 11[Tbits]  */

/* Select the response space of LIN transmission */
#define CONFSLIN_OPT_u1gRS_0BIT             (0x00U)
#define CONFSLIN_OPT_u1gRS_1BIT             (0x01U)
#define CONFSLIN_OPT_u1gRS_2BIT             (0x02U)
#define CONFSLIN_OPT_u1gRS_3BIT             (0x03U)
#define CONFSLIN_OPT_u1gRS_4BIT             (0x04U)
#define CONFSLIN_OPT_u1gRS_5BIT             (0x05U)
#define CONFSLIN_OPT_u1gRS_6BIT             (0x06U)
#define CONFSLIN_OPT_u1gRS_7BIT             (0x07U)

/* Select the inter-byte space of LIN transmission */
#define CONFSLIN_OPT_u1gIBS_0BIT            (0x00U)
#define CONFSLIN_OPT_u1gIBS_1BIT            (0x10U)
#define CONFSLIN_OPT_u1gIBS_2BIT            (0x20U)
#define CONFSLIN_OPT_u1gIBS_3BIT            (0x30U)

/* Select the Low width of the LIN wake-up transmission */
#define CONFSLIN_OPT_u1gWUTL_1BIT           (0x00U)
#define CONFSLIN_OPT_u1gWUTL_2BIT           (0x10U)
#define CONFSLIN_OPT_u1gWUTL_3BIT           (0x20U)
#define CONFSLIN_OPT_u1gWUTL_4BIT           (0x30U)
#define CONFSLIN_OPT_u1gWUTL_5BIT           (0x40U)
#define CONFSLIN_OPT_u1gWUTL_6BIT           (0x50U)
#define CONFSLIN_OPT_u1gWUTL_7BIT           (0x60U)
#define CONFSLIN_OPT_u1gWUTL_8BIT           (0x70U)
#define CONFSLIN_OPT_u1gWUTL_9BIT           (0x80U)
#define CONFSLIN_OPT_u1gWUTL_10BIT          (0x90U)
#define CONFSLIN_OPT_u1gWUTL_11BIT          (0xA0U)
#define CONFSLIN_OPT_u1gWUTL_12BIT          (0xB0U)
#define CONFSLIN_OPT_u1gWUTL_13BIT          (0xC0U)
#define CONFSLIN_OPT_u1gWUTL_14BIT          (0xD0U)
#define CONFSLIN_OPT_u1gWUTL_15BIT          (0xE0U)
#define CONFSLIN_OPT_u1gWUTL_16BIT          (0xF0U)

/* Select LIN bit error detection */
#define CONFSLIN_OPT_u1gBERE_DISABLE        (0x00U)
#define CONFSLIN_OPT_u1gBERE_ENABLE         (0x01U)

/* Select time-out error detection */
#define CONFSLIN_OPT_u1gTER_DISABLE         (0x00U)
#define CONFSLIN_OPT_u1gTER_ENABLE          (0x04U)

/* Select LIN framing error detection */
#define CONFSLIN_OPT_u1gFERE_DISABLE        (0x00U)
#define CONFSLIN_OPT_u1gFERE_ENABLE         (0x08U)

/* Select LIN sync field error detection */
#define CONFSLIN_OPT_u1gSFERE_DISABLE       (0x00U)
#define CONFSLIN_OPT_u1gSFERE_ENABLE        (0x10U)

/* Select LIN ID parity error detection */
#define CONFSLIN_OPT_u1gIPERE_DISABLE       (0x00U)
#define CONFSLIN_OPT_u1gIPERE_ENABLE        (0x40U)

/* Select type of LIN time-out error detection */
#define CONFSLIN_OPT_u1gLTES_FRAMETO        (0x00U)     /* Frame time-out    */
#define CONFSLIN_OPT_u1gLTES_RESPTO         (0x80U)     /* Response time-out */

/* Select LIN engine clock source */
#define CONFSLIN_OPT_u1gLINMCK_FCLK         (0x00U)     /* fCLK */
#define CONFSLIN_OPT_u1gLINMCK_FMX          (0x01U)     /* fMX  */

/* Timer unit selection list for the interval timer */
#define CONFSLIN_OPT_u1gTMUNIT_UNIT0        (0x00U)
#define CONFSLIN_OPT_u1gTMUNIT_UNIT1        (0x01U)

/* Timer channel selection list for the interval timer */
#define CONFSLIN_OPT_u1gTMCH_CH0            (0x00U)
#define CONFSLIN_OPT_u1gTMCH_CH1            (0x01U)
#define CONFSLIN_OPT_u1gTMCH_CH2            (0x02U)
#define CONFSLIN_OPT_u1gTMCH_CH3            (0x03U)
#define CONFSLIN_OPT_u1gTMCH_CH4            (0x04U)
#define CONFSLIN_OPT_u1gTMCH_CH5            (0x05U)
#define CONFSLIN_OPT_u1gTMCH_CH6            (0x06U)
#define CONFSLIN_OPT_u1gTMCH_CH7            (0x07U)

/* Select the timer clock */
/* (clock selection 2 or 3 is unsupported since its divider value changes with the product.) */
#define CONFSLIN_OPT_u2gTMCLKSEL_SEL0       (0x0000U)
#define CONFSLIN_OPT_u2gTMCLKSEL_SEL1       (0x8000U)

/* Timer clock divider value (Only support 0/1 clock select)            */
/* (Clock of timer operation = Clock of H/W Operation / Setting value)  */
#define CONFSLIN_OPT_u2gTMCLKDIV_NODIV      (0x0000U)   /* 1/1     */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2       (0x0001U)   /* 1/2     */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P2     (0x0002U)   /* 1/4     */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P3     (0x0003U)   /* 1/8     */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P4     (0x0004U)   /* 1/16    */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P5     (0x0005U)   /* 1/32    */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P6     (0x0006U)   /* 1/64    */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P7     (0x0007U)   /* 1/128   */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P8     (0x0008U)   /* 1/256   */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P9     (0x0009U)   /* 1/512   */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P10    (0x000AU)   /* 1/1024  */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P11    (0x000BU)   /* 1/2048  */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P12    (0x000CU)   /* 1/4096  */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P13    (0x000DU)   /* 1/8192  */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P14    (0x000EU)   /* 1/16384 */
#define CONFSLIN_OPT_u2gTMCLKDIV_DIV2P15    (0x000FU)   /* 1/32768 */

/* Select the interrupt priority */
#define CONFSLIN_OPT_u1gINTPR_LV0           (0x00U)     /* Level0(Highest priority) */
#define CONFSLIN_OPT_u1gINTPR_LV1           (0x01U)     /* Level1                   */
#define CONFSLIN_OPT_u1gINTPR_LV2           (0x02U)     /* Level2                   */
#define CONFSLIN_OPT_u1gINTPR_LV3           (0x03U)     /* Level3(Lowest priority)  */


/* -------------------------------------------------------------------- */
/* Driver Configuration                                                 */
/* If you want to change the characteristics of the driver, change the  */
/* value below.                                                         */
/*                                                                      */
/*  (Notice)                                                            */
/*  - This configuration is an item that can not be set by              */
/*    LIN Configurator.                                                 */
/*  - Set up the value of the system time in consideration of the       */
/*    oscillation error tolerance(+- 14 [%]) of the LIN specification.  */
/*  - Timeout error (TER) is disabled during auto baud rate mode.       */
/*  - During Wake-up mode, always use a fixed baud rate mode.           */
/*  - CONFSLIN_OPT_u1gLPRS_NORM_CFG is changed as following in auto-    */
/*    baudrate mode.                                                    */
/* 8[MHz] <= (LIN communication clock source)/(Divider value) <= 12[MHz]*/
/* -------------------------------------------------------------------- */
/* ==================================================================== */
/* Modify the following to suit your application.                       */
/* ==================================================================== */
/* (Related LIN) */
#define CONFSLIN_OPT_u1gLPRS_NORM_CFG   CONFSLIN_OPT_u1gLPRS_NODIV        /* Prescaler division for normal              */
#ifdef RLIN3_CFG_USE_SMC_DEFINITION
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define CONFSLIN_OPT_u1gBUSWKUP_CFG     RLIN3_CFG_CH0_BUSWAKEUP           /* Bus wake-up method                         */
#elif (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define CONFSLIN_OPT_u1gBUSWKUP_CFG     RLIN3_CFG_CH1_BUSWAKEUP           /* Bus wake-up method                         */
#endif
#else
#define CONFSLIN_OPT_u1gBUSWKUP_CFG     CONFSLIN_OPT_u1gBUSWKUP_EDGE      /* Bus wake-up method                         */
#endif /* RLIN3_CFG_USE_SMC_DEFINITION */

#define CONFSLIN_OPT_u1gNSPB_NORM_CFG   CONFSLIN_OPT_u1gNSPB_4SMPL        /* Number of bit sampling for normal          */
#define CONFSLIN_OPT_u1gLINMCK_CFG      CONFSLIN_OPT_u1gLINMCK_FCLK       /* Select LIN engine clock source             */
#define CONFSLIN_OPT_u1gRS_CFG          CONFSLIN_OPT_u1gRS_1BIT           /* Submit response space width                */
#define CONFSLIN_OPT_u1gIBS_CFG         CONFSLIN_OPT_u1gIBS_0BIT          /* Submit inter-byte space width              */

#ifdef RLIN3_CFG_USE_SMC_DEFINITION
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define CONFSLIN_OPT_u1gINTLINTRMPR_CFG RLIN3_CFG_INTLIN0TRM_PRIORITY_LEVEL /* Transmission completion interrupt priority */
#define CONFSLIN_OPT_u1gINTLINRVCPR_CFG RLIN3_CFG_INTLIN0RVC_PRIORITY_LEVEL /* Reception completion Interrupt Priority    */
#define CONFSLIN_OPT_u1gINTLINSTAPR_CFG RLIN3_CFG_INTLIN0STA_PRIORITY_LEVEL /* Error interrupt priority                   */
#elif (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define CONFSLIN_OPT_u1gINTLINTRMPR_CFG RLIN3_CFG_INTLIN1TRM_PRIORITY_LEVEL /* Transmission completion interrupt priority */
#define CONFSLIN_OPT_u1gINTLINRVCPR_CFG RLIN3_CFG_INTLIN1RVC_PRIORITY_LEVEL /* Reception completion Interrupt Priority    */
#define CONFSLIN_OPT_u1gINTLINSTAPR_CFG RLIN3_CFG_INTLIN1STA_PRIORITY_LEVEL /* Error interrupt priority                   */
#endif
#else
#define CONFSLIN_OPT_u1gINTLINTRMPR_CFG CONFSLIN_OPT_u1gINTPR_LV0         /* Transmission completion interrupt priority */
#define CONFSLIN_OPT_u1gINTLINRVCPR_CFG CONFSLIN_OPT_u1gINTPR_LV0         /* Reception completion Interrupt Priority    */
#define CONFSLIN_OPT_u1gINTLINSTAPR_CFG CONFSLIN_OPT_u1gINTPR_LV0         /* Error interrupt priority                   */
#endif /* RLIN3_CFG_USE_SMC_DEFINITION */

/* (Related interval timer) */
#ifdef RLIN3_CFG_USE_SMC_DEFINITION
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define CONFSLIN_OPT_u1gTMUNIT_CFG      RLIN3_CFG_CH0_TAU_UNIT                      /* Timer Unit                     */
#define CONFSLIN_OPT_u1gTMCH_CFG        RLIN3_CFG_CH0_TAU_CH                        /* Timer channel                  */
#define CONFSLIN_OPT_u2gTMCLKSEL_CFG    RLIN3_CFG_CH0_TAU_CLKSEL                    /* Select the timer clock         */
#define CONFSLIN_OPT_u1gINTTMPR_CFG     RLIN3_CFG_CH0_TAU_INTTMmn_PRIORITY_LEVEL    /* Timer interrupt priority       */
#elif (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define CONFSLIN_OPT_u1gTMUNIT_CFG      RLIN3_CFG_CH1_TAU_UNIT                      /* Timer Unit                     */
#define CONFSLIN_OPT_u1gTMCH_CFG        RLIN3_CFG_CH1_TAU_CH                        /* Timer channel                  */
#define CONFSLIN_OPT_u2gTMCLKSEL_CFG    RLIN3_CFG_CH1_TAU_CLKSEL                    /* Select the timer clock         */
#define CONFSLIN_OPT_u1gINTTMPR_CFG     RLIN3_CFG_CH1_TAU_INTTMmn_PRIORITY_LEVEL    /* Timer interrupt priority       */
#endif
#else
#define CONFSLIN_OPT_u1gTMUNIT_CFG      CONFSLIN_OPT_u1gTMUNIT_UNIT0      /* Timer Unit                                 */
#define CONFSLIN_OPT_u1gTMCH_CFG        CONFSLIN_OPT_u1gTMCH_CH0          /* Timer channel                              */
#define CONFSLIN_OPT_u2gTMCLKSEL_CFG    CONFSLIN_OPT_u2gTMCLKSEL_SEL1     /* Select the timer clock                     */
#define CONFSLIN_OPT_u1gINTTMPR_CFG     CONFSLIN_OPT_u1gINTPR_LV0         /* Timer interrupt priority                   */
#endif /* RLIN3_CFG_USE_SMC_DEFINITION */

/* (Related external interrupt (If wake-up pulse detection method is "low width detection", it is irrelevant.))         */
#ifdef RLIN3_CFG_USE_SMC_DEFINITION
#if (defined __LIN_CH0_P1__) || (defined __LIN_CH0_P4__)
#define CONFSLIN_OPT_u1gINTPPR_CFG      RLIN3_CFG_INTLIN0WUP_PRIORITY_LEVEL /* External interrupt priority                */
#elif (defined __LIN_CH1_P1__) || (defined __LIN_CH1_P10__) || (defined __LIN_CH1_P12__)
#define CONFSLIN_OPT_u1gINTPPR_CFG      RLIN3_CFG_INTLIN1WUP_PRIORITY_LEVEL /* External interrupt priority                */
#endif
#else
#define CONFSLIN_OPT_u1gINTPPR_CFG      CONFSLIN_OPT_u1gINTPR_LV0         /* External interrupt priority                */
#endif /* RLIN3_CFG_USE_SMC_DEFINITION */


/* ==================================================================== */
/* Basically, do not change the following.                              */
/* ==================================================================== */
/* (Related LIN) */
#define CONFSLIN_OPT_u1gLRDNFS_NORM_CFG CONFSLIN_OPT_u1gLRDNFS_USE        /* Reception noise filter for normal          */
#define CONFSLIN_OPT_u1gLPRS_WKUP_CFG   CONFSLIN_OPT_u1gLPRS_NODIV        /* Prescaler division for Wakeup              */
#define CONFSLIN_OPT_u1gNSPB_WKUP_CFG   CONFSLIN_OPT_u1gNSPB_16SMPL       /* Number of bits Sampling for Wakeup         */
#define CONFSLIN_OPT_u1gLRDNFS_WKUP_CFG CONFSLIN_OPT_u1gLRDNFS_USE        /* Reception noise filter for Wakeup          */
#define CONFSLIN_OPT_u2gWKUPBR_CFG      (19200)                           /* Baud rate for Wakeup[bps] (x>0)            */
#define CONFSLIN_OPT_u1gBLT_CFG         CONFSLIN_OPT_u1gBLT_SHORT         /* Break reception Low detection width        */
#define CONFSLIN_OPT_u1gWUTL_CFG        CONFSLIN_OPT_u1gWUTL_16BIT        /* Wake-up transmission Low width             */
#define CONFSLIN_OPT_u1gBERE_CFG        CONFSLIN_OPT_u1gBERE_ENABLE       /* Bit error detection                        */
#define CONFSLIN_OPT_u1gTER_CFG         CONFSLIN_OPT_u1gTER_DISABLE       /* Timeout error detection                    */
#define CONFSLIN_OPT_u1gFERE_CFG        CONFSLIN_OPT_u1gFERE_ENABLE       /* Framing error detection                    */
#define CONFSLIN_OPT_u1gSFERE_CFG       CONFSLIN_OPT_u1gSFERE_ENABLE      /* Sync field error detection                 */
#define CONFSLIN_OPT_u1gIPERE_CFG       CONFSLIN_OPT_u1gIPERE_ENABLE      /* ID parity error detection                  */
#define CONFSLIN_OPT_u1gLTES_CFG        CONFSLIN_OPT_u1gLTES_FRAMETO      /* Type timeout error detection               */
#define CONFSLIN_OPT_u4gTRWTCNTMAX_CFG  (256)                             /* MAX counter waiting RLIN3 state transition */

/* (Related interval timer) */
#define CONFSLIN_OPT_u2gTMCLKDIV_CFG    CONFSLIN_OPT_u2gTMCLKDIV_DIV2P15  /* Timer clock divider value      */
#define CONFSLIN_OPT_u4gWAKEUPINTVL     (171)                             /* Send wake-up interval[ms]      */
#define CONFSLIN_OPT_u4gWAKEUPINTVL3    (1710)                            /* Send wake-up interval(3rd)     */

/* -------------------------------------------------------------------- */
/* Driver configuration (the above)                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Driver Configuration Check                                           */
/* (The settting forbidden by hardware is considered as an error. )     */
/* -------------------------------------------------------------------- */
#ifdef __LIN_HW_AUTO_BR__
 #if ( CONFSLIN_OPT_u1gNSPB_NORM_CFG == CONFSLIN_OPT_u1gNSPB_16SMPL )
  #error [confslin_opt.h] Set 4 or 8 to the number of bit samplings in LIN Normal Auto Baudrate Mode.
 #endif

 #if ( CONFSLIN_OPT_u1gTER_CFG == CONFSLIN_OPT_u1gTER_ENABLE )
  #error [confslin_opt.h] Set DISABLE to the timeout error detection in LIN Normal Mode Auto Baudrate Mode.
 #endif
#else
 #if ( ( CONFSLIN_OPT_u1gNSPB_NORM_CFG == CONFSLIN_OPT_u1gNSPB_4SMPL ) || \
       ( CONFSLIN_OPT_u1gNSPB_NORM_CFG == CONFSLIN_OPT_u1gNSPB_8SMPL ) )
  #error [confslin_opt.h] Set 16 to the numner of bit samplings in LIN Normal Fixed Baudrate Mode.
 #endif
#endif

#if ( CONFSLIN_OPT_u1gNSPB_WKUP_CFG != CONFSLIN_OPT_u1gNSPB_16SMPL )
 #error [confslin_opt.h] Set 16 to the number of bit samplings in LIN Wakeup Mode.
#endif

/* Note:                                                            */
/* The timeout error must be prohibited and fCLK must be equal or   */
/* more than 1.2 times of fMX when fMX is used as LIN communication */
/* clock.                                                           */
#if ( CONFSLIN_OPT_u1gLINMCK_CFG == CONFSLIN_OPT_u1gLINMCK_FMX ) && \
    ( CONFSLIN_OPT_u1gTER_CFG    == CONFSLIN_OPT_u1gTER_ENABLE )
 #error [confslin_opt.h] Prohibited configuration : u1gLINMCK_CFG and u1gTER_CFG
#endif /* CONFSLIN_OPT_u1gLINMCK_CFG, CONFSLIN_OPT_u1gTER_CFG */


/* -------------------------------------------------------------------- */
/* Public functions                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public constants                                                     */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Public variables                                                     */
/* -------------------------------------------------------------------- */

#endif /* H_CONFSLIN_OPT */

/* End of files */
