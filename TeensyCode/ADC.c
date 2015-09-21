/*
 * SPI.c
 *
 * Created: 18/09/2015.
 * Last modified: 22/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 */

void InitADC()
{
	ADMUX |= SHIFT(REFS0) | SHIFT(ADLAR); //Set MUX values to 000000 or 000001.
	ADCSRA |= SHIFT(ADEN) | SHIFT(ADPS0); //ADSC for each conversion.
}

char ReadADC(char sens)
{
    char data;
    
    if(sens)
    {
        // Select temp pin.
        ADMUX |= SHIFT(MUX0);
    } else {
        // Select flow pin.
        ADMUX &= ~SHIFT(MUX0);
    }
    
    // Start conversion.
    ADCSRA |= SHIFT(ADSC);
    
    // Wait for conversion to finish.
    while(!(ADCSRA & SHIFT(ADIF))) {}
    
    data = ADCH;
    
    ADCSRA |= SHIFT(ADIF);
    
    return data;
}