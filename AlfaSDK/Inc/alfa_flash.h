#ifndef ALFA_FLASH_H
#define ALFA_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

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
uint8_t Alfa_FLASH_Test(void);

#ifdef __cplusplus
}
#endif

#endif
