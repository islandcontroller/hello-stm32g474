/*!****************************************************************************
 * @file
 * hw_gpio.c
 *
 * @brief
 * Hardware Layer - GPIO configuration
 *
 * @date  16.08.2026
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "stm32g4xx.h"
#include "hw_gpio.h"
#include "hw_iodef.h"


/*- Public interface ---------------------------------------------------------*/
/*!****************************************************************************
 * @brief
 * Configure GPIOs
 * 
 * @date  16.08.2026
 ******************************************************************************/
void vHW_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // PA5 - LED output
  static const GPIO_InitTypeDef sLed = {
    .Pin = LED_PIN,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Speed = GPIO_SPEED_FREQ_LOW
  };
  (void)HAL_GPIO_Init(LED_PORT, &sLed);

  // PA3,4 - UART to VCP
  static const GPIO_InitTypeDef sVcp = {
    .Pin = VCP_RX_PIN | VCP_TX_PIN,
    .Alternate = VCP_AF,
    .Mode = GPIO_MODE_AF_PP,
    .Speed = GPIO_SPEED_FREQ_HIGH
  };
  (void)HAL_GPIO_Init(VCP_PORT, &sVcp);
}

/*!****************************************************************************
 * @brief 
 * Toggle LED pin
 * 
 * @date  16.11.2026
 ******************************************************************************/
void vHW_GPIO_ToggleLED(void)
{
  (void)HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}
