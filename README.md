# PIP-BOY Replica - Fallout Style Interface

A fully functional Pip-Boy replica inspired by the Fallout game series, built with ESP32-S3 and featuring a retro-futuristic green monochrome interface.

# PATCH 1.4.3
![Version](https://img.shields.io/badge/version-1.4.3-green.svg)
## [1.4.3] - 2025-12-19

- **Fixed**: Audio beeps duration - reduced all beep durations (click: 50→20ms, select: 80→30ms, error: 200→40ms, boot: 150→50ms)
- **Fixed**: Radar blips spinning - blips now stay at FIXED positions (45°, 120°, 220°), only sweep line rotates
- **Improved**: Radar behaves like a real radar with stationary targets

## [1.4.2] - 2025-12-19

- **Fixed**: Buzzer now working - added missing PWM initialization
- **Fixed**: Non-blocking audio - removed all delays from playBeep(), button clicks no longer freeze
- **Fixed**: MAP screen optimization - eliminated full-screen redraw on radar animation (header/footer no longer flicker)
- **Fixed**: MAP screen layout - radar circle no longer overflows onto footer (adjusted centerY: 140→130, radius: 80→70)
- **Optimized**: Smoother radar animation (100ms → 50ms update interval)

## [1.4.1] - 2025-12-19

- **Fixed**: Button detection issues - removed all blocking `delay()` calls from OneButton callbacks
- **Optimized**: Eliminated curtain effect on display updates - smart redraw logic only updates changed values
- **Performance**: Reduced sensor update frequency (200ms → 500ms) and main loop delay (10ms → 5ms)

## [1.4.0] - 2025-12-19

- Altitude reference pressure (ALTITUDE_SEA_LEVEL_PRESSURE) is now configurable in config.h for correct altitude display. Set to your local sea level pressure (hPa).
## [1.3.10] - 2025-12-19

- Serial.println in all OneButton callbacks (click/long press) to display button events in the serial monitor for easier debugging.
## [1.3.9] - 2025-12-19

- Serial debug output for altitude, light sensor (raw and percent), and all playBeep() calls to help diagnose hardware and sensor issues (diagnostic patch).
#

## [1.3.8] - 2025-12-19

- Serial debug output for button states in loop() to help diagnose hardware/software issues with OneButton (diagnostic patch).

## [1.3.7] - 2025-12-19

- Suppression de tous les doublons d’objets globaux, code principal nettoyé, compilation garantie.
- Documentation et changelog mis à jour.

## [1.3.6] - 2025-12-19

- Gestion du rétroéclairage fiabilisée (PWM dédié, watchdog, plus de conflit avec le buzzer).
- Gestion des boutons refondue avec OneButton (appui court/long, boot inclus).
# Changelog

## [1.3.5] - 2025-12-18

- Fixed: `getTFT()` method now properly declared in `PipBoyUI` class, resolving compilation errors in `menu_system.h`.
- PlatformIO compatibility ensured after header update (clean build required).
![Platform](https://img.shields.io/badge/platform-ESP32--S3-blue.svg)
![License](https://img.shields.io/badge/license-MIT-orange.svg)

## Features

### 📊 Four Main Screens

- **STAT** - Real-time sensor statistics
  - Temperature (°C)
  - Humidity (%)
  - Atmospheric pressure (hPa)
  - Altitude (m)
  - Ambient light level (%)
  - Visual warnings for out-of-range values

- **DATA** - Weather information via WiFi
  - WiFi connection status
  - Signal strength (RSSI)
  - External weather conditions (OpenMeteo API)
  - External temperature and humidity

- **RADIO** - Message system
  - Broadcast messages
  - Selectable message list
  - Scrollable interface

- **MAP** - Animated radar display
  - Rotating radar sweep
  - Detection blips
  - Coordinate display

### 🎮 User Interaction

- **Button 1**: Navigate between screens (STAT → DATA → RADIO → MAP)
- **Button 2**: Context action (refresh sensors, fetch weather, select message)
- **Boot Button**: Quick return to STAT screen
- **Long Press Button 1**: Restart boot animation
- **Long Press Button 2**: Reconnect WiFi

### 🔊 Audio & Visual Feedback

- RGB LED status indicator (green/orange/red)
- NeoPixel RGB LED
- Pulsing LED effect on sensor warnings
- Buzzer with different tones:
  - Click sound (800 Hz)
  - Select sound (1200 Hz)
  - Error sound (400/300 Hz)
  - Boot sequence (600/800/1000 Hz)

### 📡 Connectivity

- WiFi multi-network support with automatic fallback
- Weather data from OpenMeteo API (no API key required)
- Real-time sensor monitoring

## Hardware Requirements

### Main Components

- **ESP32-S3 DevKitC-1 N16R8**
  - 16MB Flash
  - 8MB PSRAM
  - Dual-core Xtensa LX7 @ 240MHz

- **Display**
  - ST7789 240x240 TFT LCD (SPI)
  - PWM-controlled backlight

- **Sensors (I2C)**
  - AHT20: Temperature and humidity sensor
  - BMP280: Atmospheric pressure sensor
  - LDR: Light-dependent resistor (analog)

- **User Interface**
  - 3 push buttons
  - Passive buzzer
  - RGB LED (common cathode)
  - WS2812B NeoPixel (integrated on ESP32-S3)

### Pin Configuration

See [board_config.h](include/board_config.h) for complete pin mapping.

**Key Connections:**

#### TFT ST7789 Display (SPI)
| Signal | ESP32 Pin |
|--------|-----------|
| MOSI   | 11        |
| SCLK   | 12        |
| CS     | 10        |
| DC     | 9         |
| RST    | 13        |
| BL     | 7         |

#### I2C Bus (AHT20 + BMP280)
| Signal | ESP32 Pin | Notes |
|--------|-----------|-------|
| SDA    | 15        | 4.7kΩ pull-up to 3.3V required |
| SCL    | 16        | 4.7kΩ pull-up to 3.3V required |

#### RGB LED (Common Cathode)
| Color  | ESP32 Pin | Resistor  |
|--------|-----------|-----------|
| Red    | 21        | 220-470Ω  |
| Green  | 41        | 220-470Ω  |
| Blue   | 42        | 220-470Ω  |

#### Other Components
| Component      | ESP32 Pin | Notes                           |
|----------------|-----------|---------------------------------|
| NeoPixel       | 48        | Integrated on module            |
| Button 1       | 38        | Internal pull-up enabled        |
| Button 2       | 39        | Internal pull-up enabled        |
| Boot Button    | 0         | Already on DevKit               |
| Buzzer         | 6         | Transistor driver + 1-10kΩ base |
| Light Sensor   | 4         | Voltage divider ~10kΩ           |

## Software Setup

### Prerequisites

- [PlatformIO](https://platformio.org/) installed
- USB-C cable for ESP32-S3 programming
- WiFi network credentials

### Installation

1. **Clone or download this repository**

2. **Configure WiFi credentials**

   Create `include/secrets.h` with your WiFi networks:
   ```cpp
   #ifndef SECRETS_H
   #define SECRETS_H

   // WiFi credentials - Network 1
   #define WIFI_SSID1 "YourNetworkName1"
   #define WIFI_PASS1 "YourPassword1"

   // WiFi credentials - Network 2
   #define WIFI_SSID2 "YourNetworkName2"
   #define WIFI_PASS2 "YourPassword2"

   // Telegram Bot (optional)
   #define TELEGRAM_BOT_TOKEN "YourBotToken"
   #define TELEGRAM_CHAT_ID "YourChatID"

   // AccuWeather (optional - alternative to OpenMeteo)
   #define ACCUWEATHER_API_KEY "YourAPIKey"
   #define ACCUWEATHER_LOCATION_KEY "YourLocationKey"

   #endif
   ```

3. **Customize configuration (optional)**

   Edit [include/config.h](include/config.h) to modify:
   - Display settings (type, size, rotation)
   - Input method (buttons and/or rotary encoder)
   - Weather location (default: Bordeaux, France)
   - Sensor thresholds
   - Audio frequencies
   - LED brightness
   - Animation speeds

4. **Build and upload**
   ```bash
   # Install dependencies
   pio lib install

   # Build project
   pio run

   # Upload to ESP32-S3
   pio run --target upload

   # Monitor serial output
   pio device monitor
   ```

## Configuration

### Display Settings

In [config.h](include/config.h), you can configure:
```cpp
#define DISPLAY_TYPE_ST7789      // Display type
#define DISPLAY_WIDTH    240     // Width in pixels
#define DISPLAY_HEIGHT   240     // Height in pixels
#define DISPLAY_ROTATION 2       // 0=0°, 1=90°, 2=180°, 3=270°
#define BACKLIGHT_DEFAULT 255    // Brightness 0-255
```

### Weather Location

Default location is Bordeaux, France. Edit in [config.h](include/config.h):
```cpp
#define WEATHER_LOCATION_NAME   "Bordeaux"
#define WEATHER_LATITUDE        44.8378f
#define WEATHER_LONGITUDE       -0.5792f
#define WEATHER_TIMEZONE        "Europe/Paris"
#define WEATHER_COUNTRY_CODE    "FR"
```

### Sensor Thresholds

Customize warning thresholds in [config.h](include/config.h):
```cpp
#define TEMP_WARNING_MIN        10.0f   // °C
#define TEMP_WARNING_MAX        35.0f   // °C
#define HUMIDITY_WARNING_MIN    20.0f   // %
#define HUMIDITY_WARNING_MAX    80.0f   // %
#define PRESSURE_WARNING_MIN    980.0f  // hPa
#define PRESSURE_WARNING_MAX    1040.0f // hPa
```

## Project Structure

```
Test-Zone/
├── include/
│   ├── board_config.h      # Hardware pin definitions
│   ├── config.h            # User-configurable parameters
│   ├── secrets.h           # WiFi credentials (not in repo)
│   ├── PipBoyUI.h          # UI rendering engine
│   ├── SensorManager.h     # Sensor data management
│   ├── MenuSystem.h        # Screen navigation system
│   └── ButtonHandler.h     # Button input handling
├── src/
│   └── main.cpp            # Main application logic
├── platformio.ini          # PlatformIO configuration
├── CHANGELOG.md            # Version history (English)
├── CHANGELOG_FR.md         # Version history (French)
├── README.md               # This file
└── README_FR.md            # Documentation (French)
```

### Main Classes

#### `PipBoyUI`
Fallout-style graphical interface management:
- RobCo boot sequence animation
- Stylized headers/footers
- Tabs, text boxes, progress bars
- Circular radar with animations
- Scanline effects

#### `SensorManager`
Sensor data acquisition:
- AHT20 (temperature, humidity)
- BMP280 (pressure, altitude)
- LDR (ambient light)
- Alert detection

#### `MenuSystem`
Screen navigation and management:
- 4 screens (STAT/DATA/RADIO/MAP)
- Weather data fetching via HTTP
- Animations and effects

#### `ButtonHandler`
Advanced button management:
- Hardware debouncing
- Short/long press detection
- Pressed/released/held states

## Dependencies

Libraries automatically installed by PlatformIO:
- Adafruit BusIO ^1.17.4
- Adafruit GFX Library ^1.12.4
- Adafruit ST7735 and ST7789 Library ^1.11.0
- Adafruit AHTX0 ^2.0.5
- Adafruit BMP280 Library ^2.6.8
- Adafruit NeoPixel ^1.12.0
- ArduinoJson ^7.0.4
- UniversalTelegramBot ^1.3.0

## Usage

### Normal Operation

1. **Power on** - Device boots with visual and audio feedback
2. **Navigate** - Press Button 1 to cycle through screens
3. **Interact** - Press Button 2 for screen-specific actions:
   - STAT: Refresh sensor readings
   - DATA: Fetch weather data
   - RADIO: Select next message
   - MAP: Reset radar sweep
4. **Quick reset** - Press Boot button to return to STAT screen

### LED Status Indicators

- **Green**: Normal operation
- **Orange**: Processing/Activity
- **Red**: Error or boot button pressed
- **Pulsing Green**: Sensor warning active

## Troubleshooting

### Display Issues
- Check SPI wiring (MOSI, SCLK, CS, DC, RST)
- Verify backlight connection to pin 7
- Try different `DISPLAY_ROTATION` values in [config.h](include/config.h)

### Sensor Issues
- Verify I2C pull-up resistors (4.7kΩ to 3.3V required)
- Check I2C wiring (SDA→15, SCL→16)
- Monitor serial output for sensor detection messages

### WiFi Connection
- Verify credentials in [secrets.h](include/secrets.h)
- Check signal strength (device will retry 20 times)
- Long press Button 2 to force reconnection

### No Sound
- Check buzzer wiring and transistor driver
- Verify `ENABLE_SOUND` is `true` in [config.h](include/config.h)
- Test with different `BUZZER_VOLUME` values

## Future Enhancements

- [ ] Full Telegram Bot integration (notifications)
- [ ] AccuWeather support in addition to OpenMeteo
- [ ] INVENTORY screen with item list
- [ ] Statistics saving to SPIFFS/LittleFS
- [ ] Power saving mode (screen dimming)
- [ ] Alternative color themes (amber, white)
- [ ] WiFi configuration screen via AP mode
- [ ] Historical sensor data graphs
- [ ] RTC support for real-time clock

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Follow existing code style
4. Test thoroughly
5. Submit a pull request

## License

This project is licensed under the MIT License - see LICENSE file for details.

## Acknowledgments

- Inspired by the Pip-Boy from Bethesda's Fallout series
- Uses OpenMeteo free weather API
- Built with Arduino and PlatformIO frameworks

## Version History

See [CHANGELOG.md](CHANGELOG.md) for detailed version history.

**Current Version: 1.3.0** - Code standards compliance and improved configuration

---

**Vault-Tec wishes you excellent survival in the Wasteland!** ☢️
