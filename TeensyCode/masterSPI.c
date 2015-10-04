/*
 * masterSPI.c
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
void Push(unsigned int data);

/* Included libraries */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include <util/delay.h>
#include "inc\usb_debug_only.c"
#include "inc\print.c"

#include "inc\macros.h"
#include "inc\SPI.h"

/* Main function */
int main() {
    CPU_PRESCALE(CPU_16MHz);
    
    unsigned char tempVal = 0x0F; // 8 bit value of the temperature.
    unsigned char flowVal = 0xF0; // 8 bit value of the flow rate.
    unsigned int data; // Status of device.
   
    Init();
    
    while(1) {
        // Gather sensor values.
        tempVal++;
        flowVal--;
        
        data = ((tempVal << 8) | flowVal);
        
        Push(data);
        
        _delay_ms(1000);
    }
}

/* Description: Initialises the teensy, SPI module, transceiver and ADC module.
 */
void Init()
{
    //usb_init();
    
    InitSPI();
}

void Push(unsigned int data)
{
    // Start transmission.
    PORTB &= ~SHIFT(SS);
    
    // Send most significant.
    SPDR = (char) (data >> 8);
    
    // Wait for transmission complete.
    while(!(SPSR & SHIFT(SPIF))) {}
    
    // Send least significant.
    SPDR = (char) data;
    
    // Wait for transmission complete.
    while(!(SPSR & SHIFT(SPIF))) {}
    
    // End transmission.
    PORTB |= SHIFT(SS);
}
