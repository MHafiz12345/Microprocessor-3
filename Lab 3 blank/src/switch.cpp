#include "switch.h"
#include <avr/io.h>      

/*
 * Initializes pin PB3 as an input with a pull-up resistor and enables
 * pin change interrupt to detect button presses
 */
void initSwitchPB3() {
    // Configure PB3 as an input by clearing its bit in the data direction register
    DDRB &= ~(1 << DDB3);
    
    // Enable the internal pull-up resistor on PB3
    // This keeps the pin HIGH when button is not pressed (button will pull it LOW when pressed)
    PORTB |= (1 << PORTB3);
    
    // Enable pin change interrupts for PCINT[7:0] group (which includes PB3/PCINT3)
    // PCIE0 bit in PCICR register enables interrupts for this entire group
    PCICR |= (1 << PCIE0);
    
    // Enable interrupt specifically for PB3 (which is also known as PCINT3)
    // Setting this bit means only changes on PB3 will trigger the interrupt
    PCMSK0 |= (1 << PCINT3);
}
