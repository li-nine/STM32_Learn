/**
  ******************************************************************************
  * @file       bsp_dma.h
  * @brief      串口使用DMA通讯驱动
  * @author     li nine
  * @version    v1.0
  * @date       20220.05.04
  ******************************************************************************
  */

#include "bsp_dma.h"

extern UART_HandleTypeDef huart1;
uint8_t aRxBuffer;

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
    return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart -> Instance == USART1)
    {
        printf("%s", &aRxBuffer);
        HAL_UART_Receive_IT(&huart1, &aRxBuffer, 1);
    }
}
