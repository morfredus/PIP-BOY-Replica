# Pin Mapping Reference

Complete GPIO pinout for ESP32-S3 DevKitC-1 N16R8 in the PIP-BOY Replica project.

## Quick Reference Table

| GPIO | Component | Function | Pull | Notes |
|------|-----------|----------|------|-------|
| 0 | BUTTON_BOOT | Boot/Reset button | Pull-Up | Strap pin - reserved |
| 4 | LIGHT_SENSOR | LDR analog input | - | ADC1_CH3 |
| 5 | SD_CS | SD Card Chip Select | Pull-Up | Optional (if SD enabled) |
| 6 | BUZZER | PWM audio output | - | PWM Channel 1 |
| 7 | TFT_BL | Display backlight PWM | - | PWM Channel 0 |
| 8 | TFT_MISO | SPI MISO (display/SD) | - | Shared bus |
| 9 | TFT_DC | Display Data/Command | - | |
| 10 | TFT_CS | Display Chip Select | - | |
| 11 | TFT_MOSI | SPI MOSI (display/SD) | - | Shared bus |
| 12 | TFT_SCLK | SPI Clock (display/SD) | - | Shared bus |
| 13 | TFT_RST | Display Reset | - | |
| 15 | I2C_SDA | I2C Data (sensors) | Pull-Up 4.7kΩ | External pull-up required |
| 16 | I2C_SCL | I2C Clock (sensors) | Pull-Up 4.7kΩ | External pull-up required |
| 21 | LED_RED | RGB LED Red channel | - | 220-470Ω resistor |
| 38 | BUTTON_1 / ENCODER_CLK | Input navigation | Internal Pull-Up | Configurable |
| 39 | BUTTON_2 / ENCODER_DT | Input action/direction | Internal Pull-Up | Configurable |
| 40 | ENCODER_SW | Encoder button | Internal Pull-Up | Only if encoder enabled |
| 41 | LED_GREEN | RGB LED Green channel | - | 220-470Ω resistor |
| 42 | LED_BLUE | RGB LED Blue channel | - | 220-470Ω resistor |
| 48 | NEOPIXEL | Onboard NeoPixel | - | Reserved/Internal |

## Bus Sharing

### SPI Bus (Display + SD Card)
**Shared Signals**:
- MOSI (GPIO 11)
- MISO (GPIO 8)
- SCLK (GPIO 12)

**Unique CS Signals**:
- TFT_CS (GPIO 10) - Display
- SD_CS (GPIO 5) - SD Card

**Important**: Only ONE device can be active at a time. Proper CS management required.

### I2C Bus (Sensors)
**Shared Signals**:
- SDA (GPIO 15) - **Requires external 4.7kΩ pull-up to 3.3V**
- SCL (GPIO 16) - **Requires external 4.7kΩ pull-up to 3.3V**

**Connected Devices**:
- AHT20 (0x38) - Temperature & Humidity
- BMP280 (0x76 or 0x77) - Pressure & Altitude

## Configuration-Specific Pins

### Standard Button Mode
Pins GPIO 38 and GPIO 39 used as digital inputs with internal pull-ups.

### Rotary Encoder Mode
- GPIO 38: CLK (rotation)
- GPIO 39: DT (direction)
- GPIO 40: SW (button press)

Select mode in `include/config.h`.

## Pin Constraints

### Strap Pins (Boot Mode)
- **GPIO 0**: Must be HIGH at boot for normal operation
- **GPIO 45, 46**: Used for boot mode selection (avoid if possible)

### ADC Limitations
- ADC1 (GPIO 0-10): Available
- ADC2 (GPIO 11-20): NOT available when WiFi is active

### USB Pins (Reserved)
- GPIO 19: USB D-
- GPIO 20: USB D+

## Power Pins

| Pin | Function | Notes |
|-----|----------|-------|
| 5V | USB power input | 500mA minimum |
| 3V3 | Regulated 3.3V output | Max 600mA |
| GND | Ground | Multiple pins available |
| EN | Enable (reset) | Pull LOW to reset |

## Wiring Diagram

```
ESP32-S3 DevKitC-1
┌─────────────────────────┐
│                         │
│  [USB-C]                │
│                         │
│  GPIO 0  ●──────────────┼─ BUTTON_BOOT → GND
│  GPIO 4  ●──────────────┼─ LIGHT_SENSOR (LDR + 10kΩ)
│  GPIO 5  ●──────────────┼─ SD_CS
│  GPIO 6  ●──────────────┼─ BUZZER (via transistor)
│  GPIO 7  ●──────────────┼─ TFT_BL (PWM)
│  GPIO 8  ●──────────────┼─ TFT_MISO
│  GPIO 9  ●──────────────┼─ TFT_DC
│  GPIO 10 ●──────────────┼─ TFT_CS
│  GPIO 11 ●──────────────┼─ TFT_MOSI (+ SD_MOSI)
│  GPIO 12 ●──────────────┼─ TFT_SCLK (+ SD_SCLK)
│  GPIO 13 ●──────────────┼─ TFT_RST
│  GPIO 15 ●──────────────┼─ I2C_SDA (+ 4.7kΩ pull-up)
│  GPIO 16 ●──────────────┼─ I2C_SCL (+ 4.7kΩ pull-up)
│  GPIO 21 ●──────────────┼─ LED_RED (+ 220-470Ω)
│  GPIO 38 ●──────────────┼─ BUTTON_1 / ENCODER_CLK
│  GPIO 39 ●──────────────┼─ BUTTON_2 / ENCODER_DT
│  GPIO 40 ●──────────────┼─ ENCODER_SW (optional)
│  GPIO 41 ●──────────────┼─ LED_GREEN (+ 220-470Ω)
│  GPIO 42 ●──────────────┼─ LED_BLUE (+ 220-470Ω)
│  GPIO 48 ●──────────────┼─ NEOPIXEL (onboard)
│                         │
│  3V3 ●──────────────────┼─ Power to all 3.3V devices
│  GND ●──────────────────┼─ Common ground
│                         │
└─────────────────────────┘
```

## See Also
- [HARDWARE.md](HARDWARE.md) - Detailed component specifications
- [board_config.h](../include/board_config.h) - Pin definitions in code
- [config.h](../include/config.h) - Configuration options

---

**Version**: 1.0.0
**Last Updated**: 2025-12-19
**Project**: PIP-BOY Replica v1.5.0
