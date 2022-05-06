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
#include "bsp_at24c02_i2c.h"

#define LED_ON_ORDER            "LED0 ON"
#define LED_OFF_ORDER           "LED0 OFF"
#define AT24C02_READ_ORDER      "READ AT24C02"
#define AT24C02_WRITE_ORDER     "WRITE AT24C02"

#define MAXRXBUFFERSIZE 256U          //接收最大字节数

void Menu_Display(void);
uint16_t StrToInt(const char * str, uint8_t n);

#endif //__BSP_USART_CONTROL_H__