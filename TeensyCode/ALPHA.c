/*
 * Tranceiver.c
 *
 * Created: 13/09/2015.
 * Last modified: 14/09/2015.
 * Authors: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 */

void InitAlpha()
{
    
}

void TRxCommand(short settings)
{
    /*
    data = command | settings;
    SS = 0;
    SendCommand(data);
    SS = 1;
    
    Push configuration settings over SPI
    All commands in notes.
    
    Check status.
    
    Seperate subfunctions for enabling and disabling Tx and Rx.
    Functions to call FIFO and Send data.
    */
}

ReceiveData()
{
    //wait for nIRQ to go low
    
    PullSPI();
}

SendData()
{
    
}
