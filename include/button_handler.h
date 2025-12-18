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

    // Pour détecter les appuis longs/courts
    unsigned long button1PressTime;
    unsigned long button2PressTime;
    bool button1LongPress;
    bool button2LongPress;
    bool button1ShortPress;
    bool button2ShortPress;
    bool button1Handled;
    bool button2Handled;
    const unsigned long LONG_PRESS_DURATION = 2000; // 2 secondes (plus fiable)

public:
    ButtonHandler(int pin1, int pin2, int pinBoot) {
        button1 = new Button(pin1);
        button2 = new Button(pin2);
        buttonBoot = new Button(pinBoot);
        button1PressTime = 0;
        button2PressTime = 0;
        button1LongPress = false;
        button2LongPress = false;
        button1ShortPress = false;
        button2ShortPress = false;
        button1Handled = false;
        button2Handled = false;
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
            button1LongPress = false;
            button1ShortPress = false;
            button1Handled = false;
        }
        if (button1->isHeld() && !button1Handled) {
            if ((millis() - button1PressTime >= LONG_PRESS_DURATION) && !button1LongPress) {
                button1LongPress = true;
                button1Handled = true;
            }
        }
        if (button1->isReleased() && !button1Handled) {
            if ((millis() - button1PressTime < LONG_PRESS_DURATION)) {
                button1ShortPress = true;
            }
            button1Handled = true;
        }
        if (button1Handled && button1->isReleased()) {
            // Reset pour le prochain cycle
            button1LongPress = false;
            button1ShortPress = false;
        }

        // Gestion bouton 2
        if (button2->isPressed()) {
            button2PressTime = millis();
            button2LongPress = false;
            button2ShortPress = false;
            button2Handled = false;
        }
        if (button2->isHeld() && !button2Handled) {
            if ((millis() - button2PressTime >= LONG_PRESS_DURATION) && !button2LongPress) {
                button2LongPress = true;
                button2Handled = true;
            }
        }
        if (button2->isReleased() && !button2Handled) {
            if ((millis() - button2PressTime < LONG_PRESS_DURATION)) {
                button2ShortPress = true;
            }
            button2Handled = true;
        }
        if (button2Handled && button2->isReleased()) {
            button2LongPress = false;
            button2ShortPress = false;
        }
    }

    // Getters pour les boutons
    bool button1Pressed() { bool v = button1ShortPress; button1ShortPress = false; return v; }
    bool button2Pressed() { bool v = button2ShortPress; button2ShortPress = false; return v; }
    bool bootPressed() { return buttonBoot->isPressed(); }

    bool button1Released() { return button1->isReleased(); }
    bool button2Released() { return button2->isReleased(); }
    bool bootReleased() { return buttonBoot->isReleased(); }

    bool button1Held() { return button1->isHeld(); }
    bool button2Held() { return button2->isHeld(); }
    bool bootHeld() { return buttonBoot->isHeld(); }

    bool button1LongPressed() { bool v = button1LongPress; button1LongPress = false; return v; }
    bool button2LongPressed() { bool v = button2LongPress; button2LongPress = false; return v; }

    ~ButtonHandler() {
        delete button1;
        delete button2;
        delete buttonBoot;
    }
};

#endif // BUTTON_HANDLER_H
