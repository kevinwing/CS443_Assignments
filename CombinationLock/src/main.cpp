/**
 * @file main.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Combination lock program for Arduino Uno R3.
 * @date 2023-11-10
 *
 * If the wrong combination is entered, pressing the '#' or '*' key will reset
 */
#include "safe_control.hpp"

SafeControl safe;

void setup()
{
    // initialize safe
    safe.init();
}

void loop()
{
    // update safe
    safe.update();
}