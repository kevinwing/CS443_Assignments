/**
 * @file safe_control.hpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Class definition for interfacing with a safe
 */
#ifndef SAFE_CONTROL_HPP
#define SAFE_CONTROL_HPP

#include <Arduino.h>
#include "key_matrix.hpp"

using namespace std;

const String Code = "1234";

class SafeControl
{
private:
    enum State
    {
        OPEN,
        CLOSED,
        INVALID_CODE,
        SET_CODE,
    };
    bool checkCombination();
    void servoOpen(bool open);
    KeyMatrix keypad;

    uint8_t codeLength = 4;

    byte ledPin = PB5;
    byte servoPin = PC5;
    // pins PD0-PD7 map to Keypad pins 7-0 (respectively)
    int pulseMin = 1000;   // -60 degrees
                           //  int pulseMid = 1500;   // 0 degrees
    int pulseMax = 2000;   // +60 degrees
    int freq = 100;        // 60 Hz
    int pulseWidth = 1;    // 1 ms pulse width
    int timeFrame = 20000; // 20 ms time frame

    int closedPos = -60;
    int openPos = 60;

    String enteredCode = "";

    State state = OPEN;
    State lastState = OPEN;

public:
    SafeControl();
    void update();
    void init();
};

#endif // SAFE_CONTROL_HPP
