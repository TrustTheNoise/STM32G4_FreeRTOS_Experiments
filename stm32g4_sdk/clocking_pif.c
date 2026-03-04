#include "clocking_pif.h"

/**************************************************************************************************/
/*                                                                                                */
/*                                  Static functions declarations                                 */
/*                                                                                                */
/**************************************************************************************************/

static uint16_t setup_hsi_as_sysclk_source();
static uint16_t setup_hse_as_sysclk_source();

static uint16_t setup_pll_as_sysclk_source(uint32_t clock_source);

/****************************************************************************************/
/*                                                                                      */
/*                            Static variables declarations                             */
/*                                                                                      */
/****************************************************************************************/

uint32_t seconds_from_boot = 0;

/****************************************************************************************/
/*                                                                                      */
/*                           Global functions implementation                            */
/*                                                                                      */
/****************************************************************************************/

#if SYSTEM_CLOCK_SOURCE == HSI

uint16_t setup_system_clock( void )
{
	return setup_hsi_as_sysclk_source();
}

#elif SYSTEM_CLOCK_SOURCE == HSE

uint16_t setup_system_clock( void )
{
	return setup_hse_as_sysclk_source();
}

#elif SYSTEM_CLOCK_SOURCE == PLL_HSI || SYSTEM_CLOCK_SOURCE == PLL_HSE

uint16_t setup_system_clock( void )
{
	return setup_pll_as_sysclk_source(SYSTEM_CLOCK_SOURCE);
}

#else
	#error "SYSTEM_CLOCK_SOURCE must be defined in the _pif_definitions.h file and must contain one of the options: HSI, HSE, PLL_HSI, PLL_HSE"
#endif // #if SYSTEM_CLOCK_SOURCE == HSI



#ifdef SYSTICK_INTERRUPT_FREQUENCY_HZ
void setup_system_timer( void )
{
    // Set system timer reload register.
    SysTick->LOAD = ((SYSTEM_CLOCK_FREQUENCY_HZ) / (8 * SYSTICK_INTERRUPT_FREQUENCY_HZ)) - 1;

    // Reset counter value to 0 before start.
    SysTick->VAL = 0;

    // Enable interrupt.
    NVIC_EnableIRQ( SysTick_IRQn );

    // Start SysTick timer with default prescaler of 8 and enable interrupt.
    SysTick->CTRL |= 1 << SysTick_CTRL_ENABLE_Pos | 1 << SysTick_CTRL_TICKINT_Pos;
}
#endif // SYSTICK_INTERRUPT_FREQUENCY_HZ

//uint16_t setup_timer_timer( void )
//{
//#ifdef ENABLE_MICROSECONDS_TIMER
//	if (TIME_SROURCE_TIMER_NAME != TIM2 && TIME_SROURCE_TIMER_NAME != TIM5)
//	{
//		LOG_ERROR(WRONG_MICROSECONDS_TIMER_Error);
//		return WRONG_MICROSECONDS_TIMER_Error;
//	}
//#endif /* ENABLE_MICROSECONDS_TIMER */
//
//	return 0;
//}


uint32_t get_milliseconds_from_boot( void )
{
	return 0;
}

void test_delay( int32_t duration )
{
	__NOP();
}

/****************************************************************************************/
/*                                                                                      */
/*                           Static functions implementation                            */
/*                                                                                      */
/****************************************************************************************/

/*!
 * Enables HSI.
 * Chooses HSI as the main clock source.
 * Waits for it to start.
 * Disables HSE and PLL
 */
