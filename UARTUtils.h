
/** ============================================================================
 *  @file       UARTUtils.h
 *
 *  @brief      UART related utilities
 *
 *  The UARTUtils header file should be included in an application as follows:
 *  @code
 *  #include <UARTUtils.h>
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef __UARTUtils_H
#define __UARTUtils_H

#ifdef __cplusplus
extern "C" {
#endif

#include <xdc/std.h>

#include <stdio.h>

#if defined(__TI_COMPILER_VERSION__)
#include <file.h>
#endif

void SetupUART(void);

#ifdef __cplusplus
}
#endif

#endif /* __UARTUtils_H */
