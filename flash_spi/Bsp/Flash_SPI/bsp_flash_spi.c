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
W25X_StatusTypeDef W25xx_Write(uint32_t WriteAddr, uint8_t * WriteData, uint32_t NumOfData)
{
    uint16_t i, NumOfPage, CountOfSingle, NumOfAddr, CountOfAddr;

    //发送的数据的页数
    NumOfPage = NumOfData / W25X_WritePageSize;
    //剩下单页的数据长度
    CountOfSingle = NumOfData % W25X_WritePageSize;
    //第一页起始地址
    CountOfAddr = WriteAddr % W25X_PageSize;
    //第一页可以写入的字节数
    NumOfAddr = W25X_PageSize - CountOfAddr;
    if(CountOfAddr != 0)//第一页起始地址不是0
        if(NumOfPage != 0)
            if(CountOfSingle > NumOfAddr)
            {
                //先写第一页剩下的空间
                W25xx_WritePage(WriteAddr, WriteData, NumOfAddr);
                WriteAddr += NumOfAddr;
                WriteData += NumOfAddr;
                for(i = 0; i < NumOfPage; i++)
                {
                    W25xx_WritePage(WriteAddr, WriteData, W25X_WritePageSize);
                    WriteAddr += W25X_WritePageSize;
                    WriteData += W25X_WritePageSize;
                }
                W25xx_WritePage(WriteAddr, WriteData, CountOfSingle - NumOfAddr);
                return W25X_OK;
            }
            else
            {
                //先写第一页剩下的空间
                W25xx_WritePage(WriteAddr, WriteData, NumOfAddr);
                WriteAddr += NumOfAddr;
                WriteData += NumOfAddr;
                for(i = 0; i < NumOfPage - 1; i++)
                {
                    W25xx_WritePage(WriteAddr, WriteData, W25X_WritePageSize);
                    WriteAddr += W25X_WritePageSize;
                    WriteData += W25X_WritePageSize;
                }
                W25xx_WritePage(WriteAddr, WriteData, W25X_WritePageSize + CountOfSingle - NumOfAddr);
                return W25X_OK;
            }
        else
            if(CountOfSingle > NumOfAddr)
            {
                //先写第一页剩下的空间
                W25xx_WritePage(WriteAddr, WriteData, NumOfAddr);
                WriteAddr += NumOfAddr;
                WriteData += NumOfAddr;
                W25xx_WritePage(WriteAddr, WriteData, CountOfSingle - NumOfAddr);
                return W25X_OK;
            }
            else
            {
                //先写第一页剩下的空间
                W25xx_WritePage(WriteAddr, WriteData, CountOfSingle);
                return W25X_OK;
            }
    else
        if(NumOfPage != 0)
            if(CountOfSingle != 0)
            {
                for(i = 0; i < NumOfPage; i++)
                {
                    W25xx_WritePage(WriteAddr, WriteData, W25X_WritePageSize);
                    WriteAddr += W25X_WritePageSize;
                    WriteData += W25X_WritePageSize;
                }
                W25xx_WritePage(WriteAddr, WriteData, CountOfSingle);
                return W25X_OK;
            }
            else
            {
                for(i = 0; i < NumOfPage; i++)
                {
                    W25xx_WritePage(WriteAddr, WriteData, W25X_WritePageSize);
                    WriteAddr += W25X_WritePageSize;
                    WriteData += W25X_WritePageSize;
                }
                return W25X_OK;
            }
        else
            if(CountOfSingle != 0)
            {
                W25xx_WritePage(WriteAddr, WriteData, CountOfSingle);
                return W25X_OK;
            }
            else
            {
                return W25X_ERROR;
            }
}
/**
  * @brief      从 W25xx 中读取数据
  * @param      ReadAddr    : 所读数据的位置
  * @param      ReadData    : 指向读出数据的地址
  * @param      NumOfData   : 读出的数据的长度
  * @retval     状态
  */
W25X_StatusTypeDef W25xx_Read(uint32_t ReadAddr, uint8_t * ReadData, uint32_t NumOfData)
{
    uint8_t addr[3];

    addr[0] = (uint8_t)((ReadAddr & 0xFF0000) >> 16);
    addr[1] = (uint8_t)((ReadAddr & 0xFF00) >> 8);
    addr[2] = (uint8_t)(ReadAddr & 0xFF);
    W25xx_WaitWriteBusy();

    W25X_CS_L;

    if((W25xx_CMD(W25X_ReadData) == HAL_OK) && (SPI_Transmit(addr, 3) == HAL_OK))
        if(SPI_Receive(ReadData, NumOfData) == HAL_OK)
        {
            W25X_CS_H;
            return W25X_OK;
        }
    W25X_CS_H;

    return W25X_ERROR;
}

