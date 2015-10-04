/*
 * Transceiver.h
 *
 * Created: 18/09/2015.
 * Last modified: 22/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details
 *  Contains functions for initialising the Alpha module, transmitting and
 *  receiving a byte of data and toggling between transmitter on, receiver on
 *  and idle mode.
 * 
 *  Layout                 ______________
 *          MISO -  SDO - |   __        | - nSEL - SS
 *           PD0 - nIRQ - |  |  |       | - SCK  - SCK
 *           VCC - nFFS - |  |  | {   } | - SDI  - MOSI
 *           PD2 - FFIT - |  |  | {   } | - VDI  - PD1
 *           VCC -  CLK - |  |  |       | - GND  - GND
 *           VCC - nRES - |  |__|       | - VDD  - VCC
 *           GND -  GND - |_____________| - ANT  - Antenna
 * 
 *  Pins
 *     SDO  DO  Serial data output with bus hold
 *    nIRQ  DO  Interrupt request output (active low)
 *    nFFS  DI  FIFO select input (active low) In FIFO mode, when bit ef is set
 *              in Configuration Setting Command
 *    FFIT  DO  FIFO interrupt (active high) Number of the bits in the RX FIFO
 *              has reached the preprogrammed limit
 *     CLK  DO  Microcontroller clock output
 *    nRES  DI  Open drain reset output with internal pull-up and input buffer
 *              (active low)
 *     GND  S   Ground
 * 
 * nSEL DI Chip select input of the serial control interface (active low)
 *  SCK DI Clock input of the serial control interface
 *  SDI DI Data input of the serial control interface (SPI compatible)
 *  VDI DO Valid data indicator output
 *  GND S  Ground
 *  VDD S  Power Supply
 *  ANT IO Antenna
 */

/* Include libraries */
#include "ALPHA.c"

/* Description: Intialise the Alpha module and configure settings.
 */
void InitAlpha();

/* Description:  
 * Parameters:  
 * Return:      
 */
unsigned char ReceiveAlpha();

/* Description:  
 * Parameters:  
 * Return:      
 */
void SendAlpha(unsigned char info);

/* Description:  
 * Parameters:  
 * Return:      
 */
void SwtichAlpha(unsigned char dir);

/* Notes on settings
Configuration Setting Command, Default => 0x8008 -> 0b1000 0000 0000 1000
1000 0000 | el  | ef  | b1 b0 | x3 x2 x1 x0
For init
0x8000    | 0b0 | 0b0 | 0b01  | 0b0001
For transmit
0x8011    | 0b1 | 0b0
For receive
0x8011    | 0b0 | 0b1

Power Management Command, Default => 0x8208 -> 0b1000 0010 0000 1000
1000 0010 | er  | ebb | et  | es  | ex  | eb  | ew  | dc
For init
0x82      | 0b0 | 0b1 | 0b0 | 0b1 | 0b1 | 0b0 | 0b0 | 0b1
For transmit
0x8259    | 0b0       | 0b1
For receive
0x8259    | 0b1       | 0b0

Frequency Setting Command, Default => 0xA680 -> 0b1010 0110 1000 0000
1010   | f11 f10 f9 f8 f7 f6 f5 f4 f3 f2 f1 f0
Frequency => 432.1 MHz (for shits and giggles)
Desired freq = 430 + f * 0.0025 MHz
For init
0xA000 | 0b0011 0100 1000

Data Rate Command, Default => 0xC623 -> 0b1100 0110 0010 0011
1100 0110 | cs  | r6 r5 r4 r3 r2 r1 r0
Baud Rate => 86.21 kbps
Desired Baud Rate => (10 * 10^6) / 29 / (r + 1) / (1 + cs * 7)
For init
0xC600    | 0b0 | 0b000 0011

Rx Control Command, Default => 0x9080 -> 0b1001 0000 1000 0000
1001 0 | p16 | d1 d0 | i2 i1 i0 | g1 g0 | r2 r1 r0
For init
0x9000 | 0b1 | 0b01  | 0b100    | 0b10  | 0b100

Data Filter Command, Default => 0xC22C -> 0b1100 0010 0010 1100
1100 0010 | al  | ml | 1 | s  | 1 | f2 f1 f0
For init
0xC228    | 0b1 | 0b1    | 0b0    | 0b011

FIFO and Reset Mode Command, Default => 0xCA80 -> 0b1100 1010 1000 0000
1100 1010 | f3 f2 f1 f0 | sp  | al  | ff  | dr
For init
0xCA00    | 0b0111      | 0b1 | 0b0 | 0b1 | 0b1

Synchron Pattern Command, Default => 0xCED4 -> 0b1100 1110 1101 0100
1100 1110 | b7 b6 b5 b4 b3 b2 b1 b0
For init
0xCE00    | 0b1101 0100

AFC Command, Default => C4F7 -> 0b1100 0100 1111 0111
1100 0100 | a1 a0 | rl1 rl0 | st  | fi  | oe  | en
For init
0xC400    | 0b10  | 0b11    | 0b0 | 0b1 | 0b1 | 0b1

Tx Configuration Command, Default => 0x9800 -> 0b1001 1000 0000 0000
1001 100 | mp  | m3 m2 m1 m0 | 0 | p2 p1 p0
For init
0x9800   | 0b0 | 0b0101          | 0b110

PLLSettings Command, Default => 0xCC67 -> 0b1100 1100 0110 0111
1100 1100 0 | ob1 ob0 | lpx | ddy | ddit | 1 | bw
For init
0xCC02      | 0b00    | 0b0 | 0b0 | 0b0      | 0b0

Wake Up Timer Command, Default => 0xE196 -> 0b1110 0001 1001 0110
111    | r4 r3 r2 r1 r0 | m7 m6 m5 m4 m3 m2 m1 m0
For init
0xE000 | 0b0 0001       | 0b1001 0110

Low Duty Cycle Command, Default => 0xC80E -> 0b1100 1000 0000 1110
1100 1000 | d6 d5 d4 d3 d2 d1 d0 | en
For init
0xC800    | 0b000111             | 0b0

Low Batt and Mic Clk Div Command, Default => 0xC000 -> 0b1100 0000 0000 0000
1100 0000 | d2 d1 d0 | 0 | v3 v2 v1 v0
For init
0xC000    | 0b000        | 0b0000
*/
