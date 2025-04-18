#ifndef LED_H
#define LED_H

#include <avr/io.h>

// Initializes LED pins (PD0-PD3) as outputs
// This must be called once before using the LEDs
void initLED();

// Sets LEDs to display binary representation of input number
// Parameters:
//   num - Value to display (only lower 4 bits are used, 0-15)
// The function preserves the upper 4 bits of PORTD
// This must be implemented as a single line of code per lab requirements
void turnOnLEDWithChar(unsigned char num);

#endif
