#ifndef ALFA_UART_H
#define ALFA_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void Alfa_UART_Init(void);

void Alfa_SerialPrint(const char *str);

void Alfa_SerialPrintf(const char *fmt, ...);

uint8_t Alfa_SerialRead(char *buffer, uint16_t max_len);

#ifdef __cplusplus
}
#endif

#endif /* ALFA_UART_H */
