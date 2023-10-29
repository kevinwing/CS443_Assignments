/**
 * @file main.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief 
 * @date 2023-10-29
 * 
 * Assignment Info:
 * Design and construct a circuit on the Arduino Protoboard that does the following:
 * 
 *     Switch 1 should act as a motor-direction switch in the sequence Fwd-Off-Rev-Off-Fwd-Off...
 *     The RGB LED should light up green when forward direction is enabled, red when reverse direction is enabled, and be off
 *     when the motor is off.
 *     Switch 2 should control the motor by varying the duty cycle of a PWM signal sent to the motor through the L293D motor
 *     controller. Each press should cycle through 25%-50%-75%-100% duty cycles then repeat.
 *     Construct a "bar graph" using a row of four yellow LEDs and appropriate current-limiting resistors that light up a line of
 *     LED's corresponding to the duty cycle selected.
 *     Submit your code file with appropriate comments and a short video showing the program features.
 * 
 * Rules, hints, comments
 * 
 *     You may use the Arduino functions millis() and/or micros(), but you may not use any other functions including delay(),
 *     digitalRead(), digitalWrite(), and analogWrite().
 *     Use appropriate values for current-limiting resistors for the LED's (see prior homework assignments) and pull-up (not pull-
 *     down) resistors for the switches (see Arduino and Vanduino schematics for reference).
 *     Use whatever I/O registers and bits that you like for this project, but access the bits directly via PORTx, DDRx, PINx, etc.
 *     Use a good switch debouncing algorithm when reading the switches.
 *     Your program will need to implement a PWM algorithm in software. You are not allowed to use the AVR on-chip timers,
 *     or the Arduino function analogWrite() that makes use of them.
 *     It is simplest to use the +5V signal from the Arduino card to provide power to the motor and L293D controller. However,
 *     some of the motors in some of the Arduino Starter Kits stall when driven at 5V. If this is the case, you may have to use a
 *     9V battery to drive the motor through the L293D as shown in APB project #10.
 *     Refer to Project #10 in APB for an example PWM motor control project. However, note that project #10 uses very
 *     primitive switch debouncing, makes uses of digitalRead(), digitalWrite(), and especially analogWrite(), which you are not
 *     allowed to use, and requires an external 9V battery (which you may or may not need).
 *     It will be easier if you remove the Vanduino shield from the Arduino board for this project.
 *     I recommend a 1mS sample time for switch debouncing, and a 4mS frame time for the PWM.
 *     Use “good” programming style with descriptive headers and useful comments.
 *     Note that your algorithm used in prior homework to multiplex a 7-segment LED is a good starting point to develop your
 *     motor PWM algorithm.
 * 
 * Materials (from the Arduino Starter Kit)
 * 
 *     1 L293D motor controller
 *     1 6-9V DC motor
 *     2 pushbutton switches,
 *     4 yellow LED's
 *     1 RGB tri-color LED
 *     Assorted current-limiting and pullup resistors as needed
 *
 * Pin Assignments:
 */
#include <Arduino.h>

void setup()
{
  
}

void loop()
{
}
