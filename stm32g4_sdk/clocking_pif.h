//! File description is in the end of the file

#pragma once

#ifndef CLOCKING_PIF_H_
#define CLOCKING_PIF_H_

#include "device_definitions.h"
#include "device_mcu_includes.h"

#include "debug_utils.h"

/****************************************************************************************/
/*                                                                                      */
/*                            Clocking features definitions                             */
/*                                                                                      */
/****************************************************************************************/

// todo I don't think I like this approach much, or at all. It is possible to make a mistake and forget to define ENABLE_PIF_CLOCKING_DEFINITIONS
//          while defining some of the parameters below and this will lead to redefinition
//! To redefine constant below ENABLE_PIF_CLOCKING_DEFINITIONS definition must be added to the file, where redefinition will happen.
#ifndef ENABLE_PIF_CLOCKING_DEFINITIONS
#define HSE_CLOCK_FREQUENCY_HZ					(24000000U) //! Device specific definition. 24MHz is default for NUCLEO boards
#define SYSTEM_CLOCK_SOURCE						PLL_HSE //! Possible options: HSI, HSE, PLL_HSI, PLL_HSE
#define SYSTEM_CLOCK_FREQUENCY_HZ				(60000000U) //! System clock frequency we want to achieve after clock setup

#define CLOCK_CYCLES_PER_US						(SYSTEM_CLOCK_FREQUENCY_HZ / 1000000U)

#define SYSTICK_INTERRUPT_FREQUENCY_HZ			10 //! Frequency of SysTick_Handler() interrupt call
#endif /* ENABLE_PIF_DEBUG_DEFINITIONS */


/****************************************************************************************/
/*                                                                                      */
/*                             Global functions definitions                             */
/*                                                                                      */
/****************************************************************************************/

//! Sets up system clock with parameters given by "Clocking features definitions"
uint16_t setup_system_clock( void );

uint8_t handle_clocking_NMI_interrupt( void );

//! This function is available only if SYSTICK_INTERRUPT_FREQUENCY_HZ is defined in the interfacing.h. Otherwise system timer is not set up by this file and can be used
#ifdef SYSTICK_INTERRUPT_FREQUENCY_HZ

void setup_system_timer( void );

#endif // SYSTICK_INTERRUPT_FREQUENCY_HZ

/****************************************************************************************/
/*                                                                                      */
/*                           Clock setup related error codes                            */
/*                                                                                      */
/****************************************************************************************/

#ifndef CLOCK_ERRORS_OFFSET
	#define CLOCK_ERRORS_OFFSET					(10U)
#endif

#define HSI_RESTART_WHILE_RUNNING_Error			(1U) + CLOCK_ERRORS_OFFSET

#define HSE_RESTART_WHILE_RUNNING_Error			(3U) + CLOCK_ERRORS_OFFSET
#define HSE_FAILED_TO_START_Error				(4U) + CLOCK_ERRORS_OFFSET

#define PLL_RESTART_WHILE_RUNNING_Error			(7U) + CLOCK_ERRORS_OFFSET

#define CLOCK_CECURITY_Error					(9U) + CLOCK_ERRORS_OFFSET

#define WRONG_MICROSECONDS_TIMER_Error			(11U) + CLOCK_ERRORS_OFFSET

/****************************************************************************************/
/*                                                                                      */
/*                                 Constants definitions                                */
/*                                                                                      */
/****************************************************************************************/

//! It would be nice, if all these definitions were not here and couldn't be seen by the including file, but not sure how to implement it at the moment.

#define HSI 		1
#define HSE			2
#define PLL_HSI		3
#define PLL_HSE		4

/*!
 * The main idea behind simple clock automation functionality is to convert input frequency
 * to 4 MHz, and then use it as a base for further calculations.
 */

//! Clock prescaler values
#define HSE_PLL_M_COEFICIENT		((HSE_CLOCK_FREQUENCY_HZ / 4000000) - 1)
#define HSI_PLL_M_COEFICIENT		(3U) // HSI is always 16

