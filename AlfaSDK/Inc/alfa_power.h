#ifndef ALFA_POWER_H
#define ALFA_POWER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void Alfa_Power_Init(void);

void Alfa_Power_NormalMode(void);

void Alfa_Power_SleepMode(void);

void Alfa_Power_SleepMs(uint32_t ms);

uint8_t Alfa_Power_StopForSeconds(uint32_t seconds);

uint8_t Alfa_Power_IsStopWakeup(void);

void Alfa_Power_ClearStopWakeupFlag(void);

#ifdef __cplusplus
}
#endif

#endif /* ALFA_POWER_H */
