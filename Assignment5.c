#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

volatile uint16_t milliseconds = 0;

ISR(TIMER1_COMPA_vect)
{
	milliseconds++;
}

void timer1_init(void)
{
	TCCR1A = 0;
	TCCR1B = 0;

	// CTC mode
	TCCR1B |= (1 << WGM12);

	// 1 ms timer interrupt at 16 MHz
	OCR1A = 249;

	// Prescaler = 64
	TCCR1B |= (1 << CS11) | (1 << CS10);

	// Enable Timer1 compare match interrupt
	TIMSK1 |= (1 << OCIE1A);

	sei();
}

void delay_ms(uint16_t duration)
{
	milliseconds = 0;

	while (milliseconds < duration)
	{
		// Wait until the required time has passed
	}
}

int main(void)
{
	// PB0 = Green, PB1 = Orange, PB2 = Red
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);

	timer1_init();

	while (1)
	{
		// Green light ON for 10 seconds
		PORTB = (1 << PB0);
		delay_ms(10000);

		// Orange light ON for 3 seconds
		PORTB = (1 << PB1);
		delay_ms(3000);

		// Red light ON for 10 seconds
		PORTB = (1 << PB2);
		delay_ms(10000);
	}

	return 0;
}
