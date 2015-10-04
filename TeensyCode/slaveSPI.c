/*
 * slaveSPI.c
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
unsigned int Pull();

/* Included libraries */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "inc\usb_debug_only.c"
#include "inc\print.c"

#include "inc\macros.h"

/* Main function */
int main() {
    CPU_PRESCALE(CPU_16MHz);
    
    unsigned char tempVal; // 8 bit value of the temperature.
    unsigned char flowVal; // 8 bit value of the flow rate.
    unsigned int data;
    
    Init();
    
    while(1) {
        // Wait for chip select.
        while(PINB & SHIFT(SS)) {}
        
        data = Pull();
        
        tempVal = (char) (data >> 8);
        flowVal = (char) data;
        phex(tempVal);
        print("\t");
        phex(flowVal);
		print("\n");
    }
}

/* Description: Initialises the teensy and SPI module.
 */
void Init()
{
    usb_init();
    
    // Set pins.
    DDRB |= SHIFT(MISO);
    PORTB |= SHIFT(SS);
    
    // Set SPI modes.
    SPCR |= SHIFT(SPE);
}

unsigned int Pull()
{
    unsigned int ret = 0;
    
    // Wait for transmission complete.
    while(!(SPSR & SHIFT(SPIF))) {}
    
    // Save returned value.
    ret |= SPDR;
    
    // Wait for transmission complete.
    while(!(SPSR & SHIFT(SPIF))) {}
    
    // Save returned value.
    ret |= (SPDR << 8);
    
    return ret;
}
