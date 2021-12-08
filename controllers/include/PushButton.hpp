#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <Arduino.h>

class PushButton
{
private:
    uint8_t btnPin;
    bool state;
    bool lastState;
    unsigned long lastDebounceTime;
    const unsigned long debounceDelay = 50;

public:
    PushButton(uint8_t pin);
    void init();
    bool isPressed();
};

#endif