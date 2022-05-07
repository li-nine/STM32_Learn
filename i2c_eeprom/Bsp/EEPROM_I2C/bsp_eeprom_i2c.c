/**
  ******************************************************************************
  * @file       bsp_at24c02_i2c.c
  * @brief      利用 I2C 读写 EEPROM 的驱动
  * @author     li nine
  * @version    V1.0
  * @date       2022.05.06
  ******************************************************************************
  */

#include "bsp_eeprom_i2c.h"

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
    uint16_t i, data_num, page_num, single_cnt;

    data_num = strlen((const char *)pData);
    page_num = data_num / AT24C02_MEMORY_PAGE_SIZE;
    single_cnt = data_num % AT24C02_MEMORY_PAGE_SIZE;
    if(single_cnt)
    {
        if(page_num)
        {
            for (i = 0; i < page_num; i++)
            {
                i2c_state |= HAL_I2C_Mem_Write(&hi2c1, AT24C02_WRITE_ADDRESS,
                                               MemAddress + AT24C02_MEMORY_PAGE_SIZE * i, AT24C02_MEMORY_ADDRESS_SIZE,
                                               pData + AT24C02_MEMORY_PAGE_SIZE * i, AT24C02_MEMORY_PAGE_SIZE, 0xFFFF);
                //HAL_Delay(5);
                while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
            }
            i2c_state |= HAL_I2C_Mem_Write(&hi2c1, AT24C02_WRITE_ADDRESS,
                                           MemAddress + page_num * AT24C02_MEMORY_PAGE_SIZE, AT24C02_MEMORY_ADDRESS_SIZE,
                                           pData + page_num * AT24C02_MEMORY_PAGE_SIZE, single_cnt, 0xFFFF);
            //HAL_Delay(5);
            while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
        }
        else
        {
            i2c_state |= HAL_I2C_Mem_Write(&hi2c1, AT24C02_WRITE_ADDRESS,
                                           MemAddress, AT24C02_MEMORY_ADDRESS_SIZE,
                                           pData, single_cnt, 0xFFFF);
            //HAL_Delay(5);
            while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
        }
    }
    else
    {
        if(page_num)
        {
            for (i = 0; i < page_num; i++)
            {
                i2c_state |= HAL_I2C_Mem_Write(&hi2c1, AT24C02_WRITE_ADDRESS,
                                               MemAddress + AT24C02_MEMORY_PAGE_SIZE * i, AT24C02_MEMORY_ADDRESS_SIZE,
                                               pData + AT24C02_MEMORY_PAGE_SIZE * i, AT24C02_MEMORY_PAGE_SIZE, 0xFFFF);
                //HAL_Delay(5);
                while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
            }

        }
        else
        {
            i2c_state |= HAL_I2C_Mem_Write(&hi2c1, AT24C02_WRITE_ADDRESS,
                                           MemAddress, AT24C02_MEMORY_ADDRESS_SIZE,
                                           pData, AT24C02_MEMORY_PAGE_SIZE, 0xFFFF);
            //HAL_Delay(5);
            while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
        }
    }
    for(i = 0; i < 32; i++)
    {
        i2c_state |= HAL_I2C_Mem_Write(&hi2c1, AT24C02_WRITE_ADDRESS,
                          MemAddress + 8 * i, AT24C02_MEMORY_ADDRESS_SIZE,
                          pData + 8 * i, 8, 0xFFFF);
        HAL_Delay(5);
    }

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

    i2c_state |= HAL_I2C_Mem_Read(&hi2c1, AT24C02_READ_ADDRESS,
                                  MemAddress, AT24C02_MEMORY_ADDRESS_SIZE,
                                  pData, Size, 0XFFFF);
    printf("%s\r\n", pData);
    if(i2c_state)
        at24c02_state = AT24C02_ERROR;

    return at24c02_state;
}
