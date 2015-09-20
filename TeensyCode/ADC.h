/*
 * SPI.h
 *
 * Created: dd/mm/2015.
 * Last modified: dd/mm/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details:
 */

/* Include libraries */
#include "ADC.c"

/* Description: Intialise the ADC module.
 */
void InitADC();

/* Description: Read voltage on selected sensor pin.
 * Parameters:  sensor - Identifier of the sensor to read.
 * Return:      10 bit left adjusted value representing voltage level.
 */
char ReadADC(char sens);

/* Description: 
 * Parameters:  
 * Return:      
 */
