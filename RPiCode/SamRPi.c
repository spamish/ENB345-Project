/* Function declarations */
double* RetrieveData();
void InitSPI();

/* Constant declarations */
#define INFLOW  0
#define INTEMP  1
#define OUTFLOW 2
#define OUTTEMP 3
#define SHIFT(places) (1 << (places))

/* Description: Retrieve sensor values from Teensy's.
 * Parameters:  data - array of 4 double values.
 */
void RetrieveData(double* data)
{
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    data[INFLOW] = (double) 0.121523 * bcm2835_spi_transfer(0x55);
    data[INTEMP] = (double) 1.1 * bcm2835_spi_transfer(0x55);
    
    bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
    data[OUTFLOW] = (double) 0.121523 * bcm2835_spi_transfer(0x55);
    data[OUTTEMP] = (double) 1.1 * bcm2835_spi_transfer(0x55);
}

/* Description: Initialises the SPI module.
 */
void InitSPI()
{
    bcm2835_spi_begin();
    
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);
}
