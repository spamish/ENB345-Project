/*
 * main.c
 *
 * Created: 13/09/2015.
 * Last modified: 22/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details
 *  Runs a loop waiting for the ID to be received, then performs ADC
 *  conversions to get the sensor values and transmits them back to the base
 *  station.
 */

/* Function declarations */
void Init();

/* Included libraries */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "macros.h"

/* Main function */
int main() {
    char tempVal; // 8 bit value of the temperature.
    char flowVal; // 8 bit value of the flow rate.
    char tempCount = 0;
    char flowCount = 0;
    char i;
    int ret;
    
    Init();
    
    while(1) {
        
        for(i = 1; i >= 0; i--)
        {
            // Wait for transmission complete.
            while(!(SPSR & SHIFT(SPIF))) {}
            
            // Save returned value.
            ret |= (SPDR << (4 * i));
        }
        
        tempVal = (char) (ret>>4);
        flowVal = (char) ret;
        
        // Set LED's
        if(tempVal == tempCount)
        {
            tempCount = 0;
            PORTF ^= SHIFT(PF0);
        } else {
            tempCount++;
        }
        
        if(flowVal == flowCount)
        {
            flowCount = 0;
            PORTF ^= SHIFT(PF1);
        } else {
            flowCount++;
        }
    }
}

/* Description: Initialises the teensy and SPI module.
 */
void Init()
{
    // Set outputs.
    DDRF |= SHIFT(PF0) | SHIFT(PF1);
    PORTF &= ~SHIFT(PF0) | ~SHIFT(PF1);
    
    // Set pins.
    DDRB |= SHIFT(MISO);
    
    // Set SPI modes.
    SPCR |= SHIFT(SPE);
}
