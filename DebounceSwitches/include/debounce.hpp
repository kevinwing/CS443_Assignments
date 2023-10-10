/**
 * @file debounce.hpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Debounce switch class declaration file
 * @date 2023-10-07
 */
#ifndef DEBOUNCE_HPP
#define DEBOUNCE_HPP

#include <Arduino.h>

#define LED_PIN 5 // PORTB pin 5
#define SWITCH_PIN 3 // PORTB pin 3
#define SERIAL_BAUD 9600 // serial baud rate in bits per second
#define SERIAL_OUT_PIN 1 // PORTD pin 1
#define SERIAL_IN_PIN 0 // PORTD pin 0

typedef unsigned long TimeType;

/**
 * @brief Debounce switch class. This class counts the number of times a switch bounces and measures the
 * time until the switch stops bouncing. The switch is considered debounced when the number of bounces is
 * greater than or equal to the number of bounces specified in the constructor and the time since the last
 * 
 */
class Debounce {
private:
    using TimeType = unsigned long; // type alias for the time type
    using PinType = uint8_t; // type alias for the pin type

    PinType ledPin; // pin for the LED
    PinType switchPin; // pin for the switch
    TimeType lastBounceTime; // time of the last bounce
    TimeType bounceTime; // time between bounces
    TimeType bounceLimit; // number of bounces before the switch is considered debounced
    TimeType bounceCount; // number of bounces
    PinType switchPinState; // state of the switch pin
    
    
public:
    Debounce(uint8_t ledPin, uint8_t switchPin);
    void time(); // method to time the switch bounce and count the number of bounces using micros() 
};

#endif // DEBOUNCE_HPP