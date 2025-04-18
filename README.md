# Lab 3: LCD Display with Timer-Based Speed Feedback

![LCD Display with Speed Feedback](https://github.com/user-attachments/assets/5db5a04e-61e0-4f03-866b-60dd7a80ef85)

## 🔧 Overview

This project builds on previous labs by adding an **LCD interface** alongside LED output and button input. A 4-bit binary counter is displayed on both **LEDs** (PD0–PD3) and an **LCD screen**, with a message indicating the current **blink speed**. A pushbutton toggles between "Fast" and "Slow" modes. Timing is managed by two hardware timers: one for **millisecond delays** and another for **microsecond delays**, used for precise LCD communication.

This lab integrates several essential embedded system concepts: **bit-level LCD control**, **real-time feedback via display**, and **interrupt-driven state machines**.

## 🎯 Project Behavior

* **Binary counter** (0–15) shown via **4 LEDs** and **LCD**.
* Speed toggles between **200ms** (slow) and **100ms** (fast) on button press.
* LCD displays:
   * First line: `"Blinking rate ="`
   * Second line: `"Slow"` or `"Fast"` (updated in real time)
* Button input is debounced using a **state machine**.
* LCD communication is done using **4-bit parallel mode** via custom bit-level operations.

## ⚙️ Functional Breakdown

### LED Control (PD0–PD3)
* Displays 4-bit counter using `turnOnLEDWithChar(unsigned char num)`.

### LCD Setup and Output
* `initLCD()`: Full setup including pin configuration and LCD initialization procedure.
* `writeCharacter()` & `writeString()`: Display characters and strings.
* `moveCursor(x, y)`: Positions the cursor on the LCD screen.
* Uses **4-bit data mode** (PORTA0–3), RS (PORTB6), and E (PORTB4).

### Button Input with Interrupts
* PB3 configured with a **pull-up resistor** and **pin change interrupt (PCINT3)**.
* State machine tracks button press/release and handles **debouncing**.
* Speed toggled only on full press-and-release sequence.

### Timer Management
* **Timer0**: Millisecond precision using `delayMs()`.
* **Timer1**: Microsecond timing for LCD control via `delayUs()`.
* Both timers use **CTC mode** with interrupts to update shared counters (`ms_counter`, `us_counter`).

## 🛠️ Key Features

* Fully custom LCD driver (bitwise, not library-based)
* Real-time speed feedback on LCD
* Cleanly debounced user input
* Efficient and readable register operations
* Modular and maintainable code structure

## 📁 File Structure

```
├── lcd.c / lcd.h       # LCD interface and initialization
├── led.c / led.h       # LED control
├── switch.c / switch.h  # Button and PCINT setup
├── timer.c / timer.h    # Timer0 and Timer1 setup with delay helpers
├── main.c              # Core logic and state machine
```

## 🧠 Concepts in Use

* LCD 4-bit interface (DB4–DB7)
* Debounced input with a 4-state FSM
* Dual timer configuration for ms/us resolution
* Pin masking and bitwise control
* Hardware abstraction through clean APIs

## 📚 Build Instructions

```bash
# Compile all source files
avr-gcc -mmcu=atmega2560 -O2 -o lcd_counter.elf main.c lcd.c led.c switch.c timer.c

# Convert to hex file
avr-objcopy -O ihex -R .eeprom lcd_counter.elf lcd_counter.hex

# Flash to microcontroller (using avrdude)
avrdude -p atmega2560 -c stk500v2 -P /dev/ttyACM0 -U flash:w:lcd_counter.hex
```

## 🔌 Hardware Requirements

* AVR ATmega2560 microcontroller
* 16x2 LCD display with HD44780 controller
* 4 LEDs connected to PD0-PD3
* Push button connected to PB3
* Power supply (5V)
