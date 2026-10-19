#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Duty cycle levels
#define DUTY_25   64
#define DUTY_50   128
#define DUTY_75   191
#define DUTY_OFF  0

// Function declaration
void update_duty_cycle(uint8_t level);

int main(void)
{
	// Set PD6 (OC0A) as output for LED
	DDRD |= (1 << DDD6);

	// Set PD2 as input for push-button
	DDRD &= ~(1 << DDD2);

	// Enable internal pull-up resistor on PD2
	PORTD |= (1 << PORTD2);

	// Initialize Timer0 in Fast PWM mode
	// Non-inverting mode, output on OC0A (PD6)
	TCCR0A = (1 << COM0A1) |
	(1 << WGM01) |
	(1 << WGM00);

	// Prescaler = 64
	TCCR0B = (1 << CS01) | (1 << CS00);

	// Start with 25% duty cycle
	update_duty_cycle(0);

	uint8_t level = 0;

	while (1)
	{
		// Button pressed (active LOW)
		if (!(PIND & (1 << PIND2)))
		{
			_delay_ms(50);  // Debouncing

			if (!(PIND & (1 << PIND2)))
			{
				level++;

				if (level > 3)
				{
					level = 0;
				}

				// Update PWM duty cycle
				update_duty_cycle(level);

				// Wait until button is released
				while (!(PIND & (1 << PIND2)))
				{
					// Wait
				}

				_delay_ms(50);
			}
		}
	}
}

// Dedicated function to update PWM duty cycle
void update_duty_cycle(uint8_t level)
{
	switch (level)
	{
		case 0:
		OCR0A = DUTY_25;
		break;

		case 1:
		OCR0A = DUTY_50;
		break;

		case 2:
		OCR0A = DUTY_75;
		break;

		case 3:
		OCR0A = DUTY_OFF;
		break;

		default:
		OCR0A = DUTY_25;
		break;
	}
}


