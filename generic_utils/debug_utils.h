//* Documentation is in the end of the file

#pragma once

#ifndef DEBUG_UTILS_H_
#define DEBUG_UTILS_H_

#include <stdint.h>

// Include only if file is available. It can overwrite file configuration definitions
#if defined __has_include
    #if __has_include( <device_definitions.h> )
        #include "device_definitions.h"
    #endif
#endif

/**************************************************************************************************/
/*                                                                                                */
/*                                  Default file configurations                                   */
/*                                                                                                */
/**************************************************************************************************/

#ifndef DEBUG_ERROR_LOG_VERSION
    #define DEBUG_ERROR_LOG_VERSION                 (1U) // The most basic version is used as default one
#endif
#ifndef DEBUG_ERROR_LOG_DEPTH
    #define DEBUG_ERROR_LOG_DEPTH                   (64U)
#endif


#ifndef DEBUG_BUFFER_SIZE
    #define DEBUG_BUFFER_SIZE                       (64U)
#endif
#ifndef DEBUG_MAX_BUFFERS_COUNT
    #define DEBUG_MAX_BUFFERS_COUNT                 (32U)
#endif


// @NOTE In theory we don't need to limit number of fields every entry of the stream has to have as long as this will fit into RAM.
//      But to make things simpler to work with, we introduce a limit. If you need more than 255 different simples types to be sent
//      at the same time as a single data structure, you probably do something where you rather need a custom solution.
#ifndef DEBUG_MAX_STREAM_FIELDS_COUNT
    #define DEBUG_MAX_STREAM_FIELDS_COUNT           (32U) // Must not be bigger than 255U
#endif /* DEBUG_MAX_STREAM_FIELDS_COUNT */

/**************************************************************************************************/
/*                                                                                                */
/*                                       Global definitions                                       */
/*                                                                                                */
/**************************************************************************************************/

// Definition of error code type to improve readability
typedef uint16_t pif_error_code;

// Definitions for quick local optimisation change during debugging. If long term solution is needed use __attribute__ directly!
// These definitions are only guaranteed to work with GCC compiler
#define DEBUG_OPTIMIZATION_O0       __attribute__((optimize("-O0")))
#define DEBUG_OPTIMIZATION_O1       __attribute__((optimize("-O1")))
#define DEBUG_OPTIMIZATION_O2       __attribute__((optimize("-O2")))
#define DEBUG_OPTIMIZATION_O3       __attribute__((optimize("-O3")))
#define DEBUG_OPTIMIZATION_Os       __attribute__((optimize("-Os")))
#define DEBUG_OPTIMIZATION_Og       __attribute__((optimize("-Og")))
#define DEBUG_OPTIMIZATION_Ofast    __attribute__((optimize("-Ofast")))

#define DEBUG_NOINLINE              __attribute__((noinline))
#define DEBUG_NOREORDER             __attribute__((no_reorder))

#define DEBUG_UNUSED                __attribute__((unused))

#ifdef DEBUG_DISABLE_LOGGING
    #define LOG_ERROR(x) (x)
#else
    #define LOG_ERROR(x) debug_log_error(x)
#endif /* DEBUG_DISABLE_LOGGING */

#ifdef DEBUG_DISABLE_ASSERTING
    #define ASSERT_ERROR(condition, code) (code)
#else
    #define ASSERT_ERROR(condition, code) LOG_ERROR((condition)*(code))
#endif /* DEBUG_DISABLE_LOGGING */


/**************************************************************************************************/
/*                                                                                                */
/*                                     Global types declarations                                  */
/*                                                                                                */
/**************************************************************************************************/

typedef struct debug_error_log
{
    pif_error_code error_codes[DEBUG_ERROR_LOG_DEPTH];
} debug_error_log;


typedef struct debug_f32_buffer
{
    float values[DEBUG_BUFFER_SIZE];
    uint16_t next_write_index;
    uint16_t write_delay_access_count;
}debug_f32_buffer;

typedef struct debug_i32_buffer
{
    int32_t values[DEBUG_BUFFER_SIZE];
    uint16_t next_write_index;
    uint16_t write_delay_access_count;
}debug_i32_buffer;

