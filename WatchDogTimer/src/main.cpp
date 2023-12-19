/**
 * @file main.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Watchdog Timer example
 */
#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "debouncer.hpp"

void blinkLED();
void handleWatchdogTimer();
void wakeUpISR();

const int ledPin = 13;
const int buttonWakePin = 10;  // Replace SW2 with actual pin number
const int buttonSleepPin = 11; // Replace SW3 with actual pin number

Debouncer buttonWake;
Debouncer buttonSleep;

void setup()
{
    pinMode(ledPin, OUTPUT);
    buttonWake.begin(buttonWakePin);
    buttonSleep.begin(buttonSleepPin);

    // Set up Watchdog Timer
    wdt_reset();                                      // Reset the WDT
    WDTCSR |= (1 << WDCE) | (1 << WDE);               // Start timed sequence
    WDTCSR = (1 << WDIE) | (1 << WDP2) | (1 << WDP1); // Set interrupt mode, ~2s time-out

    // Set up sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Power-down mode
    sleep_enable();                      // Enable sleep mode
    sei();                               // Enable global interrupts

    // Set up wake-up interrupt
    attachInterrupt(digitalPinToInterrupt(buttonWakePin), wakeUpISR, FALLING);

    Serial.begin(9600);
}

void loop()
{
    blinkLED();
    handleWatchdogTimer();
}

void blinkLED()
{
    static unsigned long lastToggleTime = 0; // Stores the last time the LED was toggled
    unsigned long currentMillis = millis();  // Current time

    // Check if 250 milliseconds have passed
    if (currentMillis - lastToggleTime >= 250)
    {
        digitalWrite(ledPin, !digitalRead(ledPin)); // Toggle the LED state
        lastToggleTime = currentMillis;             // Remember the toggle time
    }
}

void handleWatchdogTimer()
{
    static unsigned long pressStartTime = 0;
    bool pressed = buttonSleep.update();

    if (pressed)
    {
        if (pressStartTime == 0)
        { // Record the time when button is first pressed
            pressStartTime = millis();
        }

        unsigned long pressDuration = millis() - pressStartTime;
        Serial.print("Button pressed for: ");
        Serial.print(pressDuration);
        Serial.println(" ms");
    }
    else
    {
        if (pressStartTime != 0)
        {
            Serial.println("Button released");
            pressStartTime = 0;
        }
    }

    if (!pressed)
    {
        wdt_reset(); // Reset WDT if the button is not pressed
    }
}

// ISR for WDT interrupt
ISR(WDT_vect)
{
    // Disable further WDT interrupts
    wdt_disable();

    // Prepare for sleep
    digitalWrite(ledPin, LOW);

    // Enter sleep mode
    // Serial.println("Entering sleep mode");
    sleep_mode();

    // After waking up
    // digitalWrite(ledPin, HIGH);
}

void wakeUpISR()
{
    // Wake up is handled. Nothing to do here
}
