/**
  ******************************************************************************
  * @file       systick.h
  * @brief      重新配置系统定时器
  * @author     li nine
  * @version    v1.0
  * @date       2022.05.03
  ******************************************************************************
  */

#include "systick.h"

/**
  * @brief      配置时钟重载值
  * @note       该函数要在主函数中调用一次
  * @param      无
  * @retval     无
  */
void SysTick_Init(void)
{
    /*
     * 1000U        1ms
     * 100000U      10us
     * 1000000U     1us
     */
    HAL_SYSTICK_Config(SystemCoreClock / 100000U);
}

