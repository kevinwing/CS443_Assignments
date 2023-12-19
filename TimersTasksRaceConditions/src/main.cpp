/**
 * @file main.cpp
 * @author Kevin Wing (wing5640@vandals.uidaho.edu)
 * @brief Timer, Tasks, Race Conditions, and Interrupts
 */
#include <Arduino.h>

// Define pins and masks
const uint8_t SEGMENT_DP_PIN = PD7; // Assign the appropriate pin number
const uint8_t SW3_PIN = PB3;        // Assign the appropriate pin number
const uint8_t CC1 = PB0;
const uint8_t CC2 = PB1;

// 7-segment hexfont
const uint8_t hexfont[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001  // F
};

// Function prototypes
void configureTimer1();
void dpAtomicToggle();
void dpToggle();
byte readSw3();

// Global variables
volatile uint8_t counter = 0;

// ISR for Timer 1
ISR(TIMER1_COMPA_vect) {
    // turn off cc2
    PORTB &= ~(1 << CC1);

    // Increment the 7-segment display on CC2
    PORTB &= ~(1 << CC2); // Disable CC2
    PORTD = hexfont[counter & 0x0F];
    PORTB |= (1 << CC2);  // Enable CC2

    // turn on cc2
    PORTB |= (1 << CC1);

    // Increment the 7-segment display on CC1
    counter = (counter + 1) & 0x0F; // Increment and wrap around after 0xF
}


int main()
{
    // Initialize ports and pins
    DDRD = 0b11111111; // Set all PORTD pins as outputs
    DDRB |= (1 << CC1) | (1 << CC2);
    DDRB &= ~(1 << SW3_PIN); // Set SW3 pin as input
    configureTimer1();

    while (1)
    {
        if (readSw3() == 0)
        {
            dpAtomicToggle(); // Atomically toggle DP
        }
        else
        {
            dpToggle(); // Non-atomically toggle DP
        }
    }

    return 0;
}

void configureTimer1()
{
    // Disable interrupts
    cli();

    // Set Timer1 in CTC (Clear Timer on Compare Match) mode
    TCCR1A = 0;             // Clear control register A
    TCCR1B = 0;             // Clear control register B
    TCCR1B |= (1 << WGM12); // Configure for CTC mode

    // Set TOP value for Timer1 to 20832: ~1ms
    OCR1A = 20832;

    // Enable Timer1 compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Set prescaler to 256 and start the timer
    TCCR1B |= (1 << CS12);

    // Re-enable interrupts
    sei();
}

void dpAtomicToggle()
{
    // writing a 1 to PINx toggles the pin state atomically
    PIND |= (1 << SEGMENT_DP_PIN); // atomically toggle DP pin
}

void dpToggle()
{
    PORTB |= (1 << CC1);
    // Toggle DP pin
    PORTD ^= (1 << SEGMENT_DP_PIN); // Toggle DP pin

    // turn off cc2
    PORTB &= ~(1 << CC1);
}

byte readSw3()
{
    // Read SW3 state and return it
    return (PIND & (1 << SW3_PIN));
}
