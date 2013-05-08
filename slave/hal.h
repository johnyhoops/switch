#ifndef HAL_H_
#define HAL_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

// returns device value
typedef int16_t (*getValueFunction)(int16_t reference);

// returns the value set
typedef int16_t (*setValueFunction)(int16_t reference, int16_t value);

typedef struct{
	getValueFunction 	getValue;
	setValueFunction 	setValue;
} HAL;

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* DEVICE_H_ */
