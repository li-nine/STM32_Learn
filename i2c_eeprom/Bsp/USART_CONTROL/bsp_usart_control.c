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
#include "math.h"

extern UART_HandleTypeDef huart1;
uint8_t aRxBuffer;                  //接收缓存
uint8_t RxBuffer[MAXRXBUFFERSIZE];  //接收数据
uint16_t aRxBuffer_cnt = 0;          //接收缓存计数
uint8_t WriteBuffer[BUFFER_SIZE];
uint8_t ReadBuffer[BUFFER_SIZE];


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
    uint i;

    /* 判断接收串口 */
    if(huart -> Instance == USART1)
    {
        RxBuffer[aRxBuffer_cnt++] = aRxBuffer;
        if(aRxBuffer_cnt >= MAXRXBUFFERSIZE)
        {
            aRxBuffer_cnt = 0;
            printf("Data is too long\r\n");
            memset(RxBuffer, 0x00, sizeof(RxBuffer));//清除缓存数据
        }
        else
        {
            if(RxBuffer[aRxBuffer_cnt - 1] == '\r' && RxBuffer[aRxBuffer_cnt - 2] == '\n')
            {
                if(strstr((const char *)RxBuffer, LED_ON_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
                    printf("LED0 ON\r\n");
                }
                else if(strstr((const char *)RxBuffer, LED_OFF_ORDER) != NULL)
                {
                    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
                    printf("LED0 OFF\r\n");
                }
                else if(strstr((const char *)RxBuffer, AT24C02_READ_ORDER) != NULL)
                {
                    if(AT24C02_Read(StrToInt((const char *)RxBuffer, 14), ReadBuffer, StrToInt((const char *)RxBuffer, 18)) == AT24C02_OK)
                        printf("%s\r\n", ReadBuffer);
                    else
                        printf("AT24C02 Read error!\r\n");
                }
                else if(strstr((const char *)RxBuffer, AT24C02_WRITE_ORDER) != NULL)
                {
                    for(i = 0; i < strlen((const char *)RxBuffer); i++)
                        WriteBuffer[i + 19] = RxBuffer[i];
                    if(AT24C02_Write(StrToInt((const char *)RxBuffer, 15), WriteBuffer) == AT24C02_OK)
                        printf("AT24C02 Write success!\r\n");
                    else
                        printf("AT24C02 Read error!\r\n");
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
    printf("---------------menu--------------\r\n");
    printf("-active---------------order----------\r\n");
    printf("--------------------------------\r\n");
    printf("LED 亮          | %s\r\n", LED_ON_ORDER);
    printf("LED 灭          | %s\r\n", LED_OFF_ORDER);
    printf("READ AT24C02 xxx xxx      | Read AT24C02\r\n");
    printf("READ AT24C02 xxx -------  | Write AT24C02\r\n");
}

uint16_t StrToInt(const char * str, uint8_t n)
{
    uint8_t i;
    uint8_t sum = 0;
    uint8_t count = 2;

    for(i = n - 1; i < 3; i++)
    {
        if(str[i] != 0x20)
            sum += (str[i] - 32) * (uint8_t)pow(10, count--);
        else
            sum += (str[i] - 48) * (uint8_t)pow(10, count--);
    }

    return sum;
}
