/* Generated configuration header file - do not edit */
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

#ifndef RLIN3_CONFIG_HEADER_FILE
#define RLIN3_CONFIG_HEADER_FILE


/***********************************************************************************************************************
 Configuration Options
 ***********************************************************************************************************************/

#define RLIN3_CFG_DEVICENAME                        (R7F124FPJ5xFB)         /* Target Device                                      */

#define RLIN3_CFG_USE_LIN_CH0                       (1)         /* Use RLIN3 ch.0                                     */
                                                                /* 0: Unuse                                           */
                                                                /* 1: P13,P14                                         */
                                                                /* 2: P42,P43                                         */

#define RLIN3_CFG_USE_LIN_CH1                       (0)         /* Use RLIN3 ch.1                                     */
                                                                /* 0: Unuse                                           */
                                                                /* 1: P10, P11                                        */
                                                                /* 2: P106,P107                                       */
                                                                /* 3: P120,P125                                       */

#define RLIN3_CFG_CH0_OPERATION_MODE                (1)         /* RLIN3(ch.0) Operation mode                         */
#define RLIN3_CFG_CH1_OPERATION_MODE                (0)         /* RLIN3(ch.1) Operation mode                         */
                                                                /* 0: Master Mode                                     */
                                                                /* 1: Slave Mode                                      */

#define RLIN3_CFG_CH0_INPUT_CLOCK                   (0)         /* RLIN3(ch.0) Input Clock                            */
#define RLIN3_CFG_CH1_INPUT_CLOCK                   (0)         /* RLIN3(ch.1) Input Clock                            */
                                                                /* 0: 40 MHz                                          */
                                                                /* 1: 32 MHz                                          */
                                                                /* 2: 24 MHz                                          */
                                                                /* 3: 20 MHz                                          */
                                                                /* 4: 16 MHz                                          */
                                                                /* 5: 12 MHz                                          */
                                                                /* 6: 10 MHz                                          */
                                                                /* 7:  8 MHz                                          */

#define RLIN3_CFG_CH0_BUSWAKEUP                     (0x00)      /* RLIN3(ch.0) Wakeup mode                            */
#define RLIN3_CFG_CH1_BUSWAKEUP                     (0x00)      /* RLIN3(ch.1) Wakeup mode                            */
                                                                /* 0x00: Falling edge detection mode                  */
                                                                /* 0x01: Dominant width detection mode                */

#define RLIN3_CFG_INTLIN0TRM_PRIORITY_LEVEL         (0)         /* INTLIN0TRM interrupts priority                     */
#define RLIN3_CFG_INTLIN0RVC_PRIORITY_LEVEL         (0)         /* INTLIN0RVC interrupts priority                     */
#define RLIN3_CFG_INTLIN0STA_PRIORITY_LEVEL         (0)         /* INTLIN0STA interrupts priority                     */
#define RLIN3_CFG_INTLIN0WUP_PRIORITY_LEVEL         (0)         /* INTLIN0WUP interrupts priority                     */
#define RLIN3_CFG_INTLIN1TRM_PRIORITY_LEVEL         (0)         /* INTLIN1TRM interrupts priority                     */
#define RLIN3_CFG_INTLIN1RVC_PRIORITY_LEVEL         (0)         /* INTLIN1RVC interrupts priority                     */
#define RLIN3_CFG_INTLIN1STA_PRIORITY_LEVEL         (0)         /* INTLIN1STA interrupts priority                     */
#define RLIN3_CFG_INTLIN1WUP_PRIORITY_LEVEL         (0)         /* INTLIN1WUP interrupts priority                     */
                                                                /* 0: Level 0 (highest)                               */
                                                                /* 1: Level 1                                         */
                                                                /* 2: Level 2                                         */
                                                                /* 3: Level 3 (Lowest)                                */

/* Slave mode Only. */
#define RLIN3_CFG_CH0_TAU_UNIT                      (0x00)      /* RLIN3(ch.0) Timer unit number                      */
#define RLIN3_CFG_CH1_TAU_UNIT                      (0x00)      /* RLIN3(ch.1) Timer unit number                      */
                                                                /* 0x00: Unit 0                                       */
                                                                /* 0x01: Unit 1                                       */

#define RLIN3_CFG_CH0_TAU_CH                        (0x00)      /* RLIN3(ch.0) Timer channel number                   */
#define RLIN3_CFG_CH1_TAU_CH                        (0x00)      /* RLIN3(ch.1) Timer channel number                   */
                                                                /* 0x00: Channel 0                                    */
                                                                /* 0x01: Channel 1                                    */
                                                                /* 0x02: Channel 2                                    */
                                                                /* 0x03: Channel 3                                    */
                                                                /* 0x04: Channel 4                                    */
                                                                /* 0x05: Channel 5                                    */
                                                                /* 0x06: Channel 6                                    */
                                                                /* 0x07: Channel 7                                    */

#define RLIN3_CFG_CH0_TAU_CLKSEL                    (0x8000)    /* RLIN3(ch.0) Select timer clock                     */
#define RLIN3_CFG_CH1_TAU_CLKSEL                    (0x8000)    /* RLIN3(ch.1) Select timer clock                     */
                                                                /* 0x0000: fMCK = CKm0                                */
                                                                /* 0x8000: fMCK = CKm1                                */

#define RLIN3_CFG_CH0_TAU_INTTMmn_PRIORITY_LEVEL    (0)         /* RLIN3(ch.0) INTTMmn interrupts priority            */
#define RLIN3_CFG_CH1_TAU_INTTMmn_PRIORITY_LEVEL    (0)         /* RLIN3(ch.1) INTTMmn interrupts priority            */
                                                                /* 0: Level 0 (highest)                               */
                                                                /* 1: Level 1                                         */
                                                                /* 2: Level 2                                         */
                                                                /* 3: Level 3 (Lowest)                                */


#define RLIN3_CFG_USE_SMC_DEFINITION                            /* Use the SMC definition                             */

#endif /* RLIN3_CONFIG_HEADER_FILE */
