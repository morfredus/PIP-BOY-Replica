#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

class Button {
private:
    int pin;
    bool lastState;
    bool currentState;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;
    bool pressed;
    bool released;

public:
    Button(int buttonPin, unsigned long debounce = 50) {
        pin = buttonPin;
        debounceDelay = debounce;
        lastState = HIGH;  // Pull-up, donc HIGH au repos
        currentState = HIGH;
        lastDebounceTime = 0;
        pressed = false;
        released = false;
    }

    void begin() {
        pinMode(pin, INPUT_PULLUP);
        currentState = digitalRead(pin);
        lastState = currentState;
    }

    void update() {
        bool reading = digitalRead(pin);
        pressed = false;
        released = false;

        // Si l'état a changé, réinitialiser le timer de debounce
        if (reading != lastState) {
            lastDebounceTime = millis();
        }

        // Si suffisamment de temps s'est écoulé
        if ((millis() - lastDebounceTime) > debounceDelay) {
            // Si l'état a vraiment changé
            if (reading != currentState) {
                currentState = reading;

                // Détection de pression (passage de HIGH à LOW car pull-up)
                if (currentState == LOW) {
                    pressed = true;
                }
                // Détection de relâchement
                else {
                    released = true;
                }
            }
        }

        lastState = reading;
    }

    bool isPressed() {
        return pressed;
    }

    bool isReleased() {
        return released;
    }

    bool isHeld() {
        return (currentState == LOW);
    }

    bool isDown() {
        return (currentState == LOW);
    }
};



class ButtonHandler {
private:
    Button* button1;
    Button* button2;
    Button* buttonBoot;

    unsigned long button1PressTime;
    unsigned long button2PressTime;
    bool button1LongPressFired;
    bool button2LongPressFired;
    const unsigned long LONG_PRESS_DURATION = 2000;

public:
    ButtonHandler(int pin1, int pin2, int pinBoot) {
        button1 = new Button(pin1);
        button2 = new Button(pin2);
        buttonBoot = new Button(pinBoot);
        button1PressTime = 0;
        button2PressTime = 0;
        button1LongPressFired = false;
        button2LongPressFired = false;
    }

    void begin() {
        button1->begin();
        button2->begin();
        buttonBoot->begin();
    }

    void update() {
        button1->update();
        button2->update();
        buttonBoot->update();

        // Gestion bouton 1
        if (button1->isPressed()) {
            button1PressTime = millis();
            button1LongPressFired = false;
        }
        if (button1->isHeld() && !button1LongPressFired) {
            if (millis() - button1PressTime >= LONG_PRESS_DURATION) {
                button1LongPressFired = true;
            }
        }
        if (button1->isReleased()) {
            button1LongPressFired = false;
        }

        // Gestion bouton 2
        if (button2->isPressed()) {
            button2PressTime = millis();
            button2LongPressFired = false;
        }
        if (button2->isHeld() && !button2LongPressFired) {
            if (millis() - button2PressTime >= LONG_PRESS_DURATION) {
                button2LongPressFired = true;
            }
        }
        if (button2->isReleased()) {
            button2LongPressFired = false;
        }
    }

    // Getters pour les boutons
    bool button1Pressed() {
        // Appui court : press/release sans long
        if (button1->isReleased() && (millis() - button1PressTime < LONG_PRESS_DURATION) && !button1LongPressFired) return true;
        return false;
    }
    bool button2Pressed() {
        if (button2->isReleased() && (millis() - button2PressTime < LONG_PRESS_DURATION) && !button2LongPressFired) return true;
        return false;
    }
    bool bootPressed() { return buttonBoot->isPressed(); }

    bool button1LongPressed() {
        if (button1LongPressFired && button1->isHeld()) {
            button1LongPressFired = false;
            return true;
        }
        return false;
    }
    bool button2LongPressed() {
        if (button2LongPressFired && button2->isHeld()) {
            button2LongPressFired = false;
            return true;
        }
        return false;
    }

    ~ButtonHandler() {
        delete button1;
        delete button2;
        delete buttonBoot;
    }
};

#endif // BUTTON_HANDLER_H
