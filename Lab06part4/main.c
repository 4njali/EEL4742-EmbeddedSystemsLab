// Timer_A continuous mode, with interrupt, cycle through red LED brightness
#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7
#define PWM_PIN BIT0

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

void main(void) {
   WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
   PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

   // configure LEDs
   P1DIR |= redLED;
   P9DIR |= greenLED;
   P1OUT &= ~redLED;
   P9OUT &= ~greenLED;

   // Divert pin to TA0.1 functionality
   P1DIR  |= PWM_PIN;  // P1DIR bit = 1
   P1SEL1 |= ~PWM_PIN; // P1SEL1 bit = 0
   P1SEL0 |= PWM_PIN; // P1SEL0 bit = 1

   // Configure ACLK to the 32 KHz crystal
   config_ACLK_to_32KHz_crystal();

   // (ACLK @ 32 KHz) (Divide by 1) (Up mode)
   TA0CCR0 = (33-1); // @ 32 KHz --> 0.001 seconds (1000 Hz)
   TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

   // Configuring Channel 1 for PWM
   TA0CCTL1 |= OUTMOD_7; // Output pattern: Reset/set
   TA0CCR1 = 0; // Modify this value between 0 and 32 to adjust the brightness level

   // Configure Channel 0 for up mode with interrupt
   TA1CCR0 = 32000;   // Fill to get 1 second @ 32 KHz
   TA1CCTL0 |= CCIE;   // Enable Channel 0 CCIE bit
   TA1CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit

   // Timer_A: ACLK, div by 1, up mode, clear TAR (leaves TAIE=0)
   TA1CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

   // Engage a low-power mode
       _low_power_mode_3();

   // Enable the global interrupt bit (call an intrinsic function)
   _enable_interrupts();

   // Infinite loop... the code waits here between interrupts
   for(;;) {}
   }


//******* Writing the ISR *******

#pragma vector = TIMER1_A0_VECTOR // Link the ISR to the vector
__interrupt void T1A0_ISR() {
   unsigned volatile int count;
   P9OUT ^= greenLED;

   ++count;
   if((count%2) == 0){
       TA0CCR1 = TA0CCR1 + 5;  // increase brightness by 5
   }
   if(TA0CCR1 > 30){
           TA0CCR1 = 0;   // reset to 0 brightness
       }


   // Hardware clears the flag (CCIFG in TA1CCTL0)
}
