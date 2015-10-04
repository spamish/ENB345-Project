/*
 * SPI.h
 *
 * Created: 18/09/2015.
 * Last modified: 22/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details
 *  Contains functions for intialising the SPI module, sending and receiving
 *  data.
 */

/* Include libraries */
#include "SPI.c"

/* Description: Initialise the pins for use in SPI transmission, intitalise the
 *              module in master mode and set the clock rate to fck/4.
 */
void InitSPI();

/* Description: Sends and receives data over SPI.
 * Parameters:  data - 2 bytes of data to be sent.
 * Return:      2 bytes of data received from SPI.
 */
unsigned int PushSPI(unsigned int data);
