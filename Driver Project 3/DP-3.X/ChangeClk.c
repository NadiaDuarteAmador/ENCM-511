/* 
 * File:   ChangeClk.c
 * Author: nadia
 *
 * Created on October 5, 2022, 7:23 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "xc.h"

void NewClk(unsigned int clkval)  
{
    char COSCNOSC;
    if (clkval == 8)  // 8MHz
    {
        COSCNOSC = 0x00;
    }
    else if (clkval == 500) // 500 kHz
    {
        COSCNOSC = 0x66;
    }
    else if (clkval== 32) // 32 kHz
    {
        COSCNOSC = 0x55; 
    }
    else // default 32 kHz
    {
        COSCNOSC = 0x55;
    }
        // Switch clock to 500 kHz
     SRbits.IPL = 7;  // Disable interrupts
     CLKDIVbits.RCDIV = 0;  // CLK division = 0
     __builtin_write_OSCCONH(COSCNOSC);   // (0x00) for 8MHz; (0x66) for 500kHz; (0x55) for 32kHz;
     __builtin_write_OSCCONL(0x01);
     OSCCONbits.OSWEN=1;
     while(OSCCONbits.OSWEN==1)
     {} 
     SRbits.IPL = 0;  //enable interrupts
}