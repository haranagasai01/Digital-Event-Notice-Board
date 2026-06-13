#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "types.h"

void lcd_scroll(s8*);

//timer
void InitTimer(void);
void tdelay_us(u32 us);
void tdelay_ms(u32 ms);
void tdelay_s(u32 s);

#endif
