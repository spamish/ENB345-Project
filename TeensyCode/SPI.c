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

int PushSPI(int data)
{
    char i;
    int ret = 0;
    
    // Start transmission.
    SPI &= ~SHIFT(SS);
    
    for(i = 1; i >= 0; i--)
    {
        // Send most significant to least significant.
        SPDR = (data >> (4 * i));
        
        // Wait for transmission complete.
        while(!(SPSR & SHIFT(SPIF))) {}
        
        // Save returned value.
        ret |= (SPDR << (4 * i));
    }
    
    // End transmission.
    SPI |= SHIFT(SS);
    
    return ret;
}
