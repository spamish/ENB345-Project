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
#include "ADC.h"
#include "SPI.h"

/* Main function */
int main() {
    char tempVal; // 8 bit value of the temperature.
    char flowVal; // 8 bit value of the flow rate.
    int data; // Status of device.
    char i;
    
    Init();
    
    while(1) {
        // Gather sensor values.
        tempVal = ReadADC(TEMP);
        flowVal = ReadADC(FLOW);
        
        data = ((tempVal<<4) | flowVal);
        
        // Start transmission.
        SPI &= ~SHIFT(SS);
        
        for(i = 1; i >= 0; i--)
        {
            // Send most significant to least significant.
            SPDR = (data >> (4 * i));
            
            // Wait for transmission complete.
            while(!(SPSR & SHIFT(SPIF))) {}
        }
        
        // End transmission.
        SPI |= SHIFT(SS);
    }
}

/* Description: Initialises the teensy, SPI module, transceiver and ADC module.
 */
void Init()
{
    InitADC();
    
    InitSPI();
}
