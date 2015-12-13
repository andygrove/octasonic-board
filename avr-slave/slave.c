// #ifndef F_CPU
// #define F_CPU 16000000UL
// #endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ACK 0x7E

// #define BV(bit)               (1 << bit)
// #define set_bit(sfr, bit)     (_SFR_BYTE(sfr) |= BV(bit))  // old sbi()
// #define clear_bit(sfr, bit)   (_SFR_BYTE(sfr) &= ~BV(bit)) // old cbi()

void spi_init_slave (void)
{

    // clear_bit(DDRB, PB5); // SCK = input
    // set_bit(DDRB, PB4); // MISO = output
    // clear_bit(DDRB, PB3); // MOSI = input
    // clear_bit(DDRB, PB2); // SS = input

    DDRB=(1<<6);                                  //MISO as OUTPUT
    SPCR=(1<<SPE);                                //Enable SPI

    //set_bit(DDRB, PB0);

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
    spi_init_slave();                             //Initialize slave SPI
    //unsigned char data;
    while(1)
    {
        /*data =*/ spi_tranceiver(ACK);

        // if (data != 0) {
        //   set_bit(PORTB, PB0);
        // } else {
        //   clear_bit(PORTB, PB0);
        // }
    }
}
