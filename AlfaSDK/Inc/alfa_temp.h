#ifndef ALFA_TEMP_H
#define ALFA_TEMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void Alfa_Temp_Init(void);

uint16_t Alfa_Temp_ReadRaw(void);

float Alfa_Temp_ReadVoltage(void);

float Alfa_Temp_ReadMCU(void);

uint8_t Alfa_Temp_IsSafe(float temp_c);

#ifdef __cplusplus
}
#endif

#endif /* ALFA_TEMP_H */
