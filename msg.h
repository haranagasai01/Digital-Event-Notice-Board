//msg.h

#ifndef _MSG_H_
#define _MSG_H_

#include "types.h"
#define TOTAL_MESSAGES 10 

typedef struct { 
    u8 hour; 
    u8 minute; 
    char text[80]; 
    u8 enabled;   // 1 = show, 0 = skip (user controlled) 
} Message; 
 
 
 
extern Message messageList[TOTAL_MESSAGES];

#endif
