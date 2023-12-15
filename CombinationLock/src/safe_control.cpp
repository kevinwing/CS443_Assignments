/**
 * @file safe_control.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Class definition for interfacing with a safe
 */
#include "safe_control.hpp"

/**
 * @brief Construct a new Safe Control:: Safe Control object
 * 
 */
SafeControl::SafeControl()
{
    // set SERVO_PIN to output
    DDRC |= (1 << servoPin);

    // set LED_PIN to output
    DDRB |= (1 << ledPin);
}

/**
 * @brief Check combination against entered code and return true if they match
 * 
 * @return true 
 * @return false 
 */
bool SafeControl::checkCombination()
{
    // check if code is correct
    if (enteredCode == Code)
    {
        return true;
    }

    return false;
}

/**
 * @brief Move servo to open or closed position
 * 
 * @param open 
 */
void SafeControl::servoOpen(bool open)
{
    int pulseWidth = open ? pulseMax : pulseMin;

    for (int i = 0; i < freq; ++i)
    {
        // set SERVO_PIN high
        PORTC |= (1 << servoPin);
        // delay for pulseWidth
        delayMicroseconds(pulseWidth);
        // set SERVO_PIN low
        PORTC &= ~(1 << servoPin);
        // delay for 20 ms to complete servo cycle
        delayMicroseconds(timeFrame);
        // delayMicroseconds(timeFrame - pulseWidth);
    }
}

/**
 * @brief Main update function for safe. Handles state machine and key presses
 * 
 */
void SafeControl::update()
{
    // get key press
    char key = keypad.getKey();

    // check if key is pressed and append to code if it is not a special key
    if (key != '\0' && key != '*' && key != '#')
    {
        // append key to code
        enteredCode += key;

        if (enteredCode.length() > codeLength)
        {
            lastState = state;    // save last state
            state = INVALID_CODE; // switch state to invalid code
        }
    }

    // state machine for safe
    switch (state)
    {
    case OPEN: // safe is open
        if (key == '#')
        {
            if (checkCombination())
            {
                // switch state to close safe
                state = CLOSED;
                servoOpen(false);

                // set LED to HIGH
                PORTB |= (1 << ledPin);

                // reset code
                enteredCode = "";
            }
            else
            {
                lastState = state;
                state = INVALID_CODE;
            }
        }
        else if (key == '*')
        {
            // set to invalid code state
            lastState = state;
            state = INVALID_CODE;
        }

        break;
    case CLOSED:
        if (key == '*')
        {
            if (checkCombination())
            {
                // switch state to open safe
                state = OPEN;
                servoOpen(true);

                // set LED to LOW
                PORTB &= ~(1 << ledPin);

                // reset code
                enteredCode = "";
            }
            else
            {
                lastState = state;
                state = INVALID_CODE;
            }
        }
        else if (key == '#')
        {
            // set to invalid code state
            lastState = state;
            state = INVALID_CODE;
        }
        break;
    case INVALID_CODE:
        // reset code
        enteredCode = "";

        // switch state to last state
        state = lastState;
        break;
    case SET_CODE:
        // TODO: implement combination update code with EEPROM storage
        break;
    default:
        break;
    }

    // set key to null
    key = '\0';
}

/**
 * @brief Initialize safe
 * 
 */
void SafeControl::init()
{
    servoOpen(true);

    // set LED to LOW
    PORTB &= ~(1 << ledPin);
}