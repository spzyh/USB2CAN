/**
  ******************************************************************************
  * File Name          : CAN.h
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __can_H
#define __can_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan;

/* USER CODE BEGIN Private defines */

extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t RxData[8];
extern CAN_TxHeaderTypeDef TxHeader;
extern uint8_t  TxData[8] ;
extern uint32_t TxMailbox;

typedef struct
{
    uint32_t mailbox;
    CAN_TxHeaderTypeDef hdr;
    uint8_t payload[8];
} CAN_TxPacketTypeDef;

typedef struct
{
    CAN_RxHeaderTypeDef hdr;
    uint8_t payload[8];
} CAN_RxPacketTypeDef;

/* USER CODE END Private defines */

void MX_CAN_Init(void);

/* USER CODE BEGIN Prototypes */


/**
  * @brief  滤波器初始化。
  */
void filter1_init(void);

/**
  * @brief  CAN接受中断。
  * @param  hcan: 	can。
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

/**
  * @brief  can初始化。
  */
void CAN_INIT(void);

/**
  * @brief  CAN发送一段大于8字节的数据。
  * @param  aData: 	待发送数据。
  * @param  len: 	数据大小。
  * @retval 发送成功/失败。
  */
HAL_StatusTypeDef CAN_SendData(uint8_t aData[], uint32_t len);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ can_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
