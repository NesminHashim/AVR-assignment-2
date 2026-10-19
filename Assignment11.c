#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// Function declarations
void ADC_init(void);
int readPotentiometer(void);
void Timer1_init(void);
void setServoPosition(int adcValue);

int main(void)
{
    // Initialize ADC
    ADC_init();

    // Initialize Timer1 for servo control
    Timer1_init();

    while (1)
    {
        // Read potentiometer value
        int adcValue = readPotentiometer();

        // Set servo position
        setServoPosition(adcValue);

        // Small delay
        _delay_ms(10);
    }

    return 0;
}

// Function 1: Initialize ADC
void ADC_init(void)
{
    // Use AVCC as reference and select ADC0
    ADMUX = (1 << REFS0);

    // Enable ADC with prescaler 128
    ADCSRA = (1 << ADEN) |
             (1 << ADPS2) |
             (1 << ADPS1) |
             (1 << ADPS0);
}

// Function 2: Read potentiometer value
int readPotentiometer(void)
{
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);

    // Wait for conversion to finish
    while (ADCSRA & (1 << ADSC))
    {
        // Wait
    }

    // Return 10-bit ADC value (0 to 1023)
    return ADC;
}

// Function 3: Initialize Timer1
void Timer1_init(void)
{
    // Set PB1 (OC1A) as output
    DDRB |= (1 << DDB1);

    // Set TOP value for 50 Hz PWM
    ICR1 = 20000;

    // Phase Correct PWM mode 10, non-inverting OC1A
    TCCR1A = (1 << COM1A1) |
             (1 << WGM11);

    TCCR1B = (1 << WGM13) |
             (1 << CS11);  // Prescaler = 8

    // Initial servo position: approximately 0 degrees
    OCR1A = 1000;
}

// Function 4: Set servo position
void setServoPosition(int adcValue)
{
    // Map ADC value (0-1023) to pulse width (1000-2000 us)
    OCR1A = 1000 + ((long)adcValue * 1000 / 1023);
}
