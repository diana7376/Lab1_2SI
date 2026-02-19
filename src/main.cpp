#include <Arduino.h>
#include <stdio.h>

#include "KeypadModule.h"
#include "LcdModule.h"
#include "LedModule.h"

const int PasswordLength = 4;
const char CorrectPassword[PasswordLength + 1] = "1234"; // null-terminated

// mici hack-uri ca să putem folosi printf pe Serial
int serial_putchar(char c, FILE* f) {
    if (c == '\n') Serial.print('\r');
    Serial.print(c);
    return 0;
}

FILE serial_stdout;

void setup() {
    Serial.begin(9600);

    // redirect printf către Serial
    fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &serial_stdout;

    InitLcd();
    InitLeds();

    PrintLcd("Ready. Use Serial");
    delay(1000);
    PrintLcd("for code.");
    printf("Enter 4-digit code: ");
}

void loop() {
    char inputPassword[PasswordLength + 1] = {0};

    // citire simplă de la Serial (fără scanf complicat)
    int idx = 0;
    while (idx < PasswordLength) {
        if (Serial.available() > 0) {
            char c = Serial.read();
            if (c == '\n' || c == '\r') continue;
            inputPassword[idx++] = c;
        }
    }
    inputPassword[PasswordLength] = '\0';

    // Validare
    if (strncmp(inputPassword, CorrectPassword, PasswordLength) == 0) {
        printf("\nAccess granted!\n");
        PrintLcd("Access Granted!");
        SetGreenLed(true);
        SetRedLed(false);
    } else {
        printf("\nInvalid code!\n");
        PrintLcd("Access Denied!");
        SetRedLed(true);
        SetGreenLed(false);
    }

    delay(2000);
    SetGreenLed(false);
    SetRedLed(false);
    PrintLcd("Ready. Use Serial");
    delay(500);
    PrintLcd("for code.");
    printf("Enter 4-digit code: ");
}
