#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// Global counters for tracking elapsed time in interrupts
volatile unsigned int ms_counter = 0;  // Counts milliseconds
volatile unsigned int us_counter = 0;  // Counts microseconds

/* 
 * Initializes Timer0 for precise millisecond timing
 * Uses CTC mode with a 64 prescaler
 */
void initTimer0() {
    // Configure Timer0 in CTC mode (Clear Timer on Compare Match)
    TCCR0A &= ~(1 << WGM00);  // Clear bit WGM00
    TCCR0A |= (1 << WGM01);   // Set bit WGM01
    TCCR0B &= ~(1 << WGM02);  // Clear bit WGM02
    
    // Set compare value for 1ms intervals
    // With 16MHz clock and 64 prescaler: 16,000,000 / 64 = 250,000Hz
    // To get 1ms: 250,000Hz / 1000Hz = 250 ticks, but count from 0 so use 249
    OCR0A = 249;
    
    // Ensure timer is stopped initially (clear all clock select bits)
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
    
    // Enable interrupt on compare match with OCR0A
    TIMSK0 |= (1 << OCIE0A);
}

/* 
 * Initializes Timer1 for precise microsecond timing
 * Uses CTC mode with an 8 prescaler for high resolution
 */
void initTimer1() {
    // Configure Timer1 in CTC mode 
    TCCR1A &= ~((1 << WGM11) | (1 << WGM10));  // Clear WGM11 and WGM10
    TCCR1B |= (1 << WGM12);   // Set WGM12
    TCCR1B &= ~(1 << WGM13);  // Clear WGM13
    
    // Set compare value for 1μs intervals
    // With 16MHz clock and 8 prescaler: 16,000,000 / 8 = 2,000,000Hz
    // Each tick takes 0.5μs, so 2 ticks are needed for 1μs
    // Count sequence: 0,1 (2 values) so OCR1A is set to 1
    OCR1A = 1;
    
    // Ensure timer is stopped initially (clear all clock select bits)
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    
    // Enable interrupt on compare match with OCR1A
    TIMSK1 |= (1 << OCIE1A);
}

/* 
 * Creates a precise delay for the specified number of milliseconds
 * using Timer0 in CTC mode
 */
void delayMs(unsigned int delay) {
    // Reset the counter before starting
    ms_counter = 0;
    
    // Clear the timer counter register
    TCNT0 = 0;
    
    // Start Timer0 with prescaler 64 (set CS01 and CS00, clear CS02)
    TCCR0B |= (1 << CS01) | (1 << CS00);
    TCCR0B &= ~(1 << CS02);
    
    // Wait until the counter reaches the requested number of milliseconds
    while (ms_counter < delay);
    
    // Stop the timer by clearing all clock select bits
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}

/* 
 * Creates a precise delay for the specified number of microseconds
 * using Timer1 in CTC mode (works up to 5000μs)
 */
void delayUs(unsigned int delay) {
 
    // Reset the counter before starting
    us_counter = 0;
    
    // Clear the timer counter register
    TCNT1 = 0;
    
    // Start Timer1 with prescaler 8 (set CS11, clear CS12 and CS10)
    TCCR1B |= (1 << CS11);
    TCCR1B &= ~((1 << CS12) | (1 << CS10));
    
    // Wait until the counter reaches the requested number of microseconds
    while (us_counter < delay);
    
    // Stop the timer by clearing all clock select bits
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

// Interrupt Service Routine that triggers when Timer0 reaches the compare value
// Increments the millisecond counter each time it triggers (every 1ms)
ISR(TIMER0_COMPA_vect) {
    ms_counter++;
}

// Interrupt Service Routine that triggers when Timer1 reaches the compare value
// Increments the microsecond counter each time it triggers (every 1μs)
ISR(TIMER1_COMPA_vect) {
    us_counter++;
}
