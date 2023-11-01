#ifndef DEBOUNCER_HPP
#define DEBOUNCER_HPP

#include <Arduino.h>

/**
 * @brief Debounce a digital input signal
 * 
 */
class Debouncer {
    private:
        uint8_t btnPin;
        uint16_t state;
    
    public:
        void begin(uint8_t button)
        {
            btnPin = button;
            state = 0;
            // set pin to input
            DDRC &= ~(1 << btnPin);
        }

        /**
         * @brief debounce algrorithm modified from Jack Ganssle, http://www.ganssle.com/debouncing-pt2.htm
         * 
         * @return true 
         * @return false 
         */
        bool debounce()
        {
            state = ((state << 1) | ((PINC >> btnPin) & 1) | 0xe000);
            return (state == 0xf000);
        }
};

#endif // DEBOUNCER_HPP