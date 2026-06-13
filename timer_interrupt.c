//timer_interrupt.c

#include "timer.h"

// global tracking flags
volatile u8 timer_fired = 0;
volatile u32 seconds_count = 0;



void Timer0_ISR(void) __irq {
    
    seconds_count++;
    
    // 900 seconds = 15 minutes
    if (seconds_count >= 30) {  //use 900 for 15 minute timer
        timer_fired = 1;   
	   T0TCR = (1 << 1);    // Reset and disable Timer0 counting
    }
    
    T0IR = (1 << 0);   // Clear the channel 0 match interrupt flag
    VICVectAddr = 0;   // Acknowledge the interrupt in the VIC controller      
}


void Init_Timer0_Interrupt(void) {
    T0TCR = (1 << 1);   // Put the timer in reset mode before configuring it       
    T0PR  = 14999;    //Prescaler value for  1ms(Set Prescale Register to divide the 15MHz clock down to 1kHz (1ms steps))       
    
    T0MR0 = 1000;    // Set Match Register 0 to 1000 counts (1000ms = 1 Second)         
    
   
    T0MCR = (1 << 0) | (1 << 1);  // Tell the timer to trigger an interrupt AND reset itself when it hits MR0
    
    // Vectored Interrupt Controller (VIC) Settings for the LPC2148
    VICVectAddr2 = (unsigned long)Timer0_ISR; // Assign ISR to Slot 2
    VICVectCntl2 = (1 << 5) | 4;              // Enable Slot 2 & assign to Timer 0 (Channel 4)
    VICIntEnable = (1 << 4);                  // Globally enable Timer 0 Interrupt
}

// Start the countdown
void Start_Scroll_Timer(void) {
    timer_fired = 0;          // Reset flag
    seconds_count = 0;        // Reset second counter
    T0TCR = (1 << 1);         // Clear physical timer registers
    T0TCR = (1 << 0);         // Start Timer 0 counting up
}

//Shutdown Countdown
void Stop_Scroll_Timer(void) {
    T0TCR = (1 << 1);         // Disable and reset
}
