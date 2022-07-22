#include <msp430fr6989.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define redLED BIT0 // Red at P1.0

#define FLAGS UCA1IFG // Contains the transmit & receive flags
#define RXFLAG UCRXIFG // Receive flag
#define TXFLAG UCTXIFG // Transmit flag
#define TXBUFFER UCA1TXBUF // Transmit buffer
#define RXBUFFER UCA1RXBUF // Receive buffer

/* main function
 * read result from joystick x-axis transmit with UART
 */
int main (void){
    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; // Enable GPIO pins
    P1DIR |= redLED; // Pins as output
    P1OUT &= ~redLED; // Red off

    // Initialize functions
    Initialize_ADC();
    Initialize_UART();

    unsigned int n = 1; // begin increment at 1
    for( ; ; ){

        ADC12CTL0 |= ADC12SC;   //start conversion

        while( ADC12BUSY == 0){};   // wait until bit is clear

        uart_write_uint16(n);   // transmit number
        uart_write_char('\n');  // new line
        uart_write_char('\r');  // carriage return

        uart_write_uint16(ADC12MEM0);   //transmit register value
        uart_write_char('\n');
        uart_write_char('\n');
        uart_write_char('\r');

        // delay loop
                int i;
                for( i = 0; i < 32000; i++){}
                n++; // increment number by 1
                P1OUT ^= redLED; // Toggle the red LED
    }

}

void Initialize_ADC() {

    // Divert the pins to analog functionality
    // X-axis: A10/P9.2, for A10 (P9DIR=x, P9SEL1=1, P9SEL0=1)
    P9SEL1 |= BIT2;
    P9SEL0 |= BIT2;

    // Turn on the ADC module
    ADC12CTL0 |= ADC12ON;

    // Turn off ENC (Enable Conversion) bit while modifying the configuration
    ADC12CTL0 &= ~ADC12ENC;

    //*************** ADC12CTL0 ***************
    // Set ADC12SHT0 (select the number of cycles that you determined)
    // 32 cycles
    ADC12CTL0 |= ADC12SHT0_3;

    //*************** ADC12CTL1 ***************
    // Set ADC12SHS (select ADC12SC bit as the trigger)
    // Set ADC12SHP bit
    // Set ADC12DIV (select the divider you determined)
    // divider is 1
    // Set ADC12SSEL (select MODOSC)
    ADC12CTL1 |= ADC12SHS_0 | ADC12SHP | ADC12DIV_0 | ADC12SSEL_0;

    //*************** ADC12CTL2 ***************
    // Set ADC12RES (select 12-bit resolution)
    // Set ADC12DF (select unsigned binary format)
    ADC12CTL2 |= ADC12RES_2 ;

    //*************** ADC12CTL3 ***************
    // Leave all fields at default values

    //*************** ADC12MCTL0 ***************
    // Set ADC12VRSEL (select VR+=AVCC, VR-=AVSS)
    // Set ADC12INCH (select channel A10)
    ADC12MCTL0 |= ADC12VRSEL_0 | ADC12INCH_10;

    // Turn on ENC (Enable Conversion) bit at the end of the configuration
    ADC12CTL0 |= ADC12ENC;

    return;
}

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

void uart_write_string( char *str){

    while( *str){
        uart_write_char( *str++);
    }
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
