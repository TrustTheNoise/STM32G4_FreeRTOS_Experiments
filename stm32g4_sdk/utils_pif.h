// Description is in the end of the file
#pragma once

#ifndef UTILS_PIF_H_
#define UTILS_PIF_H_

#include "device_definitions.h"
#include "device_mcu_includes.h"

/**************************************************************************************************/
/*                                                                                                */
/*                                       Global definitions                                       */
/*                                                                                                */
/**************************************************************************************************/

#define SEC_TO_USEC(x)           ((uint32_t)(x*1e6))
#define MSEC_TO_USEC(x)          ((uint32_t)(x*1e3))

#define CLOCK_CYCLES_PER_US                     (SYSTEM_CLOCK_FREQUENCY_HZ / 1000000U)


// In MEMCPY_OPTIMIZED, interface of memcpy function from string.h library is used.
//  Therefore, size_bytes should contain the size of the array in bytes, while stride_bytes
//  should store a value from enum data_stride_bytes, which is only needed for memcopy_dma.
#if USE_DMA_MEMCPY
    #define MEMCPY_OPTIMISED(dest_ptr, src_ptr, size_bytes, stride_bytes)    memcpy_dma(dest_ptr, src_ptr, size_bytes, stride_bytes)
#else
    #include "string.h"
    #define MEMCPY_OPTIMISED(dest_ptr, src_ptr, size_bytes, stride_bytes)    memcpy(dest_ptr, src_ptr, size_bytes)
#endif


/**************************************************************************************************/
/*                                                                                                */
/*                                     Global types declarations                                  */
/*                                                                                                */
/**************************************************************************************************/

typedef enum {
    MEM_DATA_STRIDE_1_BYTE = DMA_CCR_MSIZE_8_BITS,
    MEM_DATA_STRIDE_2_BYTES = DMA_CCR_MSIZE_16_BITS,
    MEM_DATA_STRIDE_4_BYTES = DMA_CCR_MSIZE_32_BITS,
}data_stride_bytes;


/**************************************************************************************************/
/*                                                                                                */
/*                                  Global functions declarations                                 */
/*                                                                                                */
/**************************************************************************************************/

void dummy_delay_us(const uint32_t delay_duration_us);

static inline void enable_fpu(void)
{
    SCB->CPACR |= 0b11 << 20 | 0b11 << 22; // Enables CP11 and CP10 of FPU
}
void setup_memcopy_dma( void );
void memcpy_dma(const void* dest, const void* src, const uint16_t size_bytes,
        const data_stride_bytes stride_bytes);

/**************************************************************************************************/
/*                                                                                                */
/*                                      Static error handing                                      */
/*                                                                                                */
/**************************************************************************************************/

// Overall, we are not sure if it is possible to set the processor frequency below 5 MHz.
#if SYSTEM_CLOCK_FREQUENCY_HZ < 5000000
    #error "The delay in dummy_delay will not work correctly! Please increase SYSTEM_CLOCK_FREQUENCY_HZ in the _pif_definitions.h file."
#endif


#endif /* UTILS_PIF_H_ */

/**
 * Function specific documentation can be found in .c file.
 */
