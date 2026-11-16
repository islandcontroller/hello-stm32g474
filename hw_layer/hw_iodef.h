/*!****************************************************************************
 * @file
 * hw_iodef.h
 *
 * @brief
 * Hardware Layer - I/O definitions
 *
 * @date  16.08.2026
 ******************************************************************************/

#ifndef HW_IODEF_H_
#define HW_IODEF_H_

/*- Macros -------------------------------------------------------------------*/
/*! @brief LED
 *  @{                                                                        */
#define LED_PIN                       GPIO_PIN_5
#define LED_PORT                      GPIOA
/*! @}                                                                        */

/*! @brief VCP
 *  @{                                                                        */
#define VCP_RX_PIN                    GPIO_PIN_2
#define VCP_TX_PIN                    GPIO_PIN_3
#define VCP_PORT                      GPIOA
#define VCP_AF                        GPIO_AF12_LPUART1
#define VCP_UART                      LPUART1
/*! @}                                                                        */

/*! @brief User Button
 *  @{                                                                        */
#define BTN_PIN                       GPIO_PIN_13
#define BTN_PORT                      GPIOC
/*! @}                                                                        */

#endif // HW_IODEF_H_
