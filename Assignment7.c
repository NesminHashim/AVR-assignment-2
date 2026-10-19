#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

// Count Timer2 interrupts
volatile uint8_t timer2_count = 0;

// Count Timer1 interrupts
volatile uint8_t timer1_count = 0;

int main(void)
{
	// Configure PB1 and PB2 as output pins
	DDRB |= (1 << PB1) | (1 << PB2);

	// Turn both LEDs OFF initially
	PORTB &= ~((1 << PB1) | (1 << PB2));

	// -------- TIMER1 CONFIGURATION --------
	// Timer1 CTC mode
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12);

	// Prescaler = 1024
	// Compare Match every 1 second
	OCR1A = 15624;

	// Enable Timer1 Compare Match A interrupt
	TIMSK1 |= (1 << OCIE1A);

	// Start Timer1
	TCCR1B |= (1 << CS12) | (1 << CS10);


	// -------- TIMER2 CONFIGURATION --------
	// Timer2 CTC mode
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR2A |= (1 << WGM21);

	// Prescaler = 1024
	// Compare Match every 8 milliseconds
	OCR2A = 124;

	// Enable Timer2 Compare Match A interrupt
	TIMSK2 |= (1 << OCIE2A);

	// Start Timer2
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

	// Enable global interrupts
	sei();

	// Main loop
	while (1)
	{
		// Both timers operate independently
	}
}


// Timer1 ISR: Toggle Status LED every 2 seconds
ISR(TIMER1_COMPA_vect)
{
	timer1_count++;

	if (timer1_count >= 2)
	{
		PORTB ^= (1 << PB1);
		timer1_count = 0;
	}
}


// Timer2 ISR: Toggle Heartbeat LED every 500 milliseconds
ISR(TIMER2_COMPA_vect)
{
	timer2_count++;

	if (timer2_count >= 62)
	{
		PORTB ^= (1 << PB2);
		timer2_count = 0;
	}
}

