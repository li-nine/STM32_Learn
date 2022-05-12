/**
  ******************************************************************************
  * @file       bsp_lcd.h
  * @brief      液晶屏驱动程序
  * @author     li nine
  * @version    v1.0
  * @date       2022.05.12
  ******************************************************************************
  */

#include "bsp_lcd.h"

/**
  * @brief      向 LCD 写入命令
  * @param      usCMD   : 要写入的命令（外部寄存器地址）
  * @retval     无
  */
void LCD_WriteCmd(uint16_t usCmd)
{
    LCD_CMD = usCmd;
}

/**
  * @brief      向 LCD 写入数据
  * @param      usData  : 要写入的数据
  * @retval     无
  */
void LCD_WriteData(uint16_t usData)
{
    LCD_DATA = usData;
}

