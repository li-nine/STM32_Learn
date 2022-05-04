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
            printf("Data is too long\r\n");
            memset(RxBuffer, 0x00, sizeof(RxBuffer));//清除缓存数据
        }
        else
        {
            if(RxBuffer[aRxBuffer_cnt - 1] == '\r' && RxBuffer[aRxBuffer_cnt - 2] == '\n')
            {
                if(strstr((const char *)RxBuffer, LED0_ON_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
                    printf("LED0 ON\r\n");
                }
                else if(strstr((const char *)RxBuffer, LED0_OFF_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
                    printf("LED0 OFF\r\n");
                }
                else if(strstr((const char *)RxBuffer, LED1_ON_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
                    printf("LED1 ON\r\n");
                }
                else if(strstr((const char *)RxBuffer, LED1_OFF_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
                    printf("LED1 OFF\r\n");
                }
                else if(strstr((const char *)RxBuffer, BEEP_ON_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
                    printf("BEEP ON\r\n");
                }
                else if(strstr((const char *)RxBuffer, BEEP_OFF_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
                    printf("BEEP OFF\r\n");
                }
                else
                    printf("%s", RxBuffer);
                aRxBuffer_cnt = 0;
                memset(RxBuffer, 0x00, sizeof(RxBuffer));//清除缓存数据
            }
        }
        HAL_UART_Receive_IT(&huart1, &aRxBuffer, 1);
    }
}

/**
  * @brief      显示一个菜单
  * @param      无参数
  * @retval     无返回值
  */
void Menu_Display(void)
{
    //显示菜单
    printf("--------------------------------\r\n");
    printf("---------------菜单--------------\r\n");
    printf("-行为---------------命令----------\r\n");
    printf("--------------------------------\r\n");
    printf("LED0 亮          | %s\r\n", LED0_ON_ORDER);
    printf("LED0 灭          | %s\r\n", LED0_OFF_ORDER);
    printf("LED1 亮          | %s\r\n", LED1_ON_ORDER);
    printf("LED1 灭          | %s\r\n", LED1_OFF_ORDER);
    printf("BEEP 开          | %s\r\n", BEEP_ON_ORDER);
    printf("BEEP 关          | %s\r\n", BEEP_OFF_ORDER);
}
