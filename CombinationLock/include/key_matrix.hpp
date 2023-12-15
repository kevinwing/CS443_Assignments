/**
 * @file keypad.hpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Class for interfacing with a keypad
 * @date 2023-11-20
 */
#ifndef KEY_MATRIX_HPP
#define KEY_MATRIX_HPP

#include <Arduino.h>
#include "utils.hpp"

#define nop() asm("nop \n")

using TickType = unsigned long;

const TickType DEBOUNCE_DELAY = 5000;

const uint8_t ROWS = 4;
const uint8_t COLS = 4;

/**
 * @brief Class for interfacing with a keypad
 */
class KeyMatrix
{
public:
    // TODO: fix explicit keys size issue. explicit size is required for now but
    // it should be able to be deduced from the size of the array passed in.
    KeyMatrix();
    
    char getKey();

private: 
    char keys[ROWS][COLS] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };
    uint8_t rowPins[ROWS] = {0, 7, 6, 4};
    uint8_t colPins[COLS] = {5, 3, 2, 1};
    uint8_t numRows;
    uint8_t numCols;
    char keyState;
    char lastKeyState;
    TickType lastDebounceTime;

    char getRawKey();

};

#endif // KEY_MATRIX_HPP