#include "debounce.hpp"

Debounce::Debounce(uint8_t ledPin, uint8_t switchPin) : ledPin(ledPin), switchPin(switchPin)
{
    this->lastBounceTime = 0;
    this->bounceTime = 0;
    this->bounceLimit = 0;
    this->bounceCount = 0;
    this->switchPinState = 0;
}

/*

    Write an Arduino program that characterizes the switch bouncing behavior. When the switch is pressed, the program should count the
    number of bounces and record the time spent bouncing.
    Then it should print an ASCII message to the serial port in the form
    of:
        "Switch pressed. nn high-to-low transitions occurred in mm uS."
        When the switch it released, it should do the same and report:
        "Switch released. nn low-to-high transitions occurred in mm uS."
        Submit your file(s), with appropriate comments, and a screen shot of the serial monitor window showing your
        program output on or before the due date.
        Test your switch and software with repeated button presses. Address the following as comments in your code:
            What is the longest time that the switch bounces when pressed?
            What is the highest number of bounces that occur when pressed?
            What is the longest time that the switch bounces when released?
            What is the highest number of bounces that occur when released?
            What type of variables (e.g. global, static, local, class) should be used to hold state variables like timestamps? Why?

Rules, hints, and comments:


IF USING YOUR VANDUINO SHIELD WITH THE RIBBON CABLE INSTALLED, YOU MUST
PROGRAM BITS 0 THRU 3 OF PORTB AS INPUTS. FAILURE TO DO SO MAY DAMAGE THE ATMEGA328
CPU WHEN A SWITCH IS PRESSED. 

Access AVR I/O registers directly (e.g., PORTx, PINx) instead of using Arduino functions like digitalRead() to minimize
lag and maximize reading speed.
You may find the Arduino Examples>Digital>Debounce, and Examples>Basics>DigitalReadSerial useful guides to get you
started.

Make use of bitmasking operations to streamline register access.

Make use of Arduino timer functions (e.g., delay(), millis(), micros(). etc) to simplify timing of program.

Some sources may suggest using interrupts to count bounces. This isn't easy to get right and even if you do bounces are likely to be missed.

You may also find the functions Serial.begin(), Serial.print(), and Serial.println() to be useful. You do not need to access AVR timer
registers or serial port directly.

Use “good” programming style with descriptive headers and useful comments.

In order to reduce the chance of missing a transition, your software should interrogate the AVR port connected to your
switch as often and quickly as possible. So, avoid use of the delay() function if possible, and keep your loop that reads your
switch efficient and quick.

In order to get fine enough timing resolution, you may want to consider using the Arduino micros() function.
The structure of your code is up to you, but you may want to consider the following:

    Use appropriate variables to record timestamps when button presses and releases begin.
    Use appropriate variables to record timestamps when the button stops bouncing.
    Use appropriate variables to count the number of transitions.
    Use appropriate variables to keep track of the switch state between iterations of loop().

The variables above are essentially keeping track of the state of your program across iterations of loop(). The use of these
state variables naturally leads to a program structure where you enter your loop() function repeatedly, and each time
through, you check the raw switch state, determine what is happening, and record the time that the events above occur.
When the switch has stopped bouncing and stabilized, use the timestamp and count information recorded to calculate and
output the results string to the serial port.


Note that you should report only high-to-low transitions when the switch is pressed, and low-to-high transitions when the
switch is released.

You may want to avoid writing your output string to the serial port until after you determine that the switch state has
stabilized. Serial.print() takes a relatively long time to run and may slow down your loop while looking for switch
transitions.

*/
void Debounce::time()
{
    // if the switch is pressed
    if (PINB & (1 << switchPin))
    {
        // if the switch is not bouncing
        if (micros() - this->lastBounceTime > this->bounceTime)
        {
            // if the switch is not pressed
            if (this->switchPinState == 0)
            {
                // increment the bounce count
                this->bounceCount++;
                // set the switch pin state to 1
                this->switchPinState = 1;
                // set the last bounce time to the current time
                this->lastBounceTime = micros();
            }
        }
        // if the switch is bouncing
        else
        {
            // set the last bounce time to the current time
            this->lastBounceTime = micros();
        }
    }
    // if the switch is not pressed
    else
    {
        // if the switch is not bouncing
        if (micros() - this->lastBounceTime > this->bounceTime)
        {
            // if the switch is pressed
            if (this->switchPinState == 1)
            {
                // increment the bounce count
                this->bounceCount++;
                // set the switch pin state to 0
                this->switchPinState = 0;
                // set the last bounce time to the current time
                this->lastBounceTime = micros();
            }
        }
        // if the switch is bouncing
        else
        {
            // set the last bounce time to the current time
            this->lastBounceTime = micros();
        }
    }
    // if the switch is not bouncing
    if (micros() - this->lastBounceTime > this->bounceTime)
    {
        // if the switch is pressed
        if (this->switchPinState == 1)
        {
            // print the number of bounces and the time since the last bounce
            Serial.print("Switch pressed. ");
            Serial.print(this->bounceCount);
            Serial.print(" high-to-low transitions occurred in ");
            Serial.print(micros() - this->lastBounceTime);
            Serial.println(" uS.");
            // reset the bounce count
            this->bounceCount = 0;
        }
        // if the switch is not pressed
        else
        {
            // print the number of bounces and the time since the last bounce
            Serial.print("Switch released. ");
            Serial.print(this->bounceCount);
            Serial.print(" low-to-high transitions occurred in ");
            Serial.print(micros() - this->lastBounceTime);
            Serial.println(" uS.");
            // reset the bounce count
            this->bounceCount = 0;
        }
    }
}