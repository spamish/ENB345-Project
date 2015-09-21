/*
 * SPI.c
 *
 * Created: 18/09/2015.
 * Last modified: 22/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 */

void InitADC()
{
	ADMUX |= SHIFT(REFS0)|SHIFT(ADLAR); //Set MUX values to 000000 or 000001.
	ADCSRA |= SHIFT(ADEN)|SHIFT(ADPS2)|SHIFT(ADPS0); //ADSC for each conversion.
}

char ReadADC(char sens)
{
    if(sens)
    {
        // Select temp pin.
        ADMUX |= SHIFT(0);
    } else {
        // Select flow pin.
        ADMUX &= ~SHIFT(0);
    }
    
    // Start conversion.
    ADCSRA |= SHIFT(ADSC);
    
    // Wait for conversion to finish.
    while(ADCSRA & SHIFT(ADIF)) {}
    
    return ADCH;
}