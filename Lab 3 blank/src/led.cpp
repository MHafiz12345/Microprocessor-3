#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

/*
 * Initializes the LED pins (PD0, PD1, PD2, and PD3) as outputs
 * These pins will be used to display a binary number (0-15)
 */
void initLED() {
    // Configure PD0, PD1, PD2, and PD3 as outputs by setting the corresponding bits in DDRD
    DDRD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD2) | (1 << DDD3);
}

/*
 * Sets LEDs to display binary representation of input number
 * Only affects the lower 4 bits (PD0-PD3) while preserving upper 4 bits
 * 
 * PORTD = (PORTD & 0xF0) | (num & 0x0F);
 * - (PORTD & 0xF0): Keeps upper 4 bits of PORTD unchanged
 * - (num & 0x0F): Takes only lower 4 bits from num
 * - | combines them: upper bits from PORTD, lower bits from num
 */
void turnOnLEDWithChar(unsigned char num) {
    PORTD = (PORTD & 0xF0) | (num & 0x0F);
}
