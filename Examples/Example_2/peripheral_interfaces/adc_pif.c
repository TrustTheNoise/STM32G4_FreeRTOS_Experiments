#include "adc_pif.h"
/**************************************************************************************************/
/*                                                                                                */
/*                                     Peripheral definitions                                     */
/*                                                                                                */
/**************************************************************************************************/

/*!
 * This code uses:
 *
 * GPIO:
 *  PC0 - ADC1 IN6
 *
 * ADC:
 *  ADC1
 *  -Ch6
 *
 * TIM:
 *  TIM2
 */

static ADC_TypeDef* EXAMPLE_ADC = ADC1;
#define ADC_CLOCK_SOURSE_SELECTION          RCC->CCIPR |= RCC_CCIPR_ADCxSEL_SYSCLOCK << RCC_CCIPR_ADC12SEL_Pos;
#define ADC_CLOCK_ENABLE                    RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;

/**************************************************************************************************/
/*                                                                                                */
/*                                 Static functions declarations                                  */
/*                                                                                                */
/**************************************************************************************************/

static inline void setup_adc_gpio( void );
static inline void setup_adc_peripheral( void );
static inline void setup_adc_tim_trigger( void );
static inline void adc_wake_up_from_power_down( void );

/**************************************************************************************************/
/*                                                                                                */
/*                                Global functions implementation                                 */
/*                                                                                                */
/**************************************************************************************************/

void setup_adc()
{
    setup_adc_gpio();
    setup_adc_peripheral();
    setup_adc_tim_trigger();
}

inline void adc_start_injected_sampling( void )
{
    EXAMPLE_ADC->CR |= ADC_CR_JADSTART;
}

inline void adc_wait_until_injected_sampling_is_finished( void )
{
    while(EXAMPLE_ADC->CR & ADC_CR_JADSTART);
}

/*!
 * This functions return the address of JDR1, allowing us to use a pointer to JDR1
 * as an array, so that jdr_pointer[0] = JDR1, ..., jdr_pointer[3] = JDR4
 * Of course, in this way we have access to all the memory. But if we use a pointer
 * to constant data, then we won't be able to write anything to the registers,
 * so it will work for our read-only purposes.
 */
uint32_t* adc_get_injected_measurements()
{
    return (uint32_t*)(&EXAMPLE_ADC->JDR1);
}

/**************************************************************************************************/
/*                                                                                                */
/*                                Static functions implementation                                 */
/*                                                                                                */
/**************************************************************************************************/
static inline void adc_wake_up_from_power_down( void )
{
    EXAMPLE_ADC->CR &= ~ADC_CR_DEEPPWD_Msk;
    EXAMPLE_ADC->CR |= ADC_CR_ADVREGEN;

    dummy_delay_us(20); // 20us for ADC to wake up
}


static inline void setup_adc_gpio( void )
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    const uint32_t gpio_c_moder_clear_mask = ~(GPIO_MODER_MODE0_Msk | GPIO_MODER_MODE1_Msk);
    const uint32_t gpio_c_moder_set_mask = (GPIO_ANALOG_IN_Mode << GPIO_MODER_MODE0_Pos
                                            | GPIO_ANALOG_IN_Mode << GPIO_MODER_MODE1_Pos);
    GPIOC->MODER = (GPIOC->MODER & gpio_c_moder_clear_mask) | gpio_c_moder_set_mask;

}

static inline void setup_adc_peripheral( void )
{
    // ** ADC timing setup ** //
    ADC_CLOCK_SOURSE_SELECTION

    ADC_CLOCK_ENABLE

    // @todo add line to choose ADC prescaler based on the desired ADC frequency

    // ** Wake up from power down ** //
    adc_wake_up_from_power_down();

    // ** Calibrate ADC ** //
    EXAMPLE_ADC->CR |= ADC_CR_ADCAL; // Calibrate single ended channels
    while(EXAMPLE_ADC->CR & ADC_CR_ADCAL); // Wait for the end of calibration

    // ** Set up ADC ** //
    // Channel selection
    EXAMPLE_ADC->JSQR =  6 << ADC_JSQR_JSQ1_Pos
               | ADC_JSQR_JL_1_CONVERSION << ADC_JSQR_JL_Pos;

    // Sample time selection
    EXAMPLE_ADC->SMPR1 = ADC_SMPR_6p5_CC << ADC_SMPR1_SMP6_Pos; // Change this setting to check how long the measurement delay will be


    EXAMPLE_ADC->CFGR |= ADC_CFGR_RES_12_BIT << ADC_CFGR_RES_Pos;      // Set up ADC resolution
//    EXAMPLE_ADC->CFGR2 = ADC_CFGR2_OVSR_16x << ADC_CFGR2_OVSR_Pos      // Number of samples for oversampling
//                | ADC_CFGR2_OVSS_2_BIT << ADC_CFGR2_OVSS_Pos    // Number of shifts for oversampling
//                | ADC_CFGR2_JOVSE;  // Enable oversampling for injected channels

    // ** Set up ADC interrupt ** //
    EXAMPLE_ADC->IER |= ADC_IER_JEOSIE;
    NVIC_SetPriority(ADC1_2_IRQn, 6);
    NVIC_EnableIRQ(ADC1_2_IRQn);

    // ** Start up ADC ** //
    EXAMPLE_ADC->CR |= ADC_CR_ADEN;
    while ((EXAMPLE_ADC->ISR & ADC_ISR_ADRDY) == 0); // Wait until ADC is started
    EXAMPLE_ADC->ISR = ADC_ISR_ADRDY; // Clear the ready flag for the future
}

static inline void setup_adc_tim_trigger( void )
{
    // Trigger input setup
    EXAMPLE_ADC->JSQR |= ADC_JSQR_JEXTEN_HW_TRIG_R << ADC_JSQR_JEXTEN_Pos
                | ADC_JSQR_JEXTSEL_ADC12_TIM2_TRGO << ADC_JSQR_JEXTSEL_Pos;

    // ** TIM2 setup ** //
    // Enable timers clocking.
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    // Set up prescaler and reload to count exactly 10 pulses every second and overflow every second.
    TIM2->PSC = (uint16_t)((SYSTEM_CLOCK_FREQUENCY_HZ / 1000) - 1);
    TIM2->ARR = 100 - 1;

    TIM2->CR2 = TIM234_CR2_MMS_Update << TIM_CR2_MMS_Pos; // Trigger output (for ADC) on update event

    // Generate update event to enable new register values and start timer.
    TIM2->EGR |= TIM_EGR_UG;
    TIM2->CR1 |= TIM_CR1_CEN;
}
