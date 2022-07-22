// Own design
#include <msp430fr6989.h>

#define redLED BIT0     //red LED at P1.0
#define greenLED BIT7   //green LED at P9.7
#define BUT1 BIT1       //Button S1 at P1.1
#define BUT2 BIT2       //Button S2 at P1.2

void main(void){
    WDTCTL = WDTPW | WDTHOLD;   // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // Enable the GPIO pins
    volatile unsigned int i;
    // Configure and initialize LEDs
    P1DIR |= redLED;        // Direct pin as output
    P9DIR |= greenLED;      // Direct pin as output
    P1OUT &= ~redLED;       // red Off
    P9OUT &= ~greenLED;     // green Off

    // Configure buttons
// button 1 and red LED
    P1DIR &= ~BUT1;         // Direct pin as input
    P1REN |= BUT1;          // Enable built-in resistor
    P1OUT |= BUT1;         // Set resistor as pull-up

// button 2 and green LED
    P1DIR &= ~BUT2;         // Direct pin as input
    P1REN |= BUT2;          // Enable built-in resistor
    P1OUT |= BUT2;         // Set resistor as pull-up
    // Polling the button in an infinite loop
    for(;;) {

        // Fill the if-statement below...
        if ( (P1IN & (BUT1 | BUT2) ) == 0){    // both buttons pressed
            //LEDs blink fast
            for(i=0; i<10000; i++) {} //10000 cycle delay
                P1OUT ^= redLED;     //red toggle
                P9OUT ^= greenLED;   //green toggle
        }
        else if ((P1IN & (BUT1 | BUT2)) == BUT2){    // button 1 is pressed
            //red blinks, green is off
            for(i=0; i<30000; i++) {} //30000 cycle delay
                P1OUT ^= redLED;     // red toggle
                P9OUT &= ~greenLED;  // green off
        }
        else if ((P1IN & (BUT1 | BUT2)) == BUT1){    // button 2 is pressed
            //green blinks, red is off
            for(i=0; i<30000; i++) {} //30000 cycle delay
                P9OUT ^= greenLED;   // green toggle
                P1OUT &= ~redLED;    // red off
        }
        else {    // no button is pressed
            P1OUT |= redLED;      // red on
            P9OUT |= greenLED;    // green on
        }
    }
}
