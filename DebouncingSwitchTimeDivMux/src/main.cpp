/**
 * @file main.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief A program to count the button presses of SW2 and SW3 on the Vanduino shield and display the count on the 7-segment display.
 * @date 2023-10-22
 * 
 * What is the minimum frequency that the 7-seg display can be muxed and not appear to be flickering?
 *     It seems to be around 1200 uS. At 1100 uS it is noticeable but not too bad.
 */
#include <Arduino.h>

// type aliases
using TickType = unsigned long;

// enum SwitchState
enum SwitchState
{
    NONE_PRESSED,
    INCREMENT_PRESSED,
    DECREMENT_PRESSED,
};

// Define pins and constants
const byte buttonIncrementPin = 3;    // PORTB[3]
const byte buttonDecrementPin = 2;    // PORTB[2]
const TickType debounceDelay = 50000; // the debounce time
const byte displayOnesPin = 0;    // DDRB[0]
const byte displayTensPin = 1;    // DDRB[1]
const TickType TickDelay = 10000; // 1/1000 second

// TODO: refactor to remove global variables
// Global variables
volatile byte LastIncrementButtonState = HIGH; // the previous reading from the input pin
volatile byte LastDecrementButtonState = HIGH; // the previous reading from the input pin
volatile byte IncrementButtonState = HIGH; // the current reading from the input pin
volatile byte DecrementButtonState = HIGH; // the current reading from the input pin
volatile TickType LastIncrementDebounceTime = 0; // the last time the output pin was toggled
volatile TickType LastDecrementDebounceTime = 0; // the last time the output pin was toggled
volatile long count = 0; // count of button presses

// function prototypes
SwitchState debounceButton();
void delayMicros(TickType durationMicros);
void displayDigit(byte digit, byte upPin, byte downPin);

void setup()
{
    // clear PORTB[0:1]
    PORTB &= ~((1 << displayOnesPin) | (1 << displayTensPin));

    // set PORTB[2:3] as input
    DDRB &= ~((1 << buttonIncrementPin) | (1 << buttonDecrementPin));

    // set PORTD as output
    DDRD = 0b11111111;
}

void loop()
{
    // monitor switches for button presses and increment or decrement count
    SwitchState switchChoice = debounceButton();

    // increment or decrement count based on state
    switch (switchChoice)
    {
    case INCREMENT_PRESSED:
        ++count;
        break;

    case DECREMENT_PRESSED:
        --count;
        break;

    case NONE_PRESSED: // intentional fallthrough
    default:
        break;
    }

    // roll over count to 0 if count > 99
    if (count > 99)
    {
        count = 0;
    }
    else if (count < 0)
    {
        count = 99;
    }

    // time division multiplexing
    // display tens digit
    displayDigit(count / 10, displayTensPin, displayOnesPin);
    delayMicros(TickDelay);
    // display ones digit
    displayDigit(count % 10, displayOnesPin, displayTensPin);
    delayMicros(TickDelay);
}

// function to debounce button press and release events for a given pin saving the last state of each button
SwitchState debounceButton()
{
    // read value of PORTB[2:3] into variable
    byte incrementState = PINB & (1 << buttonIncrementPin);
    byte decrementState = PINB & (1 << buttonDecrementPin);

    // check if the state of the button has changed since last read
    if (incrementState != LastIncrementButtonState)
    {
        LastIncrementDebounceTime = micros();
    }

    // check if the state of the button has changed since last read
    if (decrementState != LastDecrementButtonState)
    {
        LastDecrementDebounceTime = micros();
    }

    // if the increment button state has been stable for longer than the debounce delay then return INCREMENT_PRESSED
    if ((micros() - LastIncrementDebounceTime) > debounceDelay)
    {
        if (incrementState != IncrementButtonState)
        {
            IncrementButtonState = incrementState; // update last button state
            if (IncrementButtonState == LOW)
            {
                return INCREMENT_PRESSED;
            }
        }
    }

    // if the decrement button state has been stable for longer than the debounce delay then return DECREMENT_PRESSED
    if ((micros() - LastDecrementDebounceTime) > debounceDelay)
    {
        if (decrementState != DecrementButtonState)
        {
            DecrementButtonState = decrementState; // update last button state
            if (DecrementButtonState == LOW)
            {
                return DECREMENT_PRESSED;
            }
        }
    }

    // update last button states
    LastIncrementButtonState = incrementState;
    LastDecrementButtonState = decrementState;

    return NONE_PRESSED; // no button pressed
}

// function to delay for a specified number of microseconds
void delayMicros(TickType durationMicros)
{
    volatile TickType start = micros();
    while (micros() - start < durationMicros)
    {
        // do nothing
    }
}

// function to display a digit on the 7-segment display
void displayDigit(byte digit, byte upPin, byte downPin)
{
    // decimal font table
    static uint8_t fontTable[] = {
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
    };

    // turn off downPin
    DDRB &= ~(1 << downPin);

    // turn on upPin
    DDRB |= (1 << upPin);

    // set upPin digit to digit for display
    PORTD = fontTable[digit];
}