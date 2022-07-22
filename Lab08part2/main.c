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

// The function transmits the byte
void uart_write_char(unsigned char ch){

    // Wait for any ongoing transmission to complete
    while ( (FLAGS & TXFLAG)==0 ) {}

    // Write the byte to the transmit buffer
    TXBUFFER = ch;
}

// The function transmits the whole 16-bit integer
void uart_write_uint16(unsigned int n){

    // Wait for any ongoing transmission to complete
    while ( (FLAGS & TXFLAG)==0 ) {}

        // Fifth digit: call get_digit_ascii(n,5) for ascii value
        if (n > 9999) {
            uart_write_char( get_digit_ascii(n,5)); // call uart_write_char to transmit the digit
       }
        // Fourth digit: call get_digit_ascii(n,4) for ascii value
        if (n > 999){
            uart_write_char( get_digit_ascii(n,4)); // call uart_write_char to transmit the digit
       }
        // Third digit: call get_digit_ascii(n,3) for ascii value
        if (n > 99){
            uart_write_char( get_digit_ascii(n,3)); // call uart_write_char to transmit the digit
       }
        // Second digit: call get_digit_ascii(n,2) for ascii value
        if (n > 9){
            uart_write_char( get_digit_ascii(n,2)); // call uart_write_char to transmit the digit
        }
        // First digit: call get_digit_ascii(n,1) for ascii value
        uart_write_char( get_digit_ascii(n,1)); // call uart_write_char to transmit the digit

}

// The function disassembles integer into digits and returns ascii value of digit
unsigned char get_digit_ascii(unsigned int n, unsigned int place){

    int power = 1; // 10^0
    int i;

    // increase power to fit place ex. 3rd place is 10^3 = 1000ths place
    for ( i = 1; i < place; i++){
        power = power*10; // multiply power by 10 until i = place

    }
    int digit = (n/power) % 10; // extracts digit from specified place

    unsigned char ascii = 48 + digit;

    return ascii; // ASCII value of digit
}

/* main function
 * transmit 16-int number 0 - 65535
 * flash red LED to show ongoing activity
 */
int main (void){

    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; // Enable GPIO pins
    P1DIR |= redLED; // Pins as output
    P9DIR |= greenLED;
    P1OUT &= ~redLED; // Red on
    P9OUT &= ~greenLED; // Green off

    Initialize_UART();

    unsigned int n = 65530; // start from 65530
    // infinite loop
    for(;;) {
            uart_write_uint16(n);   // transmit number
            uart_write_char('\n');  // new line
            uart_write_char('\r');  // carriage return

            // delay loop
            int i;
            for( i = 0; i < 32600; i++){}
            n++; // increment number by 1
            P1OUT ^= redLED; // Toggle the red LED

        }
}

