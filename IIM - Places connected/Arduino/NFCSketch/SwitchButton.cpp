#include <Arduino.h>
#include "SwitchButton.h"

SwitchButton::SwitchButton(int btnPin)
{
    _btnPin = btnPin;
    pinMode(_btnPin, INPUT);
    _state = HIGH;
    _previous = LOW;
    _time = 0;
    _debounce = 500;
    _launchSignal = false;
}


/**
*  This function is used to switch off or switch on a led according to a button.
*/
void SwitchButton::makeSwitch()
{
    int reading = digitalRead(_btnPin);
    if (reading == HIGH && _previous == LOW && millis() - _time > _debounce) {
        _launchSignal = true;
        if (_state == HIGH){
            _state = LOW;
        }else{
            _state = HIGH;
        }
        
        _time = millis();
    }
    else{
       _launchSignal = false; 
    }
    _previous = reading;
}

int SwitchButton::getState()
{
  return _state;
}

boolean SwitchButton::canLaunchSignal(){
  return _launchSignal;
}
