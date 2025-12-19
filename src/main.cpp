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
#include <OneButton.h>

#include "board_config.h"
#include "config.h"
#include "secrets.h"
#include "pipboy_ui.h"
#include "sensor_manager.h"
#include "menu_system.h"

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

// Gestion boutons avec OneButton
OneButton button1(PIN_BUTTON_1, true);
OneButton button2(PIN_BUTTON_2, true);
OneButton buttonBoot(PIN_BUTTON_BOOT, true);

// PWM pour le buzzer
#define BUZZER_PWM_CHANNEL 1
#define BUZZER_PWM_FREQ 4000
#define BUZZER_PWM_RESOLUTION 8

// ========================================
// Gestion du son (Buzzer)
// ========================================

void playBeep(int frequency, int duration) {
    Serial.print("[DEBUG] playBeep freq: ");
    Serial.print(frequency);
    Serial.print(" Hz, dur: ");
    Serial.print(duration);
    Serial.println(" ms");
    ledcWriteTone(BUZZER_PWM_CHANNEL, frequency);
    ledcWrite(BUZZER_PWM_CHANNEL, 128); // 50% duty
    delay(duration);
    ledcWrite(BUZZER_PWM_CHANNEL, 0); // Stop
    ledcWriteTone(BUZZER_PWM_CHANNEL, 0);
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

    ui->drawWiFiProgress("Connecting to WiFi...", 0);

    WiFi.mode(WIFI_STA);

    // Ajouter tous les réseaux disponibles
    wifiMulti.addAP(WIFI_SSID1, WIFI_PASS1);
    wifiMulti.addAP(WIFI_SSID2, WIFI_PASS2);

    int attempts = 0;
    while (wifiMulti.run() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        ui->drawWiFiProgress("Connecting to WiFi...", attempts);
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WiFi] Connected!");
        Serial.print("[WiFi] SSID: ");
        Serial.println(WiFi.SSID());
        Serial.print("[WiFi] IP: ");
        Serial.println(WiFi.localIP());

        char okMsg[40];
        snprintf(okMsg, sizeof(okMsg), "WiFi OK - %s", WiFi.localIP().toString().c_str());
        ui->drawWiFiProgress(okMsg, attempts);

        playBeep(1000, 100);
        delay(100);
        playBeep(1200, 100);
    } else {
        Serial.println("\n[WiFi] Connection failed!");
        ui->drawWiFiProgress("WiFi FAILED", attempts);
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


    // Initialiser les boutons avec OneButton
    Serial.println("[BUTTONS] Initializing OneButton...");
    button1.attachClick([](){
        Serial.println("[BTN1] Click détecté");
        playClickSound();
        menu->nextScreen();
    });
    button2.attachClick([](){
        Serial.println("[BTN2] Click détecté");
        playSelectSound();
        menu->actionButton();
    });
    buttonBoot.attachClick([](){
        Serial.println("[BOOT] Click détecté");
        playBeep(500, 100);
        Serial.println("[SYSTEM] Reset to STAT screen");
        menu->previousScreen();
    });
    button1.attachLongPressStart([](){
        Serial.println("[BTN1] Long press détecté");
        Serial.println("[SYSTEM] Long press detected - Rebooting display...");
        playBootSound();
        ui->begin();
        menu->redraw();
    });
    button2.attachLongPressStart([](){
        Serial.println("[BTN2] Long press détecté");
        Serial.println("[SYSTEM] Long press detected - Reconnecting WiFi...");
        tft.fillScreen(PIPBOY_BLACK);
        tft.setTextSize(1);
        tft.setTextColor(PIPBOY_GREEN);
        tft.setCursor(10, 100);
        tft.print("Reconnecting WiFi...");
        WiFi.disconnect();
        initWiFi();
        menu->redraw();
    });

    // Créer le système de menu
    menu = new MenuSystem(ui, sensors);

    // Dessiner l'écran initial
    menu->redraw();

    // LED verte pour indiquer que tout est OK
    ledGreen();

    Serial.println("\n[SYSTEM] Pip-Boy ready!");
    Serial.println("========================================\n");

    playBeep(1500, 200);

    // Afficher la première page du menu dès le boot
    menu->redraw();
}

// ========================================
// Loop
// ========================================

void loop() {
    // Watchdog rétroéclairage : force la valeur à 255 à chaque itération
    setBacklight(255);

    // Tick boutons OneButton - CRUCIAL pour la détection des événements
    button1.tick();
    button2.tick();
    buttonBoot.tick();

    // DEBUG: Affichage de l'état des boutons sur le port série
    static unsigned long lastButtonDebug = 0;
    if (millis() - lastButtonDebug > 250) {
        lastButtonDebug = millis();
        Serial.print("BTN1: "); Serial.print(digitalRead(PIN_BUTTON_1));
        Serial.print(" | BTN2: "); Serial.print(digitalRead(PIN_BUTTON_2));
        Serial.print(" | BOOT: "); Serial.println(digitalRead(PIN_BUTTON_BOOT));
    }

    // DEBUG: Affichage de l'altitude et de la luminosité
    static unsigned long lastDebug = 0;
    if (millis() - lastDebug > 1000) {
        lastDebug = millis();
        Serial.print("[DEBUG] Altitude: ");
        Serial.print(sensors->getAltitude());
        Serial.print(" m | Lumière: ");
        Serial.print(sensors->getLightLevel());
        Serial.print(" (" );
        Serial.print(sensors->getLightPercent());
        Serial.println("%)");
    }

    // Mise à jour des capteurs
    sensors->update();

    // Mise à jour du menu (gère les animations comme le radar)
    menu->update();

    // Mise à jour des valeurs des capteurs uniquement sur l'écran STAT
    static unsigned long lastSensorUpdate = 0;
    if (menu != nullptr && lastSensorUpdate + 500 < millis()) {
        lastSensorUpdate = millis();
        menu->updateSensorValues();
    }

    // Gestion des LEDs en cas d'alerte
    static bool wasWarning = false;
    bool isWarning = sensors->isTemperatureWarning() ||
                     sensors->isHumidityWarning() ||
                     sensors->isPressureWarning();

    if (isWarning) {
        ledPulse();
    } else if (wasWarning && !isWarning) {
        // Retour à la LED verte après une alerte
        ledGreen();
    }
    wasWarning = isWarning;

    // Pas de delay pour permettre une réactivité maximale des boutons
    delay(5);
}
