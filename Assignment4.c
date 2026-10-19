#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// Function declaration
void updateLEDs(unsigned char count);

int main(void)
{
	unsigned char count = 0;

	// PB0, PB1, PB2 and PB3 as outputs
	DDRB = DDRB | 0b00001111;

	// PD2 as input for the push button
	DDRD = DDRD & 0b11111011;

	// Enable internal pull-up resistor on PD2
	PORTD = PORTD | 0b00000100;

	// Initially display 0000
	updateLEDs(count);

	while (1)
	{
		// Check whether the button is pressed
		if ((PIND & 0b00000100) == 0)
		{
			_delay_ms(50);  // Debounce delay

			// Check again after the delay
			if ((PIND & 0b00000100) == 0)
			{
				// Increase count, or reset after 15
				if (count == 15)
				{
					count = 0;
				}
				else
				{
					count++;
				}

				// Update the four LEDs
				updateLEDs(count);

				// Wait until the button is released
				while ((PIND & 0b00000100) == 0)
				{
					// Do nothing
				}

				_delay_ms(50);
			}
		}
	}

	return 0;
}

// Function to display the count on the LEDs
void updateLEDs(unsigned char count)
{
	PORTB = (PORTB & 0b11110000) | (count & 0b00001111);
}
