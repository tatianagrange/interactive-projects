#ifndef LedButton_h
#define LedButton_h

#include <Arduino.h>
#include "SwitchButton.h"

class LedButton: public SwitchButton
{
private:
    int _ledPin;
    
public:
    LedButton(int led, int btn);
    void switchLed();
};

#endif
