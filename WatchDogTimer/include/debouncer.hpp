#ifndef DEBOUNCER_HPP
#define DEBOUNCER_HPP

#include <Arduino.h>

using TickType = unsigned long;

const TickType debounceDelay = 1000; // the debounce time

/**
 * @brief Class for debouncing a digital input
 *
 */
class Debouncer
{
private:
    uint8_t btnPin;
    bool isPressed;
    bool lastState;
    unsigned long lastDebounceTime;
    const unsigned long debounceDelay = 50; // 50 ms debounce time

public:
    void begin(uint8_t btn)
    {
        btnPin = btn;
        isPressed = false;
        lastState = HIGH;
        pinMode(btnPin, INPUT_PULLUP);
        lastDebounceTime = 0;
    }

    bool update()
    {
        bool reading = digitalRead(btnPin);
        if (reading != lastState)
        {
            lastDebounceTime = millis();
        }

        if ((millis() - lastDebounceTime) > debounceDelay)
        {
            if (reading != isPressed)
            {
                isPressed = reading;
            }
        }

        lastState = reading;
        return !isPressed; // Return true if button is pressed (considering INPUT_PULLUP)
    }

    bool isButtonPressed()
    {
        return !isPressed; // Considering INPUT_PULLUP
    }
};

#endif // DEBOUNCER_HPP