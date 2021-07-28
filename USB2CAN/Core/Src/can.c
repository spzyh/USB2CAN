/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * can接受发送的文件
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
#include "string.h"
#include "stdio.h"
CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];
CAN_TxHeaderTypeDef TxHeader;
uint32_t TxMailbox;
uint32_t std_id = 0x00000003;

/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

    hcan.Instance = CAN1;
    hcan.Init.Prescaler = 9;
    hcan.Init.Mode = CAN_MODE_NORMAL;				//测试模式：CAN_MODE_LOOPBACK	一般收发：CAN_MODE_NORMAL
#ifdef TEST_MODE
    hcan.Init.Mode = CAN_MODE_LOOPBACK;
#endif
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_7TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_8TQ;
    hcan.Init.TimeTriggeredMode = DISABLE;
    hcan.Init.AutoBusOff = DISABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = DISABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;
    if (HAL_CAN_Init(&hcan) != HAL_OK)
    {
        Error_Handler();
    }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(canHandle->Instance==CAN1)
    {
        /* USER CODE BEGIN CAN1_MspInit 0 */

        /* USER CODE END CAN1_MspInit 0 */
        /* CAN1 clock enable */
        __HAL_RCC_CAN1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**CAN GPIO Configuration
        PA11     ------> CAN_RX
        PA12     ------> CAN_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* CAN1 interrupt Init */
        HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
        /* USER CODE BEGIN CAN1_MspInit 1 */

        /* USER CODE END CAN1_MspInit 1 */
    }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

    if(canHandle->Instance==CAN1)
    {
        /* USER CODE BEGIN CAN1_MspDeInit 0 */

        /* USER CODE END CAN1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_CAN1_CLK_DISABLE();

        /**CAN GPIO Configuration
        PA11     ------> CAN_RX
        PA12     ------> CAN_TX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

        /* CAN1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
        /* USER CODE BEGIN CAN1_MspDeInit 1 */

        /* USER CODE END CAN1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

void filter1_init(void)
{
    CAN_FilterTypeDef filter1;
    filter1.FilterBank=0;						//滤波器编号
    filter1.FilterMode=CAN_FILTERMODE_IDMASK;	//掩码模式
    filter1.FilterScale=CAN_FILTERSCALE_32BIT;
    filter1.FilterIdHigh=0x0000;
    filter1.FilterIdLow=0x0000;
    filter1.FilterMaskIdHigh=0x0000;
    filter1.FilterMaskIdLow=0x0000;
    filter1.FilterFIFOAssignment=CAN_FILTER_FIFO0;//FIFO0
    filter1.FilterActivation=ENABLE;

    if(HAL_CAN_ConfigFilter(&hcan,&filter1)!=HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

    static CAN_RxPacketTypeDef packet;

    // CAN数据接收
    if (hcan->Instance == hcan->Instance)
    {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &packet.hdr, packet.payload) == HAL_OK)		// 获得接收到的数据头和数据
        {
            //串口发送接受到的数据信息
#ifdef TEST_MODE
            printf("******** CAN RECV ********\r\n");
            printf("STID:0x%X\r\n",packet.hdr.StdId);
            printf("EXID:0x%X\r\n",packet.hdr.ExtId);
            printf("DLC :%d\r\n", packet.hdr.DLC);
            printf("DATA:");
#endif
            //串口发送接受的数据
            for(int i = 0; i < packet.hdr.DLC; i++)
            {
//				printf("0x%02X ", packet.payload[i]);
                printf("%c", packet.payload[i]);
            }
//			printf("\r\n");		//可通过0x0D 0x0A 帮助分割雷达每帧的数据
#ifdef TEST_MODE
            printf("\r\n");
#endif
            HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);						// 再次使能FIFO0接收中断
        }
    }
}

void CAN_HeaderINIT(void)
{
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.StdId = std_id;
    TxHeader.TransmitGlobalTime = DISABLE;
    TxHeader.DLC = 8;
    std_id = 0x00000003;
}


void CAN_INIT(void)
{
    MX_CAN_Init();
    filter1_init();
    HAL_CAN_Start(&hcan);
    HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
    CAN_HeaderINIT();
}


HAL_StatusTypeDef CAN_SendData(uint8_t aData[], uint32_t len)
{
    uint16_t i,j;
    uint8_t sData[8]= {0};
	
    for(i = 0; i < (len - 1)/8 + 1; i++)
    {
        for(j = 0; j <8 ; j++)
        {
            sData[j] = aData[j+8*i];
        }
#ifdef TEST_MODE
        printf("\r\n\r\n\r\n\r\n******** send data stage: %d ********\r\n", i);
        printf("send adata %d: %s\r\n", i, sData);
#endif
        if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, sData, &TxMailbox) != HAL_OK)
        {
#ifdef TEST_MODE
            printf("CAN Send Error!");
#endif
            return HAL_ERROR;
        }
    }
	
    return HAL_OK;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
