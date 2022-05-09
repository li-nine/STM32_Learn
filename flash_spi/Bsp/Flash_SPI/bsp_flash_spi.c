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
  * @brief      向 W25xx 写入数据
  * @note       确定写数据的位置已被擦除
  * @param      WriteAddr : 要写入的位置
  * @param      WriteData : 指向写入数据的地址
  * @retval     状态
  */

/**
  * @brief      从 W25xx 中读取数据
  * @param      ReadAddr    : 所读数据的位置
  * @param      ReadData    : 指向读出数据的地址
  * @param      NumOfData   : 读出的数据的长度
  * @retval     状态
  */
HAL_StatusTypeDef W25xx_Read(uint32_t ReadAddr, uint8_t * ReadData, uint32_t NumOfData)
{
    uint8_t CMD[4];
    HAL_StatusTypeDef state = HAL_OK;

    CMD[0] = W25X_ReadData;
    CMD[1] = (uint8_t)((ReadAddr & 0xFF0000U) >> 16);
    CMD[2] = (uint8_t)((ReadAddr & 0xFF00U) >> 8);
    CMD[3] = (uint8_t)(ReadAddr & 0xFFU);
    W25xx_WaitUntilBusyReset();
    W25X_CS_L;
    state |= SPI_Transmit(CMD, 4);
    state |= SPI_Receive(ReadData, NumOfData);
    W25X_CS_H;

    return state;
}
/**
  * @brief      读取 W25xx 的ID
  * @param      无参数
  * @retval     状态
  */
HAL_StatusTypeDef W25xx_GetID(void)
{
    uint8_t CMD[1] = {W25X_JedecDeviceID};
    HAL_StatusTypeDef state = HAL_OK;
    uint8_t ID[3];

    W25X_CS_L;
    state |= SPI_Transmit(CMD, 1);
    state |= SPI_Receive(ID, 3);
    W25X_CS_H;
    printf("0x%2X %2X%2X\r\n", ID[0], ID[1], ID[2]);

    return state;
}

/**
  * @brief      W25xx 写使能
  * @param      无参数
  * @retval     状态
  */
HAL_StatusTypeDef W25xx_WriteEnable(void)
{
    uint8_t CMD[1] = {W25X_WriteEnable};
    HAL_StatusTypeDef state = HAL_OK;

    W25X_CS_L;
    state |= SPI_Transmit(CMD, 1);
    W25X_CS_H;

    return state;
}

/**
  * @brief      等待 W25xx 擦写结束
  * @param      无参数
  * @retval     状态
  */
W25xx_StatusTypeDef W25xx_WaitUntilBusyReset(void)
{
    uint8_t CMD[1] = {W25X_ReadStatusReg};
    W25xx_StatusTypeDef state;
    W25X_CS_L;
    SPI_Transmit(CMD, 1);
    do{
        SPI_Receive((uint8_t *)&state, 1);
    }while((state & W25Qxx_BUSY) == W25Qxx_BUSY);
    W25X_CS_H;

    return state;
}

/**
  * @brief      扇区擦除
  * @note       调用扇区擦除指令时注意输入的地址要对齐到4KB(低12位为0)
  * @param      Sectoraddr : 擦除的扇区的地址,范围 0x000000U ~ 0x7FF000U
  * @retval     状态
  */
HAL_StatusTypeDef W25xx_SectorErase(uint32_t Sectoraddr)
{
    uint8_t CMD[4];
    HAL_StatusTypeDef state = HAL_OK;

    CMD[0] = W25X_SectorErase;
    CMD[1] = (uint8_t)((Sectoraddr & 0x00FF0000U) >> 16);
    CMD[2] = (uint8_t)((Sectoraddr & 0x0000FF00U) >> 8);
    CMD[3] = (uint8_t)(Sectoraddr & 0x000000FFU);
    state |= W25xx_WriteEnable();
    W25xx_WaitUntilBusyReset();
    W25X_CS_L;
    state |= SPI_Transmit(CMD, 4);
    W25X_CS_H;
    W25xx_WaitUntilBusyReset();

    return state;
}

/**
  * @brief      向 W25xx 页写入数据
  * @note       调用此函数前要先对目标位置进行擦除
  * @param      WriteAddr   : 写入的页地址
  * @param      WriteData   : 指向写入的数据
  * @param      NumOfData   : 写入数据的长度
  * @retval     状态
  */
HAL_StatusTypeDef W25xx_WritePage(uint32_t WriteAddr, uint8_t * WriteData, uint16_t NumOfData)
{
    uint8_t CMD[4];
    HAL_StatusTypeDef state = HAL_OK;

    CMD[0] = W25X_PageProgram;
    CMD[1] = (uint8_t)((WriteAddr & 0x00FF0000U) >> 16);
    CMD[2] = (uint8_t)((WriteAddr & 0x0000FF00U) >> 8);
    CMD[3] = (uint8_t)(WriteAddr & 0x000000FFU);
    state |= W25xx_WriteEnable();
    W25X_CS_L;
    state |= SPI_Transmit(CMD, 4);
    if(NumOfData <= W25X_WritePageSize)
    {
        state |= SPI_Transmit(WriteData, NumOfData);
    }
    else
        state |= HAL_ERROR;
    W25X_CS_H;
    W25xx_WaitUntilBusyReset();

    return state;
}

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
