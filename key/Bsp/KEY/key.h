/**
  ******************************************************************************
  * @file       key.h
  * @brief      按键驱动
  * @author     li nine
  * @version    v1.0
  * @date       2022.5.2
  ******************************************************************************
  */

#ifndef KEY_KEY_H
#define KEY_KEY_H

#include "main.h"

#define KEY_ON  0
#define KEY_OFF 1

uint8_t Key_scan(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin);

#endif //KEY_KEY_H
