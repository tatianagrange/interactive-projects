#ifndef SwitchButton_h
#define SwitchButton_h

#include <Arduino.h>

class SwitchButton
{
private:
    int _btnPin;
    int _previous;
    int _state;
    unsigned long _time;
    long _debounce;
    boolean _launchSignal;
    
public:
    SwitchButton(int btn);
    void makeSwitch();
    int getState();
    boolean canLaunchSignal();
};

#endif
