#ifndef ALFA_SRAM_H
#define ALFA_SRAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define ALFA_SRAM_SIZE_BYTES   131072UL   // 23LC1024 = 128 KB

void Alfa_SRAM_Init(void);

uint8_t Alfa_SRAM_Test(void);

uint8_t Alfa_SRAM_WriteByte(uint32_t addr, uint8_t data);
uint8_t Alfa_SRAM_ReadByte(uint32_t addr, uint8_t *data);

uint8_t Alfa_SRAM_WriteBuffer(uint32_t addr, const uint8_t *data, size_t len);
uint8_t Alfa_SRAM_ReadBuffer(uint32_t addr, uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif
