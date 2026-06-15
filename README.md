# Dino Game | Arduino Pro Mini + SSD1306 OLED

A single-button "Chrome Dino" style runner game built on Arduino Pro Mini with an SSD1306 OLED display. Press one button to jump over obstacles and rack up your score.

🎥 **Watch the full tutorial:** [YouTube link here]

---

## Features

- Single-button gameplay (jump only)
- Animated dino sprite (16x16 bitmap)
- Procedurally spaced obstacles
- Increasing difficulty as score rises
- Start screen and game-over screen with restart
- Score tracking displayed on OLED

---

## Hardware Required

| Component | Quantity |
|---|---|
| Arduino Pro Mini | 1 |
| SSD1306 OLED (128x64, I2C) | 1 |
| Push button | 1 |
| Jumper wires | — |
| Breadboard | 1 |

---

## Wiring

| OLED Pin | Arduino Pin |
|---|---|
| SDA | A4 |
| SCL | A5 |
| VCC | 5V / 3.3V (match your board variant) |
| GND | GND |

| Button | Arduino Pin |
|---|---|
| One leg | D2 |
| Other leg | GND |

> Uses `INPUT_PULLUP`, so no external resistor is needed for the button.

---

## Power Supply (Battery Option)

The Arduino Pro Mini can be powered from a 9V battery using the **RAW** pin (not VCC):

| 9V Battery | Arduino Pro Mini |
|---|---|
| Positive (+) | RAW |
| Negative (−) | GND |

> The RAW pin connects to the onboard voltage regulator, which steps 9V down to 5V (or 3.3V depending on board variant). Do **not** connect 9V directly to the VCC or 5V pin — this bypasses the regulator and can damage the board and OLED.

---

## Libraries Required

Install via Arduino Library Manager:

- `Adafruit_GFX`
- `Adafruit_SSD1306`

---

## How to Play

1. Power on the board — the start screen appears.
2. Press the button to start the game.
3. Press the button to make the dino jump over the obstacle.
4. Avoid collisions to keep your score increasing.
5. On game over, press the button to restart.

---

## Code

The full sketch is in [`dino_game.ino`](./dino_game.ino).

---

## Author

**Maradi Innovations**
YouTube: [Maradi Innovations](https://www.youtube.com/@MaradiInnovations)
GitHub: [Maradiranganath/Eagleaura_lab](https://github.com/Maradiranganath/Eagleaura_lab)

---

## License

Free to use and modify for personal and educational projects.
