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
    char counter= 0;    // count 1 to 255
    char mystring[20];  // store any string

    WDTCTL = WDTPW | WDTHOLD;     // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5;         // Disable GPIO power-on default high-impedance mode

    // Configure LEDs and button
    P1DIR |= redLED;    P1OUT &= ~redLED;
    P9DIR |= greenLED;  P9OUT &= ~greenLED;
    P1DIR &= ~button; P1REN|=button; P1OUT|=button; // button, resistor, pullup

    // Configure ACLK to the 32 KHz crystal (function call)
    config_ACLK_to_32KHz_crystal();

    // Configure Timer_A
    // Use ACLK, divide by 1, up mode, clear TAR
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

    // Set timer period for 1 second delay
    TA0CCR0 = 32767;

    // Ensure flag is cleared at the start
    TA0CTL &= ~TAIFG;

    // Configure SMCLK to 8 MHz (used as SPI clock)
    CSCTL0 = CSKEY;                 // Unlock CS registers
    CSCTL3 &= ~(BIT4|BIT5|BIT6);    // DIVS=0
    CSCTL0_H = 0;                   // Relock the CS registers

    ////////////////////////////////////////////////////////////////////////////////////////////
    Graphics_Context g_sContext;        // Declare a graphic library context
    Crystalfontz128x128_Init();         // Initialize the display

    // Set the screen orientation
    Crystalfontz128x128_SetOrientation(0);

    // Initialize the context
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

    // Set background color
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_DARK_BLUE);

////////////////////////////////////////////////////////////////////////////////////////////
    int control = 1;    // status variable
    for( ; ; ){

        // when button is pressed
        if ((P1IN & button) == 0){
            // toggle LEDs
            P1OUT ^= redLED;
            P9OUT ^= greenLED;
            // clear screen
            Graphics_clearDisplay(&g_sContext);

            control = -control; // negate control
        }

        // when control is positive 1(initial value)
        if (control == 1) {
            // display logo on screen
            extern tImage  logo4BPP_UNCOMP;
            Graphics_drawImage(&g_sContext, &logo4BPP_UNCOMP, 0, 0);
            // reset counter to 0
            counter = 0;
        }
        else {  // when counter is negative 1

            // foreground color CORAL
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_CORAL);

            // filled rectangle
            tRectangle nose = {61, 45, 67, 70};
            Graphics_fillRectangle(&g_sContext, &nose);
            // outline circle
            Graphics_drawCircle(&g_sContext, 30, 50, 10);
            // horizontal line
            Graphics_drawLineH(&g_sContext, 87, 110, 33);

            // foreground color CYAN
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_CYAN);

            // filled circle
            Graphics_fillCircle(&g_sContext, 98, 50, 10);
            // outline rectangle
            tRectangle mouth = {35, 85, 93, 100};
            Graphics_drawRectangle(&g_sContext, &mouth);
            // horizontal line
            Graphics_drawLineH(&g_sContext, 17, 40, 33);

            // set timer display
            if (counter < 10){
                sprintf(mystring, "  %d ctr", counter);
            }
            else if ( counter <100){
                sprintf(mystring, " %d ctr", counter);
            }
            else{
                sprintf(mystring, "%d ctr", counter);
            }

            // Set font for timer
            GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
            // display timer string
            Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 92, OPAQUE_TEXT);

            // Set font for Title
            GrContextFontSet(&g_sContext, &g_sFontCm16);
            // Print title
            sprintf(mystring, "Clock Master");
            Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 13, OPAQUE_TEXT);

            // when TAIFG flag is raised
            while( (TA0CTL & TAIFG) == 0) {}
            counter++; // increment number by 1

            TA0CTL &= ~TAIFG; // Clear the flag
        }
    }
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
