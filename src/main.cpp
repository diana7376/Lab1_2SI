#include <Arduino.h>

#include "KeypadModule.h"
#include "LcdModule.h"
#include "LedModule.h"

const int PasswordLength = 4;
char CurrentPassword[PasswordLength + 1] = "1234";   // can be changed at runtime

enum SystemState {
    STATE_IDLE,          // waiting for code
    STATE_TYPING_CODE,   // user entering code
    STATE_VALIDATION,    // checking code
    STATE_CHANGE_PROMPT, // user chose to change password
    STATE_TYPING_NEW,    // entering new password
    STATE_CONFIRM_NEW    // confirming new password
};

SystemState currentState = STATE_IDLE;

void ResetToIdle() {
    currentState = STATE_IDLE;
    PrintLcd("Enter code:");
    Serial.println("State -> IDLE, LCD: \"Enter code:\"");
}

void setup() {
    Serial.begin(9600);
    while (!Serial) { ; }

    Serial.println("=== System boot ===");

    InitLcd();
    InitLeds();

    ResetToIdle();
    Serial.print("Initial password = ");
    Serial.println(CurrentPassword);
}

void loop() {
    static char inputBuffer[PasswordLength + 1] = {0};
    static char newPassword[PasswordLength + 1] = {0};
    static int index = 0;

    char key = GetKeyPress();

    if (!key) return;  // nothing pressed

    Serial.print("Key pressed: ");
    Serial.println(key);

    // Global clear with '*'
    if (key == '*') {
        index = 0;
        inputBuffer[0] = '\0';
        newPassword[0] = '\0';

        if (currentState == STATE_TYPING_NEW || currentState == STATE_CONFIRM_NEW) {
            PrintLcd("Change canceled");
            Serial.println("Action: cancel password change");
            delay(1500);
        }

        ResetToIdle();
        return;
    }

    // From IDLE: 'A' enters change-password mode
    if (currentState == STATE_IDLE && key == 'A') {
        currentState = STATE_CHANGE_PROMPT;
        PrintLcd("Change password");
        Serial.println("State -> CHANGE_PROMPT, LCD: \"Change password\"");
        delay(1500);

        currentState = STATE_TYPING_NEW;
        index = 0;
        newPassword[0] = '\0';
        PrintLcd("New code:");
        Serial.println("State -> TYPING_NEW, LCD: \"New code:\"");
        return;
    }

    // Ignore '#' for simplicity
    if (key == '#') {
        Serial.println("Action: '#' ignored");
        return;
    }

    /* -------- NORMAL LOGIN FLOW -------- */

    if (currentState == STATE_IDLE || currentState == STATE_TYPING_CODE) {
        currentState = STATE_TYPING_CODE;

        if (index < PasswordLength) {
            inputBuffer[index++] = key;
            inputBuffer[index] = '\0';

            Serial.print("LOGIN buffer: \"");
            Serial.print(inputBuffer);
            Serial.println("\"");

            // masked display
            LcdInstance.clear();
            LcdInstance.setCursor(0, 0);
            LcdInstance.print("Code:");
            LcdInstance.setCursor(0, 1);
            for (int i = 0; i < index; i++) {
                LcdInstance.print('*');
            }
        }

        if (index == PasswordLength) {
            currentState = STATE_VALIDATION;
            Serial.println("State -> VALIDATION");
            Serial.print("Checking code \"");
            Serial.print(inputBuffer);
            Serial.print("\" against \"");
            Serial.print(CurrentPassword);
            Serial.println("\"");

            if (strncmp(inputBuffer, CurrentPassword, PasswordLength) == 0) {
                PrintLcd("Access Granted!");
                SetGreenLed(true);
                SetRedLed(false);
                Serial.println("Result: CORRECT code");
            } else {
                PrintLcd("Access Denied!");
                SetRedLed(true);
                SetGreenLed(false);
                Serial.println("Result: WRONG code");
            }

            delay(2000);
            SetGreenLed(false);
            SetRedLed(false);
            Serial.println("LEDs OFF, login attempt finished");

            index = 0;
            inputBuffer[0] = '\0';
            ResetToIdle();
        }

        return;
    }

    /* -------- CHANGE PASSWORD: ENTER NEW -------- */

    if (currentState == STATE_TYPING_NEW) {
        if (index < PasswordLength) {
            newPassword[index++] = key;
            newPassword[index] = '\0';

            Serial.print("NEW password buffer: \"");
            Serial.print(newPassword);
            Serial.println("\"");

            LcdInstance.clear();
            LcdInstance.setCursor(0, 0);
            LcdInstance.print("New code:");
            LcdInstance.setCursor(0, 1);
            for (int i = 0; i < index; i++) {
                LcdInstance.print('*');
            }
        }

        if (index == PasswordLength) {
            currentState = STATE_CONFIRM_NEW;
            Serial.println("State -> CONFIRM_NEW");
            Serial.print("First entry for new password = \"");
            Serial.print(newPassword);
            Serial.println("\"");

            index = 0;
            inputBuffer[0] = '\0';
            PrintLcd("Confirm code:");
            Serial.println("LCD: \"Confirm code:\"");
        }

        return;
    }

    /* -------- CHANGE PASSWORD: CONFIRM NEW -------- */

    if (currentState == STATE_CONFIRM_NEW) {
        if (index < PasswordLength) {
            inputBuffer[index++] = key;
            inputBuffer[index] = '\0';

            Serial.print("CONFIRM buffer: \"");
            Serial.print(inputBuffer);
            Serial.println("\"");

            LcdInstance.clear();
            LcdInstance.setCursor(0, 0);
            LcdInstance.print("Confirm:");
            LcdInstance.setCursor(0, 1);
            for (int i = 0; i < index; i++) {
                LcdInstance.print('*');
            }
        }

        if (index == PasswordLength) {
            Serial.print("Comparing new password \"");
            Serial.print(newPassword);
            Serial.print("\" with confirmation \"");
            Serial.print(inputBuffer);
            Serial.println("\"");

            if (strncmp(newPassword, inputBuffer, PasswordLength) == 0) {
                // apply new password
                strncpy(CurrentPassword, newPassword, PasswordLength);
                CurrentPassword[PasswordLength] = '\0';

                PrintLcd("Password saved");
                Serial.print("Password successfully changed to ");
                Serial.println(CurrentPassword);
            } else {
                PrintLcd("Mismatch!");
                Serial.println("Password change FAILED: mismatch");
            }

            delay(2000);
            index = 0;
            inputBuffer[0] = '\0';
            newPassword[0] = '\0';
            ResetToIdle();
        }

        return;
    }
}
