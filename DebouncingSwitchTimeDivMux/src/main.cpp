/**
 * @file main.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Switch Debouncing and Time Division Multiplexing (TDM) on a Vanduino Shield and Arduino Uno board.
 *
 * Requirements:
 *
 * This assignment is designed to demonstrate the concepts of switch debouncing and time division multiplexing (TDM) on a
 * Vanduino Shield and Arduino Uno board.
 *
 *     Write an Arduino sketch that counts the button presses of SW2 and SW3 on the Vanduino shield.
 *         The 7-segment display should initially display the count "00".
 *         Each time SW3 is pressed, the 7-seg display should increment the count shown.
 *         Each time SW2 is pressed, the 7-seg display should decrement the count shown.
 *         The displayed count should “roll over” to 00 after it reaches a maximum count of 99.
 *         Submit your .cpp file, with appropriate comments, and a short video
 *         showing the program features. You may alternatively demonstrate your program to me in
 *         person (or over video link from Moscow) on or before the due date.
 *
 * Rules, hints, and comments
 *
 * You may not use any of the built-in Arduino functions, other than micros(). Access all registers directly (e.g. via PORTx,
 * PINx, DDRx, etc.) and use appropriate bitmasking operations to control individual bits.
 *
 * Each switch press should change the count displayed by exactly +/- 1. I.e., the switch should not bounce. Also, the
 * response to a switch press should be "snappy" -- there should be no noticeable lag between the time a switch is pressed and
 * the display updates.
 * Your program will need to implement a switch debouncing algorithm, which must be tuned to provide the reliable, but
 * "snappy" response required. You may want to refer to the “Debouncing Guide” link on Canvas for example
 * code and algorithms.
 * Your program will need to implement Time Division Multiplexing (TDM) in order to display 2-digit numbers on the 7-
 * Segment Display. The numbers displayed should be decimal (not hex).
 * Your program should have a constant defined which controls the TDM rate. Experiment with different values for this
 * constant and answer the following question as a comment in your code:
 * What is the minimum frequency that the 7-seg display can be muxed and not appear to be flickering?
 *
 * As in prior homework projects, PORTB[2:3] must be programmed as inputs to protect the processor from
 * short-circuits caused by switch presses. However, to control the 7-Segment Cathodes from Port B outputs via TDM
 * safely, you must do the following:
 * In setup(), clear PORTB[0:1] and never set these bits afterwards.
 * In loop(), set DDRB[0] or DDRB[1] to turn on a given digit; clear DDRB[0] or DDRB[1] to turn off a given digit.
 *
 *
 * To prevent "ghosting", only light and unlight segments at times when both cathodes are off. And, be sure that there is never
 * any overlap time when both cathodes are on.
 *
 * You may find the Arduino Examples>Digital>Debounce sketch
 * Links to an external site. a useful starting point for switch debouncing. But, other
 * algorithms are available on the Resources link mentioned above.
 * Use “good” programming style with descriptive headers and useful comments.
 * I recommend calling functions that multiplex the display and debounce the switches periodically inside loop() at appropriate
 * intervals determined by global constants and the micros() function.
 * @date 2023-10-09
 */
#include <Arduino.h>

// function prototypes

void setup()
{
  // clear PORTB[0:1] and never set these bits afterwards.
  DDRB &= ~(1 << DDB0);
}

void loop()
{
  // set DDRB[0] or DDRB[1] to turn on a given digit; clear DDRB[0] or DDRB[1] to turn off a given digit.
  DDRB |= (1 << DDB0);
  DDRB &= ~(1 << DDB0);
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}