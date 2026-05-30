#include "app.h"

#include "alfa_uart.h"
#include "alfa_flash.h"
#include "alfa_delay.h"
#include "alfa_rgb.h"

void App_Setup(void)
{
    Alfa_FLASH_JEDEC_ID_t id;

    uint8_t tx[8] = {0xA5, 0x5A, 0x11, 0x22, 0x33, 0x44, 0x55, 0x99};
    uint8_t rx[8] = {0};

    uint32_t addr = 0x000100;
    uint8_t ok = 1;

    Alfa_RGB_Off();

    Alfa_SerialPrintf("\r\n");
    Alfa_SerialPrintf("=== FLASH WRITE/READ TEST ===\r\n");

    if (!Alfa_FLASH_ReadJEDEC(&id))
    {
        Alfa_SerialPrintf("JEDEC READ FAIL\r\n");
        Alfa_RGB_SetRed();
        return;
    }

    Alfa_SerialPrintf(
        "JEDEC: MFG=0x%02X DEV1=0x%02X DEV2=0x%02X EXT=0x%02X\r\n",
        id.manufacturer_id,
        id.device_id_1,
        id.device_id_2,
        id.extended_info
    );

    Alfa_SerialPrintf("Writing %d bytes at 0x%06lX...\r\n", sizeof(tx), addr);

    if (!Alfa_FLASH_WriteBuffer(addr, tx, sizeof(tx)))
    {
        Alfa_SerialPrintf("FLASH WRITE FAIL\r\n");
        Alfa_RGB_SetRed();
        return;
    }

    Alfa_SerialPrintf("Reading back...\r\n");

    if (!Alfa_FLASH_ReadBuffer(addr, rx, sizeof(rx)))
    {
        Alfa_SerialPrintf("FLASH READ FAIL\r\n");
        Alfa_RGB_SetRed();
        return;
    }

    for (uint8_t i = 0; i < sizeof(tx); i++)
    {
        Alfa_SerialPrintf(
            "[%d] TX=0x%02X RX=0x%02X\r\n",
            i,
            tx[i],
            rx[i]
        );

        if (tx[i] != rx[i])
        {
            ok = 0;
        }
    }

    if (ok)
    {
        Alfa_SerialPrintf("FLASH WRITE/READ OK\r\n");
        Alfa_RGB_SetGreen();
    }
    else
    {
        Alfa_SerialPrintf("FLASH VERIFY FAIL\r\n");
        Alfa_RGB_SetRed();
    }
}

void App_Loop(void)
{
}
