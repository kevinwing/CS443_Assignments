/**
 * @file utilities.hpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Utility functions
 */
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <Arduino.h>

/**
 * @brief Set the pin mode of a pin
 * 
 */
void setPinMode(uint8_t pin, uint8_t mode);

/**
 * @brief Write to a pin
 * 
 */
void writePin(uint8_t pin, uint8_t value);

/**
 * @brief Read from a pin
 * 
 * @return uint8_t Value read from pin
 */
uint8_t readPin(uint8_t pin);

#endif // UTILITIES_HPP