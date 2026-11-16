/*!****************************************************************************
 * @file
 * hw_uart.h
 *
 * @brief
 * Hardware Layer - Serial I/O via UART
 *
 * @date  31.08.2026
 ******************************************************************************/

#ifndef HW_UART_H_
#define HW_UART_H_

/*- Header files -------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>


/*- Public interface ---------------------------------------------------------*/
void vHW_UART_Init(void);
void vHW_UART_Flush(void);
char cHW_UART_Read(void);
void vHW_UART_Write(char c);
bool bHW_UART_IsRxAvailable(void);
bool bHW_UART_IsTxAvailable(void);

#endif // HW_UART_H_
