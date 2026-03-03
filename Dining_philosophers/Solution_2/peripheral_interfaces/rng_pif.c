#include "rng_pif.h"
#include "debug_pif.h"

void setup_rng(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;

    /* By default, the Q divider is set to 2, which is the same
     * as the R divider used in the setup_system_clock()`
     * implementation when configuring the SYSCLK with the PLL.
     *
     * Consequently, the default frequency for the RNG
     * will be exactly the same as the SYSCLK
     * because both utilize the same M and N multiplication factors and R=Q
     *
     * todo CLK48SEL is used also for USB. USB requires PLLQ to be 48 MHz
     *       (RNG does not). Account for this CLK48SEL/PLLQ constraint
     *       if **USB and RNG** are used together.
     */
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLQEN;
    RCC->CCIPR |= RCC_CCIPR_CLK48SEL_PLLQ << RCC_CCIPR_CLK48SEL_Pos;


    RNG->CR &= ~RNG_CR_CED;     // CED=0: Clock error detection is ON

    // When the CED bit in the RNG_CR register is set to 0,
    // the RNG clock frequency must be higher
    // than AHB clock frequency divided by 32

    RNG->CR |= RNG_CR_RNGEN;    // turn on RNG peripheral

    if(RNG->SR & RNG_SR_CECS)
    {
        LOG_ERROR(1337);
    }
}


uint32_t rng_get_value(void)
{
    if(RNG->SR & RNG_SR_SECS)
    {
        LOG_ERROR(1338);

        RNG->SR &= ~RNG_SR_SEIS;

        // When a noise source (or seed) error occurs you should clear
        // SEIS bit and read out 12 words from the RNG_DR register,
        // and discard each of them in order to clean the pipeline.
        for(uint8_t discarded_word = 0; discarded_word < 12; discarded_word+=1)
        {
            // Waiting for the data ready bit to be set
            while((RNG->SR & RNG_SR_DRDY) == 0){}

            // Reading and discarding the word.
            (void)RNG->DR;
        }

        // Confirm that SEIS is still cleared.
        if(RNG->SR & RNG_SR_SEIS)
        {
            LOG_ERROR(1339);
            return 0;
        }
    }

    // Waiting for the data ready bit to be set
    while( (RNG->SR & RNG_SR_DRDY) == 0){}

    uint32_t rng_returned_value = RNG->DR;

    // It is recommended to always verify that RNG_DR is different from zero
    if(rng_returned_value == 0)
    {
        LOG_ERROR(1340);
    }

    return rng_returned_value;
}
