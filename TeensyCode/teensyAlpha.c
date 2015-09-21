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
#include "ALPHA.h"

// Identifier of the individual teensy
//================================================
#define ID 0x4C // a"L"pha
//================================================

/* Main function */
int main() {
    char tempVal; // 8 bit value of the temperature.
    char flowVal; // 8 bit value of the flow rate.
    int stat = 0; // Status of device.
    
    Init();
    
    while(1) {
        // Wait for command from base.
        SwtichAlpha(RX);
        
        while(!stat)
        {
            stat = (ID & ReceiveAlpha());
        }
        
        SwtichAlpha(ID);
        
        // Gather sensor values.
        tempVal = ReadADC(TEMP);
        flowVal = ReadADC(FLOW);
        
        // Transmit sensor values.
        SwtichAlpha(TX);
        
        SendAlpha(tempVal);
        SendAlpha(flowVal);
    }
}

/* Description: Initialises the teensy, SPI module, transceiver and ADC module.
 */
void Init()
{
    // Set inputs.
    DDRD |= SHIFT(DDD1) | SHIFT(NIRQ);
    PORTD |= SHIFT(PORTD1) | SHIFT(NIRQ);
    
    InitADC();
    
    InitSPI();
    
    InitAlpha();
}
