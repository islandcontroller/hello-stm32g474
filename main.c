/*!****************************************************************************
 * @brief
 * main.c
 *
 * @brief
 * Main program
 *
 * @date  19.10.2025
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include "hw_layer.h"
#include "vt100.h"


/*- Macros -------------------------------------------------------------------*/
/// LED toggle interval in ms
#define LED_INTERVAL                  (500uL)


/*- Prototypes ---------------------------------------------------------------*/
static void vPrintCoreInfo(void);
static void vPrintSysCoreClk(void);
static void vPrintEsigInfo(void);

static void vPollLed(void);
static void vPollSerial(void);


/*- Private data -------------------------------------------------------------*/
/// Last toggle timestamp
static uint32_t ulLastToggle;


/*- Public interface ---------------------------------------------------------*/
/*!****************************************************************************
 * @brief
 * Main program entrypoint and background task
 *
 * @date  19.10.2025
 ******************************************************************************/
int main(void)
{
  vHW_Init();
  ulLastToggle = 0uL;

  printf(
    VT100_RESET_TERMINAL
    VT100_INVERT
    "--------------------------------------------------\r\n"
    "        ##                                        \r\n"
    "                                                  \r\n"
    "      ##    ######                                \r\n"
    "     ##  ##                                       \r\n"
    "    ##  ##                                        \r\n"
    "   ##    ######  ######                 @islandc_ \r\n"
    "--------------------------------------------------\r\n"
    " MCU:  STMicroelectronics STM32G474               \r\n"
    " Core: Arm Cortex-M4F                             \r\n"
    "--------------------------------------------------\r\n"
    VT100_NO_INVERT
    "\r\n"
  );
  vPrintCoreInfo();
  printf("\r\n");
  vPrintSysCoreClk();
  printf("\r\n");
  vPrintEsigInfo();
  printf("\r\nType \"?\" to show available commands.\r\n> ");
  fflush(stdout);
  vHW_FlushSerial();

  while (1)
  {
    vPollLed();
    vPollSerial();
  }
}

/*- Private functions --------------------------------------------------------*/
/*!****************************************************************************
 * @brief
 * Print core information from CPUID
 *
 * @date  25.10.2025
 ******************************************************************************/
static void vPrintCoreInfo(void)
{
  uint32_t ulCpuid = ulHW_GetCPUID();
  printf(
    "-- Core Information ------------------------------\r\n"
    "CPUID:       0x%08lX\r\n", ulCpuid
  );

  // Implementor
  uint8_t ucImpl = (uint8_t)(ulCpuid >> 24);
  printf("implementer: 0x%02X  (%s)\r\n", ucImpl, (ucImpl == 0x41u) ? "ARM" : "unknown");

  // Processor revision
  uint8_t ucVar = (uint8_t)((ulCpuid >> 20) & 0xFuL);
  printf("variant:     0x%01X   (Revision %d)\r\n", ucVar, ucVar);

  // Part number
  uint16_t uiPartno = (uint16_t)((ulCpuid >> 4) & 0xFFFuL);
  printf("partno:      0x%03X (%s)\r\n", uiPartno, (uiPartno == 0xC24u) ? "Cortex-M4" : "unknown");

  // Patch release
  uint8_t ucRev = (uint8_t)(ulCpuid & 0xFuL);
  printf("revision:    0x%01X   (Patch %d)\r\n", ucRev, ucRev);
}

/*!****************************************************************************
 * @brief
 * Print system core clock frequency
 *
 * @date  25.10.2025
 ******************************************************************************/
static void vPrintSysCoreClk(void)
{
  uint32_t ulFreq_kHz = ulHW_GetCoreClkFreq() / 1000uL;
  uint16_t uiFreq_MHz = (uint16_t)(ulFreq_kHz / 1000uL);
  uint16_t uiFreqRem_kHz = (uint16_t)(ulFreq_kHz % 1000uL);
  printf(
    "-- Clocks ----------------------------------------\r\n"
    "f_HCLK = %d.%03d MHz\r\n", uiFreq_MHz, uiFreqRem_kHz
  );
}

/*!****************************************************************************
 * @brief
 * Print FLASH size and unique ID
 *
 * @date  25.10.2025
 ******************************************************************************/
static void vPrintEsigInfo(void)
{
  printf(
    "-- ESIG ------------------------------------------\r\n"
  );

  uint16_t uiFlashSize = uiHW_GetFlashSize();
  printf("FLASH Size: %d KB\r\n", uiFlashSize);

  const uint32_t* pulUID = pulHW_GetUID();
  printf("Unique ID: %08lX %08lX %08lX\r\n", pulUID[0], pulUID[1], pulUID[2]);
}


/*- Private functions --------------------------------------------------------*/
/*!****************************************************************************
 * @brief 
 * LED blinky
 * 
 * @date  16.11.2026
 ******************************************************************************/
static void vPollLed(void)
{
  uint32_t ulNow = ulHW_GetTime();
  if ((ulNow - ulLastToggle) > LED_INTERVAL)
  {
    ulLastToggle = ulNow;
    vHW_ToggleLED();
  }
}

/*!****************************************************************************
 * @brief 
 * Serial I/O demo
 * 
 * @date  16.11.2026
 ******************************************************************************/
static void vPollSerial(void)
{
  int c = getchar();
  if (!isprint(c)) return;
  printf("%c\r\n", (char)c);
  switch (c)
  {
    case '?':
      // Show available commands
      printf(
        "Available commands:\r\n"
        "  ?  Show available commands\r\n"
        "  t  Display current system time\r\n"
        "  r  Reboot\r\n"
      );
      break;

    case 't':
      // Display current system time
      printf("%ld ms\r\n", ulHW_GetTime());
      break;

    case 'r':
      // Reboot
      vHW_Reboot();
      break;

    default:
      // unknown input
      printf("Unknown command\r\n");
      break;
  }
  printf("> ");
  fflush(stdout);
}
