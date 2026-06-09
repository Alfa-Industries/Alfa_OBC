#ifndef ALFA_UART_H
#define ALFA_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void Alfa_UART_Init(void);

/* USB CDC Debug Console */
void Alfa_SerialPrint(const char *str);
void Alfa_SerialPrintf(const char *fmt, ...);
uint8_t Alfa_SerialRead(char *buffer, uint16_t max_len);

/* USART1 Payload Port */
void Alfa_PayloadPrint(const char *str);
void Alfa_PayloadPrintf(const char *fmt, ...);

uint8_t Alfa_PayloadRead(char *buffer, uint16_t max_len);
uint8_t Alfa_PayloadSendBytes(const uint8_t *data, uint16_t len);
uint8_t Alfa_PayloadReadBytes(uint8_t *data, uint16_t len, uint32_t timeout_ms);

uint8_t Alfa_PayloadPing(void);
uint8_t Alfa_PayloadRequestStatus(char *buffer, uint16_t max_len);

#ifdef __cplusplus
}
#endif

#endif /* ALFA_UART_H */
