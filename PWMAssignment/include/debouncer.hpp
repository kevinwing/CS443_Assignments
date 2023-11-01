#ifndef DEBOUNCER_HPP
#define DEBOUNCER_HPP

#include <Arduino.h>

using TickType = unsigned long;

const TickType debounceDelay = 1000; // the debounce time

/**
 * @brief Class for debouncing a digital input
 * 
 */
class Debouncer {
    private:
        uint8_t btnPin; // pin number of button
        uint8_t state; // current state of button
        uint8_t lastState; // previous state of button
        TickType lastDebounceTime; // last time button state changed
    
    public:
        /**
         * @brief perform some setup
         * 
         */
        void begin(uint8_t btn)
        {
            btnPin = btn;
            state = HIGH;
            lastState = HIGH;
            // set pin to input
            DDRC &= ~(1 << btnPin);
        }

        /**
         * @brief debounce switch input signal and return true if pressed
         * 
         * @return true 
         * @return false 
         */
        bool debounce()
        {
            uint8_t tempState = PINC & (1 << btnPin); // read input pin

            if (tempState != lastState)
            {
                lastDebounceTime = micros();
            }

            if ((micros() - lastDebounceTime) > debounceDelay)
            {
                if (tempState != state)
                {
                    state = tempState;
                    if (state == LOW)
                    {
                        return true;
                    }
                }
            }

            lastState = tempState;

            return false;
        }
};

#endif // DEBOUNCER_HPP