__attribute__((unused)) uint16_t setup_hsi_as_sysclk_source()
{
	if((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_HSI) // HSI is already a clock source
	{
		LOG_ERROR(HSI_RESTART_WHILE_RUNNING_Error);
		return HSI_RESTART_WHILE_RUNNING_Error;
	}

	RCC->CR |= RCC_CR_HSION; // Even if it is already enabled, enabling takes the same amount of time as checking for being enabled.
	while((RCC->CR & RCC_CR_HSIRDY) == 0){} // It can't stall, therefore it doesn't need timeout. If it stall MCU is probably half dead anyways

	uint32_t rcc_cfgr_masked_value = RCC->CFGR & ~RCC_CFGR_SW_Msk;
	RCC->CFGR = rcc_cfgr_masked_value | RCC_CFGR_SW_HSI;

	RCC->CR &= ~(RCC_CR_HSEON & RCC_CR_PLLON); 	// Disable HSE and PLL because they aren't needed.
												// Maybe changed in the future if some peripherals will use those directly (without being system clock source)
	return 0;
}

/*!
 * Enables HSE if disabled.
 * Enables clock protection system and interrupt
 * Chooses HSE as the main clock source.
 * Waits for it to start.
 * Disables HSI and PLL
 */
__attribute__((unused)) uint16_t setup_hse_as_sysclk_source()
{
	if((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_HSE) // HSE is already a clock source
	{
		LOG_ERROR(HSE_RESTART_WHILE_RUNNING_Error);
		return HSE_RESTART_WHILE_RUNNING_Error;
	}

	RCC->CR |= RCC_CR_HSEON;

	uint32_t timeout = 40000; // todo implement a smarter way to timeout
	while((RCC->CR & RCC_CR_HSERDY) == 0 && timeout > 0)
	{
		timeout -= 1;
	}
	if(timeout == 0 )
	{
		RCC->CR &= ~RCC_CR_HSEON;
		return HSE_FAILED_TO_START_Error;
	}

	// todo enable clock security system (maybe?) Don't think it will be helpful in these particular circumstance

	uint32_t rcc_cfgr_masked_value = RCC->CFGR & ~RCC_CFGR_SW_Msk;
	RCC->CFGR = rcc_cfgr_masked_value | RCC_CFGR_SW_HSE;

	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE){}

	RCC->CR &= ~(RCC_CR_PLLON | RCC_CR_HSION); 	// Disable PLL and HSI because they aren't needed.
												// Maybe changed in the future if some peripherals will use those directly (without being system clock source)

	return 0;
}


__attribute__((unused)) uint16_t setup_pll_as_sysclk_source(uint32_t clock_source)
{
	if((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_PLL) // PLL is already a clock source
	{
		LOG_ERROR(PLL_RESTART_WHILE_RUNNING_Error);
		return PLL_RESTART_WHILE_RUNNING_Error;
	}

	uint32_t masked_flash_acr = FLASH->ACR & ~FLASH_ACR_LATENCY_Msk;
	FLASH->ACR = masked_flash_acr | DESIRED_FLASH_LATENCY;

	if (clock_source == PLL_HSE)
	{
		RCC->CR |= RCC_CR_HSEON;

		uint32_t timeout = 40000; // todo implement a smarter way to timeout
		while((RCC->CR & RCC_CR_HSERDY) == 0 && timeout > 0)
		{
			timeout -= 1;
		}
		if(timeout == 0 )
		{
			setup_pll_as_sysclk_source(PLL_HSI);
			LOG_ERROR(HSE_FAILED_TO_START_Error);
			return HSE_FAILED_TO_START_Error;
		}

        RCC->CR |= RCC_CR_CSSON; // Enable clock security system

		uint32_t new_rcc_pll_cfgr = RCC_PLLCFGR_PLLREN
							| PLL_R_COEFFICIENT << RCC_PLLCFGR_PLLR_Pos
							| PLL_N_COEFFICIENT << RCC_PLLCFGR_PLLN_Pos
							| HSE_PLL_M_COEFICIENT << RCC_PLLCFGR_PLLM_Pos
							| RCC_PLLCFGR_PLLSRC_HSE;

		RCC->PLLCFGR = new_rcc_pll_cfgr;
	}
	else // clock_source = PLL_HSI even if the actual value is wrong
	{
		uint32_t new_rcc_pll_cfgr = RCC_PLLCFGR_PLLREN
							| PLL_R_COEFFICIENT << RCC_PLLCFGR_PLLR_Pos
							| PLL_N_COEFFICIENT << RCC_PLLCFGR_PLLN_Pos
							| HSI_PLL_M_COEFICIENT << RCC_PLLCFGR_PLLM_Pos
							| RCC_PLLCFGR_PLLSRC_HSI;

		RCC->PLLCFGR = new_rcc_pll_cfgr;
	}

	RCC->CR |= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) == 0) {}

	uint32_t masked_rcc_pll_cfgr = RCC->CFGR & ~RCC_CFGR_SW_Msk;
	RCC->CFGR = masked_rcc_pll_cfgr | RCC_CFGR_SW_PLL;
	while ( ( RCC->CFGR & RCC_CFGR_SWS_PLL ) != RCC_CFGR_SWS_PLL ){}

	return 0;
}

/*!
 * Non-maskable interrupt handler.
 * todo, move to another location as it should contain way more than only Clock error
 */
void NMI_Handler()
{
	if(RCC->CIFR & RCC_CIFR_CSSF) // Clock security interrupt. Called, when HSE fails
	{
		// Clear the clock security system interrupt flag.
		RCC->CICR |= RCC_CICR_CSSC;

		(void)LOG_ERROR(CLOCK_CECURITY_Error);

		// Wait until PLL is fully stopped.
		while ( (RCC->CR & RCC_CR_PLLRDY) == RCC_CR_PLLRDY ){}

		setup_pll_as_sysclk_source(PLL_HSI);
	}
}

