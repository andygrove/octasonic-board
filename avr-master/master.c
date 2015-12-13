                                                    /* Blinker Demo II */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED      PB0
#define LED_DDR  DDRB
#define LED_PORT PORTB

#define DELAYTIME 200

#define setBit(sfr, bit)     (_SFR_BYTE(sfr) |= (1 << bit))
#define clearBit(sfr, bit)   (_SFR_BYTE(sfr) &= ~(1 << bit))
#define toggleBit(sfr, bit)  (_SFR_BYTE(sfr) ^= (1 << bit))

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//Initialize SPI Master Device
void spi_init_master (void)
{
    DDRB = (1<<5)|(1<<3);              //Set MOSI, SCK as Output
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); //Enable SPI, Set as Master
                                       //Prescaler: Fosc/16, Enable Interrupts
}

//Function to send and receive data
unsigned char spi_tranceiver (unsigned char data)
{
    SPDR = data;                                  //Load data into buffer
    while(!(SPSR & (1<<SPIF) ));                  //Wait until transmission complete
    return(SPDR);                                 //Return received data
}
int main(void) {

  spi_init_master();

  // Mainloop
  while (1) {
    spi_tranceiver(0x00);
    _delay_ms(DELAYTIME);
    spi_tranceiver(0x01);
    _delay_ms(DELAYTIME);
  }
  return (0);                                          /* end mainloop */
}
