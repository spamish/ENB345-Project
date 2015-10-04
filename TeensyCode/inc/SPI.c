/*
 * SPI.c
 *
 * Created: 18/09/2015.
 * Last modified: 22/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 */

void InitSPI()
{
    // Set pins.
    DDRB |= SHIFT(MOSI) | SHIFT(SCK) | SHIFT(SS);
    PORTB |= SHIFT(SS);
    
    // Set SPI modes.
    // SPCR = SPIE | SPE | DORD | MSTR | CPOL | CPHA | SPR1 | SPR0
    SPCR |= SHIFT(SPE) | SHIFT(MSTR) | SHIFT(SPR0);
    SPSR |= SHIFT(SPI2X);
}

unsigned int PushSPI(unsigned int data)
{
    unsigned int ret = 0;
    
    // Start transmission.
    PORTB &= ~SHIFT(SS);
    
    // Send most significant bits.
    SPDR = (char) (data >> 8);
    
    // Wait for transmission complete.
    while(!(SPSR & SHIFT(SPIF))) {}
    
    // Save returned value.
    ret |= (SPDR << 8);
    
    // Send least significant bits.
    SPDR = (char) data;
    
    // Wait for transmission complete.
    while(!(SPSR & SHIFT(SPIF))) {}
    
    // Save returned value.
    ret |= SPDR;
    
    // End transmission.
    PORTB |= SHIFT(SS);
    
    return ret;
}
