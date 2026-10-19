#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 0b01100000;   // PD5 and PD6 are outputs
	PORTD = 0b00000100;  // Enable pull-up on PD2

	PORTD |= (1 << PD5); // Red LED ON
	PORTD &= ~(1 << PD6); // Green LED OFF

	while (1)
	{
		if ((PIND & (1 << PD2)) == 0)
		{
			_delay_ms(50); // Button debounce

			if ((PIND & (1 << PD2)) == 0)
			{
				// If Red LED is ON
				if (PORTD & (1 << PD5))
				{
					PORTD &= ~(1 << PD5); // Red OFF
					PORTD |= (1 << PD6);  // Green ON
				}
				else
				{
					PORTD &= ~(1 << PD6); // Green OFF
					PORTD |= (1 << PD5);  // Red ON
				}

				// Wait until button is released
				while ((PIND & (1 << PD2)) == 0)
				{
				}

				_delay_ms(50);
			}
		}
	}
}
