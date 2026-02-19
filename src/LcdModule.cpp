#include "LcdModule.h"

LiquidCrystal LcdInstance(12, 11, 5, 4, 3, 2);

void InitLcd() {
    LcdInstance.begin(16, 2);
    LcdInstance.clear();
}

void PrintLcd(const char* message) {
    LcdInstance.clear();
    LcdInstance.setCursor(0, 0);
    LcdInstance.print(message);
}
