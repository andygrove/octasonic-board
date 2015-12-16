#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pinDefines.h"
#include "macros.h"

#define MAX_SENSOR_COUNT 8
unsigned int sensor_count = MAX_SENSOR_COUNT;
unsigned int sensor_data[MAX_SENSOR_COUNT];

unsigned int poll_interval_us = 50;

void spi_init_slave (void)
{
  SPI_SCK_DDR &= ~(1 << SPI_SCK);                      /* input on SCK */
  SPI_SS_DDR &= ~(1 << SPI_SS);                        /* set SS input */
  SPI_MOSI_DDR &= !(1 << SPI_MOSI);                   /* input on MOSI */

  SPI_MISO_DDR |= (1 << SPI_MISO);                   /* output on MISO */

  SPI_MOSI_PORT |= (1 << SPI_MOSI);                  /* pullup on MOSI */

  // enable SPI and SPI interrupt
  SPCR |= ((1 << SPE) | (1 << SPIE));

  SPDR = 0;

  DDRB |= (1 << PB0); // PB0 = output (LED)

  // DDRC |= (1 << PC0); // PC0 = output (TRIGGER)
  // DDRC &= ~(1 << PC1); // PC1 = input (ECHO)
}

ISR(SPI_STC_vect) {

  unsigned int data_in = SPDR;

  // first 4 bits are the command number
  unsigned int command = (data_in & 0xF0) >> 4;

  switch (command) {
    case 0:
      SPDR = 0x00;
      break;
    case 1: // set_sensor_count
      sensor_count = data_in & 0x0F;
      SPDR = 0x00;
      break;
    case 2: // get_sensor_count
      SPDR = sensor_count;
      break;
    case 3:
      SPDR = sensor_data[data_in & 0x0F];
      break;
    default:
      // 0xFF means error condition
      SPDR = 0xFF;
      break;
  }
}

int main(void)
{

  for (int i=0; i<MAX_SENSOR_COUNT; i++) {
    sensor_data[i] = 0;
  }

  spi_init_slave();                             //Initialize slave SPI
  sei();
  while(1) {
    // for (int i=0; i<MAX_SENSOR_COUNT; i++) {
    //   sensor_data[i] = sensor_data[i] + 1;
    // }

    // LED on
    PORTB |= (1 << PB0);

    // trigger
    DDRC |= (1 << PC0);
    PORTC |= (1 << PC0);
    _delay_us(10);
    PORTC &= ~(1 << PC0);
    DDRC &= ~(1 << PC0);

    // loop while echo is LOW
    do { } while (!(PINC & (1 << PC0)));

    // loop while echo is HIGH
    unsigned int count = 0;
    do {
      count = count + 1;
      _delay_us(1);

      // if (count > 10000) {
      //   break;
      // }

    } while (PINC & (1 << PC0));

    sensor_data[0] = count / 59;

    // turn LED off
    _delay_ms(100);
    PORTB &= ~(1 << PB0);
    _delay_ms(100);

  }
}
