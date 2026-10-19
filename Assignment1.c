#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	// Set PORTD Pin 5 (PD5) as output
	DDRD |= (1 << PD5);

	while (1)
	{
		// LED ON
		PORTD |= (1 << PD5);
		_delay_ms(3000);      // ON for 3 seconds

		// LED OFF
		PORTD &= ~(1 << PD5);
		_delay_ms(5000);      // OFF for 5 seconds
	}

	return 0;
}
