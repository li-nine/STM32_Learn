/**
  ******************************************************************************
  * @file       nt35310.c
  * @brief      nt35310 驱动
  * @author     li nine
  * @version    v1.0
  * @date       2022.05.13
  ******************************************************************************
  */

#include "nt35310.h"

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
  * @brief      获取 NT35310 2字节芯片ID
  * @note       命令为 D4h, 返回 4 个16bit数据
  *             D401h 第一个: 厂家ID
  *             D402h 第二个: 芯片ID第一个字节
  *             D403h 第三个: 芯片ID第二个字节
  *             D404h 第四位: 芯片版本
  * @param      无
  * @retval     芯片ID
  */
uint16_t NT35310_ReadID(void)
{
    uint16_t id;

    NT35310_WriteCmd(0xD4);
    NT35310_DELAY();
    id = NT35310_ReadData();
    NT35310_DELAY();
    id = NT35310_ReadData();
    NT35310_DELAY() ;
    id = NT35310_ReadData();
    id <<= 8;
    NT35310_DELAY() ;
    id |= NT35310_ReadData();

    return id;
}

/**
  * @brief      NT35310 初始化
  * @param      无
  * @retval     无
  */
void NT35310_Init(void)
{
    //Unlock CMD2
    NT35310_WriteCmd(0xED);
    NT35310_WriteData(0x01);
    NT35310_WriteData(0xFE);

    //Into CMD3
    NT35310_WriteCmd(0xEE);
    NT35310_WriteData(0xDE);
    NT35310_WriteData(0x21);

    //MTP
    NT35310_WriteCmd(0xF1);
    NT35310_WriteData(0x01);
    NT35310_WriteCmd(0xDF);
    NT35310_WriteData(0x10);

    //VCOM voltage -1.7875V
    NT35310_WriteCmd(0xC4);
    NT35310_WriteData(0x8F);

    //Clamp voltage
    NT35310_WriteCmd(0xC6);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xE2);
    NT35310_WriteData(0xE2);
    NT35310_WriteData(0xE2);

    //Go to  CMD2_P1
    NT35310_WriteCmd(0xBF);
    NT35310_WriteData(0xAA);

    //Gate and Source scan direction
    NT35310_WriteCmd(0xB0);
    NT35310_WriteData(0x0D);

    //Front & Back Porch Setting
    NT35310_WriteCmd(0xB1);
    NT35310_WriteData(0x80);
    NT35310_WriteData(0x8B);
    NT35310_WriteData(0x96);

    //Frame rate
    NT35310_WriteCmd(0xB2);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x02);
    NT35310_WriteData(0x03);

    //SPI & RGB interface setting
    NT35310_WriteCmd(0xB3);
    NT35310_WriteData(0x00);

    //Display inversion mode set
    NT35310_WriteCmd(0xB4);
    NT35310_WriteData(0x8B);

    //Partial and Idle Mode Timing
    NT35310_WriteCmd(0xB5);
    NT35310_WriteData(0x02);
    NT35310_WriteData(0x03);

    //Sets the equalizing
    NT35310_WriteCmd(0xB6);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x00);

    //Set the States for LED
    NT35310_WriteCmd(0xB7);
    NT35310_WriteData(0x00);

    //MTP selection
    NT35310_WriteCmd(0xB8);
    NT35310_WriteData(0x00);

    //AVDD setting
    NT35310_WriteCmd(0xC1);
    NT35310_WriteData(0x20);
    NT35310_WriteData(0x54);

    //charge pump circuit selection
    NT35310_WriteCmd(0xC2);
    NT35310_WriteData(0x0A);
    NT35310_WriteData(0x04);

    //Set the operating frequency of the step-up circuit 5
    NT35310_WriteCmd(0xC3);
    NT35310_WriteData(0x3C);

    //VCOM voltage
    NT35310_WriteCmd(0xC4);
    NT35310_WriteData(0x62);

    //Source OP output stage current
    NT35310_WriteCmd(0xC5);
    NT35310_WriteData(0x32);
    NT35310_WriteData(0x44);
    NT35310_WriteData(0x65);
    NT35310_WriteData(0x76);

    //clamp voltage selection
    NT35310_WriteCmd(0xC6);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x20);
    NT35310_WriteData(0x17);
    NT35310_WriteData(0x01);

    //RED_P gamma correction
    NT35310_WriteCmd(0xE0);
    NT35310_WriteData(0x16);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x1C);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x21);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x36);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x46);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x52);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x64);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x7A);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x8B);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x99);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xA8);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xB9);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xC4);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xCA);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xD2);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xD9);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xE0);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xF3);
    NT35310_WriteData(0x00);

    //RED_N gamma correction
    NT35310_WriteCmd(0xE1);
    NT35310_WriteData(0x16);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x1C);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x22);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x36);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x45);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x52);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x64);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x7A);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x8B);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x99);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xA8);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xB9);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xC4);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xCA);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xD2);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xD8);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xE0);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xF3);
    NT35310_WriteData(0x00);

    //GREEN_P gamma correction
    NT35310_WriteCmd(0xE2);
    NT35310_WriteData(0x05);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x0B);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x1B);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x34);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x44);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x4F);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x61);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x79);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x88);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x97);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xA6);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xB7);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xC2);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xC7);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xD1);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xD6);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xDD);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xF3);
    NT35310_WriteData(0x00);

    //GREEN_N gamma correction
    NT35310_WriteCmd(0xE3);
    NT35310_WriteData(0x05);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x0A);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x1C);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x33);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x44);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x50);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x62);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x78);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x88);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x97);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xA6);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xB7);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xC2);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xC7);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xD1);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xD5);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xDD);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xF3);
    NT35310_WriteData(0x00);

    //BLUE_P gamma correction
    NT35310_WriteCmd(0xE4);
    NT35310_WriteData(0x01);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x01);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x02);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x2A);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x3C);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x4B);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x5D);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x74);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x84);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x93);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xA2);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xB3);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xBE);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xC4);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xCD);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xD3);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xDD);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xF3);
    NT35310_WriteData(0x00);

    //BLUE_N gamma correction
    NT35310_WriteCmd(0xE5);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x02);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x29);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x3C);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x4B);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x5D);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x74);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x84);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x93);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xA2);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xB3);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xBE);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xC4);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xCD);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xD3);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xDC);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xF3);
    NT35310_WriteData(0x00);

    //GAMMA offset
    NT35310_WriteCmd(0xE6);
    NT35310_WriteData(0x11);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x34);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x56);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x76);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x77);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x66);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x88);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x99);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xBB);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x99);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x66);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x55);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x55);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x45);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x43);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x44);
    NT35310_WriteData(0x00);

    //GAMMA offset
    NT35310_WriteCmd(0xE7);
    NT35310_WriteData(0x32);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x55);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x76);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x66);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x67);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x67);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x87);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x99);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xBB);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x99);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x77);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x44);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x56);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x23);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x33);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x45);
    NT35310_WriteData(0x00);

    //GAMMA offset
    NT35310_WriteCmd(0xE8);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x99);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x87);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x88);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x77);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x66);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x88);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xAA);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0xBB);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x99);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x66);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x55);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x55);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x44);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x44);
    NT35310_WriteData(0x00);
    NT35310_WriteData(0x55);
    NT35310_WriteData(0x00);

    //Goto CMD2_P0
    NT35310_WriteCmd(0x00);
    NT35310_WriteData(0xAA);

    //Set the Interface Pixel Format
    NT35310_WriteCmd(0x3A);
    NT35310_WriteData(0x55);

    //Exit the Sleep-In Mode
    NT35310_WriteCmd(0x11);
    HAL_Delay(100);

    //Display On
    NT35310_WriteCmd(0x29);

    //Tearing Effect Line ON
    NT35310_WriteCmd(0x35);
    NT35310_WriteData(0x00);

    //Write Display Brightness
    NT35310_WriteCmd(0x51);
    NT35310_WriteData(0xFF);

    //control the "LEDPWM" pin, dimming function for CABC
    NT35310_WriteCmd(0x53);
    NT35310_WriteData(0x2C);

    //set parameters for image content based adaptive brightness control functionality
    NT35310_WriteCmd(0x55);
    NT35310_WriteData(0x82);

    //Memory Write Start Command
    NT35310_WriteCmd(0x2C);
}

