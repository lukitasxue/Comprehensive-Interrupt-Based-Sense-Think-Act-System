# Smart Gardening System (Interrupt-Driven Arduino Project)

This is a simple **Smart Gardening System** built with Arduino. It automatically waters plants based on soil moisture and ambient light, and it includes logging capabilities using **interrupts** and **timers** to make the system efficient and responsive.

[Watch the system explanation (video report)](https://youtu.be/zJxq41mIB_I)

---

## System Summary

The system uses:

- **Photoresistor (LDR)** to detect brightness (day/night)
- **Soil Moisture Sensor** to determine if the soil is dry
- **Two push buttons**:
  - One to toggle the system ON/OFF
  - One to manually request watering
- **LED** to simulate watering (ON when watering)

The system architecture is interrupt-based:
- **Pin Change Interrupts (PCI)** handle button presses
- **Timer1 Compare Interrupt** triggers every 5 seconds to log sensor values

---

## Hardware Components

- Arduino Uno / Nano
- 1x LDR + 10kΩ pull-down resistor
- 1x Soil moisture sensor
- 1x LED + 220Ω resistor
- 2x Push buttons (with internal pull-ups)
- Jumper wires and breadboard

---

## Pin Configuration

| Component           | Arduino Pin |
|--------------------|-------------|
| LED                | D13         |
| Watering Button    | D8          |
| Toggle System Btn  | D9          |
| LDR                | A0          |
| Soil Moisture      | A1          |

---

## How It Works

1. System starts in OFF state.
2. **Button D9** toggles system ON/OFF.
3. **Button D8** triggers a manual watering request.
4. If it's **daylight** and **soil is dry**, the LED turns ON to simulate watering.
5. Every **5 seconds**, the system logs light and moisture values to the Serial Monitor.

---

##  Upload & Run

1. Connect all components to the Arduino as described above.
2. Upload the `.ino` file from this repo to your Arduino board using the Arduino IDE.
3. Open the Serial Monitor (9600 baud) to see system messages and logs.
4. Use the buttons to toggle the system or manually request watering.

---

## Demo Video

- [Click here to watch the explanation & demo](https://youtu.be/zJxq41mIB_I)

---

## Notes

- `volatile` flags are used because interrupts modify global variables.
- `PCINT0_vect` is used for detecting changes on digital pins 8–13.
- Timer1 is set up in **CTC mode** with a **1024 prescaler**, triggering every 5 seconds.

---

## References

- [ATmega328P Datasheet](https://www.microchip.com/en-us/product/ATmega328P)
- [DeepBlueEmbedded: PCINT Interrupts Guide](https://deepbluembedded.com/arduino-pcint-pin-change-interrupts)
- Seminar 3 code & Arduino IDE documentation

---

Pd: AI helped enhancing the format for the above README so it looks more professional
