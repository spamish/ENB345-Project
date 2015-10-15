/*
 * SPI.c
 *
 * Created: 18/09/2015.
 * Last modified: 22/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 */

void InitSPI(char opt)
{
    switch(opt)
    {
        case MAST:
            // Set pins.
            DDRB |= SHIFT(MOSI) | SHIFT(SCK) | SHIFT(SS);
            PORTB |= SHIFT(SS);
            
            // Set SPI modes.
            // SPCR = SPIE | SPE | DORD | MSTR | CPOL | CPHA | SPR1 | SPR0
            SPCR |= SHIFT(SPE) | SHIFT(MSTR) | SHIFT(SPR0);
            SPSR |= SHIFT(SPI2X);
            break;
        
        case SLAV:
            // Set pins.
            DDRB |= SHIFT(MISO);
            PORTB |= SHIFT(SS);
            
            // Set SPI modes.
            SPCR |= SHIFT(SPE);
            break;
    }
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

unsigned char PullSPI(unsigned char data)
{
    // Wait for chip select.
    while(PINB & SHIFT(SS)) {}
    
    // Send most significant bits.
    SPDR = data;
    
    // Wait for transmission complete.
    while(!(SPSR & SHIFT(SPIF))) {}
    
    // Save returned value.
    return = SPDR;
}
