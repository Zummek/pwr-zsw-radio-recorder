#include "PushButton.hpp"

PushButton::PushButton(uint8_t pin)
    :btnPin(pin), state(false), lastState(false),
    lastDebounceTime(0)
{}

void PushButton::init()
{
    pinMode(btnPin, INPUT);
}

bool PushButton::isPressed()
{
    int reading = digitalRead(btnPin);

    if (reading != lastState)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (reading != state)
        {
            state = reading;

            if (state == HIGH)
            {
                lastState = reading;
                return true;
            }
        }
    }

    lastState = reading;

    return false;
}
