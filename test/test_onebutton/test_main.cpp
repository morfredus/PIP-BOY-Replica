#include <Arduino.h>
#include <OneButton.h>

#define BUTTON1_PIN 38
#define BUTTON2_PIN 39

OneButton button1(BUTTON1_PIN, true);
OneButton button2(BUTTON2_PIN, true);

void click1() {
  Serial.println("Button 1 click");
}
void click2() {
  Serial.println("Button 2 click");
}
void longPress1() {
  Serial.println("Button 1 long press");
}
void longPress2() {
  Serial.println("Button 2 long press");
}

void setup() {
  Serial.begin(115200);
  button1.attachClick(click1);
  button1.attachLongPressStart(longPress1);
  button2.attachClick(click2);
  button2.attachLongPressStart(longPress2);
}

void loop() {
  button1.tick();
  button2.tick();
  delay(10);
}
