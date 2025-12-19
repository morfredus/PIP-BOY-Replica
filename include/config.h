// =========================================================
// ALTITUDE REFERENCE PRESSURE (hPa)
// =========================================================
// Set this value to your local sea level pressure (in hPa) for correct altitude
#define ALTITUDE_SEA_LEVEL_PRESSURE 1015.0
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
// - ST7789_240x240: Standard square display (default)
// - ST7789V_240x320: 2 inch IPS rectangular display
// Uncomment ONE of the following:
#define DISPLAY_ST7789_240x240      // Standard 240x240 display (default)
// #define DISPLAY_ST7789V_240x320   // 2 inch IPS 240x320 display

// Display dimensions (auto-configured based on display type)
#ifdef DISPLAY_ST7789V_240x320
    #define DISPLAY_WIDTH           240
    #define DISPLAY_HEIGHT          320
#else
    // Default to 240x240
    #define DISPLAY_WIDTH           240
    #define DISPLAY_HEIGHT          240
#endif

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
// Choose ONE input method:
// - USE_BUTTONS: Standard tactile buttons (default)
// - USE_ROTARY_ENCODER: KY-040 rotary encoder module
// Uncomment ONE of the following:
#define USE_BUTTONS             // Standard buttons (default)
// #define USE_ROTARY_ENCODER   // KY-040 rotary encoder

// Button configuration (for USE_BUTTONS)
#define BUTTON_DEBOUNCE_MS      50      // Debounce delay in milliseconds
#define LONG_PRESS_DURATION_MS  2000    // Long press threshold in milliseconds

// Rotary encoder configuration (for USE_ROTARY_ENCODER)
#define ENCODER_STEPS_PER_NOTCH 4       // Pulses per detent (usually 4)
#define ENCODER_DEBOUNCE_MS     5       // Debounce delay in milliseconds
#define ENCODER_BUTTON_LONG_PRESS_MS 2000  // Long press threshold for encoder button

// =========================================================
// WEATHER CONFIGURATION
// =========================================================

// Default location for weather data
// Bordeaux, France
#define WEATHER_LOCATION_NAME   "Bordeaux"
#define WEATHER_LATITUDE        44.8378
#define WEATHER_LONGITUDE       -0.5792
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

// Temperature warning thresholds
// IMPORTANT: These values are ALWAYS in Celsius internally
// If you use Fahrenheit display (TEMPERATURE_UNIT = 'F'),
// the conversion will be done automatically for display only
#define TEMP_WARNING_MIN        10.0    // °C - Below this = warning (50°F)
#define TEMP_WARNING_MAX        35.0    // °C - Above this = warning (95°F)

// Humidity warning thresholds (in percentage)
#define HUMIDITY_WARNING_MIN    20.0    // Below this = warning
#define HUMIDITY_WARNING_MAX    80.0    // Above this = warning

// Pressure warning thresholds (in hPa)
#define PRESSURE_WARNING_MIN    980.0   // Below this = warning (storm)
#define PRESSURE_WARNING_MAX    1040.0  // Above this = warning

// I2C sensor addresses
#define BMP280_I2C_ADDRESS_PRIMARY    0x76  // Primary address (most common)
#define BMP280_I2C_ADDRESS_SECONDARY  0x77  // Secondary address (alternative)

// =========================================================
// SD CARD CONFIGURATION
// =========================================================

// Enable/disable SD Card support
// Uncomment to enable SD Card module
// #define ENABLE_SD_CARD

// SD Card mount point
#define SD_MOUNT_POINT          "/sd"

// SD Card SPI frequency (Hz)
// Common values: 4000000 (4MHz), 8000000 (8MHz), 16000000 (16MHz)
// Lower frequencies are more reliable but slower
#define SD_SPI_FREQ             4000000

// Audio files directory on SD Card
#define SD_AUDIO_DIR            "/audio"
#define SD_MUSIC_DIR            "/music"
#define SD_VIDEO_DIR            "/video"

// Supported audio formats (for future implementation)
// - WAV: Uncompressed audio
// - MP3: Compressed audio (requires decoder library)
#define AUDIO_FORMAT_WAV        ".wav"
#define AUDIO_FORMAT_MP3        ".mp3"

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
