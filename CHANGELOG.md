## [1.5.2] - 2025-12-19

### Fixed
- Compilation error: Added scope braces for variable declaration in `actionButton()` switch case
- Deprecation warnings: Replaced `setClickTicks()`/`setPressTicks()` with `setClickMs()`/`setPressMs()`

## [1.5.1] - 2025-12-19

### Fixed
- Buzzer now works correctly with non-blocking `updateBuzzer()` called in main loop
- All beep durations reduced to short bursts (20-50ms) for better responsiveness
- STATS page: Only changed sensor values are redrawn (eliminates curtain effect)
- RADIO page: Only message area is updated when changing selection (no full screen refresh)
- MAP page: Only sweep line moves, radar and blips remain static (smooth animation)
- MAP coordinates moved above radar circle to prevent overlap

### Changed
- Radar sweep animation interval: 50ms → 30ms (smoother)
- Radar sweep angle increment: 5° → 3° (more fluid)
- Main loop delay: 10ms → 5ms (more responsive)
- Sensor update interval: 500ms → 200ms (faster updates)
- WiFi connection beep simplified to single short tone
- Boot beep duration: 200ms → 50ms

### Performance
- Dramatically improved display responsiveness across all pages
- Eliminated all full-screen redraws during normal operation
- MAP page radar sweep now runs at 33 FPS (vs previous ~10 FPS with full redraw)
- STATS page updates individual lines only when values change
- RADIO page updates only message area (not header/footer)

## [1.5.0] - 2025-12-19

### Added
- Double-click support on Button 1 to force screen refresh
- OneButton timing configuration (setClickTicks, setPressTicks) for reliable button detection
- Sensor value caching system to prevent unnecessary screen redraws
- New `forceRedraw()` method in MenuSystem for explicit full screen redraw

### Fixed
- Removed all `delay()` calls from OneButton callbacks to prevent event detection blocking
- Screen flickering and curtain effect eliminated through smart partial updates
- Button events (click, double-click, long press) now reliably detected without conflicts

### Changed
- Display optimization: `redraw()` only called when screen content actually changes
- Sensor values only redrawn when they change beyond threshold (temperature ±0.1°C, humidity ±0.5%, etc.)
- Header and footer remain static during sensor value updates (no more full screen refresh)
- Reduced sensor update interval in loop from 200ms to 500ms for better performance
- Button debug output interval increased from 250ms to 1000ms to reduce serial spam

### Performance
- Dramatically reduced screen update frequency
- Eliminated redundant draw calls in main loop
- Smart caching prevents unnecessary TFT operations

## [1.4.0] - 2025-12-19

### Added
- Altitude reference pressure (ALTITUDE_SEA_LEVEL_PRESSURE) is now configurable in config.h for correct altitude display. Set to your local sea level pressure (hPa).
## [1.3.10] - 2025-12-19

### Added
- Serial.println in all OneButton callbacks (click/long press) to display button events in the serial monitor for easier debugging.
## [1.3.9] - 2025-12-19

### Added
- Serial debug output for altitude, light sensor (raw and percent), and all playBeep() calls to help diagnose hardware and sensor issues (diagnostic patch).
#
## [1.3.7] - 2025-12-19

### Fixed
- Suppression définitive de tous les doublons d’objets globaux (ui, sensors, menu, button1, button2, buttonBoot)
- Correction de l’ordre des includes et des déclarations pour garantir la compilation sur PlatformIO
- Compilation et téléversement validés sur ESP32-S3

### Documentation
- Changelog et README mis à jour pour refléter la refonte de la gestion boutons et rétroéclairage

## [1.3.6] - 2025-12-19

### Changed
- Gestion du rétroéclairage : PWM dédié, watchdog dans la loop, plus de conflit avec le buzzer
- Gestion des boutons : remplacement complet par la librairie OneButton (appui court/long fiable, boot inclus)
- Nettoyage du code principal, suppression des fragments et redéfinitions
## [1.3.5] - 2025-12-18

### Fixed
- Fixed: `getTFT()` method now properly declared in `PipBoyUI` class, resolving compilation errors in `menu_system.h`.
- Ensured PlatformIO compatibility after header update (clean build required).

### Documentation
- Version badge and documentation updated to 1.3.5 (EN/FR).
# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).




## [1.3.4] - 2025-12-18

### Fixed
- Full screen is now cleared on every page change to prevent visual artifacts and curtain effect
- Patch version incremented to 1.3.4

### Fixed
- Button handling logic reverted to a simple, reliable state (short/long press always detected)
- Sensor display is now always updated on action button press
- Patch version incremented to 1.3.3

### Changed
- Replaced UniversalTelegramBot with AsyncTelegram2 for Telegram bot integration (future-proof, ArduinoJson v7+ compatible)
- Improved button debouncing and long/short press detection logic (no more false long press on short press)
- Increased long press duration to 2000ms for better reliability
- Updated version to 1.3.2 (semver patch)


### Fixed
- **Patch 1.3.1**: Fixed ArduinoJson v7 deprecation warnings in UniversalTelegramBot library
  - Replaced all instances of deprecated `DynamicJsonDocument` with `JsonDocument`
  - Eliminates 10 deprecation warnings during compilation
  - See [docs/PATCHES.md](docs/PATCHES.md) for detailed patch documentation and maintenance instructions

### Documentation
- Added [docs/PATCHES.md](docs/PATCHES.md) to track third-party library patches
- Documented ArduinoJson v7 compatibility patch for UniversalTelegramBot

## [1.3.0] - 2025-12-18

### Changed
- **BREAKING**: All header files renamed to lowercase (snake_case) following C/C++ conventions
  - `PipBoyUI.h` → `pipboy_ui.h`
  - `SensorManager.h` → `sensor_manager.h`
  - `MenuSystem.h` → `menu_system.h`
  - `ButtonHandler.h` → `button_handler.h`
- Removed `f` suffix from float constants in config.h for improved readability
  - Temperature thresholds: `10.0f` → `10.0`, `35.0f` → `35.0`
  - Humidity thresholds: `20.0f` → `20.0`, `80.0f` → `80.0`
  - Pressure thresholds: `980.0f` → `980.0`, `1040.0f` → `1040.0`
  - Weather coordinates: `44.8378f` → `44.8378`, `-0.5792f` → `-0.5792`
- BMP280 I2C addresses now configurable in config.h instead of hardcoded
- Improved serial logging for BMP280 initialization with hex address display

### Added
- Configuration constants for BMP280 I2C addresses in config.h
  - `BMP280_I2C_ADDRESS_PRIMARY` (0x76)
  - `BMP280_I2C_ADDRESS_SECONDARY` (0x77)

## [1.2.1] - 2025-12-18

### Fixed
- SensorManager now uses configuration constants from config.h instead of hardcoded values
- Temperature, humidity, and pressure warning thresholds now properly reference TEMP_WARNING_MIN/MAX, HUMIDITY_WARNING_MIN/MAX, and PRESSURE_WARNING_MIN/MAX
- Sensor update interval now uses SENSOR_UPDATE_INTERVAL from config.h
- Added clear documentation in config.h that temperature thresholds are always stored in Celsius internally

### Changed
- Added config.h include to SensorManager.h for access to configuration constants
- Improved temperature threshold comments to clarify Celsius is used internally with Fahrenheit equivalents noted

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
