/*!****************************************************************************
 * @file
 * hw_uart.c
 *
 * @brief
 * Hardware Layer - Serial I/O via UART
 *
 * @date  31.08.2026
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "stm32g4xx.h"
#include "hw_uart.h"


/*- Private data -------------------------------------------------------------*/
/// HAL instance handle
static UART_HandleTypeDef hUart;


/*- Public interface ---------------------------------------------------------*/
/*!****************************************************************************
 * @brief
 * Configure UART hardware peripheral
 * 
 * Configuration: 8N1 @ 115200 Baud
 * 
 * @date  31.10.2026
 ******************************************************************************/
void vHW_UART_Init(void)
{
  __HAL_RCC_LPUART1_CLK_ENABLE();

  // 115200 Baud, 8N1
  hUart.Instance = LPUART1;
  hUart.Init.BaudRate = 115200uL;
  hUart.Init.ClockPrescaler = UART_PRESCALER_DIV12;
  hUart.Init.Mode = UART_MODE_TX_RX;
  (void)HAL_UART_Init(&hUart);
}

/*!****************************************************************************
 * @brief
 * Flush receive and transmit buffers
 * 
 * @date  31.10.2026
 ******************************************************************************/
void vHW_UART_Flush(void)
{
  __HAL_UART_FLUSH_DRREGISTER(&hUart);
}

/*!****************************************************************************
 * @brief
 * Read single byte from input stream
 * 
 * @return  (char)  Received byte
 * @date  31.10.2026
 ******************************************************************************/
char cHW_UART_Read(void)
{
  uint8_t ucBuf = 0u;
  (void)HAL_UART_Receive(&hUart, &ucBuf, sizeof(ucBuf), 0uL);
  return (char)ucBuf;
}

/*!****************************************************************************
 * @brief 
 * Write single byte to output stream
 * 
 * @param[in] c   Output data
 * @date  31.10.2026
 ******************************************************************************/
void vHW_UART_Write(char c)
{
  uint8_t ucBuf = (uint8_t)c;
  (void)HAL_UART_Transmit(&hUart, &ucBuf, sizeof(ucBuf), 0uL);
}

/*!****************************************************************************
 * @brief 
 * Check if data is present in the RX data register
 * 
 * @return  (bool)  Data present
 * @date  31.10.2026
 ******************************************************************************/
bool bHW_UART_IsRxAvailable(void)
{
  return __HAL_UART_GET_FLAG(&hUart, UART_FLAG_RXNE);
}

/*!****************************************************************************
 * @brief 
 * Check if TX data register is empty
 * 
 * @return  (bool)  TX data register is empty
 * @date  31.10.2026
 ******************************************************************************/
bool bHW_UART_IsTxAvailable(void)
{
  return __HAL_UART_GET_FLAG(&hUart, UART_FLAG_TXE);
} 

