#include <msp430fr6989.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define redLED BIT0 // Red at P1.0
#define greenLED BIT7 // Green at P9.7

#define FLAGS UCA1IFG // Contains the transmit & receive flags
#define RXFLAG UCRXIFG // Receive flag
#define TXFLAG UCTXIFG // Transmit flag
#define TXBUFFER UCA1TXBUF // Transmit buffer
#define RXBUFFER UCA1RXBUF // Receive buffer

// Configure UART to the popular configuration
// 9600 baud, 8-bit data, LSB first, no parity bits, 1 stop bit
// no flow control
// Initial clock: SMCLK @ 1.048 MHz with oversampling
void Initialize_UART(void){

    // Divert pins to UART functionality
    P3SEL1 &= ~(BIT4|BIT5);
    P3SEL0 |= (BIT4|BIT5);

    // Use SMCLK clock; leave other settings default
    UCA1CTLW0 |= UCSSEL_2;

    // Configure the clock dividers and modulators
    // UCBR=6, UCBRF=13, UCBRS=0x22, UCOS16=1 (oversampling)
    UCA1BRW = 6;
    UCA1MCTLW = UCBRS5|UCBRS1|UCBRF3|UCBRF2|UCBRF0|UCOS16;

    // Exit the reset state (so transmission/reception can begin)
    UCA1CTLW0 &= ~UCSWRST;
}

// The function transmits the byte
void uart_write_char(unsigned char ch){

    // Wait for any ongoing transmission to complete
    while ( (FLAGS & TXFLAG)==0 ) {}

    // Write the byte to the transmit buffer
    TXBUFFER = ch;
}

// The function returns the byte; if none received, returns NULL
unsigned char uart_read_char(void){

    unsigned char temp;

    // Return NULL if no byte received
    if( (FLAGS & RXFLAG) == 0)
        return NULL;

    // Otherwise, copy the received byte (clears the flag) and return it
    temp = RXBUFFER;

    return temp;
}

/* main function
 * transmit 0 to 9 with red LED toggle to indicate activity
 * read user input: 1 green LED on, 2 green LED off
 */
int main (void){

    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; // Enable GPIO pins
    P1DIR |= redLED; // Pins as output
    P9DIR |= greenLED;
    P1OUT &= ~redLED; // Red off
    P9OUT &= ~greenLED; // Green off

    Initialize_UART();

    unsigned char ch;
    // infinite loop
    for( ; ; ){
        //loop from characters 0 to 9
        for( ch = '0'; ch <= '9'; ch++){

            uart_write_char(ch);    // write character
            uart_write_char('\n');  // new line
            uart_write_char('\r');  // carriage return

            int i;
            //delay loop
            for( i = 0; i < 32500; i++){}

            P1OUT ^= redLED;    // toggle red LED

            unsigned char INch = uart_read_char(); // set INch as user input
            if( INch == '1'){   // if INch is character 1 turn green on
                P9OUT |= greenLED;  // set green on
            }
            else if ( INch == '2'){   // if INch is character 2 turn green off
                P9OUT &= ~greenLED; // set green off
            }

        }

    }
}
