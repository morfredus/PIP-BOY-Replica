/**
 * ========================================
 *  PIP-BOY REPLICA - Fallout Style
 * ========================================
 *
 * Hardware:
 * - ESP32-S3 DevKitC-1 N16R8
 * - TFT ST7789 240x240 (SPI)
 * - AHT20 + BMP280 (I2C)
 * - RGB LED + NeoPixel
 * - 3 Buttons + Buzzer + Light Sensor
 *
 * Screens:
 * - STAT: Sensor statistics
 * - DATA: Weather data via WiFi
 * - RADIO: Messages
 * - MAP: Radar display
 *
 * Controls:
 * - Button 1: Navigate screens
 * - Button 2: Action/Select
 * - Boot Button: Reset/Home
 */

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_NeoPixel.h>

#include "board_config.h"
#include "config.h"
#include "secrets.h"
#include "PipBoyUI.h"
#include "SensorManager.h"
#include "MenuSystem.h"
#include "ButtonHandler.h"

// ========================================
// Objets globaux
// ========================================

// Ecran TFT
Adafruit_ST7789 tft = Adafruit_ST7789(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_MOSI, PIN_TFT_SCLK, PIN_TFT_RST);

// NeoPixel
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// WiFi Multi pour plusieurs réseaux
WiFiMulti wifiMulti;

// Managers
PipBoyUI* ui;
SensorManager* sensors;
MenuSystem* menu;
ButtonHandler* buttons;

// ========================================
// Gestion du son (Buzzer)
// ========================================

void playBeep(int frequency, int duration) {
    tone(PIN_BUZZER, frequency, duration);
}

void playClickSound() {
    playBeep(800, 50);
}

void playSelectSound() {
    playBeep(1200, 80);
}

void playErrorSound() {
    playBeep(400, 150);
    delay(100);
    playBeep(300, 150);
}

void playBootSound() {
    playBeep(600, 100);
    delay(80);
    playBeep(800, 100);
    delay(80);
    playBeep(1000, 150);
}

// ========================================
// Gestion des LEDs
// ========================================

void setRGBLED(int r, int g, int b) {
    // LED RGB - GND commun (cathode commune)
    // Logique inversée: LOW = allumé, HIGH = éteint
    analogWrite(PIN_LED_RED, 255 - r);
    analogWrite(PIN_LED_GREEN, 255 - g);
    analogWrite(PIN_LED_BLUE, 255 - b);
}

void setNeoPixel(uint8_t r, uint8_t g, uint8_t b) {
    neopixel.setPixelColor(0, neopixel.Color(r, g, b));
    neopixel.show();
}

void ledGreen() {
    setRGBLED(0, 100, 0);
    setNeoPixel(0, 50, 0);
}

void ledOrange() {
    setRGBLED(100, 50, 0);
    setNeoPixel(50, 25, 0);
}

void ledRed() {
    setRGBLED(100, 0, 0);
    setNeoPixel(50, 0, 0);
}

void ledOff() {
    setRGBLED(0, 0, 0);
    setNeoPixel(0, 0, 0);
}

void ledPulse() {
    static unsigned long lastPulse = 0;
    static int brightness = 0;
    static int direction = 5;

    if (millis() - lastPulse > 20) {
        lastPulse = millis();
        brightness += direction;

        if (brightness >= 100 || brightness <= 0) {
            direction = -direction;
        }

        setRGBLED(0, brightness, 0);
        setNeoPixel(0, brightness / 2, 0);
    }
}

// ========================================
// Gestion du rétroéclairage TFT
// ========================================

// Configuration PWM pour le rétroéclairage
#define TFT_BL_PWM_CHANNEL 0
#define TFT_BL_PWM_FREQ 5000
#define TFT_BL_PWM_RESOLUTION 8

void setBacklight(uint8_t brightness) {
    // brightness: 0-255
    ledcWrite(TFT_BL_PWM_CHANNEL, brightness);
}

void initBacklight() {
    // Configuration du canal PWM pour le rétroéclairage
    ledcSetup(TFT_BL_PWM_CHANNEL, TFT_BL_PWM_FREQ, TFT_BL_PWM_RESOLUTION);
    ledcAttachPin(PIN_TFT_BL, TFT_BL_PWM_CHANNEL);

    // Allumer le rétroéclairage à pleine puissance
    setBacklight(255);
}

// ========================================
// Initialisation WiFi
// ========================================

