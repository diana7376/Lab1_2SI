#ifndef LedModule_h
#define LedModule_h

const int GreenLedPin = 10;
const int RedLedPin   = 13;

void InitLeds();
void SetGreenLed(bool state);
void SetRedLed(bool state);

#endif
