#include "alfa_flash.h"
#include "alfa_board.h"

#define AT45_CMD_READ_JEDEC_ID     0x9F
#define AT45_CMD_READ_STATUS       0xD7

#define AT45_STATUS_READY_MASK     0x80

static void FLASH_Select(void)
{
    HAL_GPIO_WritePin(nCS_FLASH_GPIO_Port, nCS_FLASH_Pin, GPIO_PIN_RESET);
}

static void FLASH_Deselect(void)
{
    HAL_GPIO_WritePin(nCS_FLASH_GPIO_Port, nCS_FLASH_Pin, GPIO_PIN_SET);
}

void Alfa_FLASH_Init(void)
{
    FLASH_Deselect();
}

uint8_t Alfa_FLASH_ReadJEDEC(Alfa_FLASH_JEDEC_ID_t *id)
{
    uint8_t cmd = AT45_CMD_READ_JEDEC_ID;
    uint8_t rx[4] = {0};

    if (id == 0)
    {
        return 0;
    }

    FLASH_Select();

    if (HAL_SPI_Transmit(&hspi3, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        FLASH_Deselect();
        return 0;
    }

    if (HAL_SPI_Receive(&hspi3, rx, 4, HAL_MAX_DELAY) != HAL_OK)
    {
        FLASH_Deselect();
        return 0;
    }

    FLASH_Deselect();

    id->manufacturer_id = rx[0];
    id->device_id_1 = rx[1];
    id->device_id_2 = rx[2];
    id->extended_info = rx[3];

    return 1;
}

uint8_t Alfa_FLASH_ReadStatus(uint8_t *status)
{
    uint8_t cmd = AT45_CMD_READ_STATUS;
    uint8_t rx = 0;

    if (status == 0)
    {
        return 0;
    }

    FLASH_Select();

    if (HAL_SPI_Transmit(&hspi3, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        FLASH_Deselect();
        return 0;
    }

    if (HAL_SPI_Receive(&hspi3, &rx, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        FLASH_Deselect();
        return 0;
    }

    FLASH_Deselect();

    *status = rx;

    return 1;
}

uint8_t Alfa_FLASH_IsReady(void)
{
    uint8_t status = 0;

    if (!Alfa_FLASH_ReadStatus(&status))
    {
        return 0;
    }

    return ((status & AT45_STATUS_READY_MASK) != 0);
}

uint8_t Alfa_FLASH_Test(void)
{
    Alfa_FLASH_JEDEC_ID_t id;
    uint8_t status = 0;

    if (!Alfa_FLASH_ReadJEDEC(&id))
    {
        return 0;
    }

    /*
     * AT45DB021E normalmente reporta fabricante 0x1F.
     * No forzamos todavía todos los bytes para mantener compatibilidad.
     */
    if (id.manufacturer_id != 0x1F)
    {
        return 0;
    }

    if (!Alfa_FLASH_ReadStatus(&status))
    {
        return 0;
    }

    if ((status & AT45_STATUS_READY_MASK) == 0)
    {
        return 0;
    }

    return 1;
}
