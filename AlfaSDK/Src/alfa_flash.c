#include "alfa_flash.h"
#include "alfa_board.h"

#define AT45_CMD_READ_JEDEC_ID                 0x9F
#define AT45_CMD_READ_STATUS                   0xD7

#define AT45_CMD_CONTINUOUS_ARRAY_READ_LF      0x03

#define AT45_CMD_BUFFER1_WRITE                 0x84
#define AT45_CMD_BUFFER1_READ_LF               0xD1
#define AT45_CMD_MAIN_MEMORY_PAGE_TO_BUFFER1   0x53
#define AT45_CMD_BUFFER1_TO_MAIN_WITH_ERASE    0x83

#define AT45_CMD_PAGE_ERASE                    0x81

#define AT45_STATUS_READY_MASK                 0x80
#define AT45_STATUS_PAGE_SIZE_MASK             0x01

#define AT45_STANDARD_PAGE_SIZE                264U
#define AT45_BINARY_PAGE_SIZE                  256U

#define AT45_MAX_PAGE_SIZE                     264U

static void FLASH_Select(void)
{
    HAL_GPIO_WritePin(nCS_FLASH_GPIO_Port, nCS_FLASH_Pin, GPIO_PIN_RESET);
}

static void FLASH_Deselect(void)
{
    HAL_GPIO_WritePin(nCS_FLASH_GPIO_Port, nCS_FLASH_Pin, GPIO_PIN_SET);
}

static uint8_t FLASH_Tx(const uint8_t *data, uint16_t len)
{
    return (HAL_SPI_Transmit(&hspi3, (uint8_t *)data, len, HAL_MAX_DELAY) == HAL_OK);
}

static uint8_t FLASH_Rx(uint8_t *data, uint16_t len)
{
    return (HAL_SPI_Receive(&hspi3, data, len, HAL_MAX_DELAY) == HAL_OK);
}

static uint32_t FLASH_BuildAddress(uint32_t page, uint16_t offset)
{
    uint16_t page_size = Alfa_FLASH_GetPageSize();

    if (page_size == AT45_BINARY_PAGE_SIZE)
    {
        return ((page << 8) | offset);
    }

    return ((page << 9) | offset);
}

