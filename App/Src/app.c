#include "app.h"

#include "alfa_uart.h"
#include "alfa_rtc.h"
#include "alfa_sram.h"
#include "alfa_flash.h"
#include "alfa_delay.h"

RTC_TimeTypeDef time;
RTC_DateTypeDef date;

void App_Setup(void)
{
    Alfa_SerialPrintf(
            "Alfa OBC SDK v1.0\r\n"
        );

    Alfa_RTC_GetTime(&time);
    Alfa_RTC_GetDate(&date);

    Alfa_SerialPrintf(
        "%02d:%02d:%02d\r\n",
        time.Hours,
        time.Minutes,
        time.Seconds
    );

    if (Alfa_SRAM_Test())
    {
        Alfa_SerialPrintf(
            "SRAM OK\r\n"
        );
    }
    else
    {
        Alfa_SerialPrintf(
            "SRAM FAIL\r\n"
        );
    }

    Alfa_FLASH_JEDEC_ID_t id;

    if (Alfa_FLASH_ReadJEDEC(&id))
    {
        Alfa_SerialPrintf(
            "FLASH MFG=0x%02X DEV1=0x%02X DEV2=0x%02X\r\n",
            id.manufacturer_id,
            id.device_id_1,
            id.device_id_2
        );
    }
}

void App_Loop(void)
{
}
