//eb_main.c (event board main.c file)
#include <lpc21xx.h>
#include "types.h"
#include "lcd.h"
#include "rtc.h"
#include "kpm.h"
#include "msg.h"
#include "global.h"
#include "timer.h"
#include "delay.h"
#include "adc.h"
#include "adc_defines.h"

#define rLED 4  //red led turn on indicates clock/standby mode
#define gLED 5  //green led turn on indicates active scrolling notice

s32 j=0;
extern s32 i;
extern volatile int startScroll;
void Display_Countdown(void);
extern volatile u8 timer_fired;

//adc global variables
f32 eAR;
u32 AdcDval;

//Event Message database schedule list
Message messageList[TOTAL_MESSAGES] = { 
    {7, 45, "Good Morning! Classes Start Soon", 1}, 
    {8, 45, "C Programming Session in Classroom number 2", 1}, 
    {10, 15, "C module theory exam in 4th floor lab1", 1}, 
    {11, 15, "C module lab exam in 4th floor lab2 and middle lab at 10:30AM", 1}, 
    {13, 45, "Lunch Break from 1PM-2PM", 1}, 
    {9, 45, "ARM Workshop on external interrupts in LAB1 at 10AM", 1}, 
    {9, 30, "ARM kit issue time from 10AM-10:30AM in middle lab", 1}, 
    {15, 15, "Only 15 mins break time for next ARM session", 1}, 
    {17, 00, " Revise today's Class programs at home!", 1}, 
    {17, 45, " End of Day-See You Tomorrow!", 1} 
};

void enable_eint0(void);
void enable_rtc_alarm(void);

int main()
{
		//intializing all peripherals and interrupts
		RTC_Init();
		InitLCD();
		Initkpm();
	  InitADC();
		Init_Timer0_Interrupt();
		enable_eint0();
		enable_rtc_alarm();
	
    // Set the initial time (hours, minutes, seconds)
		SetRTCTimeInfo(15,14,54);
    // Set the initial data (date, month, year)
		SetRTCDateInfo(2,3,2026);
    // Set initial day (SUN to SAT )
		SetRTCDay(MON);

		IODIR0|=(1<<gLED);
		IODIR0|=(1<<rLED);

    while (1) 
    {
        // Get and display the current time info on LCD
				GetRTCTimeInfo(&hour,&min,&sec);
				DisplayRTCTime(hour,min,sec);
				
				// Get and display the current date info on LCD
				GetRTCDateInfo(&date,&month,&year);
				DisplayRTCDate(date,month,year);
				// Get and display the current day info on LCD
				GetRTCDay(&day);
				DisplayRTCDay(day);
				
				if(sec%2==0){
				ReadADC(CHNO2,&eAR,&AdcDval);
				DisplayADC(&eAR);  //to display room temperature continuously in standbymode
				IOCLR0=1<<gLED;
				IOSET0=1<<rLED;
				}
			  if(startScroll){
					Start_Scroll_Timer();
					while(timer_fired==0){
							IOSET0=1<<gLED;
							IOCLR0=1<<rLED;
							lcd_scroll((s8 *)(messageList[i].text));  // i is from rtc_interrupt.c file
					}
					Stop_Scroll_Timer();	
					CmdLCD(0x01);
					delay_ms(10);
					startScroll=0;
				}

    }
}
