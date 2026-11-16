/*!****************************************************************************
 * @file
 * ioretarget.c
 *
 * @brief
 * I/O syscalls retargeting
 *
 * @date  03.03.2022
 * @date  21.03.2023  Adapted from hello-ch32v103 for hello-ch32v003
 * @date  30.07.2025  Adapted from hello-ch32v003 for hello-stm32f103
 * @date  26.10.2026  Adapted from hello-stm32f103 for hello-stm32g474
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "hw_layer.h"


/*- Macros -------------------------------------------------------------------*/
/// Timeout for a serial operation, measured in milliseconds
#define SYSCALLS_SERIAL_TIMEOUT       20uL


/*- Retargeting functions ----------------------------------------------------*/
/*!****************************************************************************
 * @brief
 * Determines whether a file descriptor is accessing a TTY device
 *
 * References:
 *  [3] _isatty, CRT Alphabetical Function Reference, Microsoft
 *  https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/isatty
 *
 * @param[in] fd          File descriptor
 * @return  (int)       1 if fd is accessing a TTY device, else 0
 * @date  03.03.2022
 ******************************************************************************/
int _isatty(int fd)
{
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
  {
    return 1;
  }
  else
  {
    errno = EBADF;
    return 0;
  }
}

/*!****************************************************************************
 * @brief
 * Reads data from a file descriptor
 *
 * This function will early-return 0 if no data is available at the start of
 * the call. Otherwise, the function will continue to read data until either
 * a timeout or @c buffer_size have been reached. The timeout is calculated
 * per-byte.
 * 
 * References:
 *  [4] _read, CRT Alphabetical Function Reference, Microsoft
 *  https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/read
 *
 * @param[in] fd          File descriptor
 * @param[out] *buffer    Buffer pointer
 * @param[in] buffer_size Maximum number of bytes to be read
 * @return  (int)       Number of bytes read
 * @date  03.03.2022
 ******************************************************************************/
int _read(int fd, void* buffer, unsigned buffer_size)
{
  if (buffer == NULL || buffer_size == 0)
  {
    errno = EINVAL;
    return -1;
  }
  else if (fd != STDIN_FILENO)
  {
    errno = EBADF;
    return -1;
  }
  else if (!bHW_IsSerialRxAvailable()) 
  {
    errno = EAGAIN;
    return -1;
  }
  else
  {
    ((char*)buffer)[0] = cHW_ReadSerial();

    // Read available data until buffer size or timeout
    for (unsigned i = 1; i < buffer_size; ++i)
    {
      uint32_t ulStart = ulHW_GetTime();
      while (!bHW_IsSerialRxAvailable())
      {
        // Exit on timeout
        if ((ulHW_GetTime() - ulStart) > SYSCALLS_SERIAL_TIMEOUT) return (int)i;
      }
      ((char*)buffer)[i] = cHW_ReadSerial();
    }
    return (int)buffer_size;
  }
}

/*!****************************************************************************
 * @brief
 * Writes data to a file descriptor
 *
 * This function will try to fill the hardware transmit buffer until either a
 * timeout occurs or the @c count is reached. The timeout is calculated per-
 * -byte.
 * 
 * References:
 *  [5] _write, CRT Alphabetical Function Reference, Microsoft
 *  https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/write
 *
 * @param[in] fd            File descriptor
 * @param[in] *buffer       Buffer of data to be written
 * @param[in] count         Number of bytes
 * @return  (int)         Number of bytes written
 * @date  03.03.2022
 ******************************************************************************/
int _write(int fd, const char* buffer, unsigned count)
{
  if (buffer == NULL || count == 0)
  {
    errno = EINVAL;
    return -1;
  }
  else if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
  {
    for (unsigned i = 0; i < count; ++i)
    {
      uint32_t ulStart = ulHW_GetTime();
      while (!bHW_IsSerialTxAvailable())
      {
        // Exit on timeout
        if ((ulHW_GetTime() - ulStart) > SYSCALLS_SERIAL_TIMEOUT) return (int)i;
      }
      vHW_WriteSerial(((const char*)buffer)[i]);
    }
    return (int)count;
  }
  else
  {
    errno = EBADF;
    return -1;
  }
}

/*!****************************************************************************
 * @brief
 * Move file pointer to location
 *
 * @note
 * "If execution is allowed to continue, these functions set errno to EBADF and
 * return -1L. On devices incapable of seeking (such as terminals and printers),
 * the return value is undefined."
 *
 * References:
 *  [6] _lseek, _lseeki64, CRT Alphabetical Function Reference, Microsoft
 *  https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/lseek-lseeki64
 *
 * @param[in] fd          File descriptor
 * @param[in] offset      Number of bytes from origin
 * @param[in] origin      Initial position
 * @return  (int)       New offset in bytes from beginning of the file
 * @date  03.03.2022
 ******************************************************************************/
int _lseek(int fd __unused, int offset __unused, int origin __unused)
{
  errno = EBADF;
  return -1;
}

/*!****************************************************************************
 * @brief
 * Closes a file descriptor
 *
 * References:
 *  [7] _close, CRT Alphabetical Function Reference, Microsoft
 *  https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/close
 *
 * @param[in] fd          File descriptor
 * @return  (int)       0, if the close was successful.
 * @date  03.03.2022
 ******************************************************************************/
int _close(int fd)
{
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
  {
    return 0;
  }
  else
  {
    errno = EBADF;
    return -1;
  }
}

/*!****************************************************************************
 * @brief
 * Gets information about an open file
 *
 * References:
 *  [8] _fstat, _fstat32, _fstat64, _fstati64, _fstat32i64, _fstat64i32, CRT
 *  Alphabetical Function Reference, Microsoft
 *  https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fstat-fstat32-fstat64-fstati64-fstat32i64-fstat64i32
 *
 * @param[in] fd          File descriptor
 * @param[out] *buffer    Buffer structure to store the results
 * @return  (int)       0, if the file status information is obtained
 * @date  03.03.2022
 ******************************************************************************/
int _fstat(int fd, struct stat* buffer)
{
  if (buffer == NULL)
  {
    errno = EINVAL;
    return -1;
  }
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
  {
    buffer->st_dev = (dev_t)fd;
    buffer->st_mode = S_IFCHR;
    buffer->st_nlink = 1;
    return 0;
  }
  else
  {
    errno = EBADF;
    return -1;
  }
}
