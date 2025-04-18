#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

// Initializes Timer1 for precise microsecond timing
// - Configures Timer1 in CTC mode with prescaler of 8
// - Sets up the timer but does not start it
// - Prepares for accurate 1μs timing intervals
// This must be called once before using delayUs()
void initTimer1();

// Creates a precise delay for the specified number of microseconds
// Parameters:
//   delay - Number of microseconds to wait (capped at 5000μs/5ms)
// Uses Timer1 to create accurate timing regardless of CPU operations
void delayUs(unsigned int delay);

// Initializes Timer0 for precise millisecond timing
// - Configures Timer0 in CTC mode with prescaler of 64
// - Sets up the timer but does not start it
// - Prepares for accurate 1ms timing intervals
// This must be called once before using delayMs()
void initTimer0();

// Creates a precise delay for the specified number of milliseconds
// Parameters:
//   delay - Number of milliseconds to wait
// Uses Timer0 to create accurate timing regardless of CPU operations
void delayMs(unsigned int delay);

#endif
