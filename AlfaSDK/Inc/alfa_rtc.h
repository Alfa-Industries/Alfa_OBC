#ifndef ALFA_RTC_H
#define ALFA_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdint.h>

#define ALFA_RTC_BKP_MAGIC_VALUE   0xA15A

void Alfa_RTC_Init(void);

/*
 * Configuración y estado
 */
uint8_t Alfa_RTC_IsConfigured(void);

uint8_t Alfa_RTC_SetDefaultIfNeeded(void);

uint8_t Alfa_RTC_SetDateTime(uint8_t hours,
                             uint8_t minutes,
                             uint8_t seconds,
                             uint8_t weekday,
                             uint8_t month,
                             uint8_t date,
                             uint8_t year);

/*
 * Lectura RTC
 */
void Alfa_RTC_GetTime(RTC_TimeTypeDef *time);
void Alfa_RTC_GetDate(RTC_DateTypeDef *date);

/*
 * Utilidades
 */
uint32_t Alfa_RTC_GetBackupRegister(void);

#ifdef __cplusplus
}
#endif

#endif