void initWiFi() {
    Serial.println("[WiFi] Connecting...");

    tft.setTextSize(1);
    tft.setTextColor(PIPBOY_GREEN);
    tft.setCursor(10, 180);
    tft.print("Connecting to WiFi...");

    WiFi.mode(WIFI_STA);

    // Ajouter tous les réseaux disponibles
    wifiMulti.addAP(WIFI_SSID1, WIFI_PASS1);
    wifiMulti.addAP(WIFI_SSID2, WIFI_PASS2);

    int attempts = 0;
    while (wifiMulti.run() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        tft.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WiFi] Connected!");
        Serial.print("[WiFi] SSID: ");
        Serial.println(WiFi.SSID());
        Serial.print("[WiFi] IP: ");
        Serial.println(WiFi.localIP());

        tft.setCursor(10, 195);
        tft.print("WiFi OK - ");
        tft.print(WiFi.localIP());

        playBeep(1000, 100);
        delay(100);
        playBeep(1200, 100);
    } else {
        Serial.println("\n[WiFi] Connection failed!");
        tft.setCursor(10, 195);
        tft.print("WiFi FAILED");
        playErrorSound();
    }

    delay(1000);
}

// ========================================
// Setup
// ========================================

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n========================================");
    Serial.println("  PIP-BOY REPLICA - BOOTING UP");
    Serial.println("========================================\n");

    // Configuration des pins LED
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);

    // LED de démarrage
    ledOrange();

    // Initialiser NeoPixel
    neopixel.begin();
    neopixel.setBrightness(50);
    setNeoPixel(50, 25, 0);

    // Initialiser l'écran TFT
    Serial.println("[DISPLAY] Initializing TFT...");
    tft.init(240, 240, SPI_MODE2);
    tft.setRotation(2);  // Orientation spécifiée
    tft.fillScreen(PIPBOY_BLACK);

    // Initialiser le rétroéclairage
    Serial.println("[DISPLAY] Initializing backlight...");
    initBacklight();

    // Son de démarrage
    playBootSound();

    // Créer l'interface Pip-Boy
    ui = new PipBoyUI(&tft);
    ui->begin();

    // Initialiser I2C
    Serial.println("[I2C] Initializing I2C bus...");
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    delay(100);

    // Initialiser les capteurs
    sensors = new SensorManager(PIN_LIGHT_SENSOR);
    if (!sensors->begin()) {
        Serial.println("[SENSORS] WARNING: No sensors detected!");
        playErrorSound();
    }

    // Initialiser WiFi
    initWiFi();

    // Initialiser les boutons
    Serial.println("[BUTTONS] Initializing buttons...");
    buttons = new ButtonHandler(PIN_BUTTON_1, PIN_BUTTON_2, PIN_BUTTON_BOOT);
    buttons->begin();

    // Créer le système de menu
    menu = new MenuSystem(ui, sensors);

    // Dessiner l'écran initial
    menu->redraw();

    // LED verte pour indiquer que tout est OK
    ledGreen();

    Serial.println("\n[SYSTEM] Pip-Boy ready!");
    Serial.println("========================================\n");

    playBeep(1500, 200);
}

// ========================================
// Loop
// ========================================

void loop() {
    // Mettre à jour les boutons
    buttons->update();

    // Mettre à jour les capteurs
    sensors->update();

    // Mettre à jour les animations
    menu->update();

    // Gestion des boutons
    if (buttons->button1Pressed()) {
        playClickSound();
        ledOrange();
        menu->nextScreen();
        delay(50);
        ledGreen();
    }

    if (buttons->button2Pressed()) {
        playSelectSound();
        ledOrange();
        menu->actionButton();
        delay(50);
        ledGreen();
    }

    if (buttons->bootPressed()) {
        playBeep(500, 100);
        ledRed();

        // Retour à l'écran STAT
        Serial.println("[SYSTEM] Reset to STAT screen");
        menu->previousScreen();

        delay(100);
        ledGreen();
    }

    // Appui long sur Button1 = redémarrer la séquence de boot
    if (buttons->button1LongPressed()) {
        Serial.println("[SYSTEM] Long press detected - Rebooting display...");
        playBootSound();
        ui->begin();
        delay(1000);
        menu->redraw();
    }

    // Appui long sur Button2 = reconnexion WiFi
    if (buttons->button2LongPressed()) {
        Serial.println("[SYSTEM] Long press detected - Reconnecting WiFi...");
        ledOrange();

        tft.fillScreen(PIPBOY_BLACK);
        tft.setTextSize(1);
        tft.setTextColor(PIPBOY_GREEN);
        tft.setCursor(10, 100);
        tft.print("Reconnecting WiFi...");

        WiFi.disconnect();
        delay(500);
        initWiFi();

        menu->redraw();
        ledGreen();
    }

    // Effet LED pulsé si des alertes sont détectées
    if (sensors->isTemperatureWarning() ||
        sensors->isHumidityWarning() ||
        sensors->isPressureWarning()) {
        ledPulse();
    }

    delay(10);
}
