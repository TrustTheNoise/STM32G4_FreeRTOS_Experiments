#pragma once

#ifndef RNG_PIF_H_
#define RNG_PIF_H_

#include "device_mcu_includes.h"
#include "device_definitions.h"

void setup_rng(void);

uint32_t rng_get_value(void);


#endif /* RNG_PIF_H_ */
