/**
 * @file main.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Combination lock program for Arduino Uno R3.
 * @date 2023-11-10
 *
 * Requirements
 *
 *     Design and construct a circuit on the Arduino Protoboard that  implements a combination safe lock like that found in hotel rooms.
 *     Submit your code with appropriate comments and a short video showing the program features.
 *     This project should use a 4x4 push button switch array and a micro servo motor.
 *     You will need to provide switch debouncing for the switch array.
 *     Generate appropriate pulses to control the servo.  The use of state machine(s) is encouraged in your architecture.
 *     The safe door is defined as "open" when the servo motor is in its maximum clockwise position, and "closed" when the servo motor is in its maximum counter-clockwise position.
 *     When the door is open, the control program should read a 4-digit combination, followed by the # key.  Allowed digits are 0-9, and A-D.  After a valid combination is entered, the door should close upon pressing the # key
 *     When the door is closed, the 4-digit combination stored above must be entered, followed by the * key to open the door.
 *     The LED on the Uno or Vanduino should light up when the door is closed to indicate its state.
 *     It is up to you to decide what actions to take when an incorrect combination is entered – e.g., wrong combination, too short, too long, etc.
 *     Document any requirements as comments in your code.  E.g., if an incorrect combination is entered, the * key must be pressed to reset the sequence.  Or, the system only looks at the last 4 digits entered to determine a valid combination.  Or, whatever operation you design.
 *
 * Rules, hints, notes
 *
 * RC servos use a control signal that is digital PWM signal with a 50 Hz frame rate. Within each 20 ms time frame, an active-high digital pulse controls the position. The pulse nominally ranges from 1.0 ms to 2.0 ms with 1.5 ms always being center of range.  You can send pulses continually even when no movement is desired, but the servo will consume power and "twitch" occasionally as it tries to maintain its position.  An option is to only send pulses for a second or so when you determine that the position needs to change.  This will reduce steady-state power and eliminate twitching.
 *
 *     Your program will need to implement a servo-control algorithm in software to generate the required pulses. You are not allowed to use the Servo library shown in the APB project #5 example.
 *     The datasheet for the switch matrix can be found here
 *
 *     Links to an external site..
 *     Use the 10 pin header above the seven segment display and ribbon cable from your Vanduino shield to make connecting the keypad easier. Note that this connector hooks into PORTD directly (the resistors for the seven segment display do not function as pullup resistors).
 *     Beware of a pin conflict between the serial port and PORTD[0:1].  Attempting to use the Serial port to display debugging messages will cause great unhappiness.  You can always turn on the Uno/Vanduino LED at places in your code as a debugging aid.  E.g.:
 *     if (x == expectedValue)   // for debugging
 *        turnLEDon();
 *     These switches are bouncy.  Use a good debouncing algorithm.  (Hint: you can debounce individual rows or columns of the switch matrix, or with a proper debounce algorithm you can debounce the decoded value returned by the switch (e.g. 0-9, A-D, *, #)).
 *     Due to a delay through the input synchronizers, your compiled code must not read from PINx immediately after writing to PORTx.  At least one machine instruction (and apparently sometimes at least two) must occur between the write and read.
 *     One way to ensure this without creating unnecessary delay is to use an inline assembly language nop instruction.  This can be achieved as follows:
 *        #define nop()   asm("nop \n") or inline void nop() {asm("nop\n");}
 *     Then you can do something like this:
 *        PORTx = y;
 *        nop();
 *        nop();
 *        z = PINx;
 *     Alternatively, you could use delayMicroseconds(1) instead of the two nop() instructions, but this slows down your code and requires use of a CPU timer.
 *     *** A hazard exists if two switch buttons are pressed simultaneously that may produce a short circuit that could
 *     damage the processor.  Take appropriate steps to prevent the hazard in your software design.  One way to eliminate the hazard is to use the following sequence of operations:
 *
 *     all ROWs should be configured as inputs to start (assuming rows are strobed outputs and columns are inputs), then:
 *       set ROWn to 1 (for safety, but possibly redundant)
 *       make ROWn an output
 *       clear ROWn to 0
 *       delay for ~1uS with nop()s or delayMicroseconds(1)
 *       read column inputs (columns should be configured as INPUT_PULLUP)
 *       set ROWn to 1 (forces high state w/o waiting for slow rise time from pullups)
 *       make all ROWs inputs again (previously setting ROWn to 1 will configure the pin as INPUT_PULLUP)
 *     ***
 *     I recommend configuring the COLs as INPUT_PULLUP to provide a pullup resistor for the switch columns.  pinMode(nn, INPUT_PULLUP) achieves this, but I want you to learn how to do it at the register level.  One way to accomplish this in
 *     setup() is:
 *     DDRx[n] = 0;      // Clear the bit you want as an input in its DDR register to make it an input
 *     PORTx[n] = 1;     // Even though you will be using PINx[n] as an input, writing a 1 to PORTx[n] will enable the pullup resistor on that input pin.
 *     Upon power on or reset, the system should open the safe to allow a combination to be entered.  You may assume that the power to the Arduino is reliable, and the state of the system (e.g. combination, open/closed state, etc.) may be stored in RAM.  (In a more realistic system, the combination would likely be stored in Flash or EEPROM non-volatile storage on the Arduino, so that power failure would not reset the combination or open the safe.)
 *     +5 points extra credit to anybody that designs their system to be secure during reset/power cycling operations.  This will involve changing reset/power-on operation and saving some information in non-volatile memory in the processor.  You may use the Arduino functions/libraries (E.g., PROGMEM, EEPROM) that access the non-volatile memory.  Note that both the Flash and EEPROM memories have a limited number of write cycles before they wear out.  This number is typically greater than 10,000 write cycles which is fine for occasional writes to save information.  But, if you include writes to non-volatile memory in a software loop, you can damage the memory quickly.  Reads from non-volatile memory are essentially unlimited and may be done repeatedly in a tight loop if desired.
 *     You may use the Arduino functions millis(), micros(), and PROGMEM or EEPROM (if you're going for the extra credit).
 *     You may use delayMicroseconds(1) for the back-to-back write/read problem described above if you really don't want to use nop(), but you may not use any other Arduino functions.
 *     The Servo motor does not need an external motor controller like we used in prior homework.  Use the +5V signal from the Arduino card to provide power to the motor.  However, it does require a 100uF decoupling capacitor.  This capacitor is polarized.  That is, it has a negative and a positive side and must be wired in the correct orientation.  If you wire it backwards, it may self-destruct.    See APB project #5 for an example.
 *     Though most features of the Vanduino shield are not used in this project, it should be attached to the Arduino Uno to provide a ribbon-cable connection to the switch array.  Disconnect the Vanduino Ribbon Cable and reconnect it to the switch array.  Note that the red stripe on the cable connects to pin 1 on the Vanduino header.  Also, look closely at the J9 and J10 jumpers as you will need to configure those appropriately. Alternatively, if your switch module does not have the 2-row connector attached, you can solder a 1-row connector to the switch module (soldering the short pins on the top side, with the plastic piece on the bottom side against the PC board, with the long pins on the connector sticking out below).  Then you can attach the switch module directly to the Uno (or Vanduino) without using a ribbon cable.
 *
 * Materials:
 *
 *     1 micro servo motor (from Arduino Starter Kit)
 *     1 100uF capacitor (from Arduino Starter Kit)
 *     1 4x4 switch matrix module (from UI)
 *     1 10-wire ribbon cable (from Vanduino)
 *     Assorted wires (from Arduino Starter Kit)
 *
 */
