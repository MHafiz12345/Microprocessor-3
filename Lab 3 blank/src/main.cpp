//----------------------------------------------------------------------//
// Author:   Muhammad Hafiz      
// Net ID:   muhammadhafiz      
// Date:     03 March 2025     
// Assignment: Lab 3
//----------------------------------------------------------------------//

#include <avr/io.h>         
#include <avr/interrupt.h>  
#include "led.h"            
#include "switch.h"         
#include "timer.h"
#include "lcd.h"         

/* 
 * States for button handling state machine
 */
typedef enum {
    WAIT_PRESS,         // Waiting for button to be pressed
    DEBOUNCE_PRESS,     // Wait a bit after button press to avoid bounce
    WAIT_RELEASE,       // Waiting for button to be released
    DEBOUNCE_RELEASE    // Wait a bit after release to avoid bounce
} State;

// Timer counter defined in timer.cpp
extern volatile unsigned int ms_counter;

// How fast the LEDs will update (milliseconds)
unsigned int blinkSpeed = 200;        

// Current value to display on LEDs (0-15)
unsigned char counterValue = 0;       

// Start in the WAIT_PRESS state
volatile State buttonState = WAIT_PRESS;       

// Flag to track if the system is in fast mode
bool isFastMode = false;

int main() {
    // Set up all hardware
    initLED();         // Configure LED pins as outputs
    initSwitchPB3();   // Configure the button with pull-up and interrupt
    initTimer0();      // Set up Timer0 for millisecond delays
    initTimer1();      // Set up Timer1 for microsecond delays (used by LCD)
    sei();             // Enable global interrupts
    
    initLCD();         // Initializes the LCD, sets up necessary pins and configurations.
    
    // Display initial messages on LCD
    moveCursor(0, 0);  // First row
    writeString("Blinking rate =");
    moveCursor(1, 0);  // Second row
    writeString("Slow");
    
    // Main program loop
    while (1) {
        // Update LEDs to show current counter value
        turnOnLEDWithChar(counterValue);
        
        // Button state machine - only handle the debounce states in main loop
        switch (buttonState) {
            case DEBOUNCE_PRESS:
                // Wait 50ms to avoid false triggers from button bounce
                delayMs(50);
                
                // Transition to wait for release state
                buttonState = WAIT_RELEASE;
                break;
                
            case DEBOUNCE_RELEASE:
                // Wait 50ms for button release to stabilize
                delayMs(50);
                
                // Toggle speed mode
                isFastMode = !isFastMode;
                
                // Update speed based on mode
                blinkSpeed = isFastMode ? 100 : 200;
                
                // Update LCD with new speed setting
                moveCursor(1, 0);  // Go to start of second line
                writeString(isFastMode ? "Fast" : "Slow");
                
                // Ready for next press
                buttonState = WAIT_PRESS;
                break;
                
            // Other states handled by ISR
            default:
                break;
        }
        
        // Wait according to current speed setting
        delayMs(blinkSpeed);
        
        // Increment counter and wrap around after 15
        counterValue++;
        if (counterValue > 0x0F) {  // 0x0F is hexadecimal for 15
            counterValue = 0;
        }
    }
    
    return 0;  // Never reached due to infinite loop
}

/* 
 * Interrupt Service Routine for pin change on PCINT0-7
 * Handles state transitions when button is pressed/released
 */
ISR(PCINT0_vect) {
    switch (buttonState) {
        case WAIT_PRESS:
            // If button is pressed (PB3 reads LOW), move to debounce state
                buttonState = DEBOUNCE_PRESS;
            break;
            
        case WAIT_RELEASE:
            // If button is released (PB3 reads HIGH), move to debounce release state
                buttonState = DEBOUNCE_RELEASE;
            break;
            
        default:
            // Don't change state during debounce periods
            break;
    }
}