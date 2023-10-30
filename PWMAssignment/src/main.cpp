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
 *     Switch 1: Port C, 4 (PC4)
 *     Switch 2: Port C, 5 (PC5)
 *
 *     LED 1 (Yellow): Port C, 0 (PC0)
 *     LED 2 (Yellow): Port C, 1 (PC1)
 *     LED 3 (Yellow): Port C, 2 (PC2)
 *     LED 4 (Yellow): Port C, 3 (PC3)
 *
 *     RGB LED (Red=Reverse, Green=Forward): PORTD[2, 4, 7]
 *         Red: Port D, 2 (PD2)
 *         Green: Port D, 4 (PD4)
 *         Blue: Port D, 7 (PD7)
 *
 *     H-Bridge (L293D):
 *         Enable 1,2: Port B, 3 (PB3) (PWM)
 *         Input 1: Port B, 0 (PB2)
 *         Input 2: Port B, 1 (PB1)
 */
#include <Arduino.h>

// Types
typedef enum MotorDirection_t
{
    MOTOR_OFF,
    MOTOR_FORWARD,
    MOTOR_REVERSE,
} MotorDirection_t;

typedef enum MotorSpeed_t
{
    MOTOR_SPEED_OFF,
    MOTOR_SPEED_25,
    MOTOR_SPEED_50,
    MOTOR_SPEED_75,
    MOTOR_SPEED_100,
} MotorSpeed_t;

typedef enum MotorEnabled_t
{
    MOTOR_DISABLED,
    MOTOR_ENABLED,
} MotorEnabled_t;

// Constants

// Variables
MotorDirection_t motorDirection = MOTOR_OFF;
MotorSpeed_t motorSpeed = MOTOR_SPEED_OFF;
MotorEnabled_t motorEnabled = MOTOR_DISABLED;

// Function Prototypes
bool isSwitch1Pressed();
bool isSwitch2Pressed();
MotorEnabled_t getMotorState();
MotorDirection_t getMotorDirection();
MotorSpeed_t getMotorSpeed();

void setup()
{
    // Set Switch 1, 2 as inputs
    DDRC &= ~((1 << DDC4) | (1 << DDC5)); // 0b00010000 | 0b00001000 = 0b00011000

    // Set LED 1, 2, 3, 4 as outputs
    DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3); // 0b00000001 | 0b00000010 | 0b00000100 | 0b00001000 = 0b00001111

    // Set RGB LED as outputs
    DDRD |= (1 << DDD2) | (1 << DDD4) | (1 << DDD7); // 0b00000100 | 0b00010000 | 0b10000000 = 0b10010100

    // Set H-Bridge as outputs
    DDRB |= (1 << DDB3) | (1 << DDB2) | (1 << DDB1); // 0b00001000 | 0b00000100 | 0b00000010 = 0b00001110

    // Set initial state of LED 1, 2, 3, 4 to off
    PORTC &= ~((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3)); // 0b00000001 | 0b00000010 | 0b00000100 | 0b00001000 = 0b00001111
}

void loop()
{
}
