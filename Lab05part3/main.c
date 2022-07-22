// Sample code that prints 430 on the LCD monitor
#include <msp430fr6989.h>

//**********************************
// Configures ACLK to 32 KHz crystal
void config_ACLK_to_32KHz_crystal() {
// By default, ACLK runs on LFMODCLK at 5MHz/128 = 39 KHz

// Reroute pins to LFXIN/LFXOUT functionality
    PJSEL1 &= ~BIT4; // set to 0
    PJSEL0 |= BIT4;    // set to 1

    // Wait until the oscillator fault flags remain cleared
    CSCTL0 = CSKEY; // Unlock CS registers
    do {
        CSCTL5 &= ~LFXTOFFG; // Local fault flag
        SFRIFG1 &= ~OFIFG; // Global fault flag
    } while((CSCTL5 & LFXTOFFG) != 0);

    CSCTL0_H = 0; // Lock CS registers
    return;
}

#define redLED BIT0 // Red at P1.0
#define greenLED BIT7 // Green at P9.7
#define but1 BIT1 // Button S1 at P1.1
#define but2 BIT2 // Button S2 at P1.2

void Initialize_LCD();

// The array has the shapes of the digits (0 to 9)
// Complete this array...
const unsigned char LCD_Num[10] = {0xFC, 0x60, 0xDB, 0xF3, 0x67, 0xB7, 0xBF, 0xE0, 0xFF, 0xF7};

int main(void) {

    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; // Enable GPIO pins
    P1DIR |= redLED; // Pins as output
    P9DIR |= greenLED;
    P1OUT &= ~redLED; // Red on
    P9OUT &= ~greenLED; // Green off

    // Configure buttons
    P1DIR &= ~but1;         // Direct pin as input
    P1REN |= but1;          // Enable built-in resistor
    P1OUT |= but1;         // Set resistor as pull-up

    P1DIR &= ~but2;         // Direct pin as input
    P1REN |= but2;          // Enable built-in resistor
    P1OUT |= but2;         // Set resistor as pull-up


    // Configure ACLK to the 32 KHz crystal (function call)
    config_ACLK_to_32KHz_crystal();

    // Configure Timer_A
    // Use ACLK, divide by 1, up mode, clear TAR
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

    // Set timer period for 1 second delay
    TA0CCR0 = 32767;

    // Ensure flag is cleared at the start
    TA0CTL &= ~TAIFG;

    // Initializes the LCD_C module
    Initialize_LCD();

    // The line below can be used to clear all the segments
    LCDCMEMCTL = LCDCLRM; // Clears all the segments

    // Display Integer
    //display_num_LCD(23);

    volatile unsigned int n = 0;
    int control = 1;

    display_num_LCD(n);
    // Flash the red and green LEDs
    for(;;) {
        // when button S1 is pressed
        if ((P1IN & but1) == 0){
            P1OUT ^= redLED;    //toggle
            P9OUT ^= greenLED; //toggle
            TA0CTL ^= MC_1; //clock toggle

            control = -control; // negate control so line 96 does not run
        }

        // when button S2 is pressed
        if ((P1IN & but2) == 0){
            n = 0; // reset timer to 0
            LCDCMEMCTL = LCDCLRM; // Clears all the segments
        }
        display_num_LCD(n); // display n to LCD

        // when control is positive 1 only
        if (control == 1) {
            // when TAIFG flag is raised
            while( (TA0CTL & TAIFG) == 0) {}
            n++; // increment number by 1

            if (n == 0) {
                LCDCMEMCTL = LCDCLRM; // Clears all the segments
            }

            P9OUT |= greenLED; // Toggle the red LED
            TA0CTL &= ~TAIFG; // Clear the flag
        } else {
            int i;
            // delay loop
            for( i = 0; i < 30000; i++){
                // wait here before checking buttons
            }
        }
    }

    return 0;

}

//**********************************************************
// Initializes the LCD_C module
// *** Source: Function obtained from MSP430FR6989’s Sample Code ***
void Initialize_LCD() {
    PJSEL0 = BIT4 | BIT5; // For LFXT

    // Initialize LCD segments 0 - 21; 26 - 43
    LCDCPCTL0 = 0xFFFF;
    LCDCPCTL1 = 0xFC3F;
    LCDCPCTL2 = 0x0FFF;

    // Configure LFXT 32kHz crystal
    CSCTL0_H = CSKEY >> 8; // Unlock CS registers
    CSCTL4 &= ~LFXTOFF; // Enable LFXT

    do {
        CSCTL5 &= ~LFXTOFFG; // Clear LFXT fault flag
        SFRIFG1 &= ~OFIFG;

    }while (SFRIFG1 & OFIFG); // Test oscillator fault flag

    CSCTL0_H = 0; // Lock CS registers

    // Initialize LCD_C
    // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

    // VLCD generated internally,
    // V2-V4 generated internally, v5 to ground
    // Set VLCD voltage to 2.60v
    // Enable charge pump and select internal reference for it
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;
    LCDCCPCTL = LCDCPCLKSYNC; // Clock synchronization enabled
    LCDCMEMCTL = LCDCLRM; // Clear LCD memory

    //Turn LCD on
    LCDCCTL0 |= LCDON;

    return;
}

// Display the digit in the correct place
void display_num_LCD(unsigned int n){
    // no display unless number has 5 digits
    if (n > 9999) { //10,000ths place 5th digit
       LCDM6 = get_LCD_digit(n,5);
   }
    // no display unless number has 4 digits
    if (n > 999){ //1000ths place 4th digit
       LCDM4 = get_LCD_digit(n,4);
   }
    // no display unless number has 3 digits
    if (n > 99){ //100ths place 3rd digit
       LCDM19 = get_LCD_digit(n,3);
   }
    // no display unless number has 2 digits
    if (n > 9){ //10ths place 2nd digit
       LCDM15 = get_LCD_digit(n,2); // display 2nd digit at LCDM15
    }
    // Ones PLace right most
    LCDM8 = get_LCD_digit(n,1); // display 1st digit at LCDM8


}

// Extract digit from n at specified place
int get_LCD_digit(unsigned int n, unsigned int place){

    int power = 1; // 10^0
    int i;
    // increase power to fit place ex. 3rd place is 10^3 = 1000ths place
    for ( i = 1; i < place; i++){
        power = power*10; // multiply power by 10 until i = place

    }
    int digit = (n/power) % 10; // extracts digit from specified place


    return LCD_Num[digit]; // return shape of digit
}
