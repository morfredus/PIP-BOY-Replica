#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include "config.h"

class SensorManager {
private:
    Adafruit_AHTX0 aht;
    Adafruit_BMP280 bmp;

    bool ahtInitialized;
    bool bmpInitialized;

    // Dernières valeurs lues
    float temperature;
    float humidity;
    float pressure;
    float altitude;
    int lightLevel;

    // Pin du capteur de lumière
    int lightSensorPin;

    // Temps de la dernière lecture
    unsigned long lastReadTime;
    const unsigned long READ_INTERVAL = SENSOR_UPDATE_INTERVAL;

public:
    SensorManager(int lightPin) {
        lightSensorPin = lightPin;
        ahtInitialized = false;
        bmpInitialized = false;
        temperature = 0.0;
        humidity = 0.0;
        pressure = 0.0;
        altitude = 0.0;
        lightLevel = 0;
        lastReadTime = 0;
    }

    // Initialiser les capteurs
    bool begin() {
        Serial.println("[SENSORS] Initializing sensors...");

        // Initialiser AHT20
        if (aht.begin()) {
            ahtInitialized = true;
            Serial.println("[SENSORS] AHT20 initialized successfully");
        } else {
            Serial.println("[SENSORS] WARNING: AHT20 not found!");
        }

        // Initialiser BMP280
        if (bmp.begin(0x76)) { // Adresse I2C par défaut 0x76, parfois 0x77
            bmpInitialized = true;

            // Configuration BMP280
            bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Mode opératoire
                           Adafruit_BMP280::SAMPLING_X2,      // Temp. oversampling
                           Adafruit_BMP280::SAMPLING_X16,     // Pressure oversampling
                           Adafruit_BMP280::FILTER_X16,       // Filtering
                           Adafruit_BMP280::STANDBY_MS_500);  // Standby time

            Serial.println("[SENSORS] BMP280 initialized successfully");
        } else {
            Serial.println("[SENSORS] WARNING: BMP280 not found! Trying 0x77...");
            if (bmp.begin(0x77)) {
                bmpInitialized = true;
                Serial.println("[SENSORS] BMP280 found at 0x77");
            }
        }

        // Configurer le pin du capteur de lumière
        pinMode(lightSensorPin, INPUT);

        return (ahtInitialized || bmpInitialized);
    }

    // Mettre à jour les lectures (à appeler dans loop)
    void update() {
        unsigned long currentTime = millis();

        if (currentTime - lastReadTime >= READ_INTERVAL) {
            lastReadTime = currentTime;

            // Lire AHT20
            if (ahtInitialized) {
                sensors_event_t humidityEvent, tempEvent;
                aht.getEvent(&humidityEvent, &tempEvent);

                temperature = tempEvent.temperature;
                humidity = humidityEvent.relative_humidity;
            }

            // Lire BMP280
            if (bmpInitialized) {
                // Si on n'a pas de température de l'AHT, utiliser celle du BMP
                if (!ahtInitialized) {
                    temperature = bmp.readTemperature();
                }
                pressure = bmp.readPressure() / 100.0F; // Convertir en hPa
                altitude = bmp.readAltitude(1013.25);   // Altitude standard
            }

            // Lire le capteur de lumière (ADC 0-4095 sur ESP32)
            lightLevel = analogRead(lightSensorPin);
        }
    }

    // Getters
    float getTemperature() { return temperature; }
    float getHumidity() { return humidity; }
    float getPressure() { return pressure; }
    float getAltitude() { return altitude; }
    int getLightLevel() { return lightLevel; }

    // Obtenir le niveau de lumière en pourcentage
    int getLightPercent() {
        return map(lightLevel, 0, 4095, 0, 100);
    }

    // Vérifier si les capteurs sont initialisés
    bool isAHTReady() { return ahtInitialized; }
    bool isBMPReady() { return bmpInitialized; }
    bool isAnyReady() { return (ahtInitialized || bmpInitialized); }

    // Obtenir une chaîne de statut
    void getStatusString(char* buffer, size_t bufferSize) {
        snprintf(buffer, bufferSize, "AHT:%s BMP:%s",
                 ahtInitialized ? "OK" : "ERR",
                 bmpInitialized ? "OK" : "ERR");
    }

    // Détecter des conditions d'alerte
    bool isTemperatureWarning() {
        return (temperature < TEMP_WARNING_MIN || temperature > TEMP_WARNING_MAX);
    }

    bool isHumidityWarning() {
        return (humidity < HUMIDITY_WARNING_MIN || humidity > HUMIDITY_WARNING_MAX);
    }

    bool isPressureWarning() {
        return (pressure < PRESSURE_WARNING_MIN || pressure > PRESSURE_WARNING_MAX);
    }
};

#endif // SENSOR_MANAGER_H
