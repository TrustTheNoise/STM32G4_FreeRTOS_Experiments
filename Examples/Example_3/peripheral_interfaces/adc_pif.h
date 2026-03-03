#pragma once

#ifndef ADC_PIF_H_
#define ADC_PIF_H_

#include "device_definitions.h"
#include "utils_pif.h"

void setup_adc( void );

void adc_start_injected_sampling( void );
void adc_wait_until_injected_sampling_is_finished( void );

uint32_t* adc_get_injected_measurements( void );

#endif /* ADC_PIF_H_ */
