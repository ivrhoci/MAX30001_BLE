/**
  ******************************************************************************
  * @file    crs_app.c
  * @author  MCD Application Team
  * @brief   Cable Replacement Service Application
  ******************************************************************************
  * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */



/* Includes ------------------------------------------------------------------*/
#include "app_common.h"

#include "dbg_trace.h"
#include "ble.h"
#include "crs_app.h"
#include "stm32_seq.h"
#include "DispTools.h"
#include "stm_queue.h"

#include "max30001.h"
#include "output_mode.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/

void CRSAPP_Notification(CRSAPP_Notification_evt_t *pNotification)
{
  switch(pNotification->CRS_Evt_Opcode)
  {
    case CRS_WRITE_EVT:
      {
        APP_DBG_MSG("CRS_WRITE_EVT: Data received: %s \n", pNotification->DataTransfered.pPayload);
      }
      break;
      
    case CRS_NOTIFY_ENABLED_EVT:
      {
        APP_DBG_MSG("CRS_NOTIFY_ENABLED_EVT\n");
      }
      break;

    case CRS_NOTIFY_DISABLED_EVT:
      {
        APP_DBG_MSG("CRS_NOTIFY_DISABLED_EVT\n");
      }
      break;

   default:
      break;
  }

  return;
}

void CRSAPP_Init(void)
{
  UTIL_SEQ_RegTask( 1<<CFG_TASK_ECG_OUTPUT_MODE , UTIL_SEQ_RFU, ecg_output_mode); //registiram task za ecg_output_mode
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); //omogucavam prekide za INTB i INT2B

  printf("CRSAPP_Init done\r\n");
  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
