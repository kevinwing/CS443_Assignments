/**
 * @file keypad.hpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief 
 */
#ifndef KEYPAD_HPP
#define KEYPAD_HPP

#include <Arduino.h>

class Keypad {
public:
    Keypad(uint8_t rows, uint8_t cols, uint8_t* rowPins, uint8_t* colPins);
    ~Keypad();


    void setDebounceTime(uint16_t debounceTime);
    void setHoldTime(uint16_t holdTime);

    void scan();
    bool isPressed(uint8_t key);
    bool isHeld(uint8_t key);

    uint8_t getKey();
    uint8_t getKeys();

private:
    uint8_t rows;
    uint8_t cols;
    uint8_t* rowPins;
    uint8_t* colPins;

    uint16_t debounceTime;
    uint16_t holdTime;

    uint8_t* keys;
    uint8_t* lastKeys;
    uint32_t* lastTimes;
};

#endif // KEYPAD_HPP