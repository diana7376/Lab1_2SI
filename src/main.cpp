#include <Arduino.h>
#include "KeypadModule.h"
#include "LcdModule.h"
#include "LedModule.h"

const int PasswordLength = 4;
const char CorrectPassword[PasswordLength + 1] = "1234";  // Null-terminated

void setup() {
  Serial.begin(9600);  // For STDIO printf/scanf via Serial Monitor
  InitLcd();
  InitLeds();
  
  PrintLcd("Ready. Use Serial\nfor code.");
  printf("Enter 4-digit code: ");
}

void loop() {
  char inputPassword[PasswordLength + 1] = {0};
  
  // Use STDIO scanf for input (type in Serial Monitor, press Enter)
  scanf("%s", inputPassword);
  
  // Validate
  if (strncmp(inputPassword, CorrectPassword, PasswordLength) == 0) {
    printf("Access granted!\n");
    PrintLcd("Access Granted!");
    SetGreenLed(true);
  } else {
    printf("Invalid code!\n");
    PrintLcd("Access Denied!");
    SetRedLed(true);
  }
  
  delay(2000);  // Show message
  SetGreenLed(false);  // Turn off both LEDs
  SetRedLed(false);
  printf("Enter 4-digit code: ");
}
