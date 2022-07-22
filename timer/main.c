// Code to print to the LCD pixel display on the Educational BoosterPack

#include "msp430fr6989.h"
#include "Grlib/grlib/grlib.h"          // Graphics library (grlib)
#include "LcdDriver/lcd_driver.h"       // LCD driver
#include <stdio.h>

#define redLED BIT0
#define greenLED BIT7
#define button BIT1

void main(void)
{
    char counter= 250;
    char mystring[20];
    unsigned int n;

    WDTCTL = WDTPW | WDTHOLD;     // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5;         // Disable GPIO power-on default high-impedance mode

    P1DIR |= redLED;    P1OUT &= ~redLED;
    P9DIR |= greenLED;  P9OUT &= ~greenLED;
    P1DIR &= ~button; P1REN|=button; P1OUT|=button; // button, resistor, pullup

    // Configure SMCLK to 8 MHz (used as SPI clock)
    CSCTL0 = CSKEY;                 // Unlock CS registers
    CSCTL3 &= ~(BIT4|BIT5|BIT6);    // DIVS=0
    CSCTL0_H = 0;                   // Relock the CS registers

    // Configure ACLK to the 32 KHz crystal (function call)
    config_ACLK_to_32KHz_crystal();

    // Configure Timer_A
    // Use ACLK, divide by 1, up mode, clear TAR
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

    // Set timer period for 1 second delay
    TA0CCR0 = 32767;

    // Ensure flag is cleared at the start
    TA0CTL &= ~TAIFG;

////////////////////////////////////////////////////////////////////////////////////////////
Graphics_Context g_sContext;        // Declare a graphic library context
Crystalfontz128x128_Init();         // Initialize the display

// Set the screen orientation
Crystalfontz128x128_SetOrientation(0);

// Initialize the context
Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

// Set background and foreground colors
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_DARK_BLUE);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_CORAL);

// Set the default font for strings
GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

// Clear the screen
Graphics_clearDisplay(&g_sContext);
////////////////////////////////////////////////////////////////////////////////////////////


for ( ; ; ){

    if (counter < 10){
        sprintf(mystring, "  %d", counter);
    }
    else if ( counter <100){
        sprintf(mystring, " %d", counter);
    }
    else{
        sprintf(mystring, "%d", counter);
    }

    Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 80, OPAQUE_TEXT);
    // when TAIFG flag is raised
    while( (TA0CTL & TAIFG) == 0) {}
    counter++; // increment number by 1

    TA0CTL &= ~TAIFG; // Clear the flag

}
/*    // Clear the screen
    Graphics_clearDisplay(&g_sContext);

    Graphics_drawStringCentered(&g_sContext, "Welcome to", AUTO_STRING_LENGTH, 64, 30, OPAQUE_TEXT);

sprintf(mystring, "EEL 4742 Lab!");
Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 55, OPAQUE_TEXT);

n = 1234;
sprintf(mystring, "%d", n);
Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 80, OPAQUE_TEXT);
*/




}

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
