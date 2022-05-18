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
#include "nt35310.h"

#define LCD_WIDTH       320
#define LCD_HEIGHT      480

#define      DEBUG_DELAY()               Delay(0x5000)
/**********************************LCD 接口******************************************/
//LCD 背光
#define LCD_BL_ON   HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET)
#define LCD_BL_OFF  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET)
//LCD 结构体
typedef struct{
    __IO uint16_t LCD_CMD;
    __IO uint16_t LCD_DATA;
}LCD_TypeDef;
//LCD使用16bit数据，所以HADDR[25~1]接入FSMC_A[23~0]以接收16bit数据
//HADDR[0]不接    A[6]接在HADDR[7]上所以地址要向左移一位
//HADDR 上的 0111 1110 = 7E   在FSMC_A上为 0011 1111     A[6] = 0
#define LCD_BASE    ((uint32_t)(0x6C000000 | 0x0000007E))
#define LCD        ((LCD_TypeDef *)LCD_BASE)
/************************************************************************************/

typedef struct{
    uint8_t dir;        // 横屏/竖屏
    uint16_t id;        // LCD ID
    uint16_t col;       // LCD 列
    uint16_t row;       // LCD 行
    uint16_t wscmd;     // LCD 开始写命令
    uint16_t colcmd;    // LCD 设置列命令
    uint16_t rowcmd;    // LCD 设置行命令
}LCDPosTypeDef;

/******************扫描方向定义*******************/
#define L2R_U2D  0 		//从左到右,从上到下
#define L2R_D2U  1 		//从左到右,从下到上
#define R2L_U2D  2 		//从右到左,从上到下
#define R2L_D2U  3 		//从右到左,从下到上

#define U2D_L2R  4 		//从上到下,从左到右
#define U2D_R2L  5 		//从上到下,从右到左
#define D2U_L2R  6 		//从下到上,从左到右
#define D2U_R2L  7		//从下到上,从右到左

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向
/**********************************************/

extern uint16_t POINT_COLOR;
extern uint16_t BACK_COLOR;
/*********************画笔颜色***********************************/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY      0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
/******************************************************************/

/***********************用户使用函数*************************************************************************/
void LCD_Init(void);
void LCD_Clear(uint16_t color);
void LCD_Fill(uint16_t StartX, uint16_t StartY, uint16_t EndX, uint16_t EndY, uint16_t color);
void LCD_DrawPoint(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t size, uint8_t  ch);
void LCD_ShowString(uint16_t x, uint16_t y, uint8_t size, uint8_t * str);










/**********************************************************************************************************/

/*************************驱动实现函数************************/
void LCD_WriteRAM(void);
uint16_t LCD_ReadData(void);
void LCD_WriteCmd(uint16_t usCmd);
void LCD_WriteData(uint16_t usData);
uint16_t LCD_Read_REG(uint16_t regval);
void LCD_Scan_Direction(uint8_t ScanDir);
void LCD_Display_Direction(uint8_t dir);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_Write_REG(uint16_t regval, uint16_t data);
/**********************************************************/
#endif//__BSP_LCD_H__
