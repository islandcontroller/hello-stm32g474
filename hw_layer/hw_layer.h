/*!****************************************************************************
 * @file
 * hw_layer.h
 *
 * @brief
 * Hardware Layer
 *
 * @date  16.08.2026
 ******************************************************************************/

#ifndef HW_LAYER_H_
#define HW_LAYER_H_

/*- Header files -------------------------------------------------------------*/
#include <stdint.h>


/*- Public interface ---------------------------------------------------------*/
void vHW_Init(void);

// System
uint32_t ulHW_GetTime(void);
uint32_t ulHW_GetCoreClkFreq(void);
uint16_t uiHW_GetFlashSize(void);
const uint32_t* pulHW_GetUID(void);
uint32_t ulHW_GetCPUID(void);
void vHW_Reboot(void);

// GPIO
void vHW_ToggleLED(void);

// UART
void vHW_FlushSerial(void);
char cHW_ReadSerial(void);
void vHW_WriteSerial(char c);
bool bHW_IsSerialRxAvailable(void);
bool bHW_IsSerialTxAvailable(void);

#endif // HW_LAYER_H_
