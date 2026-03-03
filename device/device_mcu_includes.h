// File that contains target includes for a given MCU
#ifndef DEVICE_MCU_INCLUDES_H_
#define DEVICE_MCU_INCLUDES_H_

#ifdef HAL_USAGE
#include "stm32g4xx_hal.h"
#endif

#include "stm32g474xx.h" // Target MCU
#include "_cmsis_enhancement.h" // CMSIS additions we need to write a better code


#endif /* DEVICE_MCU_INCLUDES_H_ */
