#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

// LCD connections
#define LCD_RS PB0
#define LCD_EN PB1

// Push button connection
#define BUTTON PC0

// Function declarations
void LCD_init(void);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_print(char *text);
void LCD_sendNibble(unsigned char nibble);
void LCD_setCursor(unsigned char row, unsigned char column);

unsigned char checkButtonHoldTime(void);
void updateCounterLCD(uint32_t counter);

int main(void)
{
    uint32_t counter = 0;

    // Configure LCD pins as outputs
    DDRB |= (1 << LCD_RS) | (1 << LCD_EN);
    DDRD |= 0xF0;  // PD4 to PD7 as outputs

    // Configure button as input
    DDRC &= ~(1 << BUTTON);

    // Enable internal pull-up resistor
    PORTC |= (1 << BUTTON);

    // Initialize LCD
    LCD_init();

    // Display initial counter
    updateCounterLCD(counter);

    while (1)
    {
        // Button pressed when input is LOW
        if (!(PINC & (1 << BUTTON)))
        {
            _delay_ms(30);  // Debounce delay

            if (!(PINC & (1 << BUTTON)))
            {
                // Check how long button is held
                unsigned char longPress = checkButtonHoldTime();

                if (longPress == 1)
                {
                    // Long press: reset counter
                    counter = 0;
                }
                else
                {
                    // Short press: increase counter
                    counter++;

                    // Keep the counter within 9 digits
                    if (counter > 999999999UL)
                    {
                        counter = 0;
                    }
                }

                // Update the LCD display
                updateCounterLCD(counter);
            }
        }
    }

    return 0;
}

// Send 4 bits to LCD
void LCD_sendNibble(unsigned char nibble)
{
    PORTD = (PORTD & 0x0F) | ((nibble & 0x0F) << 4);

    PORTB |= (1 << LCD_EN);
    _delay_us(1);

    PORTB &= ~(1 << LCD_EN);
    _delay_us(100);
}

// Send LCD command
void LCD_command(unsigned char command)
{
    PORTB &= ~(1 << LCD_RS);

    LCD_sendNibble(command >> 4);
    LCD_sendNibble(command & 0x0F);

    if (command == 0x01 || command == 0x02)
    {
        _delay_ms(2);
    }
}

// Send character to LCD
void LCD_data(unsigned char data)
{
    PORTB |= (1 << LCD_RS);

    LCD_sendNibble(data >> 4);
    LCD_sendNibble(data & 0x0F);
}

// Print a string on LCD
void LCD_print(char *text)
{
    while (*text)
    {
        LCD_data(*text);
        text++;
    }
}

// Set LCD cursor position
void LCD_setCursor(unsigned char row, unsigned char column)
{
    if (row == 0)
    {
        LCD_command(0x80 + column);
    }
    else
    {
        LCD_command(0xC0 + column);
    }
}

// Initialize LCD in 4-bit mode
void LCD_init(void)
{
    _delay_ms(50);

    PORTB &= ~(1 << LCD_RS);
    PORTB &= ~(1 << LCD_EN);

    LCD_sendNibble(0x03);
    _delay_ms(5);

    LCD_sendNibble(0x03);
    _delay_us(150);

    LCD_sendNibble(0x03);
    LCD_sendNibble(0x02);

    LCD_command(0x28);  // 4-bit mode, 2 lines
    LCD_command(0x0C);  // Display ON, cursor OFF
    LCD_command(0x06);  // Move cursor forward
    LCD_command(0x01);  // Clear display
}

// Measure button hold time
unsigned char checkButtonHoldTime(void)
{
    unsigned int holdTime = 0;

    // Count time while button remains pressed
    while (!(PINC & (1 << BUTTON)))
    {
        _delay_ms(10);
        holdTime++;

        // 200 x 10 ms = 2 seconds
        if (holdTime >= 200)
        {
            // Wait for button release
            while (!(PINC & (1 << BUTTON)))
            {
                _delay_ms(10);
            }

            _delay_ms(30);
            return 1;  // Long press
        }
    }

    _delay_ms(30);  // Release debounce

    return 0;  // Short press
}

// Display the 9-digit counter
void updateCounterLCD(uint32_t counter)
{
    char number[10];

    // Format number with leading zeros
    snprintf(number, sizeof(number), "%09lu",
             (unsigned long)counter);

    LCD_setCursor(0, 0);
    LCD_print("Counter:        ");

    LCD_setCursor(1, 0);
    LCD_print(number);
    LCD_print("       ");
}
