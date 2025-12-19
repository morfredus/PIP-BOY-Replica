# Hardware Configuration Guide

This document provides detailed information about the hardware components supported by the PIP-BOY Replica project.

## Table of Contents
- [Main Controller](#main-controller)
- [Display Options](#display-options)
- [Input Options](#input-options)
- [Sensors](#sensors)
- [Storage](#storage)
- [Audio Output](#audio-output)
- [LEDs](#leds)
- [Power Requirements](#power-requirements)

---

## Main Controller

### ESP32-S3 DevKitC-1 N16R8
- **MCU**: ESP32-S3 (Xtensa dual-core 32-bit LX7)
- **Flash**: 16MB
- **PSRAM**: 8MB
- **WiFi**: 802.11 b/g/n
- **Bluetooth**: BLE 5.0
- **GPIO**: 45 programmable pins
- **Operating Voltage**: 3.3V
- **USB**: USB-C (serial + JTAG debugging)

**Why ESP32-S3?**
- Sufficient RAM/Flash for graphics and data
- Native USB support
- WiFi for weather data
- Multiple SPI/I2C buses
- Hardware PWM for audio and backlight

---

## Display Options

### Option 1: ST7789 240x240 (Default)
- **Resolution**: 240x240 pixels
- **Driver**: ST7789
- **Interface**: 4-wire SPI
- **Colors**: 65K (RGB565)
- **Viewing Angle**: 160°
- **Form Factor**: Square (ideal for retro Pip-Boy look)

**Connections**:
| Display Pin | ESP32-S3 GPIO | Description |
|-------------|---------------|-------------|
| VCC | 3.3V | Power supply |
| GND | GND | Ground |
| SCL/SCLK | GPIO 12 | SPI Clock |
| SDA/MOSI | GPIO 11 | SPI Master Out |
| RES/RST | GPIO 13 | Reset |
| DC | GPIO 9 | Data/Command |
| CS | GPIO 10 | Chip Select |
| BL | GPIO 7 | Backlight (PWM) |

### Option 2: ST7789V 240x320 (2 inch IPS)
- **Resolution**: 240x320 pixels
- **Driver**: ST7789V
- **Interface**: 4-wire SPI
- **Colors**: 65K (RGB565)
- **Viewing Angle**: 170°
- **Form Factor**: Rectangular (more screen real estate)

**Connections**: Same pinout as ST7789 240x240

**Configuration**:
Edit `include/config.h`:
```cpp
// Uncomment for 240x320 display:
#define DISPLAY_ST7789V_240x320

// Comment out for 240x240 display:
// #define DISPLAY_ST7789_240x240
```

---

## Input Options

### Option 1: Tactile Buttons (Default)
Simple and reliable push buttons for navigation.

**Components**:
- 3x Tactile push buttons (normally open)
- Pull-up resistors: **internal** (no external required)

**Connections**:
| Button | ESP32-S3 GPIO | Function |
|--------|---------------|----------|
| Button 1 | GPIO 38 | Navigate screens (short press) / Reboot display (long press) |
| Button 2 | GPIO 39 | Action/Select (short press) / Reconnect WiFi (long press) |
| Boot Button | GPIO 0 | Return to STAT screen |

**Wiring**:
- One side of button → GPIO pin
- Other side of button → GND
- Internal pull-up enabled in software

### Option 2: KY-040 Rotary Encoder
More intuitive navigation with rotation and button press.

**Module Specifications**:
- 20 detents per revolution
- 4 pulses per detent (usually)
- Built-in push button
- Operating voltage: 3.3V - 5V

**Connections**:
| Encoder Pin | ESP32-S3 GPIO | Description |
|-------------|---------------|-------------|
| CLK | GPIO 38 | Clock signal (rotation) |
| DT | GPIO 39 | Data signal (direction) |
| SW | GPIO 40 | Switch (button press) |
| + | 3.3V | Power |
| GND | GND | Ground |

**Note**: Shares GPIO 38/39 with buttons (hardware-compatible)

**Configuration**:
Edit `include/config.h`:
```cpp
// Uncomment for rotary encoder:
#define USE_ROTARY_ENCODER

// Comment out for buttons:
// #define USE_BUTTONS
```

---

## Sensors

### AHT20 Temperature & Humidity Sensor
- **Interface**: I2C
- **Temperature Range**: -40°C to +85°C
- **Humidity Range**: 0% to 100% RH
- **Accuracy**: ±0.3°C, ±2% RH
- **I2C Address**: 0x38

### BMP280 Pressure & Altitude Sensor
- **Interface**: I2C
- **Pressure Range**: 300 - 1100 hPa
- **Altitude**: 0 to 9000m
- **Accuracy**: ±1 hPa, ±1m
- **I2C Address**: 0x76 or 0x77

### Light Sensor (LDR)
- **Type**: Photoresistor
- **Interface**: Analog (ADC)
- **Voltage Divider**: 10kΩ resistor recommended
- **GPIO**: GPIO 4

**Connections**:
| Component | ESP32-S3 Connection |
|-----------|---------------------|
| AHT20 SDA | GPIO 15 + 4.7kΩ pull-up to 3.3V |
| AHT20 SCL | GPIO 16 + 4.7kΩ pull-up to 3.3V |
| BMP280 SDA | GPIO 15 (shared with AHT20) |
| BMP280 SCL | GPIO 16 (shared with AHT20) |
| LDR | GPIO 4 + 10kΩ to GND |

---

## Storage

### Micro SD Card Module (Optional)
Store audio files, logs, and potentially video data.

**Module Specifications**:
- **Interface**: SPI
- **Supported Cards**: microSD, microSDHC (up to 32GB)
- **Voltage**: 3.3V or 5V (with level shifter)
- **Format**: FAT32 recommended

**Connections**:
| SD Module Pin | ESP32-S3 GPIO | Description |
|---------------|---------------|-------------|
| CS | GPIO 5 | Chip Select (unique to SD) |
| MOSI | GPIO 11 | Shared with display |
| MISO | GPIO 8 | Shared with display |
| SCK | GPIO 12 | Shared with display |
| VCC | 3.3V | Power |
| GND | GND | Ground |

**IMPORTANT**: SD Card shares SPI bus with display. Proper CS management required.

**Configuration**:
Edit `include/config.h`:
```cpp
// Uncomment to enable SD Card:
#define ENABLE_SD_CARD
```

**Directory Structure** (on SD Card):
```
/
├── audio/          # Sound effects
├── music/          # Background music
└── video/          # Video files (future)
```

---

## Audio Output

### Passive Buzzer
- **Type**: Passive piezo buzzer (requires PWM)
- **Frequency Range**: 100Hz - 10kHz
- **Operating Voltage**: 3.3V via transistor
- **GPIO**: GPIO 6

**Recommended Circuit**:
```
ESP32 GPIO 6 → 1-10kΩ resistor → NPN transistor base
Transistor collector → Buzzer (+)
Transistor emitter → GND
Buzzer (-) → 3.3V
```

**PWM Configuration**:
- **Channel**: 1
- **Frequency**: 4000 Hz (adjustable)
- **Resolution**: 8-bit (0-255)

---

## LEDs

### RGB LED (Common Cathode)
- **Type**: RGB LED with common cathode
- **GPIO Red**: GPIO 21
- **GPIO Green**: GPIO 41
- **GPIO Blue**: GPIO 42
- **Resistors**: 220Ω - 470Ω per color

### NeoPixel (WS2812B)
- **Type**: Addressable RGB LED
- **GPIO**: GPIO 48 (reserved/onboard)
- **Voltage**: 5V (with level shifter)
- **Protocol**: WS2812B timing

---

## Power Requirements

### Voltage Rails
- **Main**: 5V USB-C
- **MCU**: 3.3V (regulated by ESP32-S3 board)
- **Display**: 3.3V
- **Sensors**: 3.3V
- **SD Card**: 3.3V

### Current Consumption (Typical)
| Component | Current | Notes |
|-----------|---------|-------|
| ESP32-S3 | 50-80mA | Active WiFi |
| Display | 50-150mA | Varies with brightness |
| Sensors | 5-10mA | Combined |
| LEDs | 10-60mA | Per LED at full brightness |
| **Total** | **~200-400mA** | Typical operation |

**Power Supply Recommendations**:
- **USB Power**: 500mA minimum (USB 2.0 standard)
- **Battery**: 3.7V LiPo 1000mAh+ with boost converter
- **External**: 5V 1A regulated supply

---

## Wiring Best Practices

1. **SPI Bus**:
   - Keep wires short (<15cm)
   - Use twisted pairs for clock/data
   - Add 100nF decoupling caps near devices

2. **I2C Bus**:
   - **ALWAYS use 4.7kΩ pull-ups** to 3.3V
   - Star topology preferred (short branches)
   - Avoid capacitive load >400pF

3. **Power**:
   - Add 10µF bulk cap at ESP32-S3 VIN
   - Add 100nF ceramic caps near each IC
   - Keep GND traces wide and continuous

4. **Analog Input** (Light Sensor):
   - Use shielded wire if >10cm
   - Keep away from high-frequency signals
   - Add 100nF cap at ADC input

---

## Safety Notes

⚠️ **WARNINGS**:
- Never apply >3.6V to ESP32-S3 GPIO pins
- Do not draw >40mA from a single GPIO
- Maximum aggregate GPIO current: 500mA
- Use current-limiting resistors for LEDs
- SD Card: Use 3.3V modules or level shifters

---

## Next Steps

- See [PIN_MAPPING.md](PIN_MAPPING.md) for complete pinout diagram
- See [DISPLAY_GUIDE.md](DISPLAY_GUIDE.md) for display setup
- See [INPUT_GUIDE.md](INPUT_GUIDE.md) for input configuration
- See [SD_CARD_GUIDE.md](SD_CARD_GUIDE.md) for SD Card setup

---

**Document Version**: 1.0.0
**Last Updated**: 2025-12-19
**Project**: PIP-BOY Replica v1.5.0
