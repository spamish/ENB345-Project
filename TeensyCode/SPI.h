/*
 * SPI.h
 *
 * Created: dd/mm/2015.
 * Last modified: dd/mm/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details.
 */

/* Include libraries */
#include "SPI.c"

/* Description: Initialise the pins for use in SPI transmission, intitalise the
 *              module in master mode and set the clock rate to fck/4.
 */
void InitSPI();

/* Description:  Pushes data across the SPI.
 * Parameters:   data - Byte of data to be sent.
 */
void PushSPI(char data);

/* Description: Pulls data from the SPI read buffer.
 * Return:      One byte of data from the register.
 */
char PullSPI();
