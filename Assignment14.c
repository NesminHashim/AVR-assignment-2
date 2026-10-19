#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// Function declarations
void ADC_init(void);
int readPotentiometer(void);
void PWM_init(void);
void controlMotor(int adcValue);

int main(void)
{
    // Initialize ADC
    ADC_init();

    // Initialize Timer0 PWM
    PWM_init();

    while (1)
    {
        // Read the potentiometer
        int adcValue = readPotentiometer();

        // Control motor speed
        controlMotor(adcValue);

        // Small delay before the next reading
        _delay_ms(10);
    }

    return 0;
}

// Function 1: Initialize ADC
void ADC_init(void)
{
    // Use AVCC (5V) as reference and select ADC0
    ADMUX = (1 << REFS0);

    // Enable ADC and set prescaler to 128
    ADCSRA = (1 << ADEN) |
             (1 << ADPS2) |
             (1 << ADPS1) |
             (1 << ADPS0);
}

// Function 2: Read the potentiometer
int readPotentiometer(void)
{
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);

    // Wait for conversion to finish
    while (ADCSRA & (1 << ADSC))
    {
        // Wait
    }

    // Return the 10-bit ADC value (0 to 1023)
    return ADC;
}

// Function 3: Initialize Timer0 Fast PWM
void PWM_init(void)
{
    // Set PD6 (OC0A) as output
    DDRD |= (1 << DDD6);

    // Select Fast PWM mode and non-inverting output
    TCCR0A = (1 << WGM00) |
             (1 << WGM01) |
             (1 << COM0A1);

    // Start Timer0 with prescaler 64
    TCCR0B = (1 << CS01) | (1 << CS00);

    // Start with motor stopped
    OCR0A = 0;
}

// Function 4: Convert ADC value into motor speed
void controlMotor(int adcValue)
{
    // Convert 10-bit ADC value (0-1023)
    // into 8-bit PWM value (0-255)
    int pwmValue = adcValue / 4;

    // Apply the PWM value to Timer0
    OCR0A = pwmValue;
}
