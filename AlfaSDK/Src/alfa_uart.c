#include "alfa_uart.h"

#include "alfa_board.h"
#include "usbd_cdc_if.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define ALFA_PRINTF_BUFFER_SIZE      256
#define ALFA_USB_TX_TIMEOUT_MS       100
#define ALFA_PAYLOAD_LINE_TIMEOUT_MS 100

void Alfa_UART_Init(void)
{
}

/* ==============================
 * USB CDC Debug Console
 * ============================== */

static uint8_t Alfa_USB_Write(const uint8_t *data, uint16_t len)
{
    uint32_t start = HAL_GetTick();

    if (data == 0 || len == 0)
    {
        return 0;
    }

    while (CDC_Transmit_FS((uint8_t *)data, len) == USBD_BUSY)
    {
        if ((HAL_GetTick() - start) > ALFA_USB_TX_TIMEOUT_MS)
        {
            return 0;
        }
    }

    return 1;
}

void Alfa_SerialPrint(const char *str)
{
    if (str == 0)
    {
        return;
    }

    Alfa_USB_Write(
        (const uint8_t *)str,
        (uint16_t)strlen(str)
    );
}

void Alfa_SerialPrintf(const char *fmt, ...)
{
    char buffer[ALFA_PRINTF_BUFFER_SIZE];
    va_list args;

    if (fmt == 0)
    {
        return;
    }

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    Alfa_SerialPrint(buffer);
}

uint8_t Alfa_SerialRead(char *buffer, uint16_t max_len)
{
    if (buffer == 0 || max_len == 0)
    {
        return 0;
    }

    buffer[0] = '\0';

    /*
     * Pendiente:
     * Implementar RX buffer en usbd_cdc_if.c.
     */
    return 0;
}

/* ==============================
 * USART1 Payload Port
 * ============================== */

void Alfa_PayloadPrint(const char *str)
{
    if (str == 0)
    {
        return;
    }

    HAL_UART_Transmit(
        &huart1,
        (uint8_t *)str,
        (uint16_t)strlen(str),
        HAL_MAX_DELAY
    );
}

void Alfa_PayloadPrintf(const char *fmt, ...)
{
    char buffer[ALFA_PRINTF_BUFFER_SIZE];
    va_list args;

    if (fmt == 0)
    {
        return;
    }

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    Alfa_PayloadPrint(buffer);
}

uint8_t Alfa_PayloadSendBytes(const uint8_t *data, uint16_t len)
{
    if (data == 0 || len == 0)
    {
        return 0;
    }

    if (HAL_UART_Transmit(&huart1,
                          (uint8_t *)data,
                          len,
                          HAL_MAX_DELAY) != HAL_OK)
    {
        return 0;
    }

    return 1;
}

uint8_t Alfa_PayloadReadBytes(uint8_t *data, uint16_t len, uint32_t timeout_ms)
{
    if (data == 0 || len == 0)
    {
        return 0;
    }

    if (HAL_UART_Receive(&huart1,
                         data,
                         len,
                         timeout_ms) != HAL_OK)
    {
        return 0;
    }

    return 1;
}

uint8_t Alfa_PayloadRead(char *buffer, uint16_t max_len)
{
    uint16_t index = 0;
    uint8_t rx;

    if (buffer == 0 || max_len < 2)
    {
        return 0;
    }

    while (index < (max_len - 1))
    {
        if (HAL_UART_Receive(&huart1,
                             &rx,
                             1,
                             ALFA_PAYLOAD_LINE_TIMEOUT_MS) != HAL_OK)
        {
            break;
        }

        if (rx == '\r' || rx == '\n')
        {
            break;
        }

        buffer[index++] = (char)rx;
    }

    buffer[index] = '\0';

    return (index > 0);
}

/* ==============================
 * Simple Payload Protocol
 * ============================== */

uint8_t Alfa_PayloadPing(void)
{
    char rx[32];

    Alfa_PayloadPrint("$PING\n");

    if (!Alfa_PayloadRead(rx, sizeof(rx)))
    {
        return 0;
    }

    if (strcmp(rx, "$PONG") == 0)
    {
        return 1;
    }

    return 0;
}

uint8_t Alfa_PayloadRequestStatus(char *buffer, uint16_t max_len)
{
    if (buffer == 0 || max_len < 2)
    {
        return 0;
    }

    Alfa_PayloadPrint("$STATUS?\n");

    if (!Alfa_PayloadRead(buffer, max_len))
    {
        return 0;
    }

    return 1;
}
