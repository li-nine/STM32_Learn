/**
  ******************************************************************************
  * @file       key.c
  * @brief      按键驱动
  * @author     li nine
  * @version    v1.0
  * @date       2022.5.2
  ******************************************************************************
  */

#include "key.h"

/**
  * @brief      检查按键是否被按下
  * @note       只能检查特定的按键
  * @param      GPIOx : 该参数为 GPIO_Typedef 类型的指针,指向 GPIO 的地址
  * @param      GPIO_Pin : 选择 GPIO 的端口
  * @retval     KEY_ON  : 按键被按下
  *             KEY_OFF : 按键未被按下
  */
uint8_t Key_scan(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin)
{
    //检查按键是否被按下
    if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == KEY_ON)
    {
        //等待按键松开
        while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == KEY_ON);

        return KEY_ON;
    }
    return KEY_OFF;
}
