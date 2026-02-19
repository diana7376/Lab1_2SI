#include "KeypadModule.h"

const char Keys[RowsCount][ColsCount] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte RowPins[RowsCount] = {9, 8, 7, 6};
byte ColPins[ColsCount] = {5, 4, 3, 2};

Keypad KeypadInstance = Keypad(makeKeymap(Keys), RowPins, ColPins, RowsCount, ColsCount);

char GetKeyPress() {
  return KeypadInstance.getKey();
}
