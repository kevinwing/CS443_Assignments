/**
 * @file main.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Write an Arduino program that characterizes the switch bouncing behavior. When the switch is pressed, the program should count the
 *        number of bounces and record the time spent bouncing. Switch is pressed when the switch pin is low. Switch is not pressed when the switch
 *        pin is high. The program should print the number of bounces and the time spent bouncing when the switch is debounced.
 *
 * Questions:
 * 
 * What is the longest time that the switch bounces when pressed? 0 uS (no bounces detected)
 * What is the highest number of bounces that occur when pressed? 0
 * What is the longest time that the switch bounces when released? 5444 uS
 * What is the highest number of bounces that occur when released? 9
 * What type of variables (e.g. global, static, local, class) should be used to hold state variables like timestamps? Why? 
 *    Global variables should be used to hold state variables like timestamps because they are accessible from anywhere in the program.

 * @date 2023-10-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>

const byte buttonPin = 3;                   // PORTB pin 3
volatile byte lastButtonState = HIGH;       // Last state of the button
volatile unsigned long bounceStartTime = 0; // Timestamp when bouncing starts
volatile unsigned int bounceCount = 0;      // Number of bounces
volatile bool bouncing = false;             // Flag to indicate if the button is bouncing

void setup()
{
    DDRB &= ~(1 << buttonPin); // Set buttonPin as input
    PORTB |= (1 << buttonPin); // Enable pull-up resistor
    Serial.begin(9600);        // Initialize serial communication
}

void loop()
{
    // Read the current state of the button
    byte currentButtonState = PINB & (1 << buttonPin);

    // If the button is pressed and not bouncing and the button state has changed
    if (!bouncing && currentButtonState != lastButtonState)
    {
        bouncing = true;
        bounceStartTime = micros();
        bounceCount = 0;
    }

    // If the button is bouncing and the button state has changed
    if (bouncing && currentButtonState != lastButtonState)
    {
        bounceCount++;
        lastButtonState = currentButtonState;
    }

    // If the button is bouncing and the button state has not changed for 5ms
    if (bouncing && (micros() - bounceStartTime) > 5000)
    { // Assuming 5ms as a max bounce time
        bouncing = false;
        if (lastButtonState == LOW)
        {
            Serial.print("Switch pressed. ");
            Serial.print(bounceCount / 2); // Only high-to-low transitions
            Serial.print(" high-to-low transitions occurred in ");
            Serial.print(micros() - bounceStartTime);
            Serial.println(" uS.");
        }
        else
        {
            Serial.print("Switch released. ");
            Serial.print(bounceCount / 2); // Only low-to-high transitions
            Serial.print(" low-to-high transitions occurred in ");
            Serial.print(micros() - bounceStartTime);
            Serial.println(" uS.");
        }
    }
}
