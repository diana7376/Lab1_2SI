#include <Arduino.h>

#include "KeypadModule.h"
#include "LcdModule.h"
#include "LedModule.h"

const int PasswordLength = 4;
const char CorrectPassword[PasswordLength + 1] = "1234"; // null-terminated

void setup() {
    InitLcd();
    InitLeds();

    PrintLcd("Enter code:");
}

void loop() {
    static char inputPassword[PasswordLength + 1] = {0};
    static int index = 0;

    char key = GetKeyPress();   // read from keypad

    if (!key) {
        // nothing pressed
        return;
    }

    // '*' = clear/reset
    if (key == '*') {
        index = 0;
        inputPassword[0] = '\0';
        PrintLcd("Enter code:");
        return;
    }

    // we ignore '#' (could be used as "OK" later)
    if (key == '#') {
        return;
    }

    // store digit if we still have space
    if (index < PasswordLength) {
        inputPassword[index++] = key;
        inputPassword[index] = '\0';

        // Show **** on LCD (hide actual digits)
        LcdInstance.clear();
        LcdInstance.setCursor(0, 0);
        LcdInstance.print("Code:");
        LcdInstance.setCursor(0, 1);
        for (int i = 0; i < index; i++) {
            LcdInstance.print('*');
        }
    }

    // when we have 4 keys -> check password
    if (index == PasswordLength) {
        if (strncmp(inputPassword, CorrectPassword, PasswordLength) == 0) {
            PrintLcd("Access Granted!");
            SetGreenLed(true);
            SetRedLed(false);
        } else {
            PrintLcd("Access Denied!");
            SetRedLed(true);
            SetGreenLed(false);
        }

        delay(2000);
        SetGreenLed(false);
        SetRedLed(false);

        // prepare for next attempt
        index = 0;
        inputPassword[0] = '\0';
        PrintLcd("Enter code:");
    }
}
