#include <Arduino.h>
#include "LedModule.h"

void InitLeds() {
    pinMode(GreenLedPin, OUTPUT);
    pinMode(RedLedPin, OUTPUT);
    digitalWrite(GreenLedPin, LOW);
    digitalWrite(RedLedPin, LOW);
}

void SetGreenLed(bool state) {
    digitalWrite(GreenLedPin, state);
}

void SetRedLed(bool state) {
    digitalWrite(RedLedPin, state);
}
