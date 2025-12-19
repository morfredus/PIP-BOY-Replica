#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

// =========================================================
//         Configuration Pinout ESP32-S3 DevKitC-1 N16R8
// =========================================================

// ------------------------------------
// TFT ST7789 / ST7789V (SPI)
// ------------------------------------
// Configuration de l'écran couleur (TFT) utilisant le protocole SPI.
// Compatible avec :
// - ST7789 240x240 (standard)
// - ST7789V 240x320 (2 inch IPS)
#define PIN_TFT_MOSI              11  // SPI MOSI (Master Out Slave In) : Broche de données vers l'écran. Câblage direct.
#define PIN_TFT_SCLK              12  // SPI Clock (Horloge) : Broche de synchronisation. Câblage direct.
#define PIN_TFT_CS                10  // Chip Select : Broche pour activer spécifiquement cet écran. Câblage direct.
#define PIN_TFT_DC                9   // Data/Command : Broche pour indiquer si les données sont des pixels ou une commande. Câblage direct.
#define PIN_TFT_RST               13  // Reset écran. Pull-up/down selon librairie.
#define PIN_TFT_BL                7   // Backlight (Rétroéclairage) : Souvent utilisé avec le PWM pour ajuster la luminosité.

// ------------------------------------
// TFT ILI9341 (SPI) - Alternative
// ------------------------------------
#define PIN_TFT_MISO             8   // SPI MISO (Master In Slave Out) : Broche de données depuis l'écran. Câblage direct.

// ------------------------------------
// SD Card Module (SPI)
// ------------------------------------
// Module Micro SD SPI pour stockage de musiques et vidéos
// Note: Partage les mêmes pins MOSI/MISO/SCLK que le TFT
#define PIN_SD_CS                 5   // Chip Select pour le module SD Card. **Pull-Up recommandé.**
// MOSI, MISO, SCLK sont partagés avec le TFT (pins 11, 8, 12)

// ------------------------------------
// I2C
// ------------------------------------
// Configuration du bus de communication I2C.
#define PIN_I2C_SDA       15  // SDA (Serial Data Line). **Pull-Up 4.7kΩ vers 3.3V OBLIGATOIRE.**
#define PIN_I2C_SCL       16  // SCL (Serial Clock Line). **Pull-Up 4.7kΩ vers 3.3V OBLIGATOIRE.**

// ------------------------------------
// LED RGB
// ------------------------------------
#define PIN_LED_RED 21 // Rouge. Résistance série **220Ω - 470Ω**.
#define PIN_LED_GREEN 41 // Vert. Résistance série **220Ω - 470Ω**.
#define PIN_LED_BLUE 42 // Bleu. Résistance série **220Ω - 470Ω**.
#define PIN_NEOPIXEL 48 // NeoPixel Intégrée. Data (Réservée).

// ------------------------------------
// Boutons
// ------------------------------------
#define PIN_BUTTON_BOOT       0   // Bouton BOOT (strap). Réservé au boot.
#define PIN_BUTTON_1          38  // Bouton vers GND. Utiliser **Pull-Up interne**.
#define PIN_BUTTON_2          39  // Bouton vers GND. Utiliser **Pull-Up interne**.

// ------------------------------------
// Encodeur Rotatif KY-040 (Alternative aux boutons 1 et 2)
// ------------------------------------
// Module encodeur rotatif avec bouton intégré
// Alternative aux boutons standard pour navigation
#define PIN_ENCODER_CLK       38  // Pin CLK (Clock) - Partage avec BUTTON_1
#define PIN_ENCODER_DT        39  // Pin DT (Data) - Partage avec BUTTON_2
#define PIN_ENCODER_SW        40  // Pin SW (Switch/Button) - Nouveau pin
// Note: Les pins CLK et DT peuvent partager les mêmes GPIO que les boutons
// Le choix entre boutons et encodeur se fait dans config.h

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define PIN_BUZZER    6   // Buzzer. Transistor + **Résistance base 1–10kΩ**.
#define PIN_LIGHT_SENSOR 4  // LDR. **Diviseur ~10kΩ**.

// ------------------------------------
// Pin Map Summary / Résumé des Pins
// ------------------------------------
// GPIO 0  : BUTTON_BOOT (strap, reserved)
// GPIO 4  : LIGHT_SENSOR
// GPIO 5  : SD_CS
// GPIO 6  : BUZZER
// GPIO 7  : TFT_BL (Backlight PWM)
// GPIO 8  : TFT_MISO
// GPIO 9  : TFT_DC
// GPIO 10 : TFT_CS
// GPIO 11 : TFT_MOSI (shared with SD)
// GPIO 12 : TFT_SCLK (shared with SD)
// GPIO 13 : TFT_RST
// GPIO 15 : I2C_SDA
// GPIO 16 : I2C_SCL
// GPIO 21 : LED_RED
// GPIO 38 : BUTTON_1 / ENCODER_CLK
// GPIO 39 : BUTTON_2 / ENCODER_DT
// GPIO 40 : ENCODER_SW
// GPIO 41 : LED_GREEN
// GPIO 42 : LED_BLUE
// GPIO 48 : NEOPIXEL

#endif // BOARD_CONFIG_H