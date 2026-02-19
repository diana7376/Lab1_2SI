#ifndef LcdModule_h
#define LcdModule_h

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C LcdInstance;

void InitLcd();
void PrintLcd(const char* message);

#endif
