#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "PipBoyUI.h"
#include "SensorManager.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

enum MenuScreen {
    SCREEN_STAT = 0,
    SCREEN_DATA = 1,
    SCREEN_RADIO = 2,
    SCREEN_MAP = 3,
    SCREEN_COUNT = 4
};

class MenuSystem {
private:
    PipBoyUI* ui;
    SensorManager* sensors;
    MenuScreen currentScreen;
    int subMenuIndex;
    unsigned long lastUpdateTime;
    unsigned long animationTimer;
    int animationFrame;

    // Données météo
    String weatherCondition;
    float weatherTemp;
    int weatherHumidity;
    bool weatherDataAvailable;

    // Messages radio
    String radioMessages[5];
    int radioMessageCount;

    // Radar animation
    int radarSweepAngle;

public:
    MenuSystem(PipBoyUI* pipboyUI, SensorManager* sensorManager) {
        ui = pipboyUI;
        sensors = sensorManager;
        currentScreen = SCREEN_STAT;
        subMenuIndex = 0;
        lastUpdateTime = 0;
        animationTimer = 0;
        animationFrame = 0;
        weatherDataAvailable = false;
        radioMessageCount = 0;
        radarSweepAngle = 0;

        // Messages radio par défaut
        addRadioMessage("RobCo Ind. System Boot");
        addRadioMessage("All systems nominal");
        addRadioMessage("Vault-Tec calling...");
    }

    // Changer d'écran
    void nextScreen() {
        currentScreen = (MenuScreen)((currentScreen + 1) % SCREEN_COUNT);
        subMenuIndex = 0;
        redraw();
    }

    void previousScreen() {
        currentScreen = (MenuScreen)((currentScreen - 1 + SCREEN_COUNT) % SCREEN_COUNT);
        subMenuIndex = 0;
        redraw();
    }

    // Actions du menu
    void actionButton() {
        switch (currentScreen) {
            case SCREEN_STAT:
                // Rafraîchir les capteurs
                sensors->update();
                redraw();
                break;

            case SCREEN_DATA:
                // Tenter de récupérer la météo
                fetchWeatherData();
                redraw();
                break;

            case SCREEN_RADIO:
                // Défiler les messages
                subMenuIndex = (subMenuIndex + 1) % radioMessageCount;
                redraw();
                break;

            case SCREEN_MAP:
                // Reset du radar
                radarSweepAngle = 0;
                break;
        }
    }

    // Dessiner l'écran actuel
    void draw() {
        switch (currentScreen) {
            case SCREEN_STAT:
                drawStatScreen();
                break;
            case SCREEN_DATA:
                drawDataScreen();
                break;
            case SCREEN_RADIO:
                drawRadioScreen();
                break;
            case SCREEN_MAP:
                drawMapScreen();
                break;
        }
    }

    // Forcer un redessin complet
    void redraw() {
        ui->clearContent();
        draw();
    }

    // Mettre à jour les animations
    void update() {
        unsigned long currentTime = millis();

        // Mettre à jour l'animation toutes les 100ms
        if (currentTime - animationTimer > 100) {
            animationTimer = currentTime;
            animationFrame++;

            // Animation du radar
            if (currentScreen == SCREEN_MAP) {
                radarSweepAngle = (radarSweepAngle + 5) % 360;
                redraw();
            }
        }
    }

private:
    // Écran STAT - Statistiques des capteurs
    void drawStatScreen() {
        // Tabs
        drawTabs();

        // Contenu
        int y = 60;

        ui->drawStatLine(y, "TEMP", String(sensors->getTemperature(), 1) + " C",
                        sensors->isTemperatureWarning());
        y += 15;

        ui->drawStatLine(y, "HUMIDITY", String(sensors->getHumidity(), 0) + " %",
                        sensors->isHumidityWarning());
        y += 15;

        ui->drawStatLine(y, "PRESSURE", String(sensors->getPressure(), 0) + " hPa",
                        sensors->isPressureWarning());
        y += 15;

        ui->drawStatLine(y, "ALTITUDE", String(sensors->getAltitude(), 0) + " m", false);
        y += 20;

        // Barre de lumière
        int lightPercent = sensors->getLightPercent();
        ui->drawStatLine(y, "LIGHT", String(lightPercent) + " %", false);
        y += 12;
        ui->drawProgressBar(10, y, 220, 10, lightPercent);

        y += 25;

        // Statut des capteurs
        char statusBuffer[50];
        sensors->getStatusString(statusBuffer, sizeof(statusBuffer));
        ui->drawStatLine(y, "SENSORS", statusBuffer, !sensors->isAnyReady());

        // Footer
        ui->drawFooter("NEXT", "REFRESH");
    }

