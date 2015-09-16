/*
 * Sam.c
 *
 * Created: 13/09/2015.
 * Last modified: 14/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details.
 */

/* Include libraries */

/* Function declarations */
void ReceiveData();
short ReadADC(sensor);
void SendData(long temp, long flow);
void Init();
long Manch(short var);

/* Constant declarations */
#DEFINE TEMP    0x01
#DEFINE FLOW    0x10
#DEFINE ID      0x01
#DEFINE SHIFT(places) (1 << (places))

/* Main function */
int main() {
    unsigned short tempVal; // 10 bit value of the temperature.
    unsigned short flowVal; // 10 bit value of the flow rate.
    unsigned long tempSend; // 20 bit manchester code of the temperature.
    unsigned long flowSend; // 20 bit manchester code of the flow rate.
    
    Init();
    
    while(1) {
        // Open Rx
        
        ReceiveData();
        
        // Close Rx
        
        tempVal = ReadADC(TEMP);
        
        flowVal = ReadADC(FLOW);
        
        tempSend = Manch(tempVal);
        
        flowSend = Manch(flowVal);
        
        // Open Tx
        
        SendData(tempSend, flowSend);
        
        // Close Tx
    }
}

/* Description:  Loops until identifier is received.
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
        
        if(data == ID) {
            return;
        } */
    }
}

/* Description: Read voltage on selected sensor pin.
 * Parameters:  sensor - Identifier of the sensor to read.
 * Return:      10 bit left adjusted value representing voltage level.
 */
short ReadADC(char sensor)
{
    if(sensor == FLOW)
    {
        // Select flow pin.
    }
    else
    {
        // Select temp pin.
    }
    
    // Start conversion
    
    while(conversionInProgress) {}
    
    // Read value.
    
    return value;
}

/* Description: Transmits the data to the Raspberry Pi.
 * Parameters:  temp - Temperature sensor data.
 *              flow - Flow rate sensor data.
 */
void SendData(long temp, long flow)
{
    DataToSend = 1;
        
    while(DataToSend)
    {
        while(nIRQ) {}
        
        //Send Temp
        
        //Send Flow
    }
    
    return;
} */

/* Description: Initialises the teensy, SPI module, transceiver and ADC module.
 */
void Init()
{
    // Inti Teensy
    
    // Init SPI
    
    // Init Tranceiver
    
    // Init ADC
}

/* Description: Encodes 10 bits of data for transmission with start and stop
 *              bits. 
 * Parameters:  var - 16 bit left adjusted variable to be encoded.
 * Return:      32 bit Manchester encoded variable.
 */
long Manch(short var)
{
    long ret = 0x3300000C; // Start and stop bits.
    
    // 00 11 00 11  -- -- -- --  -- -- -- --  -- -- 11 00
    // 30 28 26 24  22 20 18 16  14 12 10 08  06 04 02 00
    
    // -- -- -- --  -- -- -- --  -- -- 10 11  12 13 14 15
    // 15 14 13 12  11 10 09 08  07 06 05 04  03 02 01 00
    for(i = 11; i > 1; i--)
    {
        // Test if left adjusted bits are 1 or 0.
        if(var & SHIFT(i + 4))
        {
            // 1 -> 10.
            ret ^= SHIFT(2 * i + 1);
        }
        else
        {
            // 0 -> 01.
            ret ^= SHIFT(2 * i);
        }
    }
    
    return ret;
}
