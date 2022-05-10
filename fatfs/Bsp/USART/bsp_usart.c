/**
  ******************************************************************************
  * @file       bsp_usart.c
  * @brief      串口驱动
  * @author     li nine
  * @version    v1.0
  * @date       2022.05.04
  ******************************************************************************
  */

#include "bsp_usart.h"

extern UART_HandleTypeDef huart1;
uint8_t aRxBuffer;//接收缓存

/**
  * @brief      printf函数的重定向
  * @note       在clion中使用
  * @note       在 main.h 中调用 stdio 库
  * @param      ch : 要发送的字符
  * @retval     ch的ASCII值
  */
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

/**
  * @brief      UART的接收回调
  * @note       是 HAL_UART_RxCpltCallback 的重定义
  * @note       务必在主函数中调用一下
  *                                 HAL_UART_Receive_IT(&huart1, &aRxBuffer, 1);
  * @param      huart : 该变量为 UART_HandleTypeDef 的指针,选择UART接口
  * @retval     无返回值
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_UART_RxCpltCallback could be implemented in the user file
     */
    /* 判断接收串口 */
    if(huart -> Instance == USART1)
    {
        printf("%s", &aRxBuffer);
        HAL_UART_Receive_IT(&huart1, &aRxBuffer, 1);
    }
}
