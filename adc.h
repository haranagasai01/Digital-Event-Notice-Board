#ifndef _ADC_H_
#define _ADC_H_

#include "types.h"

void InitADC(void);
void ReadADC(u32 chno,f32* eAR,u32 *AdcDval);
void DisplayADC(float *eAR);

#endif
