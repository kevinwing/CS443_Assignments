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
 *     Submit your code file wit                    

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
 *         Red: Port D, 7 (PD7)
 *         Blue: Port D, 4 (PD4)
 *         Green: Port D, 2 (PD2)
 *
 *     H-Bridge (L293D):
 *         Enable 1,2: Port B, 3 (PB3) (PWM)
 *         Input 1: Port B, 0 (PB2)
 *         Input 2: Port B, 1 (PB1)
 *         Output 1: Port D, 5 (PD
 */
// #include <Arduino.h>
#include "debouncer.hpp"

// Types
typedef enum MotorDirection_t
{
    MOTOR_OFF,
    MOTOR_FORWARD,
    MOTOR_REVERSE,
} MotorDirection_t;

/**
 * @brief Motor speed settings
 * 
 */
typedef enum MotorSpeed_t
{
    MOTOR_SPEED_25,
    MOTOR_SPEED_50,
    MOTOR_SPEED_75,
    MOTOR_SPEED_100,
} MotorSpeed_t;

/**
 * @brief Switch states
 * 
 */
typedef enum SwitchState_t
{
    NONE_PRESSED,
    SWITCH_1_PRESSED,
    SWITCH_2_PRESSED,
} SwitchState_t;

// Constants
#define SWITCH_1_PIN PC4
#define SWITCH_2_PIN PC5
#define RGB_RED_PIN PD7
#define RGB_BLUE_PIN PD4
#define RGB_GREEN_PIN PD2
#define LED_1_PIN PC0
#define LED_2_PIN PC1
#define LED_3_PIN PC2
#define LED_4_PIN PC3
#define ENABLE_PIN PB3
#define INPUT_1_PIN PB2
#define INPUT_2_PIN PB1
#define MAX_DUTY_CYCLE 255

// Variables
MotorDirection_t motorDirection = MOTOR_OFF;
MotorDirection_t previousMotorDirection = MOTOR_OFF;
MotorSpeed_t motorSpeed = MOTOR_SPEED_25;

uint8_t dutyCycle = MAX_DUTY_CYCLE * 0.25; // default duty cycle is 25%

// Function Prototypes
void pwm(uint8_t duty);
void delayMicros(unsigned long delay);
void setMotorSpeed();
void setMotorDirection();
void setDutyCycle();

// Switch debouncers
Debouncer switch1;
Debouncer switch2;

void setup()
{
    switch1.begin(SWITCH_1_PIN); // initialize switch 1
    switch2.begin(SWITCH_2_PIN); // initialize switch 2

    // Set LED 1, 2, 3, 4 as outputs
    DDRC |= (1 << LED_1_PIN) | (1 << LED_2_PIN) | (1 << LED_3_PIN) | (1 << LED_4_PIN); // 0b00000001 | 0b00000010 | 0b00000100 | 0b00001000 = 0b00001111

    // Set RGB LED as outputs
    DDRD |= (1 << RGB_RED_PIN) | (1 << RGB_GREEN_PIN) | (1 << RGB_BLUE_PIN); // 0b00000100 | 0b00010000 | 0b10000000 = 0b10010100

    // Set H-Bridge as outputs
    DDRB |= (1 << ENABLE_PIN) | (1 << INPUT_1_PIN) | (1 << INPUT_2_PIN); // 0b00001000 | 0b00000010 | 0b00000001 = 0b00001011

    // Set initial state of LED 1, 2, 3, 4 to off
    PORTC &= ~((1 << LED_1_PIN) | (1 << LED_2_PIN) | (1 << LED_3_PIN) | (1 << LED_4_PIN)); // 0b00000001 | 0b00000010 | 0b00000100 | 0b00001000 = 0b00001111: ~0b00001111 = 0b11110000

    // Set initial state of RGB LED to off
    PORTD &= ~((1 << RGB_RED_PIN) | (1 << RGB_GREEN_PIN) | (1 << RGB_BLUE_PIN)); // 0b00000100 | 0b00010000 | 0b10000000 = 0b10010100: ~0b10010100 = 0b01101011

    // set initial state of motor to off
    PORTB &= ~(1 << ENABLE_PIN); // 0b00001000

    // set initial motor direction to off
    PORTD &= ~((1 << RGB_RED_PIN) | (1 << RGB_GREEN_PIN) | (1 << RGB_BLUE_PIN)); // 0b00000100 | 0b00010000 | 0b10000000 = 0b10010100: ~0b10010100 = 0b01101011

    // set initial state of LED 1 to on
    PORTC |= (1 << LED_1_PIN); // 0b00000001 = 0b00000001
}

void loop()
{
    // switch 1: motor direction
    if (switch1.debounce())
    {
        // motor direction: forward -> off -> reverse -> off -> forward -> off
        setMotorDirection();
    }

    // switch 2: motor speed
    if (switch2.debounce())
    {
        // motor speed: 25% -> 50% -> 75% -> 100%
        setMotorSpeed();
    }

    // PWM
    switch (motorDirection)
    {
    case MOTOR_FORWARD: // intentional fall-through
    case MOTOR_REVERSE:
        PORTB |= (1 << ENABLE_PIN);
        delayMicros(dutyCycle);
        PORTB &= ~(1 << ENABLE_PIN);
        break;
    case MOTOR_OFF:
        PORTB &= ~(1 << ENABLE_PIN);
        break;
    default:
        break;
    }
}

/**
 * @brief custom delay function in microseconds
 * 
 * @param delay 
 */
void delayMicros(unsigned long delay)
{
    volatile unsigned long start = micros();
    while (micros() - start < delay)
    {
        // do nothing
    }
}

/**
 * @brief Set the Motor Speed. This function cycles through the motor speed settings and sets the yellow LED's accordingly
 * 
 */
void setMotorSpeed()
{
    switch (motorSpeed)
    {
    case MOTOR_SPEED_25:
        motorSpeed = MOTOR_SPEED_50;
        // set LED 1,2 on and 3,4 off
        PORTC |= (1 << LED_1_PIN) | (1 << LED_2_PIN);
        PORTC &= ~((1 << LED_3_PIN) | (1 << LED_4_PIN));
        break;

    case MOTOR_SPEED_50:
        motorSpeed = MOTOR_SPEED_75;
        // set LED 1, 2, 3 on and 4 off
        PORTC |= (1 << LED_1_PIN) | (1 << LED_2_PIN) | (1 << LED_3_PIN);
        PORTC &= ~(1 << LED_4_PIN);
        break;

    case MOTOR_SPEED_75:
        motorSpeed = MOTOR_SPEED_100;
        // set LED 1, 2, 3, 4 on
        PORTC |= (1 << LED_1_PIN) | (1 << LED_2_PIN) | (1 << LED_3_PIN) | (1 << LED_4_PIN);
        break;

    case MOTOR_SPEED_100:
        motorSpeed = MOTOR_SPEED_25;
        // set LED 1 on and 2, 3, 4 off
        PORTC |= (1 << LED_1_PIN);
        PORTC &= ~((1 << LED_2_PIN) | (1 << LED_3_PIN) | (1 << LED_4_PIN));
        break;

    default:
        break;
    }

    setDutyCycle(); // call function to set duty cycle
}

/**
 * @brief Set the Motor Direction. This function cycles through the motor direction settings and sets the RGB LED accordingly
 * 
 */
void setMotorDirection()
{
    switch (motorDirection)
    {
    case MOTOR_OFF:
        if (previousMotorDirection == MOTOR_FORWARD)
        {
            motorDirection = MOTOR_REVERSE;
            // set pins to reverse
            PORTD |= (1 << RGB_RED_PIN);
            PORTD &= ~((1 << RGB_GREEN_PIN) | (1 << RGB_BLUE_PIN));
            PORTB |= (1 << INPUT_2_PIN);
            PORTB &= ~(1 << INPUT_1_PIN);
        }
        else
        {
            motorDirection = MOTOR_FORWARD;
            // set pins to forward
            PORTD |= (1 << RGB_GREEN_PIN);
            PORTD &= ~((1 << RGB_RED_PIN) | (1 << RGB_BLUE_PIN));
            PORTB |= (1 << INPUT_1_PIN);
            PORTB &= ~(1 << INPUT_2_PIN);
        }
        previousMotorDirection = motorDirection;
        break;

    case MOTOR_FORWARD:
    case MOTOR_REVERSE:
        previousMotorDirection = motorDirection;
        motorDirection = MOTOR_OFF;
        // set RGB off
        PORTD &= ~((1 << RGB_RED_PIN) | (1 << RGB_GREEN_PIN) | (1 << RGB_BLUE_PIN));
        break;

    default:
        break;
    }
}

/**
 * @brief Set the Duty Cycle. This function sets the duty cycle based on the motor speed setting
 * 
 */
void setDutyCycle()
{
    switch (motorSpeed)
    {
    case MOTOR_SPEED_25:
        dutyCycle = MAX_DUTY_CYCLE * 0.25;
        break;

    case MOTOR_SPEED_50:
        dutyCycle = MAX_DUTY_CYCLE * 0.50;
        break;

    case MOTOR_SPEED_75:
        dutyCycle = MAX_DUTY_CYCLE * 0.75;
        break;

    case MOTOR_SPEED_100:
        dutyCycle = MAX_DUTY_CYCLE;
        break;

    default:
        break;
    }
}
