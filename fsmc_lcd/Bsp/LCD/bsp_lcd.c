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
#include "font.h"

LCDPosTypeDef lcd_pos;
uint16_t POINT_COLOR = RED;
uint16_t BACK_COLOR = WHITE;

/**
  * @brief  简单延时函数
  * @param  nCount ：延时计数值
  * @retval 无
  */
static void Delay ( __IO uint32_t nCount )
{
    while(nCount--);
}

/**
  * @brief      LCD 初始化
  * @param      无
  * @retval     无
  */
void LCD_Init(void)
{
    lcd_pos.id = NT35310_ReadID();
    printf("LCD ID: %4X\r\n", lcd_pos.id);
    if(lcd_pos.id == 0x5310)
    {
        NT35310_Init();
    }
    LCD_Display_Direction(0);
    LCD_BL_ON;
    LCD_Clear(WHITE);
}

/**
  * @brief      清屏
  * @param      color : 清屏后的颜色
  * @retval     无
  */
void LCD_Clear(uint16_t color)
{
    uint32_t index = 0;
    uint32_t TotalPrint = 0;

    TotalPrint = lcd_pos.col * lcd_pos.row;
    LCD_SetCursor(0x0000, 0x0000);
    LCD_WriteRAM();
    for(index = 0; index < TotalPrint; index++)
        LCD -> LCD_DATA = color;
}

/**
  * @brief      在指定矩形区域填充单个颜色
  * @note       (StartX, StartY), (EndX, EndY) :矩形的对角坐标
  * @param      StartX  : 开始点的横坐标
  * @param      StartY  : 开始点的纵坐标
  * @param      EndX    : 结束点的横坐标
  * @param      EndY    : 结束点的纵坐标
  * @param      color   : 填充的颜色
  * @retval     无
  */
void LCD_Fill(uint16_t StartX, uint16_t StartY, uint16_t EndX, uint16_t EndY, uint16_t color)
{
    uint16_t i, j;

    for(i = StartY; i <= EndY; i++)
    {
        LCD_SetCursor(StartX, i);
        LCD_WriteRAM();
        for(j = StartX; j <= EndX; j++)
            LCD -> LCD_DATA = color;
    }
}

/**
  * @brief      画点
  * @param      (Xpos, Ypos): 点的坐标
  * @retval     无
  */
void LCD_DrawPoint(uint16_t Xpos, uint16_t Ypos)
{
    LCD_SetCursor(Xpos, Ypos);
    LCD_WriteRAM();
    LCD -> LCD_DATA = POINT_COLOR;
}

/**
  * @brief      画线
  * @param      (x1, y1):起点坐标
  * @param      (x2, y2):终点坐标
  * @retval     无
  */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint16_t t;
    int delta_x, delta_y, distance;
    int incx, incy, uCol, uRow;
    int xerr = 0;
    int yerr = 0;

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    uCol = x1;
    uRow = y1;
    if(delta_x > 0)
        incx = 1;
    else if(delta_x == 0)
        incx = 0;
    else
    {
        incx = -1;
        delta_x =  -delta_x;
    }
    if(delta_y > 0)
        incy = 1;
    else if(delta_y == 0)
        incy = 0;
    else
    {
        incy = -1;
        delta_y =  -delta_y;
    }
    if(delta_x > delta_y)
        distance = delta_x;
    else
        distance = delta_y;
    for(t = 0; t <= distance + 1; t++)
    {
        LCD_DrawPoint(uCol, uRow);
        xerr += delta_x;
        yerr += delta_y;

        if(xerr > distance)
        {
            xerr -= distance;
            uCol += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            uRow += incy;
        }
    }
}

/**
  * @brief      画矩形
  * @param      (x1, y1),(x2, y2) : 矩形对角坐标
  * @retval     返回值
  */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_DrawLine(x1,y1,x2,y1);
    LCD_DrawLine(x1,y1,x1,y2);
    LCD_DrawLine(x1,y2,x2,y2);
    LCD_DrawLine(x2,y1,x2,y2);
}

