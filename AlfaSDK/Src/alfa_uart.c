#include "alfa_uart.h"

#include "alfa_board.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define ALFA_UART_PRINTF_BUFFER_SIZE 256

void Alfa_UART_Init(void)
{
    /*
     * USART1 ya fue inicializado
     * desde Alfa_Board_Init().
     */
}

void Alfa_SerialPrint(const char *str)
{
    if (str == NULL)
    {
        return;
    }

    HAL_UART_Transmit(
        &huart1,
        (uint8_t *)str,
        strlen(str),
        HAL_MAX_DELAY
    );
}

void Alfa_SerialPrintf(const char *fmt, ...)
{
    char buffer[ALFA_UART_PRINTF_BUFFER_SIZE];

    va_list args;

    va_start(args, fmt);

    vsnprintf(
        buffer,
        sizeof(buffer),
        fmt,
        args
    );

    va_end(args);

    Alfa_SerialPrint(buffer);
}

uint8_t Alfa_SerialRead(char *buffer, uint16_t max_len)
{
    uint16_t index = 0;
    uint8_t rx;

    if (buffer == NULL)
    {
        return 0;
    }

    if (max_len < 2)
    {
        return 0;
    }

    while (index < (max_len - 1))
    {
        if (HAL_UART_Receive(
                &huart1,
                &rx,
                1,
                10) != HAL_OK)
        {
            break;
        }

        if (rx == '\r' || rx == '\n')
        {
            break;
        }

        buffer[index++] = rx;
    }

    buffer[index] = '\0';

    return (index > 0);
}
