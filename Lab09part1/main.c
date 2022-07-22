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

/* main function
 * read manufacturer ID and device ID registers
 */
int main (void){

    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; // Enable GPIO pins
    P1DIR |= redLED; // Pins as output
    P1OUT &= ~redLED; // Red off

    Initialize_UART();
    Initialize_I2C();

    unsigned int device_data;       // save device id value
    unsigned int manufacturer_data; // save manuf. id value

    unsigned int n = 1; // counter starts with 1
    // infinite loop
    for( ; ; ){

        i2c_read_word( 0x44 , 0x7E, &device_data);          // read device id register
        i2c_read_word( 0x44 , 0x7F, &manufacturer_data);    // read manuf. id register

        uart_write_uint16(n);   // transmit counter number
        uart_write_char('\n');  // new line
        uart_write_char('\r');  // carriage return

        uart_write_uint16(device_data); // print device value
        uart_write_char('\n');  // new line
        uart_write_char('\r');  // carriage return

        uart_write_uint16(manufacturer_data); // print manuf. value
        uart_write_char('\n');  // new line
        uart_write_char('\n');  // new line
        uart_write_char('\r');  // carriage return


        // delay loop
        int i;
        for( i = 0; i < 32600; i++){}
        n++; // increment number by 1
        P1OUT ^= redLED; // Toggle the red LED

    }
}

// Configure eUSCI in I2C master mode
void Initialize_I2C(void) {

    // Enter reset state before the configuration starts...
    UCB1CTLW0 |= UCSWRST;

    // Divert pins to I2C functionality
    P4SEL1 |= (BIT1|BIT0);
    P4SEL0 &= ~(BIT1|BIT0);

    // Keep all the default values except the fields below...
    // (UCMode 3:I2C) (Master Mode) (UCSSEL 1:ACLK, 2,3:SMCLK)
    UCB1CTLW0 |= UCMODE_3 | UCMST | UCSSEL_3;

    // Clock divider = 8 (SMCLK @ 1.048 MHz / 8 = 131 KHz)
    UCB1BRW = 8;

    // Exit the reset mode
    UCB1CTLW0 &= ~UCSWRST;
}

////////////////////////////////////////////////////////////////////
// Read a word (2 bytes) from I2C (address, register)
int i2c_read_word(unsigned char i2c_address, unsigned char i2c_reg, unsigned int * data) {

    unsigned char byte1, byte2;

    // Initialize the bytes to make sure data is received every time
    byte1 = 111;
    byte2 = 111;

    //********** Write Frame #1 ***************************
    UCB1I2CSA = i2c_address; // Set I2C address
    UCB1IFG &= ~UCTXIFG0;
    UCB1CTLW0 |= UCTR; // Master writes (R/W bit = Write)
    UCB1CTLW0 |= UCTXSTT; // Initiate the Start Signal

    while ((UCB1IFG & UCTXIFG0) ==0) {}
        UCB1TXBUF = i2c_reg; // Byte = register address

    while((UCB1CTLW0 & UCTXSTT)!=0) {}

    if(( UCB1IFG & UCNACKIFG )!=0) return -1;
    UCB1CTLW0 &= ~UCTR; // Master reads (R/W bit = Read)
    UCB1CTLW0 |= UCTXSTT; // Initiate a repeated Start Signal

    //****************************************************
    //********** Read Frame #1 ***************************
    while ( (UCB1IFG & UCRXIFG0) == 0) {}
    byte1 = UCB1RXBUF;

    //****************************************************
    //********** Read Frame #2 ***************************
    while((UCB1CTLW0 & UCTXSTT)!=0) {}
        UCB1CTLW0 |= UCTXSTP; // Setup the Stop Signal

    while ( (UCB1IFG & UCRXIFG0) == 0) {}
        byte2 = UCB1RXBUF;

    while ( (UCB1CTLW0 & UCTXSTP) != 0) {}

    //****************************************************
    // Merge the two received bytes
    *data = ( (byte1 << 8) | (byte2 & 0xFF) );

    return 0;
}

////////////////////////////////////////////////////////////////////
// Write a word (2 bytes) to I2C (address, register)
int i2c_write_word(unsigned char i2c_address, unsigned char i2c_reg, unsigned int data) {

    unsigned char byte1, byte2;
    byte1 = (data >> 8) & 0xFF; // MSByte
    byte2 = data & 0xFF; // LSByte

    UCB1I2CSA = i2c_address; // Set I2C address
    UCB1CTLW0 |= UCTR; // Master writes (R/W bit = Write)
    UCB1CTLW0 |= UCTXSTT; // Initiate the Start Signal

    while ((UCB1IFG & UCTXIFG0) ==0) {}
        UCB1TXBUF = i2c_reg; // Byte = register address

    while((UCB1CTLW0 & UCTXSTT)!=0) {}

    //********** Write Byte #1 ***************************
    UCB1TXBUF = byte1;
    while ( (UCB1IFG & UCTXIFG0) == 0) {}

    //********** Write Byte #2 ***************************
    UCB1TXBUF = byte2;

    while ( (UCB1IFG & UCTXIFG0) == 0) {}
        UCB1CTLW0 |= UCTXSTP;

    while ( (UCB1CTLW0 & UCTXSTP) != 0) {}

    return 0;
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