/**
  * @brief      画圆
  * @param      (x0, y0): 圆心坐标
  * @param      r   : 半径
  * @retval     无
  */
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);       //判断下个点位置的标志

    while (a <= b)
    {
        LCD_DrawPoint(x0 + a, y0 - b);        //5
        LCD_DrawPoint(x0 + b, y0 - a);        //0
        LCD_DrawPoint(x0 + b, y0 + a);        //4
        LCD_DrawPoint(x0 + a, y0 + b);        //6
        LCD_DrawPoint(x0 - a, y0 + b);        //1
        LCD_DrawPoint(x0 - b, y0 + a);
        LCD_DrawPoint(x0 - a, y0 - b);        //2
        LCD_DrawPoint(x0 - b, y0 - a);        //7
        a++;

        //使用Bresenham算法画圆
        if (di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

/**
  * @brief      显示单个字符
  * @param      (x,y):开始坐标
  * @param      size : 字体大小
  * @param      ch : 字符
  * @retval     无
  */
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t size, uint8_t  ch)
{
    uint8_t i, j, cbyte, temp;
    uint16_t y0;

    cbyte = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
    ch -= ' ';
    y0 = y;
    for(i = 0; i < cbyte; i++)
    {
        if(size == 12)
            temp = ASCII_1206[ch][i];
        else if(size == 16)
            temp = ASCII_1608[ch][i];
        else if(size == 24)
            temp = ASCII_2412[ch][i];
        else if(size == 32)
            temp = ASCII_3216[ch][i];
        else
        {
            printf("Don't found char: %c\r\n", ch + ' ');
            return;
        }
        for(j = 0; j < 8; j++)
        {
            if(temp & 0x80)
                LCD_DrawPoint(x, y);
            temp <<= 1;
            y++;
            if(y >= lcd_pos.row)
            {
                printf("Error: over row!\r\n");
                return;
            }
            if(y - y0 == size)
            {
                y = y0;
                x++;
                if(x >= lcd_pos.col)
                {
                    printf("Error: over column!\r\n");
                    return;
                }
            }
        }
    }
}

/**
  * @brief      显示字符串
  * @param      (x,y):开始坐标
  * @param      size : 字体大小
  * @param      str : 字符串
  * @retval     无
  */
void LCD_ShowString(uint16_t x, uint16_t y, uint8_t size, uint8_t * str)
{
    while(*str != '\0')
    {
        LCD_ShowChar(x, y, size, *str);
        x += size / 2;
        str++;
    }
}


/**
  * @brief      向 LCD 写入命令
  * @param      usCmd   : 要写入的命令（外部寄存器地址）
  * @retval     无
  */
void LCD_WriteCmd(uint16_t usCmd)
{
    LCD->LCD_CMD = usCmd;
}

/**
  * @brief      向 LCD 写入数据
  * @param      usData  : 要写入的数据
  * @retval     无
  */
void LCD_WriteData(uint16_t usData)
{
    LCD->LCD_DATA = usData;
}

/**
  * @brief      读 LCD 的数据
  * @param      无
  * @retval     读取的数据
  */
uint16_t LCD_ReadData(void)
{
    return LCD->LCD_DATA;
}

/**
  * @brief      向指定 LCD 寄存器写入数据
  * @param      regval  : 要写的寄存器
  * @param      data    : 要写入的数据
  * @retval     无
  */
void LCD_Write_REG(uint16_t regval, uint16_t data)
{
    LCD_WriteCmd(regval);
    LCD_WriteData(data);
}

/**
  * @brief      读指定 LCD 寄存器的数据
  * @param      regval  : 要读取的寄存器
  * @retval     读取的数据
  */
uint16_t LCD_Read_REG(uint16_t regval)
{
    LCD_WriteCmd(regval);
    DEBUG_DELAY();
    return LCD_ReadData();
}

/**
  * @brief      告诉 LCD 开始写入数据
  * @param      无
  * @retval     无
  */
void LCD_WriteRAM(void)
{
    LCD -> LCD_CMD = lcd_pos.wscmd;
}

/**
  * @brief      设置光标位置
  * @param      Xpos : 横坐标
  * @param      Ypos : 纵坐标
  * @retval     无
  */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    if(lcd_pos.id == 0x5310)
    {
        LCD_WriteCmd(lcd_pos.colcmd);
        LCD_WriteData(Xpos >> 8);
        LCD_WriteData(Xpos & 0xFF);
        LCD_WriteCmd(lcd_pos.rowcmd);
        LCD_WriteData(Ypos >> 8);
        LCD_WriteData(Ypos & 0xFF);
    }
}

/**
  * @brief      设置 LCD 扫描方向
  * @param      ScanDir : 选择扫描方向
  * @retval     无
  */
void LCD_Scan_Direction(uint8_t ScanDir)
{
    uint16_t ScanDirReg = 0;
    uint16_t data = 0;

    switch(ScanDir)
    {
        case L2R_U2D://从左到右,从上到下
            data |= (0 << 7) | (0 << 6) | (0 << 5);
            break;

        case L2R_D2U://从左到右,从下到上
            data |= (1 << 7) | (0 << 6) | (0 << 5);
            break;

        case R2L_U2D://从右到左,从上到下
            data |= (0 << 7) | (1 << 6) | (0 << 5);
            break;

        case R2L_D2U://从右到左,从下到上
            data |= (1 << 7) | (1 << 6) | (0 << 5);
            break;

        case U2D_L2R://从上到下,从左到右
            data |= (0 << 7) | (0 << 6) | (1 << 5);
            break;

        case U2D_R2L://从上到下,从右到左
            data |= (0 << 7) | (1 << 6) | (1 << 5);
            break;

        case D2U_L2R://从下到上,从左到右
            data |= (1 << 7) | (0 << 6) | (1 << 5);
            break;

        case D2U_R2L://从下到上,从右到左
            data |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
    }
    if(lcd_pos.id == 0x5310)
    {
        ScanDirReg = NT35310ScanDirectionCmd;
    }
    LCD_Write_REG(ScanDirReg, data);
    //设置LCD访问区域
    LCD_WriteCmd(lcd_pos.colcmd);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData((lcd_pos.col - 1) >> 8);
    LCD_WriteData((lcd_pos.col - 1) & 0xFF);

    LCD_WriteCmd(lcd_pos.rowcmd);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData((lcd_pos.row - 1) >> 8);
    LCD_WriteData((lcd_pos.row - 1) & 0xFF);
}

/**
  * @brief      设置 LCD 显示方式
  * @param      dir : 0 竖屏
  *                   1 横屏
  * @retval     无返回值
  */
void LCD_Display_Direction(uint8_t dir)
{
    lcd_pos.dir = dir;
    if(dir == 0)
    {
        lcd_pos.col = LCD_WIDTH;
        lcd_pos.row = LCD_HEIGHT;
    }
    else
    {
        lcd_pos.col = LCD_HEIGHT;
        lcd_pos.row = LCD_WIDTH;
    }
    if(lcd_pos.id == 0x5310)
    {
        lcd_pos.wscmd = NT35310WriteStartCmd;
        lcd_pos.colcmd = NT35310ColumnCmd;
        lcd_pos.rowcmd = NT35310RowCmd;
    }
    LCD_Scan_Direction(DFT_SCAN_DIR);
}

