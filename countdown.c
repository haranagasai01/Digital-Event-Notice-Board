//countdown.c

#include <lpc21xx.h>
#include "types.h"
#include "timer.h"
#include "lcd.h"
#include "delay.h"

extern volatile u32 seconds_count; 

void Display_Countdown(void) {
    u32 time_left = 30 - seconds_count; // Change 30 to 900 for 15 mins
    u32 mins = time_left / 60;  //minutes left
    u32 secs = time_left % 60;  //seconds left
    
  
    CmdLCD(0xC0); 
    
    
		StrLCD("Time Left: ");  
    
    
    CharLCD((mins / 10) + '0');
    CharLCD((mins % 10) + '0');
    CharLCD(':');
    CharLCD((secs / 10) + '0');
    CharLCD((secs % 10) + '0');
}


