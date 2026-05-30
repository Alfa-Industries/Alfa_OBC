#ifndef ALFA_FLASH_H
#define ALFA_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define ALFA_FLASH_TOTAL_SIZE_BYTES     (262144UL)   /* AT45DB021E = 2 Mbit = 256 KB */

typedef struct
{
    uint8_t manufacturer_id;
    uint8_t device_id_1;
    uint8_t device_id_2;
    uint8_t extended_info;
} Alfa_FLASH_JEDEC_ID_t;

void Alfa_FLASH_Init(void);

uint8_t Alfa_FLASH_ReadJEDEC(Alfa_FLASH_JEDEC_ID_t *id);
uint8_t Alfa_FLASH_ReadStatus(uint8_t *status);
uint8_t Alfa_FLASH_IsReady(void);
uint8_t Alfa_FLASH_WaitReady(uint32_t timeout_ms);

uint16_t Alfa_FLASH_GetPageSize(void);
uint32_t Alfa_FLASH_GetTotalSize(void);

uint8_t Alfa_FLASH_ReadByte(uint32_t addr, uint8_t *data);
uint8_t Alfa_FLASH_WriteByte(uint32_t addr, uint8_t data);

uint8_t Alfa_FLASH_ReadBuffer(uint32_t addr, uint8_t *data, size_t len);
uint8_t Alfa_FLASH_WriteBuffer(uint32_t addr, const uint8_t *data, size_t len);

uint8_t Alfa_FLASH_PageErase(uint32_t page);
uint8_t Alfa_FLASH_Test(void);

#ifdef __cplusplus
}
#endif

#endif /* ALFA_FLASH_H */
