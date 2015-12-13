                                                    /* Blinker Demo II */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED      PB0
#define LED_DDR  DDRB
#define LED_PORT PORTB

#define DELAYTIME 20

#define setBit(sfr, bit)     (_SFR_BYTE(sfr) |= (1 << bit))
#define clearBit(sfr, bit)   (_SFR_BYTE(sfr) &= ~(1 << bit))
#define toggleBit(sfr, bit)  (_SFR_BYTE(sfr) ^= (1 << bit))

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

void spi_init_slave (void)
{
    DDRB=(1<<6);                                  //MISO as OUTPUT
    SPCR=(1<<SPE);                                //Enable SPI
}

//Function to send and receive data
unsigned char spi_tranceiver (unsigned char data)
{
    SPDR = data;                                  //Load data into buffer
    while(!(SPSR & (1<<SPIF) ));                  //Wait until transmission complete
    return(SPDR);                                 //Return received data
}
int main(void) {

  spi_init_slave();

  // set LED pin for output
  setBit(LED_DDR, LED);

  // Mainloop
  while (1) {

    unsigned char data = spi_tranceiver(0x55);

    if (data > 0) {
      setBit(LED_PORT, LED);
    } else {
      clearBit(LED_PORT, LED);
    }
    
  }
  return (0);                                          /* end mainloop */
}
