#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "pipboy_ui.h"
#include "sensor_manager.h"
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
public:
    PipBoyUI* ui;
    SensorManager* sensors;
    MenuScreen currentScreen;
    int subMenuIndex;
    unsigned long lastUpdateTime;
    unsigned long animationTimer;
    int animationFrame;
    String weatherCondition;
    float weatherTemp;
    int weatherHumidity;
    bool weatherDataAvailable;
    String radioMessages[5];
    int radioMessageCount;
    int radarSweepAngle;
    int previousRadarSweepAngle;  // Pour effacer l'ancienne ligne de sweep

    // Positions FIXES des blips sur le radar (ne changent jamais)
    static const int BLIP1_ANGLE = 45;
    static const int BLIP1_DISTANCE = 30;
    static const int BLIP2_ANGLE = 120;
    static const int BLIP2_DISTANCE = 50;
    static const int BLIP3_ANGLE = 220;
    static const int BLIP3_DISTANCE = 65;

    // Variables pour optimisation de l'affichage (éviter l'effet rideau)
    float lastDisplayedTemp;
    float lastDisplayedHumidity;
    float lastDisplayedPressure;
    float lastDisplayedAltitude;
    int lastDisplayedLight;

    // Cache pour éviter le redessinage inutile
    float cachedTemperature;
    float cachedHumidity;
    float cachedPressure;
    float cachedAltitude;
    int cachedLightPercent;
    bool needsFullRedraw;

    MenuSystem(PipBoyUI* pipboyUI, SensorManager* sensorManager);
    void nextScreen();
    void previousScreen();
    void actionButton();
    void draw();
    void redraw();
    void forceRedraw();
    void update();
    void updateSensorValues();
    void addRadioMessage(const char* message);

private:
    void drawStatScreen();
    void drawDataScreen();
    void drawRadioScreen();
    void drawMapScreen();
    void drawTabs();
    void fetchWeatherData();
};

// --- Définition des méthodes inline ---

inline void MenuSystem::draw() {
    if (needsFullRedraw) {
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
        needsFullRedraw = false;
    }
}

inline void MenuSystem::redraw() {
    // Ne redessine que si nécessaire
    if (needsFullRedraw) {
        draw();
    }
}

inline void MenuSystem::forceRedraw() {
    // Force le redessin complet
    needsFullRedraw = true;
    draw();
}

inline MenuSystem::MenuSystem(PipBoyUI* pipboyUI, SensorManager* sensorManager) {
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

        // Initialiser le cache avec des valeurs impossibles pour forcer le premier affichage
        cachedTemperature = -999.0f;
        cachedHumidity = -999.0f;
        cachedPressure = -999.0f;
        cachedAltitude = -999.0f;
        cachedLightPercent = -1;
        needsFullRedraw = true;

        addRadioMessage("RobCo Ind. System Boot");
        addRadioMessage("All systems nominal");
        addRadioMessage("Vault-Tec calling...");
    }

inline void MenuSystem::nextScreen() {
    ui->getTFT()->fillScreen(PIPBOY_BLACK);
    currentScreen = (MenuScreen)((currentScreen + 1) % SCREEN_COUNT);
    subMenuIndex = 0;
    needsFullRedraw = true;
    redraw();
}

inline void MenuSystem::previousScreen() {
    ui->getTFT()->fillScreen(PIPBOY_BLACK);
    currentScreen = (MenuScreen)((currentScreen - 1 + SCREEN_COUNT) % SCREEN_COUNT);
    subMenuIndex = 0;
    needsFullRedraw = true;
    redraw();
}

