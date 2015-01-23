#include <Arduino.h>
#include "LedButton.h"

LedButton::LedButton(int ledPin, int btnPin): SwitchButton(btnPin)
{
    _ledPin = ledPin;
    pinMode(_ledPin, OUTPUT);
}

/**
*  This function is used to switch off or switch on a led according to a button.
*/
void LedButton::switchLed()
{
    SwitchButton::makeSwitch();
    int myState = SwitchButton::getState();
    digitalWrite(_ledPin, myState);
}
