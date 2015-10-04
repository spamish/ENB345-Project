/*
 * Tranceiver.c
 *
 * Created: 13/09/2015.
 * Last modified: 22/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 */

void InitAlpha()
{
    unsigned int data;
    data = 0x8011; // Configuration Setting
    PushSPI(data);
    
    data = 0x8219; // Power Management
    PushSPI(data);
    
    data = 0xA348; // Frequency Setting
    PushSPI(data);
    
    data = 0xC621; // Data Rate
    PushSPI(data);
    
    data = 0x9594; // Receiver Control
    PushSPI(data);
    
    data = 0xC2EB; // Data Filter
    PushSPI(data);
    
    data = 0xCA7B; // FIFO and Reset Mode
    PushSPI(data);
    
    data = 0xCED4; // Syncronise Pattern
    PushSPI(data);
    
    data = 0xC4B7; // Automatic Frequency Control
    PushSPI(data);
    
    data = 0x9856; // Transmitter Configuration Control
    PushSPI(data);
    
    data = 0xCC02; // PLL Setting
    PushSPI(data);
}

unsigned char ReceiveAlpha()
{
    unsigned int data;
    
    // Wait for nIRQ to go low
    while(PIND & SHIFT(NIRQ)) {}
    
    // Read from FIFO
    data = PushSPI(READ);
    
    return (unsigned char) data;
}

void SendAlpha(unsigned char info)
{
    unsigned int data = SEND;
    
    // Wait for nIRQ to go low
    while(PIND & SHIFT(NIRQ)) {}
    
    data |= info;
    
    // Transmit character
    PushSPI(data);
}

void SwtichAlpha(unsigned char dir)
{
    unsigned int data;
    
    switch(dir)
    {
        case TX:
            // Enable transmitter
            data = 0x8091;
            PushSPI(data);
            
            data = 0x8279;
            PushSPI(data);
            
            break;
        
        case RX:
            // Enable receiver
            data = 0x8051;
            PushSPI(data);
            
            data = 0x8299;
            PushSPI(data);
            
            break;
        
        default:
            // Disable
            data = 0x8011;
            PushSPI(data);
            
            data = 0x8219;
            PushSPI(data);
            
            break;
    }
}
