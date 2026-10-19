#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	// Set all PORTB pins as output
	DDRB = 0xFF;

	while (1)
	{
		// LED 1 ON
		PORTB = 0b00000001;
		_delay_ms(300);

		// LED 2 ON
		PORTB = 0b00000010;
		_delay_ms(300);

		// LED 3 ON
		PORTB = 0b00000100;
		_delay_ms(300);

		// LED 4 ON
		PORTB = 0b00001000;
		_delay_ms(300);

		// LED 5 ON
		PORTB = 0b00010000;
		_delay_ms(300);

		// LED 6 ON
		PORTB = 0b00100000;
		_delay_ms(300);

		// LED 7 ON
		PORTB = 0b01000000;
		_delay_ms(300);

		// LED 8 ON
		PORTB = 0b10000000;
		_delay_ms(300);
	}

	return 0;
}
