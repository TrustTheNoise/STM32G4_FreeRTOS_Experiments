#include <utils_pif.h>

/**
 * extern indicates that the function or variable is defined elsewhere.
 * This means the linker will search for it in other files.
 * This function is implemented in assembly and is intended
 * to create a delay in program execution.
 *
 * @param iterations - the number of cycles required for a delay in milliseconds.
 */
extern void while_delay_asm(uint32_t iterations);

// this is number of cycles that needed to complete one loop iteration in the while_delay_asm function
#define CYCLES_FOR_WHILE_DELAY               5


/**
 * @brief A delay that is based on the execution time of a loop in the processor.
 *
 * This delay is not consistent! It will do precision delays only if there are no interrupts during counting. Every interrupt takes time from the
 * Delay counter and therefore delay is prolonged for the amount of time that interrupts take from it. Though this delay doesn't need
 * any specific peripheral and therefore can be handy. The smaller the needed delay time, the more precise it will likely be.
 * Checks before delay loop take 8-18 cycles(depends on optimisation flags).
 * Final delay duration is calculated as: 8-18 cycles + delay_duration_us + all time taken by interrupts during counting
 *
 * @note The maximum delay time depends on the processor frequency. There is a check in the function for exceeding the maximum delay.
 */
void dummy_delay_us(const uint32_t delay_duration_us)
{
    // Check for exceeding the maximum delay value. Sequence of operations
    // in maximum delay formula is important because otherwise, overflow will occur.
	if (delay_duration_us > (UINT32_MAX / CLOCK_CYCLES_PER_US * CYCLES_FOR_WHILE_DELAY) )
	{
		// todo add error to showcase that user asked for too much time in the delay
		return;
	}

	// Sequence of operations in maximum delay formula is important because otherwise, overflow will occur.
	const uint32_t counts_for_delay = (delay_duration_us / CYCLES_FOR_WHILE_DELAY) * CLOCK_CYCLES_PER_US;

	while_delay_asm(counts_for_delay);
}


void setup_memcopy_dma( void )
{
#ifdef DEBUG_PIF_DMA_VALIDATION
    if (DMA2_Channel6->CPAR != 0)
    {
        LOG_ERROR(1126); // [#manual] Error + dma_number * 10 + channel_number
    }
#endif

    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    DMA2_Channel6->CCR |= DMA_CCR_MEM2MEM | DMA_CCR_MINC | DMA_CCR_PINC;
}


void memcpy_dma(const void* dest, const void* src, const uint16_t size_bytes,
        const data_stride_bytes stride_bytes)
{
    DMA2_Channel6->CCR &= ~DMA_CCR_EN;

    DMA2_Channel6->CCR |= stride_bytes << DMA_CCR_MSIZE_Pos | stride_bytes << DMA_CCR_PSIZE_Pos;
    DMA2_Channel6->CNDTR = size_bytes / (1U << stride_bytes);

    DMA2_Channel6->CMAR = (uint32_t)dest;
    DMA2_Channel6->CPAR = (uint32_t)src;

    DMA2_Channel6->CCR |= DMA_CCR_EN;

    // waiting for DMA to finish
    while( (DMA2->ISR & DMA_ISR_TCIF6) == 0) {}

    // clear DMA transfer flag because otherwise "while flag checking"
    // will be skipped (transfer complete flag remains set)
    DMA2->IFCR |= DMA_IFCR_CTCIF6;
}
