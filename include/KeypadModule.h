#ifndef KeypadModule_h
#define KeypadModule_h

#include <Keypad.h>

const byte RowsCount = 4;
const byte ColsCount = 4;

extern const char Keys[RowsCount][ColsCount];
extern byte RowPins[RowsCount];
extern byte ColPins[ColsCount];
extern Keypad KeypadInstance;

char GetKeyPress();

#endif
