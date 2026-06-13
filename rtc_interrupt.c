//rtc_interrupt.c 

#include <lpc21xx.h>
#include "types.h"
#include "lcd.h"
#include "rtc.h"
#include "global.h"
#include "msg.h"
#include "timer.h"

s32 i=0;

volatile int startScroll = 0; 

void check_enable(void){
	for(i=0;i<TOTAL_MESSAGES;i++){
				if(((messageList[i].hour) ==HOUR) && ((messageList[i].minute)==MIN) && (SEC==0)){
						
					if(messageList[i].enabled){ 
							  startScroll=1;
							  Start_Scroll_Timer();
								break;
				}
			}
		}
	}


void rtc_alarm_isr(void) __irq {
   
			check_enable(); // Look for matching messages to display
	
			ILR=0X03;  // Clear the RTC interrupt flags so it can trigger next time
			VICVectAddr = 0;   // Acknowledge the interrupt in the Vectored Interrupt Controller (VIC)         
}


void enable_rtc_alarm(void) {
   
			CIIR=1<<1;  // Set RTC to generate an interrupt every time the minute changes
			ILR=0x03;   // Clear any existing power-on interrupt flags

    // Vectored Interrupt Controller (VIC) Settings for the LPC2148
    VICIntSelect &= ~(1 << 13);           // Set RTC (Channel 13) to operate in normal IRQ mode
    VICVectAddr1 = (unsigned int)rtc_alarm_isr;  // Link slot 1 directly to our ISR function
    VICVectCntl1 = (1 << 5) | 13;         // Enable Slot 1 and link to RTC
    VICIntEnable = (1 << 13);             // Enable RTC Interrupt
}
