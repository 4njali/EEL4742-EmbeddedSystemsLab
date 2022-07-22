// Timer_A continuous mode, with interrupt, flashes LEDs
#include <msp430fr6989.h>

#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7
#define but1 BIT1 // Button S1 at Port 1.1
#define but2 BIT2 // Button S2 at Port 1.2

void main(void) {
    WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

    P1DIR |= redLED; // Direct pin as output
    P9DIR |= greenLED; // Direct pin as output
    P1OUT &= ~redLED; // Turn LED Off
    P9OUT &= ~greenLED; // Turn LED Off

    // Configuring buttons with interrupt
    P1DIR &= ~(but1|but2); // 0: input
    P1REN |= (but1|but2); // 1: enable built-in resistors
    P1OUT |= (but1|but2);  // 1: built-in resistor is pulled up to Vcc
    P1IE  |= 1; // 1: enable interrupts
    P1IES |= 1; // 1: interrupt on falling edge
    P1IFG |= 0; // 0: clear the interrupt flags

    // Enable the global interrupt bit (call an intrinsic function)
    _enable_interrupts();

    // Infinite loop... the code waits here between interrupts
    for(;;) {}
    }

//******* Writing the ISR *******
#pragma vector = PORT1_VECTOR // Link the ISR to the vector
__interrupt void Port1_ISR() {
    // Detect button 1 (BUT1 in P1IFG is 1)
    if ((P1IFG & but1) == but1) {
    // Toggle the red LED
    P1OUT ^= redLED;
    // Clear BUT1 in P1IFG
    P1IFG &= ~but1;
    }
    // Detect button 2 (BUT2 in P1IFG is 1)
    if( (P1IFG & but2) == but2 ) {
    // Toggle the green LED
    P9OUT ^= greenLED;
    // Clear BUT2 in P1IFG
    P1IFG &= ~but2;
    }
}
