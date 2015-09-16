/*
 * SamRPi.c
 *
 * Created: 17/09/2015.
 * Last modified: 17/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details.
 */

/* Include libraries */

/* Function declarations */
void ReceiveData();
void SendData(char ID);
void Init();
void Manch();

/* Constant declarations */
#DEFINE HOT 0x01
#DEFINE COL 0x10
#DEFINE SHIFT(places) (1 << (places))

/* Main function */
int main() {
    unsigned short hotTemp; // Value of the hot water temperature.
    unsigned short hotFlow; // Value of the hot water flow rate.
    unsigned short colTemp; // Value of the cold water temperature.
    unsigned short colFlow; // Value of the cold water flow rate.
    
    Init();
    
    while(1) {
        // Open Tx
        
        SendData(HOT);
        
        // Close Tx
        
        // Open Rx
        
        ReceiveData();
        
        // Close Rx
        
        // Open Tx
        
        SendData(COL);
        
        // Close Tx
        
        // Open Rx
        
        ReceiveData();
        
        // Close Rx
        
        // MATH!!!
    }
}

/* Description:  Loops until data is received.
 */
void ReceiveData()
{
    while(1)
    {
        /*DataToReceive = 1;
        
        while(DataToReceive)
        {
            while(nIRQ) {}
            
            Read FIFO
            
            Check Data to Receive
            
            if(NoMoreData)
            {
                DataToReceive = 0;
            }
        }
        
        Seperate Data
        
        Decode data
        Manch(data);
        
        Save Flow and Temp
        */
    }
}

/* Description: Transmits send command to Teensy.
 * Parameters:  id - Identifier of Teensy.
 */
void SendData(char id)
{
    DataToSend = 1;
        
    while(DataToSend)
    {
        while(nIRQ) {}
        
        // Send id
    }
    
    return;
} */

/* Description: Initialises the Raspberry Pi, SPI module and transceiver.
 */
void Init()
{
    // Inti RPi
    
    // Init SPI
    
    // Init Tranceiver
}

/* Description: Decodes data received, removing the start and stop bits. 
 * Parameters:  var - 16 bit left adjusted variable to be encoded.
 * Return:      32 bit Manchester encoded variable.
 */
short Manch(long var)
{
    long ret = 0x00; // Decoded variable.
    
    // 00 11 00 11  -- -- -- --  -- -- -- --  -- -- 11 00
    // 30 28 26 24  22 20 18 16  14 12 10 08  06 04 02 00
    
    // 00 00 00 00  00 00 -- --  -- -- -- --  -- -- -- --
    // 15 14 13 12  11 10 09 08  07 06 05 04  03 02 01 00
    for(i = 15; i >= 0; i--)
    {
        // Test if bits are legally coded.
        if((var & SHIFT(2 * i + 1)) ^ (var & SHIFT(2 * i)))
        {
            // If encoded bits represent one then flip, otherwise leave.
            if(var & SHIFT(2 * i + 1))
            {
                ret ^= SHIFT(i - 2);
            }
        }
    }
    
    return ret;
}
