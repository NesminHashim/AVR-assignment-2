#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

// Emergency request flag
volatile uint8_t emergency_flag = 0;

// Millisecond counter
volatile uint16_t milliseconds = 0;

// Timer1 interrupt: occurs every 1 ms
ISR(TIMER1_COMPA_vect)
{
	milliseconds++;
}

// External Interrupt 0: emergency button pressed
ISR(INT0_vect)
{
	emergency_flag = 1;
}

// Initialize Timer1
void timer1_init(void)
{
	TCCR1A = 0;
	TCCR1B = 0;

	// CTC mode
	TCCR1B |= (1 << WGM12);

	// 1 ms interval at 16 MHz, prescaler 64
	OCR1A = 249;

	// Prescaler = 64
	TCCR1B |= (1 << CS11) | (1 << CS10);

	// Enable Timer1 compare interrupt
	TIMSK1 |= (1 << OCIE1A);
}

// Initialize external interrupt INT0
void int0_init(void)
{
	// PD2 (INT0) as input
	DDRD &= ~(1 << PD2);

	// Enable internal pull-up resistor
	PORTD |= (1 << PD2);

	// Trigger INT0 on falling edge
	EICRA |= (1 << ISC01);
	EICRA &= ~(1 << ISC00);

	// Enable INT0
	EIMSK |= (1 << INT0);
}

// Delay using Timer1, without _delay()
void delay_ms(uint16_t duration)
{
	milliseconds = 0;

	while (milliseconds < duration)
	{
		// Wait for timer interrupts
	}
}

// Turn on Green LED
void green_light(void)
{
	PORTB = (1 << PB0);
}

// Turn on Orange LED
void orange_light(void)
{
	PORTB = (1 << PB1);
}

// Turn on Red LED
void red_light(void)
{
	PORTB = (1 << PB2);
}

int main(void)
{
	// PB0 = Green, PB1 = Orange, PB2 = Red
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);

	timer1_init();
	int0_init();

	sei();

	while (1)
	{
		// GREEN: normal duration 10 seconds
		green_light();
		delay_ms(10000);

		if (emergency_flag)
		{
			emergency_flag = 0;
			green_light();
			delay_ms(10000);
		}

		// ORANGE: normal duration 3 seconds
		orange_light();
		delay_ms(3000);

		if (emergency_flag)
		{
			emergency_flag = 0;
			green_light();
			delay_ms(10000);
		}

		// RED: normal duration 10 seconds
		red_light();
		delay_ms(10000);

		if (emergency_flag)
		{
			emergency_flag = 0;
			green_light();
			delay_ms(10000);
		}
	}

	return 0;
}