typedef struct debug_u32_buffer
{
    uint32_t values[DEBUG_BUFFER_SIZE];
    uint16_t next_write_index;
    uint16_t write_delay_access_count;
}debug_u32_buffer;

typedef struct debug_i16_buffer
{
    int16_t values[DEBUG_BUFFER_SIZE];
    uint16_t next_write_index;
    uint16_t write_delay_access_count;
}debug_i16_buffer;

typedef struct debug_u16_buffer
{
    uint16_t values[DEBUG_BUFFER_SIZE];
    uint16_t next_write_index;
    uint16_t write_delay_access_count;
}debug_u16_buffer;

typedef struct debug_u8_buffer
{
    uint8_t values[DEBUG_BUFFER_SIZE];
    uint16_t next_write_index;
    uint16_t write_delay_access_count;
}debug_u8_buffer;

typedef enum DEBUG_DATA_TYPE
{
    NO_Type = 0,
    F32_Type = 1,
    I32_Type = 2,
    U32_Type = 3,
    I16_Type = 4,
    U16_Type = 5,
    U8_Type = 6,
}DEBUG_DATA_TYPE;

// Stores all possible buffers
typedef struct debug_com_buffers
{
    void* buffers_values[DEBUG_MAX_BUFFERS_COUNT];
    uint8_t buffers_types[DEBUG_MAX_BUFFERS_COUNT];
    uint8_t next_free_buffer_index;
    uint8_t read_requests_count;
} debug_com_buffers;


/**
 * todo update the strcut comments with proper init semantics we were using somewhere else
 *
 * Example:
 *  We would like to send 5 points (x, y) stored as f32, therefore our entry will be {f32, f32}.
 *  Every message will have 5 entries (to not send messages too often, or because we have 5 different points we want to send at a time).
 *  In this situation all 5 points will be by default saved as 5 separate lines in the log .csv file.
 *  In this case our message will have message_byte_sieze 2*4*5 = 40 bytes (meaning size of 10 floats) as we will not have any alignment issues.
 *  entries_per_message_count = 5, entry_fields_types = {F32_Type, F32_Type}, entry_fields_count = 2.
 *  In this example it is possible to also create a single 10 float entry like (x1, y1, x2, y2, ... x5, y5), but by default it will
 *  be saved as a single line in the .csv file. The appropriate approach should be selected based on the goal.
 */
typedef struct debug_com_stream
{
	void* message;
	uint32_t timeout_ms; // number of ms after which master can consider stream as inactive
	uint16_t message_byte_size; // number of bytes per single stream message (number of bytes per entry * number of entries per message)
	uint16_t entries_per_message_count; // Number of entries that are contained in a single message
	uint8_t id; // Any unique number to identify the stream on the master side
	uint8_t entry_fields_count; // Number of unique basic types(like u8, u16, i32, f32, etc) per message entry
	uint8_t is_active; // bool to show if stream is actively subscribed to. Init to 0
	uint8_t entry_fields_types[DEBUG_MAX_STREAM_FIELDS_COUNT];
} debug_com_stream;

/**************************************************************************************************/
/*                                                                                                */
/*                                  Global functions declarations                                 */
/*                                                                                                */
/**************************************************************************************************/

void debug_shutdown_system_cbk( void );

/*                                   Error log related functions                                  */
/**************************************************************************************************/
pif_error_code debug_log_error( const pif_error_code error_code );
const debug_error_log* const debug_get_error_log_ptr( void );


/*                                 Debug buffers related functions                                */
/**************************************************************************************************/
void debug_add_value_to_f32_buffer( debug_f32_buffer* target_buffer, float value );
void debug_reset_f32_buffer( debug_f32_buffer* target_buffer );

void debug_add_value_to_i32_buffer( debug_i32_buffer* target_buffer, int32_t value );
void debug_reset_i32_buffer( debug_i32_buffer* target_buffer );

void debug_add_value_to_u32_buffer( debug_u32_buffer* target_buffer, uint32_t value );
void debug_reset_u32_buffer( debug_u32_buffer* target_buffer );

