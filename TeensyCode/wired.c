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

#include "inc/macros.h"
#include "inc/ADC.h"
#include "inc/SPI.h"

/* Main function */
int main() {
    unsigned char tempVal; // 8 bit value of the temperature.
    unsigned char flowVal; // 8 bit value of the flow rate.
    
    unsigned int data;
    
    Init();
    
    while(1) {
        // Gather sensor values.
        tempVal = ReadADC(TEMP);
        flowVal = ReadADC(FLOW);
        
        PullSPI(tempVal);
        PullSPI(flowVal);
    }
}

/* Description: Initialises the teensy, SPI module, transceiver and ADC module.
 */
void Init()
{
    InitADC();
    
    InitSPI(SLAV);
}
