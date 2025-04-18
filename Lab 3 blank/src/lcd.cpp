//----------------------------------------------------------------------//
// Author:   Muhammad Hafiz      
// Net ID:   muhammadhafiz      
// Date:     03 March 2025     
// Assignment: Lab 3
//----------------------------------------------------------------------//

#include "lcd.h"
#include "timer.h"
#include <util/delay.h>
#include <string.h>

// Initializes all pins related to the LCD as outputs
// - PORTA0-3: Data pins (DB4-DB7)
// - PORTB4: Enable pin (E)
// - PORTB6: Register Select pin (RS)
// This must be called before any LCD operations
void initLCDPins(){
    // Set PORTA0-3 as outputs for data pins DB4-DB7
    DDRA |= (1<<DDA0) | (1<<DDA1) | (1<<DDA2) | (1<<DDA3);
    // Set PORTB4 (E) and PORTB6 (RS) as outputs for control pins
    DDRB |= (1<<DDB4) | (1<<DDB6);
}

// Sends 4 bits of command data to the LCD
// Parameters:
//   data - The 4-bit command (only lower 4 bits are used)
//   delay - Time in microseconds to wait after sending command
// This function handles the proper signaling sequence:
// 1. Places data on PORTA0-3
// 2. Sets RS low for command mode
// 3. Pulses the enable pin
// 4. Waits for the specified delay
void fourBitCommandWithDelay(unsigned char data, unsigned int delay){
    // Preserve upper 4 bits of PORTA and set lower 4 bits from data
    PORTA = (PORTA & 0xF0) | (data & 0x0F);
    // Set RS low (command mode)
    PORTB &= ~(1<<PORTB6);
    // Set Enable pin high
    PORTB |= (1<<PORTB4);
    // Short delay for LCD to register the enable signal
    delayUs(1);
    // Set Enable pin low to latch the data
    PORTB &= ~(1<<PORTB4);
    // Wait for the specified delay time
    delayUs(delay);
}

// Sends a full 8-bit command to the LCD in two 4-bit transmissions
// Parameters:
//   command - The 8-bit command byte to send
//   delay - Time in microseconds to wait after sending command
// Since the LCD is in 4-bit mode, each byte is sent as two nibbles:
// 1. First sends the upper 4 bits (shifted right by 4)
// 2. Then sends the lower 4 bits
void eightBitCommandWithDelay(unsigned char command, unsigned int delay){
    // Send upper 4 bits of command first
    fourBitCommandWithDelay(command >> 4, delay);
    // Send lower 4 bits of command
    fourBitCommandWithDelay(command, delay);
}

// Writes a character to the LCD display
// Parameters:
//   character - ASCII character to display
// Similar to command functions but sets RS high for data mode
// Uses 46μs delay as specified in the datasheet for character writes
void writeCharacter(unsigned char character){
    // Send upper 4 bits of character
    PORTA = (PORTA & 0xF0) | ((character & 0xF0) >> 4);
    // Set RS high (data mode)
    PORTB |= (1<<PORTB6);
    // Pulse Enable pin
    PORTB |= (1<<PORTB4);
    delayUs(1);
    PORTB &= ~(1<<PORTB4);
    
    // Send lower 4 bits of character
    PORTA = (PORTA & 0xF0) | (character & 0x0F);
    // Pulse Enable pin again
    PORTB |= (1<<PORTB4);
    delayUs(1);
    PORTB &= ~(1<<PORTB4);
    
    // Standard character write delay
    delayUs(46);
}

// Writes a complete string to the LCD
// Parameters:
//   string - Null-terminated string to display
// Iterates through the string, sending each character until null terminator
void writeString(const char *string){
    // Process each character until null terminator
    while (*string != '\0') {
        writeCharacter(*string);
        string++;  // Move to next character
    }
}

// Moves the cursor to a specific position on the LCD
// Parameters:
//   x - Row (0 for first row, 1 for second row)
//   y - Column (0-15, depending on display width)
// This also enables the cursor for visibility
void moveCursor(unsigned char x, unsigned char y){
    if (x == 0) {
        // First row: Address starts at 0x80
        eightBitCommandWithDelay(0x80 + y, 100);
        // Enable cursor
        eightBitCommandWithDelay(0x0F, 100);
    } else {
        // Second row: Address starts at 0xC0
        eightBitCommandWithDelay(0xC0 + y, 100);
        // Enable cursor
        eightBitCommandWithDelay(0x0F, 100);
    }
}

// Initializes the LCD according to the procedure in the datasheet
// This sequence configures the LCD in 4-bit mode, sets up display parameters,
// and prepares it for character display
// This must be called after initLCDPins and before any other LCD functions
void initLCDProcedure(){
    // Initial wait after power-on
    delayMs(15);

    // Special initialization sequence for 4-bit mode
    // Sending 0x3 three times followed by 0x2 as specified in datasheet
    fourBitCommandWithDelay(0x3, 4100);
    fourBitCommandWithDelay(0x3, 100);
    fourBitCommandWithDelay(0x3, 100);
    
    // Switch to 4-bit mode
    fourBitCommandWithDelay(0x2, 100);

    // Function set: 4-bit mode, 2 lines, 5x7 font
    eightBitCommandWithDelay(0x28, 53); 

    // Display off
    eightBitCommandWithDelay(0x08, 53);

    // Clear display (needs longer delay)
    eightBitCommandWithDelay(0x01, 1640);

    // Entry Mode Set: Increment cursor, no display shift
    eightBitCommandWithDelay(0x06, 53);

    // Display on, cursor off, blinking off
    eightBitCommandWithDelay(0x0C, 53);
}

// Complete LCD initialization
// Sets up pins and runs the initialization procedure
// This is the main function to call to prepare the LCD for use
void initLCD(){
    initLCDPins();    // Configure hardware pins
    initLCDProcedure(); // Initialize the LCD controller
}