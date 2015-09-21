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

#include "macros.h"
#include "ADC.h"

/* Main function */
int main() {
    char tempVal; // 8 bit value of the temperature.
    char flowVal; // 8 bit value of the flow rate.
    char tempCount = 0;
    char flowCount = 0;
    
    Init();
    
    while(1) {
        // Gather sensor values.
        tempVal = ReadADC(TEMP);
        flowVal = ReadADC(FLOW);
        
        // Set LED's
        if(tempVal == tempCount)
        {
            tempCount = 0;
            PORTB ^= SHIFT(PB0);
        } else {
            tempCount++;
        }
        
        if(flowVal == flowCount)
        {
            flowCount = 0;
            PORTB ^= SHIFT(PB1);
        } else {
            flowCount++;
        }
        
		_delay_ms(2);
    }
}

/* Description: Initialises the teensy and ADC module.
 */
void Init()
{
    // Set outputs.
    DDRB |= SHIFT(PB0) | SHIFT(PB1);
    PORTB &= ~SHIFT(PB0) | ~SHIFT(PB1);
    
    InitADC();
}
