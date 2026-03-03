// Description is in the end of the file
#pragma once

#ifndef DEVICE_UART_DEBUG_PROTOCOL_H_
#define DEVICE_UART_DEBUG_PROTOCOL_H_

#include <stdint.h>

#include "device_definitions.h"
#include "device_mcu_includes.h"

#include "debug_pif.h"
#include "debug_utils.h"

/**************************************************************************************************/
/*                                                                                                */
/*                                      File configuration                                        */
/*                                                                                                */
/**************************************************************************************************/

// Number of messages that can be simultaneously stored in the debug queue.
//  Actual number is this value +1 because after value is loaded into DMA we don't need to store it in the queue any longer
//  So we have can have DEBUG_ITF_TX_QUEUE_LENGTH number of messages in the queue and 1 more being currently transmitted.
// 	Therefore if we only make a single request when UART doesn't send anything, this request will get directly into DMA
//	without being added to the queue, as there is no practical need to do that.
#define DEBUG_ITF_TX_QUEUE_LENGTH               (6U)

/**************************************************************************************************/
/*                                                                                                */
/*                                   UART debug protocol codes                                    */
/*                                                                                                */
/**************************************************************************************************/

#define DEBUG_ITF_ESTABLISH_CONNECTION_Code     (0x01U)
#define DEBUG_ITF_CLOSE_CONNECTION_Code         (0x02U)
#define DEBUG_ITF_KEEP_ALIVE_Code               (0x03U)

#define DEBUG_ITF_READ_ERROR_LOG_Code           (0x08U)

#define DEBUG_ITF_READ_BUFFERS_PROPERTIES_Code  (0x10U)

#define DEBUG_ITF_START_DATA_STREAMING_Code     (0x31U)
#define DEBUG_ITF_STREAM_MESSAGE_START_Code     (0x32U)
#define DEBUG_ITF_STOP_DATA_STREAMING_Code      (0x33U)

#define DEBUG_ITF_GENERIC_REQUEST_BASE_Code     (0x40U)


/**************************************************************************************************/
/*                                                                                                */
/*                                  Global functions declarations                                 */
/*                                                                                                */
/**************************************************************************************************/

void setup_uart_debug_interface( void );

void debug_itf_queue_message(uint8_t* message, uint32_t message_length);
void debug_itf_update_message_queue( void );

void debug_itf_handle_rx(uint8_t* message, uint32_t message_length);

void debug_itf_handle_generic_request_1_cbk( void );
void debug_itf_handle_generic_request_2_cbk( void );
void debug_itf_handle_generic_request_3_cbk( void );
void debug_itf_handle_generic_request_4_cbk( void );
void debug_itf_handle_generic_request_5_cbk( void );
void debug_itf_handle_generic_request_6_cbk( void );
void debug_itf_handle_generic_request_7_cbk( void );
void debug_itf_handle_generic_request_8_cbk( void );
void debug_itf_handle_generic_request_9_cbk( void );
void debug_itf_handle_generic_request_10_cbk( void );
void debug_itf_handle_generic_request_11_cbk( void );
void debug_itf_handle_generic_request_12_cbk( void );
void debug_itf_handle_generic_request_13_cbk( void );
void debug_itf_handle_generic_request_14_cbk( void );
void debug_itf_handle_generic_request_15_cbk( void );
void debug_itf_handle_generic_request_16_cbk( void );

#endif /* DEVICE_UART_DEBUG_PROTOCOL_H_ */

/**
 * This is a version of the device debug protocol moved from the M1.F1 project. It is developed inside M1.F1 and will be updated from there when needed. This version is good enough for it purpose
 */

