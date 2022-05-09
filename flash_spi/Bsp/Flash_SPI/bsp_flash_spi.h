/**
  ******************************************************************************
  * @file       bsp_flash_spi.h
  * @brief      使用 SPI 读写 Flash 的驱动
  * @author     li nine
  * @version    v1.0
  * @date       2022.05.08
  ******************************************************************************
  */

#ifndef __BSP_FLASH_SPI_H__
#define __BSP_FLASH_SPI_H__

#include "main.h"
#include "stm32f4xx_hal_spi.h"
#include "spi.h"

typedef enum{
    W25X_OK       = 0x00U,
    W25X_ERROR    = 0X01U
}W25X_StatusTypeDef;

/* W25xx 指令 */
#define W25X_WriteEnable            0x06
#define W25X_WriteDisable           0x04
#define W25X_ReadStatusReg          0x05
#define W25X_WriteStatusReg         0x01
#define W25X_ReadData               0x03
#define W25X_FastReadData           0x0B
#define W25X_FastReadDual           0x3B
#define W25X_PageProgram            0x02
#define W25X_BlockErase             0xD8
#define W25X_SectorErase            0x20
#define W25X_ChipErase              0xC7
#define W25X_PowerDown              0xB9
#define W25X_ReleasePowerDown       0xAB
#define W25X_DeviceID               0xAB
#define W25X_ManufactDeviceID       0x90
#define W25X_JedecDeviceID          0x9F
#define W25X_Enter4ByteMode         0xB7
#define W25X_ReadStatusRegister3    0x15

#define WIP_Flag                    0x01

#define W25X_PageSize        256
#define W25X_WritePageSize   256
#define W25X_CS_Port SPI1_CS_GPIO_Port
#define W25X_CS_Pin  SPI1_CS_Pin
#define W25X_CS_L    HAL_GPIO_WritePin(W25X_CS_Port, W25X_CS_Pin, GPIO_PIN_RESET)
#define W25X_CS_H    HAL_GPIO_WritePin(W25X_CS_Port, W25X_CS_Pin, GPIO_PIN_SET)



uint32_t W25xx_GetID(void);
static uint8_t W25xx_GetSR(void);
static void W25xx_WaitWriteBusy(void);
HAL_StatusTypeDef W25xx_CMD(uint8_t CMD);
W25X_StatusTypeDef W25xx_WriteEnable(void);
W25X_StatusTypeDef W25xx_SectorErase(uint32_t Sectoraddr);
HAL_StatusTypeDef SPI_Transmit(uint8_t * pData, uint16_t Size);
HAL_StatusTypeDef SPI_Receive(uint8_t * pData, uint16_t Size);
W25X_StatusTypeDef W25xx_Write(uint32_t WriteAddr, uint8_t * WriteData);
W25X_StatusTypeDef W25xx_Read(uint32_t ReadAddr, uint8_t * ReadData, uint32_t NumOfData);
HAL_StatusTypeDef SPI_TransmitReceive(uint8_t * pTxData, uint8_t * pRxData, uint16_t Size);
W25X_StatusTypeDef W25xx_WritePage(uint32_t WriteAddr, uint8_t * WriteData, uint16_t NumOfData);


#endif//__BSP_FLASH_SPI_H__
