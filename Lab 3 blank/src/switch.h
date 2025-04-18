//----------------------------------------------------------------------//
// Author:   Muhammad Hafiz      
// Net ID:   muhammadhafiz      
// Date:     03 March 2025     
// Assignment: Lab 3
//----------------------------------------------------------------------//

#ifndef SWITCH_H
#define SWITCH_H

#include <avr/io.h>
#include <avr/interrupt.h>

// Initializes pin PB3 as an input with pull-up resistor and configures interrupts
// - Sets PB3 as input with internal pull-up resistor enabled
// - Enables pin change interrupts for PCINT0-7 group
// - Configures PCINT3 (PB3) to trigger the interrupt
// This must be called once before using the button
void initSwitchPB3();

#endif