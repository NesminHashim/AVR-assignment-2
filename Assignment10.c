#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// Function declarations
void ADC_init(void);
int readTemperature(void);
int calculateSpeed(int temperature);
void controlMotor(int speed);
void PWM_init(void);

int main(void)
{
    // Set PD2 as input for push button
    DDRD &= ~(1 << DDD2);

    // Enable internal pull-up resistor
    PORTD |= (1 << PORTD2);

    // Initialize ADC and PWM
    ADC_init();
    PWM_init();

    unsigned char systemOn = 0;

    while (1)
    {
        // Check whether button is pressed
        if (!(PIND & (1 << PIND2)))
        {
            _delay_ms(50);  // Debounce delay

            if (!(PIND & (1 << PIND2)))
            {
                // Toggle system ON/OFF
                systemOn = !systemOn;

                // Wait until button is released
                while (!(PIND & (1 << PIND2)))
                {
                    // Wait
                }

                _delay_ms(50);
            }
        }

        if (systemOn == 1)
        {
            // Read temperature from LM35
            int temperature = readTemperature();

            // Calculate motor speed
            int speed = calculateSpeed(temperature);

            // Control motor speed
            controlMotor(speed);
        }
        else
        {
            // System OFF: stop motor
            OCR0A = 0;
        }

        _delay_ms(200);
    }

    return 0;
}

// Function 1: Initialize ADC
void ADC_init(void)
{
    // Use AVCC as reference and ADC0 as input
    ADMUX = (1 << REFS0);

    // Enable ADC, set prescaler to 128
    ADCSRA = (1 << ADEN) |
             (1 << ADPS2) |
             (1 << ADPS1) |
             (1 << ADPS0);
}

// Function 2: Read temperature from LM35
int readTemperature(void)
{
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);

    // Wait until conversion finishes
    while (ADCSRA & (1 << ADSC))
    {
        // Wait
    }

    // Read ADC value
    int adcValue = ADC;

    // Convert ADC value to temperature in Celsius
    int temperature = (adcValue * 500L) / 1024;

    return temperature;
}

// Function 3: Calculate motor speed
int calculateSpeed(int temperature)
{
    if (temperature < 25)
    {
        return 64;    // Approximately 25% speed
    }
    else if (temperature < 35)
    {
        return 128;   // Approximately 50% speed
    }
    else
    {
        return 230;   // Approximately 90% speed
    }
}

// Function 4: Initialize PWM
void PWM_init(void)
{
    // Set PD6 (OC0A) as output
    DDRD |= (1 << DDD6);

    // Fast PWM mode, non-inverting output
    TCCR0A = (1 << WGM00) |
             (1 << WGM01) |
             (1 << COM0A1);

    // Start Timer0 with prescaler 64
    TCCR0B = (1 << CS01) | (1 << CS00);

    // Initially stop motor
    OCR0A = 0;
}

// Function 5: Control motor
void controlMotor(int speed)
{
    // Set PWM duty cycle
    OCR0A = speed;
}
