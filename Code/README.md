# Custom Deej Mixer Project

This Arduino code is designed for a custom deej mixer project that uses analog sliders and NeoPixel LED strips to control and visualize PC volume levels (or any other software controls, via deej). It reads 8 analog sliders, drives 5 NeoPixel LED strips, and features button-controlled LED modes and automatic sleep for power saving.

## Main Components & Their Roles

### Sliders (Analog Potentiometers)
- **8 sliders** are connected to analog pins (A0–A7). Each slider sends its position to the Arduino, which then forwards the values over serial to your PC (for deej or other uses).

### NeoPixel Strips
- **5 strips** (5 LEDs each) are connected to digital pins D2–D6. These strips provide visual feedback using various lighting modes.

### Button (Mode Selector)
- A single **pushbutton** (on D8) cycles through LED modes or wakes the device from sleep.

### Mode Indicator LED
- An **indicator LED** (on D7) shows whether the mixer is in "all LEDs off" mode or an active mode.

### EEPROM
- Remembers the last selected LED mode, even after the Arduino loses power.

## How the Code Works

### Setup Phase
- Initializes all NeoPixel strips and LEDs.
- Configures all pins (sliders, button, indicator).
- Loads the last-used LED mode from EEPROM.
- Runs a startup animation (ripple effect) to show everything is working.

### Main Loop
- **Button Handling:** When the button is pressed, it cycles to the next LED mode (or wakes from sleep). The new mode is saved in EEPROM.
- **Slider Reading:** Reads all 8 slider positions, storing their values and sending them over serial in a format deej can read.
- **Sleep Management:** If there’s no activity for 20 seconds, the mixer goes into sleep mode (all LEDs off, low power). Any slider movement or button press wakes it up.
- **LED Modes:** Depending on the current mode:
  - Mode 0: All LEDs off.
  - Mode 1: Strips display slider levels in bar-graph style and color.
  - Mode 2: All LEDs show a moving rainbow animation.
  - Mode 3: A ripple effect with changing colors moves across the strips.
- **Indicator LED:** Turns on/off to show if you're in "all off" mode or a visual mode.

## Functions
- `updateSliderValues()`: Reads sliders, checks for movement, wakes from sleep if needed.
- `sendSliderValues()`: Formats and sends slider values over serial.
- `enterSleepMode()` / `wakeFromSleep()`: Manage entering or leaving sleep.
- Other helper functions handle LED updates and serial printing.

## Why Use This Code?
- **Visual Feedback:** Instantly see your slider positions and modes with vibrant lighting.
- **Power Saving:** Mixer sleeps when not in use.

Feel free to customize and expand upon this project to suit your needs!
