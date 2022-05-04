/**
  ******************************************************************************
  * @file       bsp_usart_control.c
  * @brief      串口控制驱动
  * @author     li nine
  * @version    v1.0
  * @date       2022.05.04
  ******************************************************************************
  */

#include "bsp_usart_control.h"
#include <string.h>

#define LED0_ON_ORDER   "LED0 ON"
#define LED0_OFF_ORDER  "LED0 OFF"
#define LED1_ON_ORDER   "LED1 ON"
#define LED1_OFF_ORDER  "LED1 OFF"
#define BEEP_ON_ORDER   "BEEP ON"
#define BEEP_OFF_ORDER  "BEEP OFF"
#define MAXRXBUFFERSIZE 256U          //接收最大字节数

extern UART_HandleTypeDef huart1;
uint8_t aRxBuffer;                  //接收缓存
uint8_t RxBuffer[MAXRXBUFFERSIZE];  //接收数据
uint8_t aRxBuffer_cnt = 0;          //接收缓存计数


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

    /* 判断接收串口 */
    if(huart -> Instance == USART1)
    {
        RxBuffer[aRxBuffer_cnt++] = aRxBuffer;
        if(aRxBuffer_cnt > MAXRXBUFFERSIZE - 1)
        {
            aRxBuffer_cnt = 0;
            printf("数据溢出");
            memset(RxBuffer, 0x00, sizeof(RxBuffer));
        }
        else
        {
            if(RxBuffer[aRxBuffer_cnt - 1] == 0x0A && RxBuffer[aRxBuffer_cnt - 2] == 0x0D)
            {
                if(strstr((const char *)RxBuffer, LED0_ON_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
                    printf("LED0灯 亮\r\n");
                }
                else if(strstr((const char *)RxBuffer, LED0_OFF_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
                    printf("LED0灯 灭\r\n");
                }
                else if(strstr((const char *)RxBuffer, LED1_ON_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
                    printf("LED1灯 亮\r\n");
                }
                else if(strstr((const char *)RxBuffer, LED1_OFF_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
                    printf("LED1灯 灭\r\n");
                }
                else if(strstr((const char *)RxBuffer, BEEP_ON_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
                    printf("蜂鸣器 开\r\n");
                }
                else if(strstr((const char *)RxBuffer, BEEP_OFF_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
                    printf("蜂鸣器 关\r\n");
                }
                else
                    printf("%s", RxBuffer);
                memset(RxBuffer, 0x00, sizeof(RxBuffer));
            }
        }
        HAL_UART_Receive_IT(&huart1, &aRxBuffer, 1);
    }
}
