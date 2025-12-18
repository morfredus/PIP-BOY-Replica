# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.2.0] - 2025-12-18

### Added
- Configuration system with `config.h` file for easy customization
  - Display configuration (type, dimensions, rotation, SPI mode)
  - Backlight brightness default setting
  - Input method selection (buttons and/or rotary encoder)
  - Weather location defaults (Bordeaux, France)
  - Sensor thresholds and update intervals
  - Audio/buzzer frequency definitions
  - LED brightness levels
  - Animation parameters
  - System settings (baud rate, delays, WiFi timeout)
- Comprehensive documentation (README and CHANGELOG in English and French)

### Changed
- Separated configuration parameters from hardware pin definitions
- Weather location now configurable via `config.h` (default: Bordeaux, France)

## [1.1.0] - 2025-12-18

### Added
- TFT backlight management using PWM
  - `initBacklight()` function to configure PWM channel
  - `setBacklight(uint8_t brightness)` function for brightness control (0-255)
  - Backlight connected to PIN_TFT_BL (pin 7)
  - Default brightness set to maximum (255)

### Changed
- Backlight now properly initialized during setup sequence
- Enhanced display initialization with backlight control

## [1.0.0] - 2025-12-18

### Added
- WiFi multi-network support using WiFiMulti
  - Automatic connection to best available network
  - Support for multiple WiFi credentials from `secrets.h`
  - Enhanced WiFi status logging with connected SSID display
- Pip-Boy Fallout-style user interface
  - STAT screen: Sensor statistics (temperature, humidity, pressure, altitude, light)
  - DATA screen: Weather data via WiFi (OpenMeteo API)
  - RADIO screen: Message display system
  - MAP screen: Animated radar display
- Hardware support
  - ESP32-S3 DevKitC-1 N16R8 (16MB Flash, 8MB PSRAM)
  - ST7789 240x240 TFT display (SPI)
  - AHT20 temperature/humidity sensor (I2C)
  - BMP280 pressure sensor (I2C)
  - RGB LED + NeoPixel
  - Light sensor (LDR)
  - 3 buttons + buzzer
- User interaction
  - Button 1: Navigate between screens
  - Button 2: Action/Select (refresh sensors, fetch weather, select messages)
  - Boot button: Return to STAT screen
  - Long press Button 1: Reboot display sequence
  - Long press Button 2: Reconnect WiFi
- Visual and audio feedback
  - LED status indicators (green/orange/red)
  - LED pulsing on sensor warnings
  - Buzzer sounds for clicks, selections, errors, and boot
- Sensor monitoring with warning thresholds
- Weather data fetching from OpenMeteo API
- Animated radar sweep on MAP screen

### Changed
- Project renamed from "Test Zone" to "PIP-BOY Replica"
- Proper semantic versioning implemented

### Fixed
- String concatenation issues in MenuSystem.h
- Private member access violations in PipBoyUI.h
- WiFi credential references aligned with secrets.h structure

## [0.0.0] - Initial Development

### Added
- Initial project structure
- Basic hardware configuration
- Pin definitions in board_config.h
