#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function declarations
void UART_init(void);
void UART_sendChar(char data);
void UART_sendString(char *text);
char UART_receiveChar(void);
void UART_receiveLine(char *buffer, int size);

void PWM_init(void);
void setBrightness(int percentage);

int main(void)
{
    char command[16];
    char message[40];
    int brightness;

    // Initialize UART and PWM
    UART_init();
    PWM_init();

    UART_sendString("LED Control Ready\r\n");
    UART_sendString("Send ON, OFF, or a number from 0 to 100\r\n");

    while (1)
    {
        // Receive a command from the serial terminal
        UART_receiveLine(command, sizeof(command));

        // Ignore empty commands
        if (command[0] == '\0')
        {
            continue;
        }

        // Convert command to uppercase
        for (int i = 0; command[i] != '\0'; i++)
        {
            command[i] = toupper((unsigned char)command[i]);
        }

        // Command 1: Turn LED ON
        if (strcmp(command, "ON") == 0)
        {
            setBrightness(100);

            UART_sendString("LED is now ON\r\n");
        }

        // Command 2: Turn LED OFF
        else if (strcmp(command, "OFF") == 0)
        {
            setBrightness(0);

            UART_sendString("LED is now OFF\r\n");
        }

        // Command 3: Set brightness using a number
        else
        {
            char *end;
            long value = strtol(command, &end, 10);

            // Check whether the whole command is a valid number
            if (end != command && *end == '\0'
                && value >= 0 && value <= 100)
            {
                brightness = (int)value;

                setBrightness(brightness);

                // Prepare the response message
                itoa(brightness, message, 10);

                UART_sendString("Brightness set to ");
                UART_sendString(message);
                UART_sendString("%\r\n");
            }
            else
            {
                UART_sendString("Invalid command. Use ON, OFF, or 0-100\r\n");
            }
        }
    }

    return 0;
}

// Function 1: Initialize UART at 9600 baud
void UART_init(void)
{
    // Baud rate for 16 MHz clock and 9600 baud
    UBRR0H = 0;
    UBRR0L = 103;

    // Enable transmitter and receiver
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // 8 data bits, no parity, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Function 2: Send one character
void UART_sendChar(char data)
{
    // Wait until transmit buffer is empty
    while (!(UCSR0A & (1 << UDRE0)))
    {
        // Wait
    }

    UDR0 = data;
}

// Function 3: Send a string
void UART_sendString(char *text)
{
    while (*text != '\0')
    {
        UART_sendChar(*text);
        text++;
    }
}

// Function 4: Receive one character
char UART_receiveChar(void)
{
    // Wait until a character is received
    while (!(UCSR0A & (1 << RXC0)))
    {
        // Wait
    }

    return UDR0;
}

// Function 5: Receive a complete command
void UART_receiveLine(char *buffer, int size)
{
    int i = 0;
    char data;

    while (1)
    {
        data = UART_receiveChar();

        // Enter or carriage return ends the command
        if (data == '\r' || data == '\n')
        {
            break;
        }

        // Store characters if space is available
        if (i < size - 1)
        {
            buffer[i] = data;
            i++;
        }
    }

    // End the string
    buffer[i] = '\0';
}

// Function 6: Initialize Timer0 PWM
void PWM_init(void)
{
    // Set PD6 (OC0A) as output
    DDRD |= (1 << DDD6);

    // Fast PWM, non-inverting mode
    TCCR0A = (1 << WGM00) |
             (1 << WGM01) |
             (1 << COM0A1);

    // Start Timer0 with prescaler 64
    TCCR0B = (1 << CS01) | (1 << CS00);

    // Initially turn LED OFF
    OCR0A = 0;
}

// Function 7: Update LED brightness
void setBrightness(int percentage)
{
    // Convert 0-100% into 0-255 PWM value
    OCR0A = (percentage * 255L) / 100;
}
