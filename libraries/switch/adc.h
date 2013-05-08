#ifndef ADC_H_
#define ADC_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "hal.h"

// ADC channel assignment
#define ADC_IN2		0
#define ADC_IN1		1

extern HAL adc;

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* ADC_H_ */