inline void MenuSystem::actionButton() {
    switch (currentScreen) {
        case SCREEN_STAT:
            sensors->update();
            needsFullRedraw = true;
            redraw();
            break;
        case SCREEN_DATA:
            fetchWeatherData();
            needsFullRedraw = true;
            redraw();
            break;
        case SCREEN_RADIO:
            // Mise à jour de la sélection sans tout redessiner
            subMenuIndex = (subMenuIndex + 1) % radioMessageCount;
            // Effacer uniquement la zone des messages (pas tout l'écran)
            ui->getTFT()->fillRect(0, 80, 240, 135, PIPBOY_BLACK);
            // Redessiner uniquement les messages
            int y = 80;
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
            break;
        case SCREEN_MAP:
            radarSweepAngle = 0;
            needsFullRedraw = true;
            redraw();
            break;
    }
}

    inline void MenuSystem::drawRadioScreen() {
        ui->drawHeader("RADIO");
        ui->clearContent();
        drawTabs();
        int y = 60;
        ui->drawStatLine(y, "RADIO", "Broadcasting...", false);
        y += 20;
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
inline void MenuSystem::update() {
    unsigned long currentTime = millis();
    if (currentScreen == SCREEN_MAP && currentTime - animationTimer > 30) {  // Animation plus fluide
        animationTimer = currentTime;
        animationFrame++;

        // Paramètres du radar (doivent correspondre à drawMapScreen)
        int centerX = 120;
        int centerY = 140;
        int radius = 60;

        // Effacer l'ancienne ligne de sweep en la redessinant en noir
        float prevRad = previousRadarSweepAngle * 3.14159 / 180.0;
        int prevSweepX = centerX + radius * cos(prevRad);
        int prevSweepY = centerY + radius * sin(prevRad);
        ui->getTFT()->drawLine(centerX, centerY, prevSweepX, prevSweepY, PIPBOY_BLACK);

        // Calculer le nouvel angle (plus petit incrément pour plus de fluidité)
        radarSweepAngle = (radarSweepAngle + 3) % 360;
        previousRadarSweepAngle = radarSweepAngle;

        // Dessiner la nouvelle ligne de sweep SANS redessiner tout le radar
        float rad = radarSweepAngle * 3.14159 / 180.0;
        int sweepX = centerX + radius * cos(rad);
        int sweepY = centerY + radius * sin(rad);
        ui->drawRadarSweepLine(centerX, centerY, sweepX, sweepY);

        // PAS de redraw() ici - juste la ligne de sweep bouge
    }
}


inline void MenuSystem::drawStatScreen() {
    // ui->getTFT()->fillScreen(PIPBOY_BLACK); // SUPPRIMÉ
    ui->drawHeader("STATISTICS");
    ui->clearContent();
    drawTabs();
    updateSensorValues();
    ui->drawFooter("NEXT", "REFRESH");
}

inline void MenuSystem::drawDataScreen() {
    // ui->getTFT()->fillScreen(PIPBOY_BLACK); // SUPPRIMÉ
    ui->drawHeader("DATA");
    ui->clearContent();
    drawTabs();
    int y = 60;
    if (WiFi.status() != WL_CONNECTED) {
        ui->drawStatLine(y, "STATUS", "WiFi disconnected", true);
        y += 15;
        ui->drawStatLine(y, "SSID", WiFi.SSID().c_str(), false);
    } else {
        ui->drawStatLine(y, "WiFi", "Connected", false);
        y += 15;
        String rssiStr = String(WiFi.RSSI()) + " dBm";
        ui->drawStatLine(y, "RSSI", rssiStr.c_str(), false);
        y += 20;
        if (weatherDataAvailable) {
            ui->drawStatLine(y, "WEATHER", weatherCondition.c_str(), false);
            y += 15;
            String tempExtStr = String(weatherTemp, 1) + " C";
            ui->drawStatLine(y, "TEMP EXT", tempExtStr.c_str(), false);
            y += 15;
            String humidityExtStr = String(weatherHumidity) + " %";
            ui->drawStatLine(y, "HUMIDITY EXT", humidityExtStr.c_str(), false);
        } else {
            ui->drawBlinkingMessage(10, y, "Press BTN2 to fetch");
        }
    }
    ui->drawFooter("NEXT", "FETCH");
}

inline void MenuSystem::drawMapScreen() {
    // Dessiner l'écran initial (header, tabs, footer, radar)
    ui->drawHeader("MAP");
    ui->clearContent();
    drawTabs();

    // Paramètres du radar ajustés pour ne pas déborder sur le footer
    int centerX = 120;
    int centerY = 140;  // Centré verticalement
    int radius = 60;    // Réduit pour laisser de la place aux coordonnées

    // Dessiner le radar statique
    ui->drawRadar(centerX, centerY, radius);

    // Dessiner la ligne de sweep initiale
    float rad = radarSweepAngle * 3.14159 / 180.0;
    int sweepX = centerX + radius * cos(rad);
    int sweepY = centerY + radius * sin(rad);
    ui->drawRadarSweepLine(centerX, centerY, sweepX, sweepY);

    // Dessiner les blips à leurs positions FIXES (ne changent jamais)
    ui->drawRadarBlip(centerX, centerY, BLIP1_DISTANCE, BLIP1_ANGLE, false);
    ui->drawRadarBlip(centerX, centerY, BLIP2_DISTANCE, BLIP2_ANGLE, false);
    ui->drawRadarBlip(centerX, centerY, BLIP3_DISTANCE, BLIP3_ANGLE, true);

    // Afficher les coordonnées EN DEHORS du cercle (en haut)
    char coordBuffer[30];
    snprintf(coordBuffer, sizeof(coordBuffer), "N:%d E:%d",
             (int)(sensors->getAltitude() + 1000),
             (int)(sensors->getPressure()));
    ui->drawStatLine(55, "COORD", coordBuffer, false);  // Position au-dessus du radar

    ui->drawFooter("NEXT", "SCAN");

    // Mémoriser l'angle initial
    previousRadarSweepAngle = radarSweepAngle;
}

inline void MenuSystem::drawTabs() {
    const char* tabNames[] = {"STAT", "DATA", "RADIO", "MAP"};
    for (int i = 0; i < SCREEN_COUNT; i++) {
        ui->drawTab(i, SCREEN_COUNT, tabNames[i], i == currentScreen);
    }
}

inline void MenuSystem::fetchWeatherData() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[WEATHER] WiFi not connected");
        return;
    }
    HTTPClient http;
    String url = "http://api.open-meteo.com/v1/forecast?latitude=48.8566&longitude=2.3522&current_weather=true&hourly=temperature_2m,relativehumidity_2m";
    Serial.print("[WEATHER] Fetching: ");
    Serial.println(url);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode == 200) {
        String payload = http.getString();
        Serial.println("[WEATHER] Response received");
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (!error) {
            weatherTemp = doc["current_weather"]["temperature"];
            weatherCondition = "Clear";
            int weatherCode = doc["current_weather"]["weathercode"];
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
inline void MenuSystem::updateSensorValues() {
    if (currentScreen != SCREEN_STAT) return;

    // Récupérer les valeurs actuelles
    float currentTemp = sensors->getTemperature();
    float currentHumidity = sensors->getHumidity();
    float currentPressure = sensors->getPressure();
    float currentAltitude = sensors->getAltitude();
    int currentLightPercent = sensors->getLightPercent();

    // Mettre à jour uniquement les valeurs qui ont changé (pas d'effet rideau)
    int y = 60;

    // Température
    if (abs(currentTemp - cachedTemperature) > 0.1f || cachedTemperature == -999.0f) {
        cachedTemperature = currentTemp;
        String tempStr = String(currentTemp, 1) + " C";
        ui->updateStatLine(y, "TEMP", tempStr.c_str(), sensors->isTemperatureWarning());
    }
    y += 15;

    // Humidité
    if (abs(currentHumidity - cachedHumidity) > 0.5f || cachedHumidity == -999.0f) {
        cachedHumidity = currentHumidity;
        String humidityStr = String(currentHumidity, 0) + " %";
        ui->updateStatLine(y, "HUMIDITY", humidityStr.c_str(), sensors->isHumidityWarning());
    }
    y += 15;

    // Pression
    if (abs(currentPressure - cachedPressure) > 0.5f || cachedPressure == -999.0f) {
        cachedPressure = currentPressure;
        String pressureStr = String(currentPressure, 0) + " hPa";
        ui->updateStatLine(y, "PRESSURE", pressureStr.c_str(), sensors->isPressureWarning());
    }
    y += 15;

    // Altitude
    if (abs(currentAltitude - cachedAltitude) > 0.5f || cachedAltitude == -999.0f) {
        cachedAltitude = currentAltitude;
        String altitudeStr = String(currentAltitude, 0) + " m";
        ui->updateStatLine(y, "ALTITUDE", altitudeStr.c_str(), false);
    }
    y += 20;

    // Lumière (valeur + barre de progression)
    if (currentLightPercent != cachedLightPercent || cachedLightPercent == -1) {
        cachedLightPercent = currentLightPercent;
        String lightStr = String(currentLightPercent) + " %";
        ui->updateStatLine(y, "LIGHT", lightStr.c_str(), false);
        y += 12;
        // Redessiner la barre de progression
        ui->drawProgressBar(10, y, 220, 10, currentLightPercent);
    } else {
        y += 12;
    }

    // Note: on ne met pas à jour le statut des capteurs à chaque fois pour éviter le clignotement
}


inline void MenuSystem::addRadioMessage(const char* message) {
    if (radioMessageCount < 5) {
        radioMessages[radioMessageCount] = String(message);
        radioMessageCount++;
    }
}

#endif // MENU_SYSTEM_H
