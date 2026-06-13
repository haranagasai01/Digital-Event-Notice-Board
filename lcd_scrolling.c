//lcd_scrolling.c

#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"

#define SPEED 200  //200 means 200 ms delay
#define MAX 16     //for lcd compatability 16 for 16*2 LCD

extern volatile u8 timer_fired;

void Display_Countdown(void);

void lcd_scroll(s8 *str){
		
		s8 i,j,k;
		s8 *chr;
		CmdLCD(0x01);
		delay_ms(2);
		//delay_ms(50);
	
	//PHASE 1: Right-to-Left Entry Phase
	
	  for(i=MAX-1;i>=0;i--){
		 if(timer_fired){
			 CmdLCD(0x01); // Terminate frame instantly on high-priority event trigger
			 return; 
		 }
			CmdLCD(0x80+i);  // Position cursor dynamically at decreasing offsets
			if(*str){
				for(j=0;j<MAX-i;j++){
					if(timer_fired){
						CmdLCD(0x01);
						return; 
						}
					if(*(str+j) !='\0')
						CharLCD(*(str+j));
					else
						break;
				}
				Display_Countdown();  //Function call to display countdown in Second line
			}
			/*for(j=0;j<i;j++){
				CharLCD(' ');
			}*/
			delay_ms(SPEED);
		}	
	
		//PHASE 2: Sliding Window Phase
		while(*str){
		if(timer_fired){
			CmdLCD(0x01);
			return; 
		 }
		//	k=MAX;
			chr=str;
			CmdLCD(0x80);
			for (k=0;k<MAX; k++){
				if(timer_fired){
					CmdLCD(0x01);
					return; 
		   }
				if(*chr !='\0'){
				CharLCD(*chr++);
				}
				else{
					CharLCD(' ');
				}
			}
			Display_Countdown();
			
			str++;
			delay_ms(SPEED);
		}
		CmdLCD(0x80);
		CharLCD(' ');
		delay_ms(1000);
}
