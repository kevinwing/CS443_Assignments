/**
 * @file keypad.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Class definition for interfacing with a keypad
 */
#include "key_matrix.hpp"

/**
 * @brief Construct a new Key Matrix:: Key Matrix object
 * 
 */
KeyMatrix::KeyMatrix()
{
    this->numRows = ROWS;
    this->numCols = ROWS;
    this->keyState = '\0';
    this->lastKeyState = '\0';
    this->lastDebounceTime = 0;

    // set Row pins to input
    for (int i = 0; i < this->numRows; i++)
    {
        DDRD &= ~(1 << rowPins[i]);
    }

    // set Col pins to input_pullup
    for (uint8_t i = 0; i < this->numCols; i++)
    {
        DDRD &= ~(1 << this->colPins[i]);
        PORTD |= (1 << this->colPins[i]);
    }
}

/**
 * @brief Get the key pressed on the keypad and debounce it
 * 
 * @return char 
 */
char KeyMatrix::getKey()
{
    // get key press
    char rawKeyPress = this->getRawKey();
    // debounce key press
    if (rawKeyPress != this->lastKeyState)
    {
        this->lastDebounceTime = micros();
    }

    if ((micros() - this->lastDebounceTime) > DEBOUNCE_DELAY)
    {
        if (rawKeyPress != this->keyState)
        {
            this->keyState = rawKeyPress;
            if (this->keyState != '\0')
            {
                return this->keyState;
            }
        }
    }

    this->lastKeyState = rawKeyPress;
    return '\0';
}

/**
 * @brief Get the raw key pressed on the keypad
 * 
 * @return char 
 */
char KeyMatrix::getRawKey()
{
    // loop through rows, set ROWn to 1, make ROWn an output, clear ROWn to 0, delay for ~1uS, read column inputs, set ROWn to 1, make all ROWs inputs again
    for (int i = 0; i < this->numRows; i++)
    {
        // set ROWn to 1
        PORTD |= (1 << this->rowPins[i]);

        // make ROWn an output
        DDRD |= (1 << this->rowPins[i]);

        // clear ROWn to 0
        PORTD &= ~(1 << this->rowPins[i]);

        // delay for ~1uS
        nop();
        nop();

        // read column inputs
        for (int j = 0; j < this->numCols; j++)
        {
            // check if column is low
            if (!(PIND & (1 << this->colPins[j])))
            {
                // set ROWn to 1
                PORTD |= (1 << this->rowPins[i]);
                // make all ROWs inputs again
                for (int k = 0; k < this->numRows; k++)
                {
                    // make ROWn an input
                    DDRD &= ~(1 << this->rowPins[k]);
                    // enable pullup
                    // PORTD |= (1 << ROW_PINS[k]);
                }
                // return key
                return this->keys[i][j];
            }
        }

        // set ROWn to 1
        PORTD |= (1 << this->rowPins[i]);
        // make all ROWs inputs again
        for (uint8_t k = 0; k < this->numRows; k++)
        {
            // make ROWn an input
            DDRD &= ~(1 << this->rowPins[k]);
        }
    }

    return '\0';
}