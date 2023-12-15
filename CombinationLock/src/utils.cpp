/**
 * @file utils.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Definition of utility functions
 */
#include "utils.hpp"

/**
 * @brief Delay for a given number of microseconds
 * 
 * @param us 
 */
void delayMicros(TickType us)
{
    volatile TickType i;
    for (i = 0; i < us; ++i)
    {
        // do nothing
    }
}
