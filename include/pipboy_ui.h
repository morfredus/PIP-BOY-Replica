#ifndef PIPBOY_UI_H
#define PIPBOY_UI_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// Couleurs Pip-Boy (thème vert phosphorescent)
#define PIPBOY_GREEN     0x07E0  // Vert principal
#define PIPBOY_DARKGREEN 0x0320  // Vert foncé
#define PIPBOY_BLACK     0x0000  // Noir
#define PIPBOY_LIGHTGREEN 0x9FF6 // Vert clair pour highlights

class PipBoyUI {
private:
    Adafruit_ST7789* tft;
    uint16_t screenWidth;
    uint16_t screenHeight;
    unsigned long scanlineTimer;
    bool scanlinePhase;

public:
    PipBoyUI(Adafruit_ST7789* display) {
        tft = display;
        screenWidth = 240;
        screenHeight = 240;
        scanlineTimer = 0;
        scanlinePhase = false;
    }

    // Initialisation de l'écran Pip-Boy
    void begin() {
        tft->fillScreen(PIPBOY_BLACK);
        drawBootSequence();
    }

    // Séquence de démarrage style Pip-Boy
    void drawBootSequence() {
        tft->fillScreen(PIPBOY_BLACK);
        tft->setTextColor(PIPBOY_GREEN);
        tft->setTextSize(1);

        tft->setCursor(10, 50);
        tft->println("ROBCO INDUSTRIES");
        tft->println("  UNIFIED OPERATING");
        tft->println("  SYSTEM");
        delay(500);

        tft->setCursor(10, 100);
        tft->println("COPYRIGHT 2075-2077");
        tft->println("ROBCO INDUSTRIES");
        delay(500);

        tft->setCursor(10, 150);
        tft->println("-Server 1-");
        delay(300);
        tft->println("> RUN DEBUG/ACCOUNTS.F");
        delay(800);

        tft->fillScreen(PIPBOY_BLACK);
    }

    // Dessiner l'en-tête Pip-Boy
    void drawHeader(const char* title) {
        // Ligne du haut
        tft->drawFastHLine(0, 0, screenWidth, PIPBOY_GREEN);
        tft->drawFastHLine(0, 1, screenWidth, PIPBOY_GREEN);

        // Titre centré
        tft->setTextSize(2);
        tft->setTextColor(PIPBOY_BLACK, PIPBOY_GREEN);
        int16_t x1, y1;
        uint16_t w, h;
        tft->getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
        int x = (screenWidth - w) / 2;
        tft->fillRect(0, 2, screenWidth, 18, PIPBOY_GREEN);
        tft->setCursor(x, 5);
        tft->println(title);

        // Ligne du bas de l'en-tête
        tft->drawFastHLine(0, 20, screenWidth, PIPBOY_GREEN);
        tft->drawFastHLine(0, 21, screenWidth, PIPBOY_GREEN);
    }

    // Dessiner le pied de page avec les boutons
    void drawFooter(const char* btn1Text, const char* btn2Text) {
        int footerY = screenHeight - 25;

        // Ligne du haut du pied
        tft->drawFastHLine(0, footerY, screenWidth, PIPBOY_GREEN);
        tft->drawFastHLine(0, footerY + 1, screenWidth, PIPBOY_GREEN);

        // Texte des boutons
        tft->setTextSize(1);
        tft->setTextColor(PIPBOY_GREEN);

        tft->setCursor(10, footerY + 6);
        tft->print("[BTN1] ");
        tft->print(btn1Text);

        tft->setCursor(10, footerY + 15);
        tft->print("[BTN2] ");
        tft->print(btn2Text);
    }

    // Dessiner une boîte de texte stylisée
    void drawTextBox(int x, int y, int w, int h, const char* text, bool selected = false) {
        uint16_t color = selected ? PIPBOY_LIGHTGREEN : PIPBOY_GREEN;
        uint16_t bgColor = selected ? PIPBOY_DARKGREEN : PIPBOY_BLACK;

        // Bordure
        tft->drawRect(x, y, w, h, color);
        if (selected) {
            tft->drawRect(x + 1, y + 1, w - 2, h - 2, color);
        }

        // Fond
        tft->fillRect(x + 2, y + 2, w - 4, h - 4, bgColor);

        // Texte
        tft->setTextColor(color);
        tft->setTextSize(1);
        tft->setCursor(x + 5, y + 5);
        tft->print(text);
    }

