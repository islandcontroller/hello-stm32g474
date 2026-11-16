/*!****************************************************************************
 * @file
 * hw_clk.c
 *
 * @brief
 * Hardware Layer - Clocks configuration
 *
 * @date  16.08.2026
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "stm32g4xx.h"
#include "hw_clk.h"


/*- Public interface ---------------------------------------------------------*/
/*!****************************************************************************
 * @brief
 * Configure system clock tree
 * 
 * - 24 MHz HSE as system clock source
 * 
 * @date  16.08.2026
 ******************************************************************************/
void vHW_CLK_Init(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  HAL_RCC_OscConfig(&(const RCC_OscInitTypeDef){
    .OscillatorType = RCC_OSCILLATORTYPE_HSE,
    .HSEState = RCC_HSE_ON
  });
  HAL_RCC_ClockConfig(&(const RCC_ClkInitTypeDef){
    .ClockType = RCC_CLOCKTYPE_ALL,
    .SYSCLKSource = RCC_SYSCLKSOURCE_HSE,
    .AHBCLKDivider = RCC_SYSCLK_DIV1,
    .APB1CLKDivider = RCC_HCLK_DIV1,
    .APB2CLKDivider = RCC_HCLK_DIV1,
  }, FLASH_LATENCY_0);

  __HAL_RCC_LSI_DISABLE();
  __HAL_RCC_HSI_DISABLE();

  SystemCoreClockUpdate();
}