    // Écran DATA - Données météo
    void drawDataScreen() {
        drawTabs();

        int y = 60;

        if (WiFi.status() != WL_CONNECTED) {
            ui->drawStatLine(y, "STATUS", "WiFi disconnected", true);
            y += 15;
            ui->drawStatLine(y, "SSID", WiFi.SSID().c_str(), false);
        } else {
            ui->drawStatLine(y, "WiFi", "Connected", false);
            y += 15;
            ui->drawStatLine(y, "RSSI", String(WiFi.RSSI()) + " dBm", false);
            y += 20;

            if (weatherDataAvailable) {
                ui->drawStatLine(y, "WEATHER", weatherCondition.c_str(), false);
                y += 15;
                ui->drawStatLine(y, "TEMP EXT", String(weatherTemp, 1) + " C", false);
                y += 15;
                ui->drawStatLine(y, "HUMIDITY EXT", String(weatherHumidity) + " %", false);
            } else {
                ui->drawBlinkingMessage(10, y, "Press BTN2 to fetch");
            }
        }

        ui->drawFooter("NEXT", "FETCH");
    }

    // Écran RADIO - Messages
    void drawRadioScreen() {
        drawTabs();

        int y = 60;

        ui->drawStatLine(y, "RADIO", "Broadcasting...", false);
        y += 20;

        // Afficher les messages
        for (int i = 0; i < radioMessageCount && i < 5; i++) {
            bool selected = (i == subMenuIndex);
            char msgBuffer[30];
            snprintf(msgBuffer, sizeof(msgBuffer), "%s", radioMessages[i].c_str());

            if (selected) {
                ui->drawTextBox(10, y, 220, 20, msgBuffer, true);
            } else {
                ui->drawStatLine(y + 5, ">", msgBuffer, false);
            }
            y += 25;
        }

        ui->drawFooter("NEXT", "SELECT");
    }

    // Écran MAP - Radar
    void drawMapScreen() {
        drawTabs();

        // Dessiner le radar
        int centerX = 120;
        int centerY = 140;
        int radius = 80;

        ui->drawRadar(centerX, centerY, radius);

        // Ligne de balayage animée
        float rad = radarSweepAngle * 3.14159 / 180.0;
        int sweepX = centerX + radius * cos(rad);
        int sweepY = centerY + radius * sin(rad);
        ui->tft->drawLine(centerX, centerY, sweepX, sweepY, PIPBOY_LIGHTGREEN);

        // Quelques blips aléatoires pour l'effet
        ui->drawRadarBlip(centerX, centerY, 30, (radarSweepAngle + 45) % 360, false);
        ui->drawRadarBlip(centerX, centerY, 50, (radarSweepAngle + 120) % 360, false);
        ui->drawRadarBlip(centerX, centerY, 65, (radarSweepAngle + 220) % 360, true);

        // Coordonnées
        char coordBuffer[30];
        snprintf(coordBuffer, sizeof(coordBuffer), "N:%d E:%d",
                 (int)(sensors->getAltitude() + 1000),
                 (int)(sensors->getPressure()));
        ui->drawStatLine(60, "COORD", coordBuffer, false);

        ui->drawFooter("NEXT", "SCAN");
    }

    // Dessiner les tabs
    void drawTabs() {
        const char* tabNames[] = {"STAT", "DATA", "RADIO", "MAP"};

        for (int i = 0; i < SCREEN_COUNT; i++) {
            ui->drawTab(i, SCREEN_COUNT, tabNames[i], i == currentScreen);
        }
    }

    // Récupérer les données météo via OpenMeteo (pas besoin de clé API)
    void fetchWeatherData() {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("[WEATHER] WiFi not connected");
            return;
        }

        HTTPClient http;

        // API OpenMeteo (gratuite, pas de clé nécessaire)
        String url = "http://api.open-meteo.com/v1/forecast?latitude=48.8566&longitude=2.3522&current_weather=true&hourly=temperature_2m,relativehumidity_2m";

        Serial.print("[WEATHER] Fetching: ");
        Serial.println(url);

        http.begin(url);
        int httpCode = http.GET();

        if (httpCode == 200) {
            String payload = http.getString();
            Serial.println("[WEATHER] Response received");

            // Parser le JSON
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
                weatherTemp = doc["current_weather"]["temperature"];
                weatherCondition = "Clear"; // OpenMeteo ne donne pas de texte descriptif
                int weatherCode = doc["current_weather"]["weathercode"];

                // Interpréter le code météo
                if (weatherCode == 0) weatherCondition = "Clear";
                else if (weatherCode <= 3) weatherCondition = "Cloudy";
                else if (weatherCode <= 67) weatherCondition = "Rainy";
                else if (weatherCode <= 77) weatherCondition = "Snowy";
                else weatherCondition = "Stormy";

                weatherHumidity = doc["hourly"]["relativehumidity_2m"][0];
                weatherDataAvailable = true;

                Serial.println("[WEATHER] Data parsed successfully");
            } else {
                Serial.print("[WEATHER] JSON parse error: ");
                Serial.println(error.c_str());
            }
        } else {
            Serial.print("[WEATHER] HTTP error: ");
            Serial.println(httpCode);
        }

        http.end();
    }

    // Ajouter un message radio
    void addRadioMessage(const char* message) {
        if (radioMessageCount < 5) {
            radioMessages[radioMessageCount] = String(message);
            radioMessageCount++;
        }
    }
};

#endif // MENU_SYSTEM_H
