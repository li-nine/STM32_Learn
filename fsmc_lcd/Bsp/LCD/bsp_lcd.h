/**
  ******************************************************************************
  * @file       bsp_lcd.h
  * @brief      液晶屏驱动程序
  * @author     li nine
  * @version    v1.0
  * @date       2022.05.12
  ******************************************************************************
  */

#ifndef __BSP_LCD_H__
#define __BSP_LCD_H__

#include "main.h"
#include "fsmc.h"

//LCD使用16bit数据，所以HADDR[25~0]接入FSMC_A[23~0]以接收16bit数据
//HADDR[0]不接    A[6]接在HADDR[7]上所以地址要向左移一位
#define LCD_CMD   *(__IO uint16_t *)((uint32_t)0x6C000080)
#define LCD_DATA  *(__IO uint16_t *)((uint32_t)0x6C00007E)

#endif//__BSP_LCD_H__
