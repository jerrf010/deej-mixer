# deej-mixer

A DIY PC mixer with LED feedback, faders, potentiometers, and custom 3D-printed parts, powered by Arduino and compatible with [deej](https://github.com/omriharel/deej).  
Inspired by community projects and fully open source.

---

## 📦 Models Used

- [Slider Potentiometer Knob (MakerWorld)](https://makerworld.com/en/models/472363-slider-potentiometer-knob#profileId-382162)
- [Potentiometer Knob (MakerWorld)](https://makerworld.com/en/models/1299774-potentiometer-knob#profileId-1331903)
- [Mixer PC volumes with LED – Deej (Printables)](https://www.printables.com/model/919405-mixer-pc-volumes-with-led-deej)

---

## 📝 Materials List

All parts, quantities, and purchase links are available in this shared spreadsheet:  
[Materials list - Google Sheets](https://docs.google.com/spreadsheets/d/1POHebsY6CT1eLJ3NvEvcW8S9I_YbqTaPaNE3qmyI9Bc/edit?gid=1036020309#gid=1036020309)

### Key Components

- **Microcontroller:** Arduino Uno, Pro Micro, or Mega Mini (Nano works for up to 8 sliders)
- **Faders:** 5x Slide Potentiometer 10k, 75mm x 9mm
- **Rotary Pots:** 4x 10k potentiometers + 4x Knobs
- **LEDs:** 5x strips WS2812B (Eco), 5 LEDs each (25 LEDs total)
- **Button:** 1x 12mm momentary push button (waterproof with LED recommended)
- **Misc:** M4x12mm screws (x4), USB cable

---

## 🖼️ Images

| ![IMG_4483](https://github.com/user-attachments/assets/dc6bd204-a7f6-4b4b-9808-9bdf0be897c8) | ![IMG_4482](https://github.com/user-attachments/assets/147f382a-5fcc-4537-8e75-d48ec0f00b1b) |
|:--------------------------------------------------------------------------------------------:|:--------------------------------------------------------------------------------------------:|
| ![IMG_4485](https://github.com/user-attachments/assets/504e9a38-119e-4ed4-8a58-076bbad25607) | ![IMG_4484](https://github.com/user-attachments/assets/6e4d08d5-9b30-440d-91f5-ab9cddd73e48) |

---

## 🛠️ Features

- **8 Analog Sliders / Faders** (A0–A7): tactile mixer control for your PC.
- **5x5 WS2812B RGB LED Matrix**: animated feedback, four modes (off, rainbow, ripple, breathing).
- **4 Rotary Potentiometers**: assignable, e.g., for extra mixer channels or custom controls.
- **12mm Push Button**: switches LED modes.
- **EEPROM Mode Storage**: remembers last mode on reboot.
- **Sleep Mode**: LEDs turn off after inactivity, wake on touch.
- **Fully customizable pinout** in code.

---

## ⚡ Wiring Overview

- **Sliders:** A0–A8 (GND/VCC as appropriate)
- **LED DATA:** D6
- **Button:** D2 and GND
- **Power:** USB (5V, GND)

*You may change pinouts in code to fit your hardware layout.*

---

## 🖨️ Assembly & Print Info

- All 3D files are available on [Printables](https://www.printables.com/model/919405-mixer-pc-volumes-with-led-deej) and [MakerWorld](https://makerworld.com).
- Use the referenced knob models for a professional look and fit.
- Check if your selected Arduino fits in the printed base before assembly.

---

## 💻 Software

- [deej](https://github.com/omriharel/deej) for Windows/Linux/Mac audio volume control
- Arduino code: see project folder or adapt provided sketches

---

## 🔧 Customization

- Change LED effects, pinout, and sleep timer in Arduino code.
- Easily swap or remix case and knob models.

---

## 📝 Credits

- **Original model:** [SimGear Customs (RaFFaueQ)](https://www.printables.com/model/919405-mixer-pc-volumes-with-led-deej)
- **Remixed from:** Fed2828
- **Knobs:** MakerWorld users
- **Project inspired by:** [deej](https://github.com/omriharel/deej) and open source hardware enthusiasts

---

## 📜 License

Open source – share, remix, print, and improve!

---

*Enjoy your custom PC mixer! For questions or help, see the community or open an issue.*
