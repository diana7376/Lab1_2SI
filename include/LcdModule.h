#ifndef LcdModule_h
#define LcdModule_h

#include <Arduino.h>
#include <LiquidCrystal.h>

extern LiquidCrystal LcdInstance;

void InitLcd();
void PrintLcd(const char* message);

#endif
