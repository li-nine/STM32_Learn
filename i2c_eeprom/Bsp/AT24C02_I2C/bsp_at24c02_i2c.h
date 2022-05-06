/**
  ******************************************************************************
  * @file       bsp_at24c02_i2c.h
  * @brief      程序文件的功能
  * @author     li nine
  * @version    V1.0
  * @date       2022.05.06
  ******************************************************************************
  */

#ifndef __BSP_AT24C02_I2C_H__
#define __BSP_AT24C02_I2C_H__

#include "main.h"
#include "stm32f4xx_hal_i2c.h"

#define BUFFER_SIZE                     256
#define AT24C02_WRITE_ADDRESS           0xA0
#define AT24C02_READ_ADDRESS            0xA1
#define AT24C02_MEMORY_PAGE_SIZE        8
#define AT24C02_MEMORY_ADDRESS_SIZE     8

typedef enum{
    AT24C02_OK      = 0,
    AT24C02_ERROR   = 1
}AT24C02_StateTypedef;

AT24C02_StateTypedef AT24C02_Write(uint16_t MemAddress, uint8_t * pData);
AT24C02_StateTypedef AT24C02_Read(uint16_t MemAddress, uint8_t * pData, uint16_t Size);

#endif//__BSP_AT24C02_I2C_H__
