#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
extern "C"{
#endif

#include "hal.h"

enum i2c_reference{
	i2c_kInput = 0,
	i2c_kInputNoGain
};

HAL* i2c(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* I2C_H */
