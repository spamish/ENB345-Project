/*
 * SPI.c
 *
 * Created: dd/mm/2015.
 * Last modified: dd/mm/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 */

void InitADC()
{
	ADMUX |= SHIFT(REFS0)|SHIFT(ADLAR); //Set MUX values to 000000 or 000001.
	ADCSRA |= SHIFT(ADEN)|SHIFT(ADPS2)|SHIFT(ADPS0); //ADSC for each conversion.
}

char ReadADC(char sens)
{
    if(sens == FLOW)
    {
        // Select flow pin.
        ADMUX &= ~SHIFT(MUX0);
    }
    else
    {
        // Select temp pin.
        ADMUX |= SHIFT(MUX0);
    }
    
    // Start conversion.
    ADCSRA |= SHIFT(ADSC);
    
    // Wait for conversion to finish.
    while(ADCSRA & SHIFT(ADIF)) {}
    
    return ADCH;
}