#include <Arduino.h>
#include <EEPROM.h>

#define nop() asm("nop \n")

// type definitions
using TickType = unsigned long;

// global constants
const int ROWS = 4;
const int COLS = 4;
/*
Row 1: 0
Row 2: 7
Row 3: 6
Row 4: 4
*/
const int ROW_PINS[ROWS] = {0, 7, 6, 4}; // map to rows 0-3 (respectively)
const int COL_PINS[COLS] = {5, 3, 2, 1}; // map to cols 0-3 (respectively)
// keypad map
const char KEYS[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

const int CodeLength = 4;

// const byte SERVO_PIN = PB1;
// const byte LED_PIN = PB5;
// pins PD0-PD7 map to Keypad pins 7-0 (respectively)
const int pulseMin = 1000; // -60 degrees
const int pulseMid = 1500; // 0 degrees
const int pulseMax = 2000; // +60 degrees
const int freq = 50;       // 50 Hz
// const int pulseWidth = 1;  // 1 ms pulse width
const int timeFrame = 20000; // 20 ms time frame

const int closedPos = -60;
const int openPos = 60;

const TickType debounceDelay = 1000; // 50 ms debounce delay

// global variables
volatile char keyState = '\0';
volatile char lastState = '\0';
volatile TickType lastDebounceTime = 0;
volatile char CodeArray[CodeLength] = {'\0'};
volatile char StoredCode[CodeLength] = {'\0'};

enum State
{
    OPEN,
    CLOSED,
    // ENTER_CODE, 
    ENTER_CODE_OPEN,
    ENTER_CODE_CLOSED,
    // INVALID_CODE,
    INVALID_CODE_OPEN,
    INVALID_CODE_CLOSED,
    RESET_CODE,
};

State state = CLOSED;

// function prototypes
void controlServo(bool open);
void delayMicros(TickType us);
char getKeyPress();
char getRawKeyPress();

void setup()
{
    // read first byte of EEPROM to determine if code has been set
    byte count = EEPROM.read(0);
    if (count == 0xFF)
    {
        // code has not been set
        // set code to default: 0000
        for (int i = 0; i < CodeLength; i++)
        {
            StoredCode[i] = '0';
        }
        // write count to EEPROM
        EEPROM.write(0, CodeLength);
        // write code to EEPROM
        for (int i = 1; i < CodeLength; i++)
        {
            EEPROM.write(i, StoredCode[i - 1]);
        }
    }
    else
    {
        // read stored code from EEPROM
        for (int i = 0; i < CodeLength; i++)
        {
            StoredCode[i] = EEPROM.read(i);
        }
    }

    // configure rows as outputs to start
    for (int i = 0; i < ROWS; i++)
    {
        DDRD |= (1 << ROW_PINS[i]);  // set to output
        PORTD |= (1 << ROW_PINS[i]); // set to high
    }

    // set columns as input_pullup
    for (int i = 0; i < COLS; i++)
    {
        DDRD &= ~(1 << COL_PINS[i]); // set as input
        PORTD |= (1 << COL_PINS[i]); // enable pullup
    }

    DDRB |= (1 << PB5); // set builtin LED to output

    // set servo pin to output
    DDRB |= (1 << PB1);
    // move servo to closed position
    controlServo(false);
}

void loop()
{
    static int codeIndex = 0;
    // get key press
    char key = getKeyPress();
    
    switch (state)
    {
    case OPEN:
        // set LED to HIGH
        PORTB |= (1 << PB5);
        // check for key press
        if (key != '\0')
        {
            // check for valid key press
            if (key == '#')
            {
                // close safe
                controlServo(false);
                // set state to CLOSED
                state = CLOSED;
            }
            else
            {
                // set state to ENTER_CODE_OPEN
                state = ENTER_CODE_OPEN;
            }
        }
        break;
    case CLOSED:
        // set LED to LOW
        PORTB &= ~(1 << PB5);
        // check for key press
        if (key != '\0')
        {
            // check for valid key press
            if (key == '*')
            {
                // open safe
                controlServo(true);
                // set state to OPEN
                state = OPEN;
            }
            else
            {
                // set state to ENTER_CODE_CLOSED
                state = ENTER_CODE_CLOSED;
            }
        }
        break;
    case ENTER_CODE_OPEN:
        // check for key press
        if (key != '\0')
        {
            // check for valid key press
            if (key == '#')
            {
                // close safe
                controlServo(false);
                // set state to CLOSED
                state = CLOSED;
            }
            else if (key == '*')
            {
                // reset code
                codeIndex = 0;
                // set state to RESET_CODE
                state = RESET_CODE;
            }
            else
            {
                // check if key is valid
                if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'D'))
                {
                    // add key to code array
                    CodeArray[codeIndex] = key;
                    // increment code index
                    codeIndex++;
                    // check if code is complete
                    if (codeIndex == CodeLength)
                    {
                        // check if code is valid
                        bool valid = true;
                        for (int i = 0; i < CodeLength; i++)
                        {
                            if (CodeArray[i] != StoredCode[i])
                            {
                                valid = false;
                            }
                        }
                        // check if code is valid
                        if (valid)
                        {
                            // open safe
                            controlServo(false);
                            // set state to OPEN
                            state = CLOSED;
                        }
                        else
                        {
                            // set state to INVALID_CODE_OPEN
                            state = INVALID_CODE_OPEN;
                        }
                    }
                }
            }
        }
        break;
    case ENTER_CODE_CLOSED:
        // check for key press
        if (key != '\0')
        {
            // check for valid key press
            if (key == '*')
            {
                // reset code
                codeIndex = 0;
                // set state to RESET_CODE
                state = RESET_CODE;
            }
            else
            {
                // check if key is valid
                if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'D'))
                {
                    // add key to code array
                    CodeArray[codeIndex] = key;
                    // increment code index
                    codeIndex++;
                    // check if code is complete
                    if (codeIndex == CodeLength)
                    {
                        // check if code is valid
                        bool valid = true;
                        for (int i = 0; i < CodeLength; i++)
                        {
                            if (CodeArray[i] != StoredCode[i])
                            {
                                valid = false;
                            }
                        }
                        // check if code is valid
                        if (valid)
                        {
                            // open safe
                            controlServo(true);
                            // set state to OPEN
                            state = OPEN;
                        }
                        else
                        {
                            // set state to INVALID_CODE_CLOSED
                            state = INVALID_CODE_CLOSED;
                        }
                    }
                }
            }
        }
        break;
    case INVALID_CODE_OPEN:
        // flash LED 5 times
        for (int i = 0; i < 5; i++)
        {
            PORTB |= (1 << PB5); // set LED to HIGH
            delayMicros(500000); // 500 ms
            PORTB &= ~(1 << PB5); // set LED to LOW
            delayMicros(500000); // 500 ms
        }
        // clear code array
        for (int i = 0; i < CodeLength; i++)
        {
            CodeArray[i] = '\0';
        }
        // reset code index
        codeIndex = 0;
        // set state to ENTER_CODE_OPEN
        state = ENTER_CODE_OPEN;
        break;
    case INVALID_CODE_CLOSED:
        // flash LED 5 times
        for (int i = 0; i < 5; i++)
        {
            PORTB |= (1 << PB5); // set LED to HIGH
            delayMicros(500000); // 500 ms
            PORTB &= ~(1 << PB5); // set LED to LOW
            delayMicros(500000); // 500 ms
        }
        // clear code array
        for (int i = 0; i < CodeLength; i++)
        {
            CodeArray[i] = '\0';
        }
        // reset code index
        codeIndex = 0;
        // set state to ENTER_CODE_CLOSED
        state = ENTER_CODE_CLOSED;
        break;
    case RESET_CODE:
        break;
    
    default:
        break;
    }
}

