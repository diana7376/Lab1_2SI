#include "LcdModule.h"

// I2C LCD: address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C LcdInstance(0x27, 16, 2);

void InitLcd() {
    LcdInstance.init();        // if this fails, change to LcdInstance.begin(16, 2);
    LcdInstance.backlight();
    LcdInstance.clear();
}

void PrintLcd(const char* message) {
    LcdInstance.clear();
    LcdInstance.setCursor(0, 0);
    LcdInstance.print(message);
}