//! PLL R and N coefficient selection based on the frequency range
#if (SYSTEM_CLOCK_FREQUENCY_HZ <= 24000000)
	#define PLL_R_COEFFICIENT		3 // Divide by 8. Gives setup step of 500 KHz
	#define PLL_N_COEFFICIENT		(SYSTEM_CLOCK_FREQUENCY_HZ*8/4000000)

	#if (SYSTEM_CLOCK_FREQUENCY_HZ / 500000) * 500000 != SYSTEM_CLOCK_FREQUENCY_HZ
		#error "System frequency below 24Mhz must be a multiple of 500 KHz(500000). Please change the SYSTEM_CLOCK_FREQUENCY_HZ value in interfacing.h"
	#endif

#elif (SYSTEM_CLOCK_FREQUENCY_HZ <= 48000000)
	#define PLL_R_COEFFICIENT		1 // Divide by 4. Gives setup step of 1 Mhz
	#define PLL_N_COEFFICIENT		(SYSTEM_CLOCK_FREQUENCY_HZ*4/4000000)

	#if (SYSTEM_CLOCK_FREQUENCY_HZ / 1000000) * 1000000 != SYSTEM_CLOCK_FREQUENCY_HZ
		#error "System frequency between 24MHz and 48MHz must be a multiple of 1 Mhz(1000000). Please change the SYSTEM_CLOCK_FREQUENCY_HZ value in interfacing.h"
	#endif

#else
	#define PLL_R_COEFFICIENT		0 // Divide by 2. Gives setup step of 2 MHz
	#define PLL_N_COEFFICIENT		(SYSTEM_CLOCK_FREQUENCY_HZ*2/4000000)

	#if (SYSTEM_CLOCK_FREQUENCY_HZ / 2000000) * 2000000 != SYSTEM_CLOCK_FREQUENCY_HZ
		#error "System frequency higher than 48 MHz must be a multiple of 2 Mhz(2000000). Please change the SYSTEM_CLOCK_FREQUENCY_HZ value in interfacing.h"
	#endif
#endif

//! Flash wait states
/*! Depending on the used SYSTEM_MAIN_FREQUENCY we need to change flash access latency. See RM chapter 3.3.3. */
#if (SYSTEM_CLOCK_FREQUENCY_HZ > 120000000)
	#define DESIRED_FLASH_LATENCY				FLASH_ACR_LATENCY_4WS
#elif (SYSTEM_CLOCK_FREQUENCY_HZ > 90000000)
	#define DESIRED_FLASH_LATENCY				FLASH_ACR_LATENCY_3WS
#elif (SYSTEM_CLOCK_FREQUENCY_HZ > 60000000)
	#define DESIRED_FLASH_LATENCY				FLASH_ACR_LATENCY_2WS
#elif (SYSTEM_CLOCK_FREQUENCY_HZ > 30000000)
	#define DESIRED_FLASH_LATENCY				FLASH_ACR_LATENCY_1WS
#else
	#define DESIRED_FLASH_LATENCY				FLASH_ACR_LATENCY_0WS
#endif

/****************************************************************************************/
/*                                                                                      */
/*                               Definitions error checking                             */
/*                                                                                      */
/****************************************************************************************/

#if SYSTEM_CLOCK_FREQUENCY_HZ > 150000000
	#error "STM32G4 can't run on frequency higher than 150 MHz. Please lower the SYSTEM_CLOCK_FREQUENCY_HZ value in the interfacing.h file"
#endif

#if ((HSE_CLOCK_FREQUENCY_HZ / 4000000) * 4000000 != HSE_CLOCK_FREQUENCY_HZ)
	#error "This implementation only works with HSE frequencies multiple of 4 MHz."
#endif


