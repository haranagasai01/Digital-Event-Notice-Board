//admin_mode.c

#include <lpc21xx.h>
#include "delay.h"
#include "types.h"
#include "kpm.h"
#include "lcd.h"
#include "rtc.h"
#include "msg.h"


extern s32 hour, min, sec; 
u32 nHOUR = 0, nMIN = 0, nSEC = 0, nDAY=0, nMON=0, nYY=0,nWeek=0;
u32 eH=0, eM=0 , eE=1;
s32 e=0;

void rtc_edit(void) {
    u8 ch;

    //Clear screen and show Menu
    CmdLCD(0x01);
		StrLCD("  Admin Mode ");
		delay_ms(500);
		CmdLCD(0x01);
    StrLCD("1.Time 2.Date");
		CmdLCD(0xc0);
		StrLCD("3.Day 4.en/dis");
    
    //2.Wait for a valid choice (Blocking)
    do {
        ch = keyscan();
    } while (ch != '1' && ch !='2' && ch!='3' && ch!='4' && ch != 'C'); // 'C' to cancel

		//OPTION 1: Update Real-Time Clock Time
		
    if (ch == '1') {
      
        CmdLCD(0x01);
        StrLCD("Set Hour (0-23):");
        nHOUR = ReadNum();
      
        CmdLCD(0x01);
        StrLCD("Set Min (0-59):");
        nMIN = ReadNum();
        
       
        CmdLCD(0x01);
        StrLCD("Set Sec (0-59):");
        nSEC = ReadNum();

       // Validate that user inputs fall within correct limits before applying
			if(((nHOUR<=23))&&((nMIN<=59)) && ((nSEC<=59)))
					SetRTCTimeInfo(nHOUR, nMIN, nSEC);
			
			 CmdLCD(0x01);
        StrLCD("Time Updated!");
        delay_ms(1000);
			}
		
			//OPTION 2: Update Real-Time Clock Date
			
			else if(ch == '2'){
			
			CmdLCD(0x01);
        StrLCD("Set DD (1-31):");
        nDAY = ReadNum();
				
				CmdLCD(0x01);
        StrLCD("Set MM (1-12):");
        nMON = ReadNum();
				
				CmdLCD(0x01);
        StrLCD("Set YYYY (year):");
        nYY = ReadNum();
        
        //// Ensure date fields match standard calendar limits
				if(((nDAY>=1) && (nDAY<=31))&&((nMON>=1) && (nMON<=12)) && (nYY>=2000))
						SetRTCDateInfo(nDAY,nMON,nYY);

        CmdLCD(0x01);
        StrLCD("Date Updated!");
        delay_ms(1000);
			}
			//OPTION 3: Update Current Day of the Week
			
			else if(ch == '3'){
				CmdLCD(0x01);
        StrLCD("Set DAY 0.SUN ");
				CmdLCD(0xc0);
				StrLCD("  1.MON- 6.SAT");
        nWeek = ReadNum();
				if(nWeek<=6)
					SetRTCDay(nWeek);
				CmdLCD(0x01);
				CmdLCD(0x80);
				StrLCD("Day Updated!");
        delay_ms(1000);
				
			}
			
			//OPTION 4: Enable or Disable Scheduled Messages
			else if(ch== '4'){
				StrLCD("enter time");
				
				CmdLCD(0x01);
        StrLCD("Set Hour (0-23):");
        eH = ReadNum();
      
        CmdLCD(0x01);
        StrLCD("Set Min (0-59):");
        eM = ReadNum();
				// Loop through the message slots to find the matching event time
				for(e=0;e<10;e++){
					if((messageList[e].hour==eH) && (messageList[e].minute==eM)){
							CmdLCD(0x01);
							StrLCD("en =1,dis =0");
							eE = ReadNum();
							if(eE==1){
								messageList[e].enabled=1; // click 1 to enable scheduled message
							}
							else if(eE==0){
								messageList[e].enabled=0; // click 0 to disable scheduled message
							}
					}
				}
			}
    
    
    CmdLCD(0x01); 
}
