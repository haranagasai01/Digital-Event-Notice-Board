#ifndef _LCD_H_
#define _LCD_H_

//lcd.h
#include "types.h"
void InitLCD(void);
void WriteLCD(u8);
void CmdLCD(u8);
void CharLCD(u8);
void StrLCD(s8*);
void U32LCD(s32);
void FloatLCD(f32 fnum,s32 dp);
void BinLCD(u32 n,u8 nbd);
void Buildcgram(u8* cg,u8 bytes);

#endif