static void FLASH_AddressToBytes(uint32_t address, uint8_t out[3])
{
    out[0] = (uint8_t)((address >> 16) & 0xFF);
    out[1] = (uint8_t)((address >> 8) & 0xFF);
    out[2] = (uint8_t)(address & 0xFF);
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

    if (!FLASH_Tx(&cmd, 1))
    {
        FLASH_Deselect();
        return 0;
    }

    if (!FLASH_Rx(rx, 4))
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

    if (!FLASH_Tx(&cmd, 1))
    {
        FLASH_Deselect();
        return 0;
    }

    if (!FLASH_Rx(&rx, 1))
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

uint8_t Alfa_FLASH_WaitReady(uint32_t timeout_ms)
{
    uint32_t start = HAL_GetTick();

    while ((HAL_GetTick() - start) < timeout_ms)
    {
        if (Alfa_FLASH_IsReady())
        {
            return 1;
        }
    }

    return 0;
}

uint16_t Alfa_FLASH_GetPageSize(void)
{
    uint8_t status = 0;

    if (!Alfa_FLASH_ReadStatus(&status))
    {
        return AT45_STANDARD_PAGE_SIZE;
    }

    /*
     * Bit 0:
     * 0 = page size estándar DataFlash, 264 bytes
     * 1 = page size binario, 256 bytes
     */
    if (status & AT45_STATUS_PAGE_SIZE_MASK)
    {
        return AT45_BINARY_PAGE_SIZE;
    }

    return AT45_STANDARD_PAGE_SIZE;
}

uint32_t Alfa_FLASH_GetTotalSize(void)
{
    return ALFA_FLASH_TOTAL_SIZE_BYTES;
}

uint8_t Alfa_FLASH_ReadByte(uint32_t addr, uint8_t *data)
{
    return Alfa_FLASH_ReadBuffer(addr, data, 1);
}

uint8_t Alfa_FLASH_WriteByte(uint32_t addr, uint8_t data)
{
    return Alfa_FLASH_WriteBuffer(addr, &data, 1);
}

uint8_t Alfa_FLASH_ReadBuffer(uint32_t addr, uint8_t *data, size_t len)
{
    uint8_t cmd[4];

    if (data == 0 || len == 0)
    {
        return 0;
    }

    if ((addr + len) > ALFA_FLASH_TOTAL_SIZE_BYTES)
    {
        return 0;
    }

    if (!Alfa_FLASH_WaitReady(100))
    {
        return 0;
    }

    cmd[0] = AT45_CMD_CONTINUOUS_ARRAY_READ_LF;
    cmd[1] = (uint8_t)((addr >> 16) & 0xFF);
    cmd[2] = (uint8_t)((addr >> 8) & 0xFF);
    cmd[3] = (uint8_t)(addr & 0xFF);

    FLASH_Select();

    if (!FLASH_Tx(cmd, 4))
    {
        FLASH_Deselect();
        return 0;
    }

    if (!FLASH_Rx(data, (uint16_t)len))
    {
        FLASH_Deselect();
        return 0;
    }

    FLASH_Deselect();

    return 1;
}

static uint8_t FLASH_PageToBuffer1(uint32_t page)
{
    uint8_t cmd[4];
    uint8_t addr_bytes[3];
    uint32_t address = FLASH_BuildAddress(page, 0);

    FLASH_AddressToBytes(address, addr_bytes);

    cmd[0] = AT45_CMD_MAIN_MEMORY_PAGE_TO_BUFFER1;
    cmd[1] = addr_bytes[0];
    cmd[2] = addr_bytes[1];
    cmd[3] = addr_bytes[2];

    FLASH_Select();

    if (!FLASH_Tx(cmd, 4))
    {
        FLASH_Deselect();
        return 0;
    }

    FLASH_Deselect();

    return Alfa_FLASH_WaitReady(100);
}

static uint8_t FLASH_WriteBuffer1(uint16_t offset, const uint8_t *data, size_t len)
{
    uint8_t cmd[4];
    uint8_t addr_bytes[3];

    FLASH_AddressToBytes(offset, addr_bytes);

    cmd[0] = AT45_CMD_BUFFER1_WRITE;
    cmd[1] = addr_bytes[0];
    cmd[2] = addr_bytes[1];
    cmd[3] = addr_bytes[2];

    FLASH_Select();

    if (!FLASH_Tx(cmd, 4))
    {
        FLASH_Deselect();
        return 0;
    }

    if (!FLASH_Tx(data, (uint16_t)len))
    {
        FLASH_Deselect();
        return 0;
    }

    FLASH_Deselect();

    return 1;
}

static uint8_t FLASH_Buffer1ToPageWithErase(uint32_t page)
{
    uint8_t cmd[4];
    uint8_t addr_bytes[3];
    uint32_t address = FLASH_BuildAddress(page, 0);

    FLASH_AddressToBytes(address, addr_bytes);

    cmd[0] = AT45_CMD_BUFFER1_TO_MAIN_WITH_ERASE;
    cmd[1] = addr_bytes[0];
    cmd[2] = addr_bytes[1];
    cmd[3] = addr_bytes[2];

    FLASH_Select();

    if (!FLASH_Tx(cmd, 4))
    {
        FLASH_Deselect();
        return 0;
    }

    FLASH_Deselect();

    return Alfa_FLASH_WaitReady(100);
}

uint8_t Alfa_FLASH_PageErase(uint32_t page)
{
    uint8_t cmd[4];
    uint8_t addr_bytes[3];
    uint32_t address = FLASH_BuildAddress(page, 0);

    FLASH_AddressToBytes(address, addr_bytes);

    if (!Alfa_FLASH_WaitReady(100))
    {
        return 0;
    }

    cmd[0] = AT45_CMD_PAGE_ERASE;
    cmd[1] = addr_bytes[0];
    cmd[2] = addr_bytes[1];
    cmd[3] = addr_bytes[2];

    FLASH_Select();

    if (!FLASH_Tx(cmd, 4))
    {
        FLASH_Deselect();
        return 0;
    }

    FLASH_Deselect();

    return Alfa_FLASH_WaitReady(100);
}

uint8_t Alfa_FLASH_WriteBuffer(uint32_t addr, const uint8_t *data, size_t len)
{
    uint16_t page_size;
    uint32_t current_addr;
    size_t remaining;
    const uint8_t *current_data;

    if (data == 0 || len == 0)
    {
        return 0;
    }

    if ((addr + len) > ALFA_FLASH_TOTAL_SIZE_BYTES)
    {
        return 0;
    }

    page_size = Alfa_FLASH_GetPageSize();

    current_addr = addr;
    remaining = len;
    current_data = data;

    while (remaining > 0)
    {
        uint32_t page = current_addr / page_size;
        uint16_t offset = current_addr % page_size;
        uint16_t chunk = page_size - offset;

        if (chunk > remaining)
        {
            chunk = remaining;
        }

        /*
         * Algoritmo seguro de modificación parcial:
         * 1. Copiar página actual al buffer interno.
         * 2. Escribir datos nuevos sobre el buffer.
         * 3. Programar buffer hacia la página con borrado interno.
         */
        if (!FLASH_PageToBuffer1(page))
        {
            return 0;
        }

        if (!FLASH_WriteBuffer1(offset, current_data, chunk))
        {
            return 0;
        }

        if (!FLASH_Buffer1ToPageWithErase(page))
        {
            return 0;
        }

        current_addr += chunk;
        current_data += chunk;
        remaining -= chunk;
    }

    return 1;
}

uint8_t Alfa_FLASH_Test(void)
{
    Alfa_FLASH_JEDEC_ID_t id;
    uint8_t status = 0;
    uint8_t tx[8] = {0xA5, 0x5A, 0x11, 0x22, 0x33, 0x44, 0x55, 0x99};
    uint8_t rx[8] = {0};
    uint32_t test_addr = 0x000100;
    uint8_t ok = 1;

    if (!Alfa_FLASH_ReadJEDEC(&id))
    {
        return 0;
    }

    /*
     * AT45DB021E: fabricante esperado 0x1F.
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

    if (!Alfa_FLASH_WriteBuffer(test_addr, tx, sizeof(tx)))
    {
        return 0;
    }

    if (!Alfa_FLASH_ReadBuffer(test_addr, rx, sizeof(rx)))
    {
        return 0;
    }

    for (uint8_t i = 0; i < sizeof(tx); i++)
    {
        if (tx[i] != rx[i])
        {
            ok = 0;
        }
    }

    return ok;
}
