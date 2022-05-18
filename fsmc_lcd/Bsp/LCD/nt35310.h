/**
  ******************************************************************************
  * @file       nt35310.h
  * @brief      nt35310 驱动
  * @author     li nine
  * @version    v1.0
  * @date       2022.05.13
  ******************************************************************************
  */

#ifndef __NT35310_H__
#define __NT35310_H__

#include "bsp_lcd.h"

#define      NT35310_DELAY()               Delay(0x5000)
/*************************定义驱动库底层函数**************************************/
#define NT35310_ReadData()              LCD_ReadData()
#define NT35310_WriteData(data)         LCD_WriteData(data)
#define NT35310_WriteCmd(regval)        LCD_WriteCmd(regval)
/*****************************************************************************/


#define NT35310ColumnCmd            0x2A
#define NT35310RowCmd               0x2B
#define NT35310WriteStartCmd        0x2C
#define NT35310DisplayOFFCmd        0x28
#define NT35310DisplayONCmd         0x29
#define NT35310ScanDirectionCmd     0x36

void NT35310_Init(void);
uint16_t NT35310_ReadID(void);

#endif//__NT35310_H__
