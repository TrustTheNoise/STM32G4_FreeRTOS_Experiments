#pragma once

#ifndef DAC_PIF_H_
#define DAC_PIF_H_

#include "device_definitions.h"
#include "utils_pif.h"

void setup_dac( void );
void dac_set_channel_1_value(uint32_t value);

#endif /* DAC_PIF_H_ */

/*!
 * This file contain exampled for DAC usage on STM32.
 *
 * Used GPIOs must not be used by other examples and other code while functions from this file are used.
 *
 * Behavior of particular example function is explained in the comment above the function.
 *
 * Improvement ideas:
 *  todo 1: Implement setup and usage examples for 8 bits right aligned and 12 bits left aligned for DAC data format
 *  todo 2: Implement setup and usage examples for 8 and 12 bits DAC dual mode;
 *  todo 3: Implement setup and usage examples of signed data input for DAC
 *  todo 4: Implement setup and usage examples for external DAC trigger
 *  todo 5: Implement setup and usage examples for noise generation on DAC signal
 *  todo 6: Implement setup and usage examples for triangle wave generation on DAC signal
 *  todo 7: Implement setup and usage examples for sawtooth wave generation on DAC signal
 *  todo 8: Implement setup and usage examples for sample and hold mode for DAC signal
 *  todo 9: Implement setup and usage exampled for DAC calibration
 *
 * General conclusion: DAC peripheral seems very straightforward and documented well in the RM. One read through is recommended for everyone
 *  who plans to actively use DAC.
 */
