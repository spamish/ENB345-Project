/*
 * testADC.c
 *
 * Created: 22/09/2015.
 * Last modified: 22/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details
 *  Uses PWM to test that the ADC is correctly sampling from two independent
 *  channels.
 */

/* Function declarations */
void Init();

/* Included libraries */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include <util/delay.h>
#include "inc\usb_debug_only.c"
#include "inc\print.c"

#include "inc\macros.h"
#include "inc\ADC.h"

/* Main function */
int main() {
    unsigned char tempVal; // 8 bit value of the temperature.
    unsigned char flowVal; // 8 bit value of the flow rate.
    
    Init();
    
    while(1) {
        // Gather sensor values.
        tempVal = ReadADC(TEMP);
        flowVal = ReadADC(FLOW);
        
        phex(tempVal);
		print("\t");
        phex(flowVal);
		print("\n");
        _delay_ms(500);
    }
}

/* Description: Initialises the teensy and ADC module.
 */
void Init()
{
    usb_init();
    
    InitADC();
}
