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


#define W25Qxx_JedecDeviceID    0x9F


#define W25Q128_PageSize        256
#define W25Q128_WritePageSize   256
#define W25Q128_CS_Port SPI1_CS_GPIO_Port
#define W25Q128_CS_Pin  SPI1_CS_Pin
#define W25Q128_CS_L   HAL_GPIO_WritePin(W25Q128_CS_Port, W25Q128_CS_Pin, GPIO_PIN_RESET);
#define W25Q128_CS_H  HAL_GPIO_WritePin(W25Q128_CS_Port, W25Q128_CS_Pin, GPIO_PIN_SET);



HAL_StatusTypeDef W25Qxx_GetID(void);
HAL_StatusTypeDef SPI_Transmit(uint8_t * pData, uint16_t Size);
HAL_StatusTypeDef SPI_Receive(uint8_t * pData, uint16_t Size);
HAL_StatusTypeDef SPI_TransmitReceive(uint8_t * pTxData, uint8_t * pRxData, uint16_t Size);


#endif//__BSP_FLASH_SPI_H__
