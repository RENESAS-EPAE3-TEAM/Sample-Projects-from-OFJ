/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_rlin3_m_callout.c
 * Description  : This is source file of callout functions for LIN master driver.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.02.2022 1.00     Initial Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "platform.h"
#include "r_rlin3_config.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if (RLIN3_CFG_USE_LIN_CH0 != 0) && (RLIN3_CFG_CH0_OPERATION_MODE == 0)
#define LIN_MASTER_CH0  0
#endif
#if (RLIN3_CFG_USE_LIN_CH1 != 0) && (RLIN3_CFG_CH1_OPERATION_MODE == 0)
#define LIN_MASTER_CH1  1
#endif

#if   defined(LIN_MASTER_CH0)
#include "conflin_0.h"

#elif defined(LIN_MASTER_CH1)
#include "conflin_1.h"

#endif

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
#if defined(LIN_MASTER_CH0) && defined(LIN_MASTER_CH1)
extern u1 ApMLin_u1gInitIfc_1(void); /* l_ifc_init */
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/


/*********************************************************************************************************************/
/* User call out function                                                                                            */
/*********************************************************************************************************************/
#if defined(__LIN_CH0__) || defined(__LIN_CH1__)

/**********************************************************************************************************************
 * Function Name: l_sys_irq_disable
 * Description  : This function disables interrupt.
 * Arguments    : Nothing
 * Return Value : [CC-RL] IE bit of PSW(bit 7).
 *                [ICCRL78] Global interrupt status
 * Misc.        : Nothing
 *********************************************************************************************************************/
l_irqmask l_sys_irq_disable(void)
{
    l_irqmask u1tPreIrq;

#if defined(__CCRL__)
    u1tPreIrq = __get_psw();                                /* Save current PSW value.                              */
    u1tPreIrq = (u1)0x80U & u1tPreIrq;                      /* Return IE (bit 7) only.                              */

#elif defined(__ICCRL78__)
    u1tPreIrq = __get_interrupt_state();                    /* Save current global interrupt status.                */

#else
    #error "This file for use with CC-RL or ICCRL78 only"

#endif

    BSP_DISABLE_INTERRUPT();

    return u1tPreIrq;
}
/**********************************************************************************************************************
 End of function l_sys_irq_disable
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: l_sys_irq_restore
 * Description  : This function enables interrupt.
 * Arguments    : previous -
 *                    [CC-RL] IE value of last l_sys_irq_disable.
 *                    [ICCRL78] Global interrupt status of last l_sys_irq_disable.
 * Return Value : Nothing
 * Misc.        : Driver sets returned value from l_sys_irq_disable to this parameter.
 *********************************************************************************************************************/
void l_sys_irq_restore(l_irqmask previous)
{
#if defined(__CCRL__)
    if (previous == (l_irqmask)0x80U)                           /* Check IE flag.                                    */
    {
        BSP_ENABLE_INTERRUPT();
    }
    else
    {
        BSP_DISABLE_INTERRUPT();
    }
#elif defined(__ICCRL78__)
     __set_interrupt_state(previous);
#endif

    return;
}
/**********************************************************************************************************************
 End of function l_sys_irq_restore
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: l_sys_call_wake_up
 * Description  : This function notifies bus wakeup to application.
 * Arguments    : iii -
 *                    Name of LIN interface. 
 * Return Value : Nothing
 * Misc.        : Nothing
 *********************************************************************************************************************/
void l_sys_call_wake_up(l_ifc_handle iii)
{
#ifdef LIN_MASTER_CH0
  /* Edit user code of channel 0 if necessary */
#endif
#ifdef LIN_MASTER_CH1
  /* Edit user code of channel 1 if necessary */
#endif

    return;
}
/**********************************************************************************************************************
 End of function l_sys_call_wake_up
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: l_sys_call_fatal_error
 * Description  : This function notifies fatal error to application.
 * Arguments    : iii -
 *                    Name of LIN interface. 
 * Return Value : Nothing
 * Misc.        : This function is called when an occurred error cannot be recovered in LIN driver.
 *                Driver has been disconnected from LIN bus before this function.
 *                l_ifc_connect() should be called in this function to start LIN communication.
 *********************************************************************************************************************/
void l_sys_call_fatal_error(l_ifc_handle iii) 
{
#ifdef LIN_MASTER_CH0
    if(iii == LIN_MASTER_CH0)
    {
      if(l_ifc_init(LIN_MASTER_CH0) != L_SUCCESS)
      {
          /*---------------------------------------------------------*/
          /* Please retry l_ifc_init if it failed.                   */
          
      }
    }
#endif
#ifdef LIN_MASTER_CH1
    if(iii == LIN_MASTER_CH1)
    {
      if(l_ifc_init(LIN_MASTER_CH1) != L_SUCCESS)
      {
          /*---------------------------------------------------------*/
          /* Please retry l_ifc_init if it failed.                   */
          
      }
    }
#endif

    return;
}
/**********************************************************************************************************************
 End of function l_sys_call_fatal_error
 *********************************************************************************************************************/

#endif /* #if defined(__LIN_CH0__) || defined(__LIN_CH1__) */

/* end of file */