void debug_add_value_to_i16_buffer( debug_i16_buffer* target_buffer, int16_t value );
void debug_reset_i16_buffer( debug_i16_buffer* target_buffer );

void debug_add_value_to_u16_buffer( debug_u16_buffer* target_buffer, uint16_t value );
void debug_reset_u16_buffer( debug_u16_buffer* target_buffer );

void debug_add_value_to_u8_buffer( debug_u8_buffer* target_buffer, uint8_t value );
void debug_reset_u8_buffer( debug_u8_buffer* target_buffer );

void debug_register_com_buffer( void* buffer_pointer, DEBUG_DATA_TYPE buffer_type );
void debug_unregister_all_com_buffers( void );
debug_com_buffers* debug_get_com_buffer( void );


/*                                 Debug streams related functions                                */
/**************************************************************************************************/
pif_error_code debug_register_com_stream( debug_com_stream* stream_instance );
void debug_unregister_com_stream( void );

debug_com_stream* debug_get_active_com_stream( void );

void debug_update_com_stream( void );

/**************************************************************************************************/
/*                                                                                                */
/*                                      Static error handing                                      */
/*                                                                                                */
/**************************************************************************************************/
/*                                 Error log related error checkers                               */
/**************************************************************************************************/
#if DEBUG_ERROR_LOG_VERSION == 1

    #if DEBUG_ERROR_LOG_DEPTH > UINT16_MAX / 2
        // This requirement is needed to ensure that a whole error log can be sent using UART with a single DMA request.
        #error "Error log must not take more than UINT16_MAX bytes. Please use smaller value for DEBUG_ERROR_LOG_DEPTH definition."
    #endif /* DEBUG_ERROR_LOG_DEPTH > 0xFFFF */

#endif /* DEBUG_ERROR_LOG_VERSION == 1 */


/*                               Debug buffers related error checkers                             */
/**************************************************************************************************/
#if DEBUG_BUFFER_SIZE > UINT16_MAX / 4
	// This requirement is needed to ensure that a whole buffer can be sent using UART with a single DMA request. The biggest simple buffer is 4 bytes long,
	//	therefore we need to limit buffer size to UINT16_MAX /4.
    #error "Debug buffer size must not take more than UINT16_MAX bytes. Please change smaller DEBUG_BUFFER_SIZE value"
#endif /* DEBUG_BUFFER_SIZE > UINT16_MAX / 4 */

#if DEBUG_MAX_BUFFER_COUNT > 32
    #error "DEBUG_MAX_BUFFER_COUNT must be <= 32. Otherwise it will break debug interface addressing approach. Please select value of DEBUG_MAX_BUFFER_COUNT to be <= 32"
#endif /* DEBUG_MAX_BUFFER_COUNT > 16 */

/*                               Debug streams related error checkers                             */
/**************************************************************************************************/
#if DEBUG_MAX_STREAM_FIELDS_COUNT > 255
    #error "DEBUG_MAX_STREAM_FIELDS_COUNT must be <= 255 due to u8 type"
#endif /* DEBUG_MAX_STREAM_FIELDS_COUNT > 255 */

/**************************************************************************************************/
/*                                                                                                */
/*                          Full Defines template for _pif_derinitions.h                          */
/*                                                                                                */
/**************************************************************************************************/

#if 0
/**************************************************************************************************/
/*                                                                                                */
/*                                      debug_pif definitions                                     */
/*                                                                                                */
/**************************************************************************************************/

#define DEBUG_ERROR_LOG_VERSION                 (1U)
#define DEBUG_ERROR_LOG_DEPTH                   (64U)

#define DEBUG_BUFFER_SIZE                       (64U)

#define DEBUG_MAX_STREAM_FIELDS_COUNT           (32U)

#endif /* 0 */

#endif /* DEBUG_UTILS_H_ */

/**
 *
 * Debug related build flags:
 *  - DEBUG_PIF_DMA_VALIDATION - enables DMA channel intersection checks in all pif files. All found intersections will be added
 *          as errors to the error_log.
 *  - DEBUG_DISABLE_LOGGING - switches all LOG_ERROR calls into the content of these calls. Meaning removes all the error logging overhead without affecting other behaviours.
 *
 */

