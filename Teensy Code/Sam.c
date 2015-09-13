/*
 * TeensyTwat.c
 *
 * Created: 13/09/2015.
 * Last modified: 14/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details.
 */

/* Include libraries */

/* Function declarations */

/* Global variables */
int sensor_val;
char TEMP = 0;
char FLOW = 1;
char ID = 0;

/* Main function */
int main() {
    // Inti Teensy
    
    // Init SPI
    
    // Init Tranceiver
    
    // Init ADC
    
    while(1) {
        // Open Rx
        
        // function ReceiveData();
        
        // Close Rx
        
        // tempVal = ReadADC(TEMP);
        
        // flowVal = ReadADC(FLOW);
        
        // Open Tx
        
        // function SendData(Temp, Flow);
        
        // Close Tx
    }
}

/* function ReceiveData()
{
    while(1)
    {
        DataToReceive = 1;
        
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
        }
    }
} */

/* int ReadADC(sensor)
{
    Select ADC Channel
    
    Start Conversion
    
    while(conversionInProgress) {}
    
    return value;
} */

/* void SendData(Temp, Flow)
{
    DataToSend = 1;
        
    while(DataToSend)
    {
        while(nIRQ) {}
        
        Send Temp
        
        Send Flow
    }
    
    return;
} */