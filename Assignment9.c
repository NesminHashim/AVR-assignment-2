#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // Step 1: Set PD6 (OC0A) as output for LED
    DDRD |= (1 << DDD6);

    // Step 2: Set PD2 as input for push button
    DDRD &= ~(1 << DDD2);

    // Enable internal pull-up resistor for button
    PORTD |= (1 << PORTD2);

    // Step 3: Set Timer0 to Fast PWM mode
    TCCR0A = (1 << WGM00) | (1 << WGM01);

    // Non-inverting PWM output on OC0A (PD6)
    TCCR0A |= (1 << COM0A1);

    // Step 4: Set initial duty cycle to 25%
    OCR0A = 64;

    // Step 5: Start Timer0 with prescaler 64
    TCCR0B = (1 << CS01) | (1 << CS00);

    // Duty cycle selection
    unsigned char level = 0;

    while (1)
    {
        // Button is pressed when PD2 becomes LOW
        if (!(PIND & (1 << PIND2)))
        {
            _delay_ms(50);  // Debounce delay

            // Check if button is still pressed
            if (!(PIND & (1 << PIND2)))
            {
                level++;

                if (level > 3)
                {
                    level = 0;
                }

                // Update PWM duty cycle
                if (level == 0)
                {
                    OCR0A = 64;   // 25%
                }
                else if (level == 1)
                {
                    OCR0A = 128;  // 50%
                }
                else if (level == 2)
                {
                    OCR0A = 191;  // 75%
                }
                else
                {
                    OCR0A = 0;    // LED OFF
                }

                // Wait until button is released
                while (!(PIND & (1 << PIND2)))
                {
                    // Wait
                }

                _delay_ms(50);  // Release debounce
            }
        }
    }

    return 0;
}
