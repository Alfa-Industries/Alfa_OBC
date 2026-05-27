#include "alfa_sram.h"
#include "alfa_board.h"

#define SRAM_CMD_READ      0x03
#define SRAM_CMD_WRITE     0x02
#define SRAM_CMD_RDMR      0x05
#define SRAM_CMD_WRMR      0x01

#define SRAM_MODE_BYTE     0x00
#define SRAM_MODE_PAGE     0x80
#define SRAM_MODE_SEQ      0x40

static void SRAM_Select(void)
{
    HAL_GPIO_WritePin(nCS_RAM_GPIO_Port, nCS_RAM_Pin, GPIO_PIN_RESET);
}

static void SRAM_Deselect(void)
{
    HAL_GPIO_WritePin(nCS_RAM_GPIO_Port, nCS_RAM_Pin, GPIO_PIN_SET);
}

static uint8_t SRAM_WriteMode(uint8_t mode)
{
    uint8_t tx[2] = {SRAM_CMD_WRMR, mode};

    SRAM_Select();
    HAL_StatusTypeDef st = HAL_SPI_Transmit(&hspi1, tx, 2, HAL_MAX_DELAY);
    SRAM_Deselect();

    return (st == HAL_OK);
}

void Alfa_SRAM_Init(void)
{
    SRAM_Deselect();

    /*
     * Sequential mode permite leer/escribir buffers continuos.
     */
    SRAM_WriteMode(SRAM_MODE_SEQ);
}

uint8_t Alfa_SRAM_WriteByte(uint32_t addr, uint8_t data)
{
    return Alfa_SRAM_WriteBuffer(addr, &data, 1);
}

uint8_t Alfa_SRAM_ReadByte(uint32_t addr, uint8_t *data)
{
    return Alfa_SRAM_ReadBuffer(addr, data, 1);
}

uint8_t Alfa_SRAM_WriteBuffer(uint32_t addr, const uint8_t *data, size_t len)
{
    uint8_t cmd[4];

    if (data == 0 || len == 0)
    {
        return 0;
    }

    if ((addr + len) > ALFA_SRAM_SIZE_BYTES)
    {
        return 0;
    }

    cmd[0] = SRAM_CMD_WRITE;
    cmd[1] = (uint8_t)((addr >> 16) & 0xFF);
    cmd[2] = (uint8_t)((addr >> 8) & 0xFF);
    cmd[3] = (uint8_t)(addr & 0xFF);

    SRAM_Select();

    if (HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY) != HAL_OK)
    {
        SRAM_Deselect();
        return 0;
    }

    if (HAL_SPI_Transmit(&hspi1, (uint8_t *)data, len, HAL_MAX_DELAY) != HAL_OK)
    {
        SRAM_Deselect();
        return 0;
    }

    SRAM_Deselect();

    return 1;
}

uint8_t Alfa_SRAM_ReadBuffer(uint32_t addr, uint8_t *data, size_t len)
{
    uint8_t cmd[4];

    if (data == 0 || len == 0)
    {
        return 0;
    }

    if ((addr + len) > ALFA_SRAM_SIZE_BYTES)
    {
        return 0;
    }

    cmd[0] = SRAM_CMD_READ;
    cmd[1] = (uint8_t)((addr >> 16) & 0xFF);
    cmd[2] = (uint8_t)((addr >> 8) & 0xFF);
    cmd[3] = (uint8_t)(addr & 0xFF);

    SRAM_Select();

    if (HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY) != HAL_OK)
    {
        SRAM_Deselect();
        return 0;
    }

    if (HAL_SPI_Receive(&hspi1, data, len, HAL_MAX_DELAY) != HAL_OK)
    {
        SRAM_Deselect();
        return 0;
    }

    SRAM_Deselect();

    return 1;
}

uint8_t Alfa_SRAM_Test(void)
{
    uint8_t tx[8] = {0xA5, 0x5A, 0x11, 0x22, 0x33, 0x44, 0x55, 0x99};
    uint8_t rx[8] = {0};
    uint32_t test_addr = 0x000100;

    if (!Alfa_SRAM_WriteBuffer(test_addr, tx, sizeof(tx)))
    {
        return 0;
    }

    if (!Alfa_SRAM_ReadBuffer(test_addr, rx, sizeof(rx)))
    {
        return 0;
    }

    for (uint32_t i = 0; i < sizeof(tx); i++)
    {
        if (tx[i] != rx[i])
        {
            return 0;
        }
    }

    return 1;
}
