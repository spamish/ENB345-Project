/*
 * SPI.h
 *
 * Created: 18/09/2015.
 * Last modified: 20/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details
 *  Contains functions for intialising the ADC module and reading data.
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
