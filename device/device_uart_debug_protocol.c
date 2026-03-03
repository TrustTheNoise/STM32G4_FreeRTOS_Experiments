#include <device_uart_debug_protocol.h>

// todo implement data streaming

/**************************************************************************************************/
/*                                                                                                */
/*                                    Local types declarations                                    */
/*                                                                                                */
/**************************************************************************************************/

/**
 * Type that stores length of the COM message and pointer to it. So that we can store multiple messages at
 *  the same time for complex communication methods
 */
typedef struct debug_tx_request
{
    uint32_t length;
    uint8_t* message;
} debug_tx_request;

/**
 * queue of message that need to be sent using debug interface
 */
static struct
{
    debug_tx_request requests[DEBUG_ITF_TX_QUEUE_LENGTH];
    uint16_t write_index;
    uint16_t read_index;
    uint16_t active_queue_size;
    uint8_t tx_is_busy;
} tx_queue;

/**************************************************************************************************/
/*                                                                                                */
/*                                  Static variables declarations                                 */
/*                                                                                                */
/**************************************************************************************************/

// todo Not sure that connection establishing functionality is needed
static uint8_t debug_connection_is_established = 0;

//* Predefined answers for different possible debug interface requests
//* 0xAA 0x55 - prefix that must be present in all debug interface messages
static uint8_t message_ack[3] = { 0xAA, 0x55, 0xAA };
static uint8_t message_nack[3] = { 0xAA, 0x55, 0x55 };
static uint8_t message_error_log_properties[6] = { 0xAA, 0x55, DEBUG_ITF_READ_ERROR_LOG_Code, 0x00, 0x00, 0x00 }; // u8 - log version, u16 - log size
static uint8_t message_buffers_properties[6] = { 0xAA, 0x55, DEBUG_ITF_READ_BUFFERS_PROPERTIES_Code, 0x00, 0x00, 0x00 }; // u8 - number of buffers, u16 - size of buffers
static uint8_t message_buffer_description[4] = { 0xAA, 0x55, 0x00, 0x00 }; // u8 - buffer type
static uint8_t message_stream_properties[13] = { 0xAA, 0x55, DEBUG_ITF_START_DATA_STREAMING_Code, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
                                                // u8 - stream id, u8 - number of fields, u16 - entries per message, u32 - stream timeout in ms, u16 - bytes per message
static uint8_t message_stream_message_start[3] = { 0xAA, 0x55, DEBUG_ITF_STREAM_MESSAGE_START_Code };

/**************************************************************************************************/
/*                                                                                                */
/*                                  Global functions definitions                                  */
/*                                                                                                */
/**************************************************************************************************/

/**
 * @brief sets up everything needed for debug interface to work properly
 */
void setup_uart_debug_interface( void )
{
    // Todo either specify UART baud rate as
    setup_debug_uart( 500000 );
    setup_debug_uart_dma_interrupt();
}


/**
 * @brief Sends new message using UART if queue is empty. Queues the message if TX is busy. Logs error if queue is full.
 */
void debug_itf_queue_message(uint8_t* message, uint32_t message_length)
{

    if(tx_queue.write_index == tx_queue.read_index)
    {
        if (tx_queue.tx_is_busy == 0)
        {
            // There were no ongoing transaction, so we can send directly
            tx_queue.tx_is_busy = 1;

            debug_uart_send_message_dma(message, message_length);
            return;
        }
        else if (tx_queue.active_queue_size == DEBUG_ITF_TX_QUEUE_LENGTH)
        {
            // This means that we are in overflow. Write index caught up to read index
            LOG_ERROR(4214);
            return; // In case of overflow we log error, but do not overwrite the queue.
        }
    }

    // Add message to the TX queue
    tx_queue.requests[tx_queue.write_index].length = message_length;
    tx_queue.requests[tx_queue.write_index].message = message;

    tx_queue.active_queue_size += 1;
    tx_queue.write_index += 1;
    if(tx_queue.write_index == DEBUG_ITF_TX_QUEUE_LENGTH)
    {
        tx_queue.write_index = 0;
    }
}

/**
 * @brief Sends next message if available from the queue. Resets communication if no message is available.
 *
 * @note This function must be called in the TX DMA TCIF interrupt handler, because the function expects
 *  to be called right after previous message was fully sent.
 */
void debug_itf_update_message_queue( void )
{
    const uint32_t queue_length_is_zero = (tx_queue.write_index == tx_queue.read_index);
    if(queue_length_is_zero && tx_queue.active_queue_size == 0)
    {
        // If code interrupted here with an interrupt that will queue new message, message will not be sent
        tx_queue.tx_is_busy = 0;
        return;
    }

    uint32_t read_index = tx_queue.read_index;
    debug_uart_send_message_dma(tx_queue.requests[read_index].message, tx_queue.requests[read_index].length);

    read_index += 1;
    if ( read_index == DEBUG_ITF_TX_QUEUE_LENGTH )
    {
        read_index = 0;
    }

    tx_queue.read_index = read_index;
    tx_queue.active_queue_size -= 1;
}

/**
 * @brief handles all debug inteface RX requests.
 *
 *
 * Current implementation doesn't support multiple requests sent as a single UART message, each request needs to be sent separately one by one.
 *
 * todo Write a documentation for the protocol and link documentation in this comment
 */
void debug_itf_handle_rx(uint8_t* message, uint32_t message_length)
{
	if(message_length <= 2)
	{
        LOG_ERROR(3221); // Invalid message length
        debug_itf_queue_message(message_nack, sizeof(message_nack));
        return;
	}

	if(message[0] != 0xAA || message[1] != 0x55)
	{
		LOG_ERROR(3222); // Invalid message prefix. Message is not a part of debug protocol, or buffer doesn't store the beginning of the message.
        debug_itf_queue_message(message_nack, sizeof(message_nack));
        return;
	}


	if(message_length == 3) // one byte requests
	{
		if( message[2] == DEBUG_ITF_ESTABLISH_CONNECTION_Code )
		{
			debug_connection_is_established = 1;
			debug_itf_queue_message(message_ack, sizeof(message_ack));
			return;
		}

		if( message[2] == DEBUG_ITF_CLOSE_CONNECTION_Code )
		{
			debug_connection_is_established = 0;
			debug_itf_queue_message(message_ack, sizeof(message_ack));
			return;
		}

		if( message[2] == DEBUG_ITF_KEEP_ALIVE_Code )
		{
			if(debug_connection_is_established == 0)
			{
				// NACK keep alive if connection wasn't established. Can help to find unintentional MCU reset
				debug_itf_queue_message(message_nack, sizeof(message_nack));
			}
			else
			{
				debug_itf_queue_message(message_ack, sizeof(message_ack));
			}
			return;
		}

		if( message[2] == DEBUG_ITF_READ_ERROR_LOG_Code )
		{
			// Write error log depth to bytes 4 and 5 of the error log
			uint16_t* value_ptr = (uint16_t*)(&message_error_log_properties[3]);
			*value_ptr = DEBUG_ERROR_LOG_DEPTH;

			message_error_log_properties[5] = DEBUG_ERROR_LOG_VERSION;
			debug_itf_queue_message(message_error_log_properties, sizeof(message_error_log_properties));

			// Send error log
			debug_itf_queue_message((uint8_t*)debug_get_error_log_ptr(), sizeof(debug_error_log));
			return;
		}
		if( message[2] == DEBUG_ITF_READ_BUFFERS_PROPERTIES_Code )
		{
			debug_com_buffers* buffers = debug_get_com_buffer();
			message_buffers_properties[3] = buffers->next_free_buffer_index;
			uint16_t* value_ptr = (uint16_t*)(&message_buffers_properties[4]);
			*value_ptr = DEBUG_BUFFER_SIZE; // Number of points in a buffer
			debug_itf_queue_message(message_buffers_properties, sizeof(message_buffers_properties));
			buffers->read_requests_count += 1;
			return;
		}

		// Read one of available buffers
		if ( message[2] > DEBUG_ITF_READ_BUFFERS_PROPERTIES_Code && message[2] <= (DEBUG_ITF_READ_BUFFERS_PROPERTIES_Code + DEBUG_MAX_BUFFERS_COUNT) )
		{
			uint8_t requested_index = message[2] - DEBUG_ITF_READ_BUFFERS_PROPERTIES_Code - 1;
			debug_com_buffers* buffers = debug_get_com_buffer();
			if( requested_index > buffers->next_free_buffer_index )
			{
				debug_itf_queue_message(message_nack, sizeof(message_nack));
			}
			debug_itf_queue_message(message_ack, sizeof(message_ack));

			uint8_t buffer_type = buffers->buffers_types[requested_index];
			uint8_t* buffer_pointer = (uint8_t*)(buffers->buffers_values[requested_index]);

			uint16_t data_size = DEBUG_BUFFER_SIZE;

			message_buffer_description[2] = message[2];
			message_buffer_description[3] = buffer_type;
			debug_itf_queue_message(message_buffer_description, sizeof(message_buffer_description));

			if( buffer_type == F32_Type || buffer_type == I32_Type || buffer_type == U32_Type )
			{
				data_size *= 4;
			}
			else if( buffer_type == I16_Type || buffer_type == U16_Type )
			{
				data_size *= 2;
			}

			// Send buffer description first and than buffer itself
			debug_itf_queue_message(buffer_pointer, data_size);
			return;
		}

		if( message[2] == DEBUG_ITF_START_DATA_STREAMING_Code )
		{
            debug_itf_queue_message(message_ack, sizeof(message_ack));
            debug_com_stream* active_stream = debug_get_active_com_stream();
            if(active_stream == (void*)(0))
            {
                message_stream_properties[3] = 0; // Stream id 0 is forbidden, client should treat it as no stream is registered
                debug_itf_queue_message(message_stream_properties, sizeof(message_stream_properties));
                return;
            }
            else
            {
                message_stream_properties[3] = active_stream->id;
                message_stream_properties[4] = active_stream->entry_fields_count;

                uint16_t* u16_value_ptr = (uint16_t*)(&message_stream_properties[5]);
                *u16_value_ptr = active_stream->entries_per_message_count;

                uint32_t *u32_value_ptr = (uint32_t*)(&message_stream_properties[7]);
                *u32_value_ptr = active_stream->timeout_ms;

                u16_value_ptr = (uint16_t*)(&message_stream_properties[11]);
                *u16_value_ptr = active_stream->message_byte_size;

                debug_itf_queue_message(message_stream_properties, sizeof(message_stream_properties));
            }

            if(active_stream->entry_fields_count >= 3)
            {
                debug_itf_queue_message(active_stream->entry_fields_types, active_stream->entry_fields_count);
            }
            else
            {
                // Send at least 3 bytes to not brick the DMA interrupt. This situation is supposed to be handled on the client as well
                debug_itf_queue_message(active_stream->entry_fields_types, 3);
            }

            active_stream->is_active = 1;
			return;
		}

		if( message[2] == DEBUG_ITF_STOP_DATA_STREAMING_Code )
		{
            debug_com_stream* active_stream = debug_get_active_com_stream();
		    active_stream->is_active = 0;

            debug_itf_queue_message(message_ack, sizeof(message_ack));

		    return;
		}

		// Call one of the generic functions
		if( message[2] >= DEBUG_ITF_GENERIC_REQUEST_BASE_Code && message[2] < DEBUG_ITF_GENERIC_REQUEST_BASE_Code + 16)
		{
		    debug_itf_queue_message(message_ack, sizeof(message_ack));

		    uint8_t selector = message[2] - DEBUG_ITF_GENERIC_REQUEST_BASE_Code;

		    switch(selector)
		    {
		    case 0: debug_itf_handle_generic_request_1_cbk(); break;
		    case 1: debug_itf_handle_generic_request_2_cbk(); break;
		    case 2: debug_itf_handle_generic_request_3_cbk(); break;
		    case 3: debug_itf_handle_generic_request_4_cbk(); break;
		    case 4: debug_itf_handle_generic_request_5_cbk(); break;
		    case 5: debug_itf_handle_generic_request_6_cbk(); break;
		    case 6: debug_itf_handle_generic_request_7_cbk(); break;
		    case 7: debug_itf_handle_generic_request_8_cbk(); break;
		    case 8: debug_itf_handle_generic_request_9_cbk(); break;
		    case 9: debug_itf_handle_generic_request_10_cbk(); break;
		    case 10: debug_itf_handle_generic_request_11_cbk(); break;
		    case 11: debug_itf_handle_generic_request_12_cbk(); break;
		    case 12: debug_itf_handle_generic_request_13_cbk(); break;
		    case 13: debug_itf_handle_generic_request_14_cbk(); break;
		    case 14: debug_itf_handle_generic_request_15_cbk(); break;
		    case 15: debug_itf_handle_generic_request_16_cbk(); break;
		    }

		    return;
		}


	} /* message_length == 3 */
}


void debug_update_com_stream( void )
{
    debug_com_stream* active_stream = debug_get_active_com_stream();
    if(active_stream == (void*)(0))
    {
        return;
    }

    if(active_stream->is_active == 0)
    {
        return;
    }

    debug_itf_queue_message(message_stream_message_start, sizeof(message_stream_message_start));
    debug_itf_queue_message(active_stream->message, active_stream->message_byte_size);
}



// Weak empty definitions for all generic requests. All these functions are intended to be redefined in application code
__attribute__((weak)) void debug_itf_handle_generic_request_1_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_2_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_3_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_4_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_5_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_6_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_7_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_8_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_9_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_10_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_11_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_12_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_13_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_14_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_15_cbk( void ) {}
__attribute__((weak)) void debug_itf_handle_generic_request_16_cbk( void ) {}


/**************************************************************************************************/
/*                                                                                                */
/*                                        Interrupt handlers                                      */
/*                                                                                                */
/**************************************************************************************************/

/**
 * @brief debug UART interrupt handler
 */
void USART3_IRQHandler()
{
    // UART data receive sections
    if( USART3->ISR & USART_ISR_RTOF ) // UART message received
    {
        // Read and do something with data received from DMA
        const uint16_t received_dma_size = debug_uart_get_received_message_len();
        uint8_t* received_buffer = debug_uart_get_receive_buffer();

        // Reset RX DMA after every timeout to make DMA write back from 0.
        // Given that we work with pointers, this can cause first bytes of the buffer to be overwritten
        // If a new message from master arrives right away. But behaviour like this is hard to effectively
        // prevent, and therefore it will be considered as an error on the side of the master implementation.
        debug_uart_reset_rx_dma();

        debug_itf_handle_rx(received_buffer, received_dma_size);

        USART3->ICR |= USART_ICR_RTOCF; // Clear the timeout interrupt flag
    }

    // UART error handling section
    if (USART3->ISR & USART_ISR_ORE) // Overrun error detected;
    {
        USART3->ICR = USART_ICR_ORECF;
        LOG_ERROR(533); // USART1_OVERRUN_Error
    }

    if (USART3->ISR & USART_ISR_NE) // Noise error detected;
    {
        USART3->ICR = USART_ICR_NECF;
        LOG_ERROR(522); // USART1_NOISE_DETECTION_Error
    }

    if(USART3->ISR & USART_ISR_FE) // Framing error detected;
    {
        USART3->ICR = USART_ICR_FECF;
        LOG_ERROR(511); // USART1_FRAMING_Error
    }
}


uint32_t dma_transfers_count = 0; // @note debug value only, should be removed in actual code
/**
 * @brief debug UART TX DMA interrupt handler
 */
void DMA2_CH2_IRQHandler( void )
{
    dma_transfers_count += 1;
    if(DMA2->ISR & DMA_ISR_TCIF2)
    {
        debug_itf_update_message_queue();

        // Clear full transfer complete, half transfer complete and global interrupt flags for DMA channel.
        //  Even though CGIFx will clear all other flags by itself, all 3 flags are cleared manually for possible
        //  Compatibility reasons with other STM32 families
        DMA2->IFCR = DMA_IFCR_CTCIF2 | DMA_IFCR_CHTIF2 | DMA_IFCR_CGIF2;
    }
}

