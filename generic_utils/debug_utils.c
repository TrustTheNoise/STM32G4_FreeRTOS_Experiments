#include "debug_utils.h"


static debug_error_log error_log;
static debug_com_buffers com_buffers;
static debug_com_stream* active_com_stream = (void*)(0);

uint32_t error_log_write_index = 0;
uint32_t total_errors_count = 0;


const debug_error_log* const debug_get_error_log_ptr( void )
{
    return &error_log;
}



// todo implement error type?? (like critical and not critical. critical error should probably cause partial system shutdown).
uint16_t debug_log_error( const pif_error_code error_code )
{
    if ( error_code == 0 )
    {
        return 0;
    }

    total_errors_count += 1;

    // We skip writing if error log is full
    if (error_log_write_index == DEBUG_ERROR_LOG_DEPTH)
    {
        return error_code;
    }

    error_log.error_codes[error_log_write_index] = error_code;
    error_log_write_index += 1;

    return error_code;
}

/**
 * @bried A simplest way to block main thread in case of a critical error. Function is supposed to be redefined in the application
 */
__attribute__((weak)) void debug_shutdown_system_cbk( void )
{
    while(1)
    {
        // do nothing in the infinite loop
    }
}


// todo add description
void debug_add_value_to_f32_buffer(debug_f32_buffer* target_buffer, float value)
{
    if(target_buffer->write_delay_access_count > 0)
    {
        target_buffer->write_delay_access_count -= 1;
        return;
    }

    if(target_buffer->next_write_index == DEBUG_BUFFER_SIZE)
    {
        return;
    }
    target_buffer->values[target_buffer->next_write_index] = value;
    target_buffer->next_write_index += 1;
}


// todo add description
void debug_reset_f32_buffer(debug_f32_buffer* target_buffer)
{
    target_buffer->next_write_index = 0;
}


void debug_add_value_to_i32_buffer(debug_i32_buffer* target_buffer, int32_t value)
{
    if(target_buffer->write_delay_access_count > 0)
    {
        target_buffer->write_delay_access_count -= 1;
        return;
    }

    if(target_buffer->next_write_index == DEBUG_BUFFER_SIZE)
    {
        return;
    }
    target_buffer->values[target_buffer->next_write_index] = value;
    target_buffer->next_write_index += 1;
}


void debug_reset_i32_buffer(debug_i32_buffer* target_buffer)
{
    target_buffer->next_write_index = 0;
}


void debug_add_value_to_u32_buffer( debug_u32_buffer* target_buffer, uint32_t value )
{
    if(target_buffer->write_delay_access_count > 0)
    {
        target_buffer->write_delay_access_count -= 1;
        return;
    }

    if(target_buffer->next_write_index == DEBUG_BUFFER_SIZE)
    {
        return;
    }
    target_buffer->values[target_buffer->next_write_index] = value;
    target_buffer->next_write_index += 1;
}


void debug_reset_u32_buffer( debug_u32_buffer* target_buffer )
{
    target_buffer->next_write_index = 0;
}


void debug_add_value_to_i16_buffer( debug_i16_buffer* target_buffer, int16_t value )
{
    if(target_buffer->write_delay_access_count > 0)
    {
        target_buffer->write_delay_access_count -= 1;
        return;
    }

    if(target_buffer->next_write_index == DEBUG_BUFFER_SIZE)
    {
        return;
    }
    target_buffer->values[target_buffer->next_write_index] = value;
    target_buffer->next_write_index += 1;
}


void debug_reset_i16_buffer( debug_i16_buffer* target_buffer )
{
    target_buffer->next_write_index = 0;
}


void debug_add_value_to_u16_buffer( debug_u16_buffer* target_buffer, uint16_t value )
{
    if(target_buffer->write_delay_access_count > 0)
    {
        target_buffer->write_delay_access_count -= 1;
        return;
    }

    if(target_buffer->next_write_index == DEBUG_BUFFER_SIZE)
    {
        return;
    }
    target_buffer->values[target_buffer->next_write_index] = value;
    target_buffer->next_write_index += 1;
}


void debug_reset_u16_buffer( debug_u16_buffer* target_buffer )
{
    target_buffer->next_write_index = 0;
}

// TODO I really don't like that i have many exactly same functions with different types, though I'm not sure if this can be handled differently
//          but if it is possible - something to do later.
void debug_add_value_to_u8_buffer( debug_u8_buffer* target_buffer, uint8_t value )
{
    if(target_buffer->write_delay_access_count > 0)
    {
        target_buffer->write_delay_access_count -= 1;
        return;
    }

    if(target_buffer->next_write_index == DEBUG_BUFFER_SIZE)
    {
        return;
    }
    target_buffer->values[target_buffer->next_write_index] = value;
    target_buffer->next_write_index += 1;
}


void debug_reset_u8_buffer( debug_u8_buffer* target_buffer )
{
    target_buffer->next_write_index = 0;
}


void debug_register_com_buffer( void* buffer_pointer, DEBUG_DATA_TYPE buffer_type )
{
    if(com_buffers.next_free_buffer_index == DEBUG_MAX_BUFFERS_COUNT)
    {
        return;
    }

    if(buffer_pointer == (void*)(0))
    {
        return;
    }

    if(buffer_type > U8_Type || buffer_type == NO_Type) // Unknown buffer type
    {
        return;
    }

    com_buffers.buffers_types[com_buffers.next_free_buffer_index] = buffer_type;

    // @Note this conversion is only possible because we store pointer to buffer values as first field of the buffer.
    com_buffers.buffers_values[com_buffers.next_free_buffer_index] = buffer_pointer;

    com_buffers.next_free_buffer_index += 1;
}


void debug_unregister_all_com_buffers( void )
{
	com_buffers.next_free_buffer_index = 0;
}


debug_com_buffers* debug_get_com_buffer( void )
{
    return &com_buffers;
}


pif_error_code debug_register_com_stream( debug_com_stream* stream_instance )
{
    if(stream_instance == (void*)(0))
    {
        return LOG_ERROR(5686); // Empty stream instance is given
    }

    if(active_com_stream != (void*)(0))
    {
        return LOG_ERROR(5687); // Overwrite of already registered stream
    }


    active_com_stream = stream_instance;
    return 0;
}


void debug_unregister_com_stream( void )
{
    if(active_com_stream != (void*)(0))
    {
        active_com_stream->is_active = 0;
        active_com_stream = (void*)(0);
    }
}


debug_com_stream* debug_get_active_com_stream( void )
{
    return active_com_stream;
}


/**
 * @brief Sends current state of the message stored in active_com_stream if stream is registered and open.
 *
 * @note this function must be implemented in the file responsible for debug interface handling
 */
__attribute__((weak)) void debug_update_com_stream( void )
{

}