    // Dessiner un menu tab
    void drawTab(int index, int totalTabs, const char* tabName, bool selected) {
        int tabWidth = screenWidth / totalTabs;
        int x = index * tabWidth;
        int y = 25;
        int h = 20;

        uint16_t color = selected ? PIPBOY_LIGHTGREEN : PIPBOY_DARKGREEN;
        uint16_t textColor = selected ? PIPBOY_BLACK : PIPBOY_GREEN;

        // Fond du tab
        tft->fillRect(x, y, tabWidth - 2, h, color);
        tft->drawRect(x, y, tabWidth - 2, h, PIPBOY_GREEN);

        // Texte centré
        tft->setTextSize(1);
        tft->setTextColor(textColor);
        int16_t x1, y1;
        uint16_t w, th;
        tft->getTextBounds(tabName, 0, 0, &x1, &y1, &w, &th);
        int textX = x + (tabWidth - 2 - w) / 2;
        tft->setCursor(textX, y + 6);
        tft->print(tabName);
    }

    // Dessiner une ligne de stat
    void drawStatLine(int y, const char* label, const char* value, bool warning = false) {
        tft->setTextSize(1);
        uint16_t color = warning ? PIPBOY_LIGHTGREEN : PIPBOY_GREEN;

        tft->setTextColor(color);
        tft->setCursor(10, y);
        tft->print(label);
        tft->print(": ");

        if (warning) {
            tft->print("!");
        }
        tft->print(value);
    }

    // Dessiner une barre de progression
    void drawProgressBar(int x, int y, int w, int h, int percent) {
        // Bordure
        tft->drawRect(x, y, w, h, PIPBOY_GREEN);

        // Remplissage
        int fillWidth = (w - 4) * percent / 100;
        tft->fillRect(x + 2, y + 2, fillWidth, h - 4, PIPBOY_GREEN);

        // Reste vide
        tft->fillRect(x + 2 + fillWidth, y + 2, w - 4 - fillWidth, h - 4, PIPBOY_BLACK);
    }

    // Effet scanline animé
    void updateScanlines() {
        unsigned long currentTime = millis();
        if (currentTime - scanlineTimer > 2000) {
            scanlineTimer = currentTime;
            scanlinePhase = !scanlinePhase;

            if (scanlinePhase) {
                // Dessiner des lignes subtiles
                for (int y = 50; y < screenHeight - 30; y += 4) {
                    tft->drawFastHLine(0, y, screenWidth, PIPBOY_DARKGREEN);
                }
            }
        }
    }

    // Dessiner un radar circulaire (pour écran MAP)
    void drawRadar(int centerX, int centerY, int radius) {
        // Cercles concentriques
        for (int r = radius; r > 0; r -= 20) {
            tft->drawCircle(centerX, centerY, r, PIPBOY_DARKGREEN);
        }

        // Lignes croisées
        tft->drawLine(centerX - radius, centerY, centerX + radius, centerY, PIPBOY_DARKGREEN);
        tft->drawLine(centerX, centerY - radius, centerX, centerY + radius, PIPBOY_DARKGREEN);

        // Cercle principal
        tft->drawCircle(centerX, centerY, radius, PIPBOY_GREEN);
        tft->drawCircle(centerX, centerY, radius - 1, PIPBOY_GREEN);

        // Point central
        tft->fillCircle(centerX, centerY, 3, PIPBOY_LIGHTGREEN);
    }

    // Dessiner un blip sur le radar
    void drawRadarBlip(int centerX, int centerY, int distance, int angle, bool hostile = false) {
        float rad = angle * 3.14159 / 180.0;
        int x = centerX + distance * cos(rad);
        int y = centerY + distance * sin(rad);

        uint16_t color = hostile ? PIPBOY_LIGHTGREEN : PIPBOY_GREEN;
        tft->fillCircle(x, y, 2, color);
    }

    // Effacer la zone de contenu (entre header et footer)
    void clearContent() {
        tft->fillRect(0, 48, screenWidth, screenHeight - 48 - 30, PIPBOY_BLACK);
    }

    // Dessiner un message clignotant
    void drawBlinkingMessage(int x, int y, const char* message) {
        if ((millis() / 500) % 2 == 0) {
            tft->setTextColor(PIPBOY_LIGHTGREEN);
            tft->setTextSize(1);
            tft->setCursor(x, y);
            tft->print(message);
        }
    }

    // Dessiner une ligne sur le radar (pour le sweep animé)
    void drawRadarSweepLine(int x1, int y1, int x2, int y2) {
        tft->drawLine(x1, y1, x2, y2, PIPBOY_LIGHTGREEN);
    }
};

#endif // PIPBOY_UI_H
