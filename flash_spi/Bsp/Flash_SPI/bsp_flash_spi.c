/**
  ******************************************************************************
  * @file       bsp_flash_spi.c
  * @brief      使用 SPI 读写 Flash 的驱动
  * @author     li nine
  * @version    v1.0
  * @date       2022.05.08
  ******************************************************************************
  */

#include "bsp_flash_spi.h"

extern SPI_HandleTypeDef hspi1;

/**
  * @brief      向 FLASH 写入数据
  * @note       确定写数据的位置已被擦除
  * @param      WriteAddr : 要写入的位置
  * @param      WriteData : 指向写入数据的地址
  * @retval     状态
  */

/**
  * @brief      从 W25Qxx 中读取数据
  * @param      ReadAddr    : 所读数据的位置
  * @param      ReadData    : 指向读入数据的地址
  * @retval     状态
  */

/**
  * @brief      读取 W25Qxx 的ID
  * @param      无参数
  * @retval     状态
  */
HAL_StatusTypeDef W25Qxx_GetID(void)
{
    HAL_StatusTypeDef state = HAL_OK;
    uint8_t ID[3];
    uint8_t CMD[1] = {W25Qxx_JedecDeviceID};

    W25Q128_CS_ON;
    state |= SPI_Transmit(CMD, 1);
    state |= SPI_Receive(ID, 0x0003U);
    W25Q128_CS_OFF;
    printf("0x%2X", ID[0]);
    printf("%2X", ID[1]);
    printf("%2X\r\n", ID[2]);

    return state;
}

/**
  * @brief      W25Qxx 写使能
  * @param      无参数
  * @retval     状态
  */


/**
  * @brief      等待 W25Qxx 擦写结束
  * @param      无参数
  * @retval     无返回值
  */


/**
  * @brief      扇区擦除
  * @note       调用扇区擦除指令时注意输入的地址要对齐到4KB
  * @param      Sectoraddr : 擦除的扇区的地址
  * @retval     状态
  */


/**
  * @brief      函数作用
  * @note       备注
  * @param      参数
  * @retval     返回值
  */

/**
  * @brief      SPI 发送特定长度的数据
  * @param      pData   : 指向发送数据的地址
  * @param      Size    : 发送数据的长度
  * @retval     状态
  */
HAL_StatusTypeDef SPI_Transmit(uint8_t * pData, uint16_t Size)
{
    return HAL_SPI_Transmit(&hspi1, pData, Size, 1000);
}

/**
  * @brief      SPI 接收特定长度的数据
  * @param      pData   : 指向接收数据的地址
  * @param      Size    : 发送数据的长度
  * @retval     状态
  */
HAL_StatusTypeDef SPI_Receive(uint8_t * pData, uint16_t Size)
{
    return HAL_SPI_Receive(&hspi1, pData, Size, 1000);
}

/**
  * @brief      SPI 发送并接收特定长度的数据
  * @param      pTxData : 指向发送数据的地址
  * @param      pRxData : 指向接收数据的地址
  * @param      Size    : 发送和接收数据的长度
  * @retval     状态
  */
HAL_StatusTypeDef SPI_TransmitReceive(uint8_t * pTxData, uint8_t * pRxData, uint16_t Size)
{
    return HAL_SPI_TransmitReceive(&hspi1, pTxData, pRxData, Size, 1000);
}
