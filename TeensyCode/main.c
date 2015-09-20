/*
 * main.c
 *
 * Created: 13/09/2015.
 * Last modified: 14/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details.
 */

/* Function declarations */
void Init();
char Manch(char var, char lev);

/* Included libraries */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "ADC.h"
#include "SPI.h"
#include "ALPHA.h"

/* Main function */
int main() {
    char tempVal; // 8 bit value of the temperature.
    char flowVal; // 8 bit value of the flow rate.
    char tempHig; // 8 bit higher manchester code of the temperature.
    char tempLow; // 8 bit lower manchester code of the temperature.
    char flowHig; // 8 bit higher manchester code of the flow rate.
    char flowLow; // 8 bit lower manchester code of the flow rate.
    
    Init();
    
    while(1) {
        // Open Rx
        
        ReceiveData();
        
        // Close Rx
        
        tempVal = ReadADC(TEMP);
        
        flowVal = ReadADC(FLOW);
        
        tempHig = Manch(tempVal, 1);
        tempLow = Manch(tempVal, 0);
        
        flowHig = Manch(flowVal, 1);
        flowLow = Manch(flowVal, 0);
        
        // Open Tx
        
        SendData(tempHig, tempLow, flowHig, flowLow);
        
        // Close Tx
    }
}

/* Description: Initialises the teensy, SPI module, transceiver and ADC module.
 */
void Init()
{
    // Set inputs.
    DDRD = SHIFT(DDD1) | SHIFT(NIRQ);
    PORTD = SHIFT(PORTD1) | SHIFT(NIRQ);
    
    InitADC();
    
    InitSPI();
    
    InitAlpha();
}

/* Description: Encodes 10 bits of data for transmission with start and stop
 *              bits. 
 * Parameters:  var - 16 bit left adjusted variable to be encoded.
 * Return:      32 bit Manchester encoded variable.
 */
char Manch(char var, char lev)
{
    char ret; // Start and stop bits.
    char i;
    // -- -- -- --  -- -- -- --  -- -- -- --  -- -- -- --
    // 15 14 13 12  11 10 09 08  07 06 05 04  03 02 01 00
    
    // --    --     --    --     --    --     --    --
    // 07    06     05    04     03    02     01    00
    for(i = 3; i >= 0; i--)
    {
        // Test if left adjusted bits are 1 or 0.
        if(var & SHIFT(i + 4 * lev))
        {
            // 1 -> 10.
            ret ^= SHIFT(2 * i + 1);
        }
        else
        {
            // 0 -> 01.
            ret^= SHIFT(2 * i);
        }
    }
    
    return ret;
}
