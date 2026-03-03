#include "dac_pif.h"

/****************************************************************************************/
/*                                                                                      */
/*                            Peripheral definitions                                    */
/*                                                                                      */
/****************************************************************************************/

/*!
 * This code uses:
 *
 * GPIO:
 *   PA4 - DAC1 AO1
 *
 */

static DAC_TypeDef* const EXAMPLE_DAC = DAC1;
#define DAC_CLOCK_ENABLE                RCC->AHB2ENR |= RCC_AHB2ENR_DAC1EN;


/****************************************************************************************/
/*                                                                                      */
/*                            Static functions declarations                             */
/*                                                                                      */
/****************************************************************************************/

static inline void setup_dac_gpio( void );
static inline void setup_dac_peripheral( void );

/****************************************************************************************/
/*                                                                                      */
/*                           Global functions implementation                            */
/*                                                                                      */
/****************************************************************************************/

void setup_dac( void )
{
    setup_dac_gpio();
    setup_dac_peripheral();
}

void dac_set_channel_1_value(uint32_t value)
{
    EXAMPLE_DAC->DHR12R1 = value;
}

/****************************************************************************************/
/*                                                                                      */
/*                           Static functions implementation                            */
/*                                                                                      */
/****************************************************************************************/

static inline void setup_dac_gpio( void )
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    const uint32_t gpio_a_moder_clear_mask = ~(GPIO_MODER_Msk << GPIO_MODER_MODE4_Pos);
    const uint32_t gpio_a_moder_set_mask = (GPIO_ANALOG_IN_Mode << GPIO_MODER_MODE4_Pos);
    GPIOA->MODER = (GPIOA->MODER & gpio_a_moder_clear_mask) | gpio_a_moder_set_mask;
}

static inline void setup_dac_peripheral( void )
{
    DAC_CLOCK_ENABLE

        // Decrease DAC frequency if system clock is too fast - recommendation from RM.
#if SYSTEM_CLOCK_FREQUENCY_HZ > 160000000
        EXAMPLE_DAC->MCR |= 2 << DAC_MCR_HFSEL_Pos;
#elif SYSTEM_CLOCK_FREQUENCY_HZ > 80000000
        EXAMPLE_DAC->MCR |= 1 << DAC_MCR_HFSEL_Pos;
#endif /* SYSTEM_CLOCK_FREQUENCY_HZ > 80000000 */

    EXAMPLE_DAC->CR |= DAC_CR_EN1;
    //! todo Add timeout error
    while((EXAMPLE_DAC->SR & DAC_SR_DAC1RDY) == 0 ); // wait until both channels are on
}
