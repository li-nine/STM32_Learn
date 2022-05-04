/**
  ******************************************************************************
  * @file       key_exit.c
  * @brief      按键中断的驱动
  * @author     li nine
  * @version    v1.0
  * @date       2022.5.2
  ******************************************************************************
  */

#include "key_exit.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    UNUSED(GPIO_Pin);

    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}