// servo control function
void controlServo(bool open)
{
    int pulseWidth = open ? pulseMax : pulseMin;

    for (int i = 0; i < freq; ++i)
    {
        // set PB1 high
        PORTB |= (1 << PB1);
        // delay for pulseWidth
        delayMicroseconds(pulseWidth);
        // set PB1 low
        PORTB &= ~(1 << PB1);
        // delay for 20 ms - pulseWidth
        delayMicroseconds(timeFrame - pulseWidth);
    }
}

// delay microseconds function
void delayMicros(TickType us)
{
    volatile TickType i;
    for (i = 0; i < us; ++i)
    {
        // do nothing
    }
}

// get key press function
//   set ROWn to 1 (for safety, but possibly redundant)
//   make ROWn an output
//   clear ROWn to 0
//   delay for ~1uS with nop()s or delayMicroseconds(1)
//   read column inputs (columns should be configured as INPUT_PULLUP)
//   set ROWn to 1 (forces high state w/o waiting for slow rise time from pullups)
//   make all ROWs inputs again (previously setting ROWn to 1 will configure the pin as INPUT_PULLUP)

char getKeyPress()
{
    // get key press
    char RawKeyPress = getRawKeyPress();
    // debounce key press
    if (RawKeyPress != lastState)
    {
        lastDebounceTime = micros();
    }

    if ((micros() - lastDebounceTime) > debounceDelay)
    {
        if (RawKeyPress != keyState)
        {
            keyState = RawKeyPress;
            if (keyState != '\0')
            {
                return keyState;
            }
        }
    }

    lastState = RawKeyPress;
    return '\0';
}

// get key press helper
char getRawKeyPress()
{
    char key = '\0'; // default key value

    // scan rows for key press
    for (int i = 0; i < ROWS; i++)
    {
        // set ROWn LOW
        PORTD &= ~(1 << ROW_PINS[i]);
        nop();
        nop();
        // read column inputs (columns should be configured as INPUT_PULLUP)
        for (int j = 0; j < COLS; j++)
        {
            byte colState = (PIND & (1 << COL_PINS[j])) >> COL_PINS[j]; // get column state
            if (colState == 0)
            {
                key = KEYS[i][j];
            }
        }
        // set ROWn to 1 (forces high state w/o waiting for slow rise time from pullups)
        PORTD |= (1 << ROW_PINS[i]);
    }
    return key;
}