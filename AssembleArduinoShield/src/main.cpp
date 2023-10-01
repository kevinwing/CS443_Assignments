/**
 * @file main.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Assemble Arduino Shield (Vanduino)
 * @date 2023-10-01
 *
 * Homework questions:
 *
 * Explain why PORTB, bits 0-3 must be configured as inputs. What might happen if this is ignored? Why?
 *     The pin if set as an output will could be forced to the opposite voltage when the switch is pressed/released.
 *     This could damage the pin by creating a short circuit and potentially damage the MCU.
 *
 * Explain the operation of J9 and J10 on the Vanduino board.
 *      J9 and J10 are jumper header pins that allow for configuration of the 7-segment display. When jumpered accross
 *      pins 1-2 the power to the display is routed through the switch for manual operation for left(J9) and right(J10) display.
 *      When pins 2-3 are jumpered the display will always be on for each side jumpered in this fashion.
 * Specify the jumper position needed for your program.
 *      The jumper position needed is 1-2 with pin 3 empty. This allows for the switch, when pressed, to supply power to the display.
 */
#include <Arduino.h>

// function prototypes
void countdown();
void countup();

// hex font array
const byte hexFont[16] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001  // F
};

void setup()
{
    // initialize serial monitor
    // Serial.begin(9600);

    // set PORTB pin 5 to output for LED
    DDRB |= 0b00100000;

    // set D0-D7 as output for 7-segment display
    DDRD = 0b11111111;

    /*
    Explain why PORTB, bits 0-3 must be configured as inputs. What might happen if this is ignored? Why?
    The pin if set as an output will could be forced to the opposite voltage when the switch is pressed/released.
    This could damage the pin by creating a short circuit and potentially damage the MCU.
    */
    // set PORTB pins 0-3 as input for buttons
    DDRB &= 0b11110000;

    // set A0 as input
    DDRC &= 0b11111110;
}

void loop()
{
    int analogVal = analogRead(A0);
    // check if PORTB pin 3 is LOW
    if (!(PINB & 0b00001000))
    {
        // turn on LED
        PORTB |= 0b00100000;
    }
    else if (!(PINB & 0b00000100))
    {
        // toggle LED
        PORTB ^= 0b00100000;
        delay(500);
    }
    else if (!(PINB & 0b00000010))
    {
        // set PORTD pin 7 to HIGH
        PORTD = 0b10000000;
        delay(analogVal);
        PORTD = 0b00000000;

        for (int i = 15; i >= 0; --i)
        {
            PORTD = hexFont[i];
            delay(analogVal);

            if (PINB & 0b00000010)
            {
                break;
            }

        }

    }
    else if (!(PINB & 0b00000001))
    {
        PORTD = 0b00000000; // ensure 7-segment is blank

        for (int i = 0; i < 16; ++i)
        {
            PORTD = hexFont[i];
            delay(analogVal);

            if (PINB & 0b00000001)
            {
                break;
            }
        }

        PORTD = 0b10000000;
        delay(analogVal);
    }
    else
    {
        // turn off LED
        PORTB &= 0b11011111;
    }
}
