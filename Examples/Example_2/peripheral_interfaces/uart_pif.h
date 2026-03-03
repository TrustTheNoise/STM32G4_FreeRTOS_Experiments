//* Description can be found in the end of the file
#pragma once

#ifndef UART_PIF_H_
#define UART_PIF_H_

#include "device_definitions.h"
#include "debug_pif.h"

/**************************************************************************************************/
/*                                                                                                */
/*                                      File configuration                                        */
/*                                                                                                */
/**************************************************************************************************/

#ifndef UART_1_BUFFER_SIZE
    #define UART_1_BUFFER_SIZE          (256U)  // The size of the programmable receive buffer.
#endif /* UART_1_BUFFER_SIZE */                 // The max possible length of a single received message must
                                                // not be bigger than this value.
                                                // todo rename to something more appropriate
/**************************************************************************************************/
/*                                                                                                */
/*                                     Functions declarations                                     */
/*                                                                                                */
/**************************************************************************************************/

void setup_uart1( const uint32_t desired_uart_baud_rate );

void uart1_send_byte( const uint8_t byte_to_send );

void uart1_send_break_charactes( void );

void uart1_send_message_dma( const uint8_t *message, const uint32_t message_length );

uint32_t uart1_get_received_message_len( void );

uint8_t* uart1_get_receive_buffer(void);

void uart1_reset_rx_dma( void );

#endif /* UART_PIF_H_ */

// EOF
