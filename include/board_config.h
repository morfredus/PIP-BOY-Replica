#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

// =========================================================
//         Configuration Pinout ESP32-S3 DevKitC-1 N16R8
// =========================================================


// ------------------------------------
// TFT ST7789 (SPI) 240x240 - 240x320
// ------------------------------------
// Configuration de l'écran couleur (TFT) utilisant le protocole SPI.
#define PIN_TFT_MOSI              11  // SPI MOSI (Master Out Slave In) : Broche de données vers l'écran. Câblage direct.
#define PIN_TFT_SCLK              12  // SPI Clock (Horloge) : Broche de synchronisation. Câblage direct.
#define PIN_TFT_CS                10  // Chip Select : Broche pour activer spécifiquement cet écran. Câblage direct.
#define PIN_TFT_DC                9   // Data/Command : Broche pour indiquer si les données sont des pixels ou une commande. Câblage direct.
#define PIN_TFT_RST               13  // Reset écran. Pull-up/down selon librairie.
#define PIN_TFT_BL                7   // Backlight (Rétroéclairage) : Souvent utilisé avec le PWM pour ajuster la luminosité.

// ------------------------------------
// TFT ILI9341 (SPI) 240x320
// ------------------------------------
// Support de l'écran ILI9341 (SPI) 240x320 :
// Utilise les mêmes broches que le ST7789, avec en plus :
#define PIN_TFT_MISO              8   // SPI MISO (Master In Slave Out) : Broche de données depuis l'écran. Câblage direct.

// ------------------------------------
// I2C
// ------------------------------------
// Configuration du bus de communication I2C.
#define PIN_I2C_SDA       15  // SDA (Serial Data Line). **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.**
#define PIN_I2C_SCL       16  // SCL (Serial Clock Line). **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.**

// ------------------------------------
// LED RGB
// ------------------------------------
#define PIN_LED_RED 21 // Rouge. Résistance série **220O - 470O**.
#define PIN_LED_GREEN 41 // Vert. Résistance série **220O - 470O**.
#define PIN_LED_BLUE 42 // Bleu. Résistance série **220O - 470O**.
#define PIN_NEOPIXEL 48 // NeoPixel Intégrée. Data (Réservée).

// ------------------------------------
// Boutons
// ------------------------------------
#define PIN_BUTTON_BOOT       0   // Bouton BOOT (strap). Réservé au boot.
#define PIN_BUTTON_1          38  // Bouton vers GND. Utiliser **Pull-Up interne**.
#define PIN_BUTTON_2          39  // Bouton vers GND. Utiliser **Pull-Up interne**.

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define PIN_BUZZER    6   // Buzzer. Transistor + **Résistance base 1–10kO**.
#define PIN_LIGHT_SENSOR 4  // LDR. **Diviseur ~10kO**.


#endif // BOARD_CONFIG_H