#if (SYSTEM_CLOCK_SOURCE == PLL_HSI || SYSTEM_CLOCK_SOURCE == PLL_HSE)
	// PLLN output can't be lower than 96 MHz
	#if (4000000 * PLL_N_COEFFICIENT < 96000000)
		#error "Desired system frequency is too small to be achieved using given clock source. Please increase SYSTEM_CLOCK_FREQUENCY_HZ in the interfacing.h file or change the clock source"
	#endif /* #if (4000000 * PLL_N_COEFFICIENT < 96000000) */

	// PLLN output can't be higher than 344 MHz
	#if (4000000 * PLL_N_COEFFICIENT > 344000000)
		#error "Desired system frequency is too high to be achieved using given clock source. Please decrease SYSTEM_CLOCK_FREQUENCY_HZ in the interfacing.h file or change the clock source"
	#endif // #if (4000000 * PLL_N_COEFFICIENT > 344000000)
#endif /* #if (SYSTEM_CLOCK_SOURCE == PLL_HSI || SYSTEM_CLOCK_SOURCE == PLL_HSE) */

#if (SYSTEM_CLOCK_SOURCE == HSI && SYSTEM_CLOCK_FREQUENCY_HZ != 16000000)
	#error "System frequency must be equal to 16 MHz when using HSI as a clock source. Please change SYSTEM_CLOCK_FREQUENCY_HZ to 16000000 in the interfacing.h file"
#endif /* (SYSTEM_CLOCK_SOURCE == HSI && SYSTEM_CLOCK_FREQUENCY_HZ != 16000000) */

#if (SYSTEM_CLOCK_SOURCE == HSE && SYSTEM_CLOCK_FREQUENCY_HZ != HSE_CLOCK_FREQUENCY_HZ)
	#error "System frequency must be equal to HSE clock frequency when using HSE as a clock source. Please change SYSTEM_CLOCK_FREQUENCY_HZ in the interfacing_definitions.h file"
#endif /* SYSTEM_CLOCK_SOURCE == HSE && SYSTEM_CLOCK_FREQUENCY_HZ != HSE_CLOCK_FREQUENCY_HZ */

#ifndef SYSTEM_CLOCK_SOURCE
#error "SYSTEM_CLOCK_SOURCE must be defined with one of these options: HSI, HSE, PLL_HSI, PLL_HSE"
#endif

#ifndef SYSTEM_CLOCK_FREQUENCY_HZ
#error "SYSTEM_CLOCK_FREQUENCY_HZ must be defined and must contain desired system frequency in hertz"
#endif

#if (SYSTEM_CLOCK_SOURCE == HSE || SYSTEM_CLOCK_SOURCE == PLL_HSE)

#ifndef HSE_CLOCK_FREQUENCY_HZ
#error "HSE_CLOCK_FREQUENCY_HZ must be defined and should contain HSE frequency in hertz"
#endif /* #ifndef HSE_CLOCK_FREQUENCY_HZ */

#endif /* #if (SYSTEM_CLOCK_SOURCE == HSE || SYSTEM_CLOCK_SOURCE == PLL_HSE) */

#ifdef SYSTICK_INTERRUPT_FREQUENCY_HZ
#if (((SYSTEM_CLOCK_FREQUENCY_HZ) / (8 * SYSTICK_INTERRUPT_FREQUENCY_HZ)) - 1) > 0x00FFFFFF
#error "SysTick timer can't support such low system interrupt frequency. Increase the value of the SYSTICK_INTERRUPT_FREQUENCY_HZ or lower the system frequency by lowering SYSTEM_CLOCK_FREQUENCY_HZ"
#endif
#endif // SYSTICK_INTERRUPT_FREQUENCY_HZ


#endif /* CLOCKING_PIF_H_ */

/*!
 * Current implementation supports HSI, HSE, PLL from HSI and PLL from HSE clock sources with frequency range up to 150MHz.
 * Clock setup coefficient are selected automatically depending on the desired frequency and support 3 precision ranges:
 *	- up to 24Mhz frequency can be setup with 0.5 MHz precision;
 * 	- between 24 MHz and 48 MHz frequency can be set up with 1 MHz precision;
 * 	- between 48 MHz and 150 MHz frequency can be set up with 2 MHz precision.
 *
 * This implementation currently doesn't support any other clock sources and coefficients.
 *
 * @note This implementation assumes that all other prescalers before periphery are equal to 1 and therefore everything
 * 	can be expressed through SYSTEM_CLOCK_FREQUENCY_HZ.
 */
