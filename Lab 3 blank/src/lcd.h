#ifndef LCD_H
#define LCD_H

#include <avr/io.h>

// Initializes the LCD by setting up pins and running initialization procedure
void initLCD();

// Sends a 4-bit command to the LCD with specified delay
void fourBitCommandWithDelay(unsigned char data, unsigned int delay);

// Sends a full 8-bit command to the LCD with specified delay
void eightBitCommandWithDelay(unsigned char command, unsigned int delay);

// Writes a single character to the LCD
void writeCharacter(unsigned char character);

// Writes a string of characters to the LCD
void writeString(const char *string);

// Moves the cursor to a specific position (x=row, y=column)
void moveCursor(unsigned char x, unsigned char y);

// Sets up the pins used to communicate with the LCD
void initLCDPins();

// Runs the LCD initialization sequence according to the datasheet
void initLCDProcedure();

#endif
