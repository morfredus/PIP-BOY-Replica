#ifndef CONFIG_H
#define CONFIG_H

// =========================================================
//         PIP-BOY REPLICA - Configuration File
// =========================================================
// This file contains non-sensitive configuration parameters
// that can be easily modified without recompiling the entire project.
// For WiFi credentials and API keys, see secrets.h

// =========================================================
// DISPLAY CONFIGURATION
// =========================================================

// Display type selection
// Supported displays:
// - ST7789: 240x240 or 240x320 TFT display
// - ST7735: 128x160 TFT display (smaller option)
#define DISPLAY_TYPE_ST7789     // Use ST7789 display

// Display dimensions
#define DISPLAY_WIDTH           240
#define DISPLAY_HEIGHT          240

// Display rotation (0, 1, 2, or 3)
// 0 = 0°, 1 = 90°, 2 = 180°, 3 = 270°
#define DISPLAY_ROTATION        2

// Display SPI mode
#define DISPLAY_SPI_MODE        SPI_MODE2

// Backlight default brightness (0-255)
// 0 = Off, 255 = Full brightness
#define BACKLIGHT_DEFAULT       255

// =========================================================
// INPUT CONFIGURATION
// =========================================================

// Input method selection
#define USE_BUTTONS             true    // Enable button input
#define USE_ROTARY_ENCODER      false   // Enable rotary encoder (not yet implemented)

// Button configuration
#define BUTTON_DEBOUNCE_MS      50      // Debounce delay in milliseconds
#define LONG_PRESS_DURATION_MS  1000    // Long press threshold in milliseconds

// =========================================================
// WEATHER CONFIGURATION
// =========================================================

// Default location for weather data
// Bordeaux, France
#define WEATHER_LOCATION_NAME   "Bordeaux"
#define WEATHER_LATITUDE        44.8378f
#define WEATHER_LONGITUDE       -0.5792f
#define WEATHER_TIMEZONE        "Europe/Paris"
#define WEATHER_COUNTRY_CODE    "FR"

// Weather update interval (in milliseconds)
// Default: 600000ms = 10 minutes
#define WEATHER_UPDATE_INTERVAL 600000

// Temperature unit
// Options: 'C' for Celsius, 'F' for Fahrenheit
#define TEMPERATURE_UNIT        'C'

// =========================================================
// SENSOR CONFIGURATION
// =========================================================

// Sensor update interval (in milliseconds)
#define SENSOR_UPDATE_INTERVAL  2000    // Update every 2 seconds

// Temperature warning thresholds (in Celsius)
#define TEMP_WARNING_MIN        10.0f   // Below this = warning
#define TEMP_WARNING_MAX        35.0f   // Above this = warning

// Humidity warning thresholds (in percentage)
#define HUMIDITY_WARNING_MIN    20.0f   // Below this = warning
#define HUMIDITY_WARNING_MAX    80.0f   // Above this = warning

// Pressure warning thresholds (in hPa)
#define PRESSURE_WARNING_MIN    980.0f  // Below this = warning (storm)
#define PRESSURE_WARNING_MAX    1040.0f // Above this = warning

// =========================================================
// AUDIO CONFIGURATION
// =========================================================

// Enable/disable sound effects
#define ENABLE_SOUND            true

// Volume level (PWM duty cycle, 0-255)
// Note: Hardware volume control may vary
#define BUZZER_VOLUME           128     // 50% volume

// Sound frequencies (in Hz)
#define SOUND_FREQ_CLICK        800
#define SOUND_FREQ_SELECT       1200
#define SOUND_FREQ_ERROR_1      400
#define SOUND_FREQ_ERROR_2      300
#define SOUND_FREQ_BOOT_1       600
#define SOUND_FREQ_BOOT_2       800
#define SOUND_FREQ_BOOT_3       1000

// =========================================================
// LED CONFIGURATION
// =========================================================

// LED brightness levels (0-255)
#define LED_BRIGHTNESS_LOW      50
#define LED_BRIGHTNESS_MEDIUM   128
#define LED_BRIGHTNESS_HIGH     255

// NeoPixel brightness (0-255)
#define NEOPIXEL_BRIGHTNESS     50

// LED pulse animation speed
#define LED_PULSE_SPEED         5       // Brightness change per cycle
#define LED_PULSE_INTERVAL_MS   20      // Update interval in milliseconds

// =========================================================
// ANIMATION CONFIGURATION
// =========================================================

// Screen animation settings
#define ANIMATION_FRAME_MS      100     // Animation frame update interval

// Radar sweep speed (degrees per frame)
#define RADAR_SWEEP_SPEED       5

// Message blink interval (in milliseconds)
#define MESSAGE_BLINK_MS        500

// =========================================================
// SERIAL CONFIGURATION
// =========================================================

// Serial baud rate for debugging
#define SERIAL_BAUD_RATE        115200

// Enable debug output
#define ENABLE_DEBUG            true

// =========================================================
// SYSTEM CONFIGURATION
// =========================================================

// Boot delay (in milliseconds)
#define BOOT_DELAY_MS           1000

// Main loop delay (in milliseconds)
#define LOOP_DELAY_MS           10

// WiFi connection timeout (number of attempts)
#define WIFI_MAX_ATTEMPTS       20
#define WIFI_RETRY_DELAY_MS     500

#endif // CONFIG_H
