/**
  ******************************************************************************
  * @file       bsp_at24c02_i2c.c
  * @brief      程序文件的功能
  * @author     li nine
  * @version    V1.0
  * @date       2022.05.06
  ******************************************************************************
  */

#include "bsp_at24c02_i2c.h"

extern I2C_HandleTypeDef hi2c1;

/**
  * @brief      向AT24C02写入数据
  * @note       AT24C02内部只有 2K bit, 只能写入256个字节的数据
  * @param      MemAddress : 选择写入AT24C02中的位置, 取值0 ~ 255
  * @param      pData : 指向写入数据的指针
  * @retval     AT24C02_OK : 表示写入正常
  *             AT24C02_ERROR : 表示写入失败
  */
AT24C02_StateTypedef AT24C02_Write(uint16_t MemAddress, uint8_t * pData)
{
    HAL_StatusTypeDef i2c_state = HAL_OK;
    AT24C02_StateTypedef at24c02_state = AT24C02_OK;
    uint16_t i, data_len;

    data_len = strlen((const char *)pData);
    if(data_len <= 8)
    {
        i2c_state |= HAL_I2C_Mem_Write(&hi2c1, AT24C02_WRITE_ADDRESS, MemAddress, AT24C02_MEMORY_ADDRESS_SIZE, pData, data_len, 0xFFFF);
    }
    if(data_len > 8 && data_len <= 256) {
        for (i = 0; i < data_len / 8; i++)
            i2c_state |= HAL_I2C_Mem_Write(&hi2c1, AT24C02_WRITE_ADDRESS, MemAddress + i * 8,
                                       AT24C02_MEMORY_ADDRESS_SIZE + i * 8, pData + i * 8, AT24C02_MEMORY_PAGE_SIZE,
                                       0xFFFF);
        if (i == data_len / 8)
            i2c_state |= HAL_I2C_Mem_Write(&hi2c1, AT24C02_WRITE_ADDRESS, MemAddress + i * 8,
                                       AT24C02_MEMORY_ADDRESS_SIZE + i * 8, pData + i * 8, data_len % 8, 0xFFFF);
    }
    else
        i2c_state |= HAL_ERROR;
    if(i2c_state)
        at24c02_state = AT24C02_ERROR;

    return at24c02_state;
}

/**
  * @brief      从AT24C02中读数据
  * @param      MemAddress : 选择读取AT24C02中的位置, 取值0 ~ 255
  * @param      pData : 指向读取的数据的指针
  * @param      Size : 读取的数据的字节长的
  * @retval     AT24C02_OK : 表示读取正常
  *             AT24C02_ERROR : 表示读取失败
  */
AT24C02_StateTypedef AT24C02_Read(uint16_t MemAddress, uint8_t * pData, uint16_t Size)
{
    HAL_StatusTypeDef i2c_state = HAL_OK;
    AT24C02_StateTypedef at24c02_state = AT24C02_OK;

    i2c_state |= HAL_I2C_Mem_Read(&hi2c1, AT24C02_READ_ADDRESS, MemAddress, AT24C02_MEMORY_ADDRESS_SIZE, pData, Size, 0xFFFF);
    printf("%s\r\n", pData);
    if(i2c_state)
        at24c02_state = AT24C02_ERROR;

    return at24c02_state;
}