/**
  * @brief      读取 W25xx 的ID
  * @param      无参数
  * @retval     返回值 0x1000000 获取 ID 错误
  */
uint32_t W25xx_GetID(void)
{

    uint32_t ID = 0;

    W25X_CS_L;
    if(W25xx_CMD(W25X_JedecDeviceID) == HAL_OK)
    {
        if(SPI_Receive((uint8_t *)&ID, 3) == HAL_OK)
        {
            W25X_CS_H;
            return ((ID & 0xFF) << 16) | (ID & 0xFF00) | ((ID & 0xFF0000) >> 16);
        }
    }
    W25X_CS_H;

    return 1 << 24;
}

/**
  * @brief      W25xx 写使能
  * @param      无参数
  * @retval     状态
  */
W25X_StatusTypeDef W25xx_WriteEnable(void)
{

    W25X_CS_L;
    if(W25xx_CMD(W25X_WriteEnable) == HAL_OK)
    {
        W25X_CS_H;
        return W25X_OK;
    }
    W25X_CS_H;

    return W25X_ERROR;
}

/**
  * @brief      获取 W25xx 的状态
  * @param      无参数
  * @retval     状态值
  */
static uint8_t W25xx_GetSR(void)
{
    uint8_t state;

    W25X_CS_L;

    if(W25xx_CMD(W25X_ReadStatusReg) == HAL_OK)
        if(SPI_Receive(&state, 1) == HAL_OK)
        {
            W25X_CS_H;
            return state;
        }

    W25X_CS_H;

    return 0;
}

/**
  * @brief      等待 W25xx 擦写结束
  * @param      无参数
  * @retval     状态
  */
static void W25xx_WaitWriteBusy(void)
{
    while((W25xx_GetSR() & WIP_Flag) == WIP_Flag);
}

/**
  * @brief      扇区擦除
  * @note       调用扇区擦除指令时注意输入的地址要对齐到4KB(低12位为0)
  * @param      Sectoraddr : 擦除的扇区序号,范围 0 ~ 0xFFF000
  * @retval     状态
  */
W25X_StatusTypeDef W25xx_SectorErase(uint32_t Sectoraddr)
{
    uint8_t addr[3];

    addr[0] = (uint8_t)((Sectoraddr & 0xFF0000) >> 16);
    addr[1] = (uint8_t)((Sectoraddr & 0xFF00) >> 8);
    addr[2] = (uint8_t)(Sectoraddr & 0xFF);
    if(W25xx_WriteEnable() != W25X_OK)
        return W25X_ERROR;
    W25xx_WaitWriteBusy();
    W25X_CS_L;
    if((W25xx_CMD(W25X_SectorErase) == HAL_OK) && (SPI_Transmit(addr, 3) == HAL_OK))
    {
        W25X_CS_H;
        W25xx_WaitWriteBusy();
        return W25X_OK;
    }
    W25X_CS_H;

    return W25X_ERROR;
}

/**
  * @brief      向 W25xx 页写入数据
  * @note       调用此函数前要先对目标位置进行擦除
  * @param      WriteAddr   : 写入的页地址
  * @param      WriteData   : 指向写入的数据
  * @param      NumOfData   : 写入数据的长度
  * @retval     状态
  */
W25X_StatusTypeDef W25xx_WritePage(uint32_t WriteAddr, uint8_t * WriteData, uint16_t NumOfData)
{
    uint8_t addr[3];

    addr[0] = (uint8_t)((WriteAddr & 0xFF0000U) >> 16);
    addr[1] = (uint8_t)((WriteAddr & 0xFF00U) >> 8);
    addr[2] = (uint8_t)(WriteAddr & 0xFFU);
    if(W25xx_WriteEnable() != W25X_OK)
        return W25X_ERROR;
    W25xx_WaitWriteBusy();
    W25X_CS_L;
    if((W25xx_CMD(W25X_PageProgram) == HAL_OK) && (SPI_Transmit(addr, 3) == HAL_OK))
    {
        if(SPI_Transmit(WriteData, NumOfData) == HAL_OK)
        {
            W25X_CS_H;
            W25xx_WaitWriteBusy();
            return W25X_OK;
        }
    }
    W25X_CS_H;

    return W25X_ERROR;
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

/**
  * @brief      发送指令
  * @param      CMD : W25xx指令
  * @retval     状态
  */
HAL_StatusTypeDef W25xx_CMD(uint8_t CMD)
{
    return SPI_Transmit(&CMD, 1);
}
