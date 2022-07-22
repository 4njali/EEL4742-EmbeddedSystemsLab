#include <msp430fr6989.h>

//**********************************
// Configures ACLK to 32 KHz crystal
void config_ACLK_to_32KHz_crystal() {
// By default, ACLK runs on LFMODCLK at 5MHz/128 = 39 KHz

// Reroute pins to LFXIN/LFXOUT functionality
    PJSEL1 &= ~BIT4;
    PJSEL0 |= BIT4;

    // Wait until the oscillator fault flags remain cleared
    CSCTL0 = CSKEY; // Unlock CS registers

    do {
        CSCTL5 &= ~LFXTOFFG; // Local fault flag
        SFRIFG1 &= ~OFIFG; // Global fault flag
    } while((CSCTL5 & LFXTOFFG) != 0);

    CSCTL0_H = 0; // Lock CS registers
    return;
}

#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7
#define but1 BIT1 // Button S1 at P1.1
#define but2 BIT2 // Button S2 at P1.2

// Flashing the LED with Timer_A, up mode, via polling
void main(void) {

    WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

    P1DIR |= redLED; // Direct pin as output
    P9DIR |= greenLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED Off
    P9OUT &= ~greenLED; // Turn LED Off

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
    // Use ACLK, divide by 1, continuous mode, clear TAR
    TA0CTL = TASSEL_1 | ID_0 | MC_2 | TACLR;

    // Set timer period
    //TA0CCR0 = 35767;

    // Ensure flag is cleared at the start
    TA0CTL &= ~TAIFG;

    // Infinite loop
        for(;;) {
            // button 1 and 2 pressed
            if ( (P1IN & (but1 | but2) ) == 0){
                // toggle both LEDs
                while( (TA0CTL & TAIFG) == 0) {}
                    P1OUT ^= redLED; // Toggle the red LED
                    P9OUT ^= greenLED;
                    TA0CTL &= ~TAIFG; // Clear the flag
            }
            // button 1 pressed
            else if ((P1IN & (but1 | but2)) == but2){
                // toggle red while green is off
                while( (TA0CTL & TAIFG) == 0) {}
                    P1OUT ^= redLED; // Toggle the red LED
                    P9OUT &= ~greenLED;
                    TA0CTL &= ~TAIFG; // Clear the flag
            }
            // button 2 pressed
            else if ((P1IN & (but1 | but2)) == but1){
                // toggle green while red is off
                while( (TA0CTL & TAIFG) == 0) {}
                    P1OUT &= ~redLED;
                    P9OUT ^= greenLED;// Toggle the green LED
                    TA0CTL &= ~TAIFG; // Clear the flag
            }
            // nothing is pressed
            else {
                    P1OUT &= ~redLED;  // Turn red LED off
                    P9OUT &= ~greenLED;// green off
            }
        }

    }
