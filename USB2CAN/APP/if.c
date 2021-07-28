/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : if.c
  * @brief          : Main program body
  * @author			: zhuyuhao
  ******************************************************************************
  * @attention
  *
  * 主运行函数
  *
  ******************************************************************************
  */
#include "main.h"
#include "usart.h"
#include "can.h"
#include "string.h"
#include "stdio.h"

/**
  * @brief  轮询是否收到串口数据，收到则从can接口发出。
  */

void run(void)
{
    if(recv_end_flag == 1)  //接收完成标志
    {
#ifdef TEST_MODE
        //串口发送串口接收到的数据
        printf("len: %d  data: %s",rx_len, rx_buffer);
#endif
        //CAN发送串口接受到的数据
        CAN_SendData(rx_buffer, rx_len);
        memset(rx_buffer, 0, rx_len);
        rx_len = 0;						//清除计数
        recv_end_flag = 0;				//清除接收完成标志
    }
    else
    {

    }

    HAL_UART_Receive_DMA(&huart1, rx_buffer, BUFFER_SIZE);
}
