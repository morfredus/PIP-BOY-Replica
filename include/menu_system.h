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
    // Efface l'écran uniquement si on change de menu
    bool needClear = false;
    switch (currentScreen) {
        case SCREEN_STAT:
            sensors->update();
            needsFullRedraw = true;
            break;
        case SCREEN_DATA:
            fetchWeatherData();
            needsFullRedraw = true;
            break;
        case SCREEN_RADIO:
            needClear = true;
            subMenuIndex = (subMenuIndex + 1) % radioMessageCount;
            needsFullRedraw = true;
            break;
        case SCREEN_MAP:
            needClear = true;
            radarSweepAngle = 0;
            needsFullRedraw = true;
            break;
    }
    if (needClear) ui->getTFT()->fillScreen(PIPBOY_BLACK);
    redraw();
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
    if (currentScreen == SCREEN_MAP && currentTime - animationTimer > 100) {
        animationTimer = currentTime;
        animationFrame++;
        radarSweepAngle = (radarSweepAngle + 5) % 360;
        needsFullRedraw = true;
        redraw();
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
    // ui->getTFT()->fillScreen(PIPBOY_BLACK); // SUPPRIMÉ
    ui->drawHeader("MAP");
    ui->clearContent();
    drawTabs();
    int centerX = 120;
    int centerY = 140;
    int radius = 80;
    ui->drawRadar(centerX, centerY, radius);
    float rad = radarSweepAngle * 3.14159 / 180.0;
    int sweepX = centerX + radius * cos(rad);
    int sweepY = centerY + radius * sin(rad);
    ui->drawRadarSweepLine(centerX, centerY, sweepX, sweepY);
    ui->drawRadarBlip(centerX, centerY, 30, (radarSweepAngle + 45) % 360, false);
    ui->drawRadarBlip(centerX, centerY, 50, (radarSweepAngle + 120) % 360, false);
    ui->drawRadarBlip(centerX, centerY, 65, (radarSweepAngle + 220) % 360, true);
    char coordBuffer[30];
    snprintf(coordBuffer, sizeof(coordBuffer), "N:%d E:%d",
             (int)(sensors->getAltitude() + 1000),
             (int)(sensors->getPressure()));
    ui->drawStatLine(60, "COORD", coordBuffer, false);
    ui->drawFooter("NEXT", "SCAN");
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

    // Vérifier si au moins une valeur a changé (avec un seuil de tolérance)
    bool hasChanged = (abs(currentTemp - cachedTemperature) > 0.1f) ||
                      (abs(currentHumidity - cachedHumidity) > 0.5f) ||
                      (abs(currentPressure - cachedPressure) > 0.5f) ||
                      (abs(currentAltitude - cachedAltitude) > 0.5f) ||
                      (currentLightPercent != cachedLightPercent);

    // Ne redessiner que si une valeur a changé
    if (!hasChanged && cachedTemperature != -999.0f) {
        return;
    }

    // Mettre à jour le cache
    cachedTemperature = currentTemp;
    cachedHumidity = currentHumidity;
    cachedPressure = currentPressure;
    cachedAltitude = currentAltitude;
    cachedLightPercent = currentLightPercent;

    // Effacer uniquement la zone de contenu (pas l'en-tête ni le pied de page)
    ui->clearContent();

    // Redessiner les onglets
    drawTabs();

    // Redessiner les valeurs
    int y = 60;
    String tempStr = String(currentTemp, 1) + " C";
    ui->drawStatLine(y, "TEMP", tempStr.c_str(), sensors->isTemperatureWarning());
    y += 15;
    String humidityStr = String(currentHumidity, 0) + " %";
    ui->drawStatLine(y, "HUMIDITY", humidityStr.c_str(), sensors->isHumidityWarning());
    y += 15;
    String pressureStr = String(currentPressure, 0) + " hPa";
    ui->drawStatLine(y, "PRESSURE", pressureStr.c_str(), sensors->isPressureWarning());
    y += 15;
    String altitudeStr = String(currentAltitude, 0) + " m";
    ui->drawStatLine(y, "ALTITUDE", altitudeStr.c_str(), false);
    y += 20;
    String lightStr = String(currentLightPercent) + " %";
    ui->drawStatLine(y, "LIGHT", lightStr.c_str(), false);
    y += 12;
    ui->drawProgressBar(10, y, 220, 10, currentLightPercent);
    y += 25;
    char statusBuffer[50];
    sensors->getStatusString(statusBuffer, sizeof(statusBuffer));
    ui->drawStatLine(y, "SENSORS", statusBuffer, !sensors->isAnyReady());
}


inline void MenuSystem::addRadioMessage(const char* message) {
    if (radioMessageCount < 5) {
        radioMessages[radioMessageCount] = String(message);
        radioMessageCount++;
    }
}

#endif // MENU_SYSTEM_H
