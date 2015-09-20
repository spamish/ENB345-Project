/*
 * SPI.c
 *
 * Created: dd/mm/2015.
 * Last modified: dd/mm/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 */

void InitSPI()
{
    char spi;
    // Set pins.
    DDRB = SHIFT(MOSI) | SHIFT(SCK);
    // Set SPI modes.
    // SPCR = SPIE | SPE | DORD | MSTR | CPOL | CPHA | SPR1 | SPR0
    // Default value,  0x00
    SPCR = SHIFT(SPE) | SHIFT(MSTR);
}

void PushSPI(char data)
{
    // Start transmission
    SPDR = data;
    // Wait for transmission complete
    while(!(SPSR & SHIFT(SPIF))) {}
}

char PullSPI()
{
    // Start transmission
    SPDR = 0x00;
    
    while(!(SPSR & SHIFT(SPIF))) {}
    
    return SPDR;
}
