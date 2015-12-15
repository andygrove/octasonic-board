#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pinDefines.h"
#include "macros.h"

#define max_sensor_count 8

unsigned int sensor_count = max_sensor_count;
unsigned int sensor_data[max_sensor_count];
unsigned int poll_interval_us = 50;

void spi_init_slave (void)
{
  SPI_SCK_DDR &= ~(1 << SPI_SCK);                      /* input on SCK */
  SPI_SS_DDR &= ~(1 << SPI_SS);                        /* set SS input */
  SPI_MOSI_DDR &= !(1 << SPI_MOSI);                   /* input on MOSI */

  SPI_MISO_DDR |= (1 << SPI_MISO);                   /* output on MISO */

  SPI_MOSI_PORT |= (1 << SPI_MOSI);                  /* pullup on MOSI */

  SPCR |= (1 << SPE);                                        /* enable */
}

//Function to send and receive data
unsigned char spi_tranceiver (unsigned char data)
{
  SPDR = data;                                  //Load data into buffer
  while(!(SPSR & (1<<SPIF) ));                  //Wait until transmission complete
  return(SPDR);                                 //Return received data
}

int main(void)
{
  // fake data for now
  for (int i=0; i<max_sensor_count; i++) {
    sensor_data[i] = (i+1) * 11;
  }

  spi_init_slave();                             //Initialize slave SPI

  while(1) {

    unsigned int data_in = spi_tranceiver(0);

    // first 4 bits are the command number
    unsigned int command = (data_in & 0xF0) >> 4;

    switch (command) {
      case 0:
        // ignore
        break;
      case 1: // set_sensor_count
        sensor_count = data_in & 0x0F;
        break;
      case 2: // get_sensor_count
        spi_tranceiver(sensor_count);
        break;
      case 3: // get_sensor_reading
        spi_tranceiver(sensor_data[data_in & 0x0F]);
        break;
      default:
        // 0xFF means error condition
        spi_tranceiver(0xFF);
        break;
    }

    // now monitor the ultra sonic sensors

  }
}
