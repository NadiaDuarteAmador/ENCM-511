/* 
 * File:   IOs.c
 * Author: nadia
 *
 * Created on October 4, 2022, 5:28 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <math.h>
#include <errno.h>
#include "ChangeClk.c"
#include "TimeDelay.c"
#include "IOs.c"


/*
 * 
 */

// IO INTERRUPTS 
void IOinit (void) {
    
    // LED
    TRISBbits.TRISB8 = 0; // Configure RB8 as digital output
    LATBbits.LATB8 = 0; // Make RB8 digital output
    // PUSH BUTTON 1
    TRISAbits.TRISA4 = 1; // Configure RA4 as digital output
    CNPU1bits.CN0PUE = 1; // Enable PU resistor on RA4/CN0
    CNEN1bits.CN0IE = 1; // Enables Change of Notification on CN0
    // PUSH BUTTON 2
    TRISBbits.TRISB4 = 1; // Configure RB4 as digital output
    CNPU1bits.CN1PUE = 1; // Enable PU resistor on RB4/CN1
    CNEN1bits.CN1IE = 1; // Enables Change of Notification on CN1
    // PUSH BUTTON 3
    TRISAbits.TRISA2 = 1; // Configure RA2 as digital output
    CNPU2bits.CN30PUE = 1; // Enable PU resistor on RA2/CN30
    CNEN2bits.CN30IE = 1; // Enables Change of Notification on CN30
    
    // CN Interrupt Settings
    IFS1bits.CNIF = 0; // Clear interrupt flag CN
    IPC4bits.CNIP = 6; // Priority levels (7 highest, 1 lowest, 0 disabled interupt)
    IEC1bits.CNIE = 1; // Enable CN interrupts
    

    return;
}


void IOcheck(void)
{
    // IEC1bits.CNIE = 0; // disable CN interrupts to avoid debounces
    // delay_ms(400,1);   // 400 msec delay to filter out debounces
    // IEC1bits.CNIE = 1; // enable CN interrupts to detect pb release
    
    uint8_t PBsPressed = PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4; // Counts how many Push Buttons are being pressed. (0 = All, 1 = Two, 2 = One, 3 = None)
    
        if (PBsPressed == 3) // No Push Buttons are pressed
        {
            LATBbits.LATB8 = 0; // LED OFF
        }
        else if (PORTAbits.RA2 == 0) // While PB1 is pressed
        {
            if ((PORTAbits.RA2 == 0) && (PORTBbits.RB4 == 0)) // If both PB1 and PB2 are pressed
            {
                LATBbits.LATB8 = 1;  // LED ON
                delay_ms(1); // Calling delay function in TimeDelay.c with 1ms
            }
            else // If only PB1 is pressed
            {
                LATBbits.LATB8 = 1;  // LED ON
                delay_ms(1000); // Calling delay function in TimeDelay.c with 1s
            }
         
        }
        else if (PORTBbits.RB4 == 0)  // While PB2 is pressed
        {
            if ((PORTAbits.RA2 == 0) && (PORTBbits.RB4 == 0)) // If both PB1 and PB2 are pressed
            {
                LATBbits.LATB8 = 1;  // LED ON
                delay_ms(1); // Calling delay function in TimeDelay.c with 1 ms
            }
            else 
            {
                delay_ms(2000); // Calling delay function in TimeDelay.c with 2s delay
                LATBbits.LATB8 = 1;  // LED ON  
            }
        }           
        else if (PORTAbits.RA4 == 0) // While PB3 is pressed
        {
            delay_ms(3000); // Calling delay function in TimeDelay.c with 3s delay
            LATBbits.LATB8 = 1;  // LED ON     
        } 
}

// Change of Notification pin Interrupt subroutine
// When a button is pressed, cpu wakes up and comes here, sets flag to 1, goes back to line after idle in main
// To avoid interrupt nesting, clearing interrupts to avoid jumping between addresses and just reseting your code
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    CNflag = 1; // global user defined flag - use only if needed
    IFS1bits.CNIF = 0;  //clear IF flag
    Nop();
    
    return;
}