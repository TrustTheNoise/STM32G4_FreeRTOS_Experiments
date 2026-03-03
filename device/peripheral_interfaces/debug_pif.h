// Description can be found in the end of the file
#pragma once

#ifndef DEVICE_DEBUG_PIF_H_
#define DEVICE_DEBUG_PIF_H_

#include "device_definitions.h"
#include "device_mcu_includes.h"

#include "debug_utils.h"



/**************************************************************************************************/
/*                                                                                                */
/*                                  Default file configurations                                   */
/*                                                                                                */
/**************************************************************************************************/

// The size of the UART RX software queue. The max possible length of a single received message must be smaller
//  than this value.
#ifndef DEBUG_UART_BUFFER_SIZE
    #define DEBUG_UART_BUFFER_SIZE          (64U)   // Must not be smaller than 3 or bigger than 2^16-1
#endif

/**************************************************************************************************/
/*                                                                                                */
/*                                  Global functions declarations                                 */
/*                                                                                                */
/**************************************************************************************************/

void setup_debug_uart( const uint32_t desired_uart_baud_rate );
void setup_debug_uart_dma_interrupt( void );

void debug_uart_send_message_dma( const uint8_t *message, const uint32_t message_length );

uint32_t debug_uart_get_received_message_len( void );
uint8_t* debug_uart_get_receive_buffer( void );

void debug_uart_reset_rx_dma( void );

/**************************************************************************************************/
/*                                                                                                */
/*                                      Static error handing                                      */
/*                                                                                                */
/**************************************************************************************************/

#if DEBUG_UART_BUFFER_SIZE < 3 || DEBUG_UART_BUFFER_SIZE > UINT16_MAX
    #error "DEBUG_UART_BUFFER_SIZE must be >2 and < 2^16-1 for DMA to work properly. Change DEBUG_UART_BUFFER_SIZE value accordingly"
#endif

/**************************************************************************************************/
/*                                                                                                */
/*                          Full Defines template for _pif_derinitions.h                          */
/*                                                                                                */
/**************************************************************************************************/

#if 0
/**************************************************************************************************/
/*                                                                                                */
/*                                   device_debug_pif definitions                                 */
/*                                                                                                */
/**************************************************************************************************/

#define DEBUG_UART_BUFFER_SIZE                 (64U)

#endif /* 0 */

#endif /* DEVICE_DEBUG_PIF_H_ */

/**
 * todo write explanation as it is very importnat
 */
