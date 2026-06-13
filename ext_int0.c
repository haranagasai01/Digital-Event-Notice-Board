//ext_int0.c

#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
#include "rtc.h"
#include "lcd.h"

void rtc_edit(void);
	
#define EINT0 14
void eint0_isr(void)__irq{
	
	rtc_edit();   // Jump straight to the interactive Admin editing menu
	
	EXTINT=1<<0;  // Clear the EINT0 hardware interrupt flag so it can handle the next press
	
	VICVectAddr=0;  // Acknowledge the interrupt in the VIC controller to finish the cycle
	
	
}

void enable_eint0(void){

		PINSEL0|=0x0000000C;  // Configure pin P0.1 to act as the EINT0 input pin instead of normal GPIO
		
		// Vectored Interrupt Controller (VIC) Settings for the LPC2148
		VICIntSelect=0<<EINT0; // Set EINT0 to normal IRQ mode (0) instead of FIQ mode
	  VICIntEnable=1<<EINT0; // Enable the global interrupt for EINT0 (Channel 14)
	  VICVectCntl0=(1<<5)|EINT0; // Turn on VIC slot 0 and assign it to EINT0 hardware
		VICVectAddr0=(unsigned int)(eint0_isr);  // Link slot 0 directly to our button press ISR function
	
		EXTMODE |= (1 << 0);    // Set EINT0 to Edge Sensitive (1)
    EXTPOLAR &= ~(1 << 0); // Set to Falling Edge (0) because it's Active Low button
  
}
