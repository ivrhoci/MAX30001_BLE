/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"
#include "arm_math.h"
//#include "stm32_seq.h"
//#include "stm32_lpm.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern char USB_command[30];
extern char USB_parameter[6];
extern _Bool USB_new_command;
extern uint32_t reg;
#define INTB_Pin GPIO_PIN_11  //PC11 -> CN10_35
#define INT2B_Pin GPIO_PIN_10 //PC10 -> CN10_29
#define INT_ICM_Pin GPIO_PIN_4    //PC4 -> CN10_1

#define NSS_SPI1_Pin GPIO_PIN_4 //PA4 -> CN10_17
#define NSS_SPI2_Pin GPIO_PIN_12 //PB12 -> CN10_16
#define ICM_CS_Pin GPIO_PIN_4 // PA4

#define ICM_CS_GPIO_Port GPIOA //
#define INTB_GPIO_Port GPIOC //
#define INT2B_GPIO_Port GPIOC //

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void usb_command(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
