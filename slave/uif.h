#ifndef UIF_H
#define UIF_H

#include "hal.h"

#ifdef __cplusplus
extern "C"{
#endif

enum uif_reference {
	uif_kKeypad = 0,
	uif_kDisplayFormat,
	uif_kDisplayInteger,
	uif_kDisplayString,

};

enum uif_format{
	uif_kNoFlash = 0,
	uif_kFlash = 128
};

HAL* uif(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UIF_H */
