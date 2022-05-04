/**
  ******************************************************************************
  * @file       bsp_usart_control.h
  * @brief      串口控制驱动
  * @author     li nine
  * @version    v1.0
  * @date       2022.05.04
  ******************************************************************************
  */

#ifndef __BSP_USART_CONTROL_H__
#define __BSP_USART_CONTROL_H__

#include "main.h"

#define LED0_ON_ORDER   "LED0 ON"
#define LED0_OFF_ORDER  "LED0 OFF"
#define LED1_ON_ORDER   "LED1 ON"
#define LED1_OFF_ORDER  "LED1 OFF"
#define BEEP_ON_ORDER   "BEEP ON"
#define BEEP_OFF_ORDER  "BEEP OFF"
#define MAXRXBUFFERSIZE 256U          //接收最大字节数

void Menu_Display(void);

#endif //__BSP_USART_CONTROL_H__