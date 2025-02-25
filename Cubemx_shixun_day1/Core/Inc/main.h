/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOA
#define BL_Pin GPIO_PIN_4
#define BL_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_5
#define CS_GPIO_Port GPIOA
#define IA4_Pin GPIO_PIN_6
#define IA4_GPIO_Port GPIOA
#define IB4_Pin GPIO_PIN_7
#define IB4_GPIO_Port GPIOA
#define IA2_Pin GPIO_PIN_0
#define IA2_GPIO_Port GPIOB
#define IB2_Pin GPIO_PIN_1
#define IB2_GPIO_Port GPIOB
#define LCD_RES_Pin GPIO_PIN_12
#define LCD_RES_GPIO_Port GPIOB
#define Trig_Pin GPIO_PIN_14
#define Trig_GPIO_Port GPIOB
#define Echo_Pin GPIO_PIN_6
#define Echo_GPIO_Port GPIOC
#define Echo_EXTI_IRQn EXTI9_5_IRQn
#define OUT1_Pin GPIO_PIN_7
#define OUT1_GPIO_Port GPIOC
#define OUT2_Pin GPIO_PIN_8
#define OUT2_GPIO_Port GPIOC
#define OUT3_Pin GPIO_PIN_9
#define OUT3_GPIO_Port GPIOC
#define DC_Pin GPIO_PIN_12
#define DC_GPIO_Port GPIOA
#define BEEP_Pin GPIO_PIN_15
#define BEEP_GPIO_Port GPIOA
#define OUT4_Pin GPIO_PIN_10
#define OUT4_GPIO_Port GPIOC
#define OUT5_Pin GPIO_PIN_11
#define OUT5_GPIO_Port GPIOC
#define IA3_Pin GPIO_PIN_6
#define IA3_GPIO_Port GPIOB
#define IB3_Pin GPIO_PIN_7
#define IB3_GPIO_Port GPIOB
#define IA1_Pin GPIO_PIN_8
#define IA1_GPIO_Port GPIOB
#define IB1_Pin GPIO_PIN_9
#define IB1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
