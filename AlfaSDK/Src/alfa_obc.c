#include "alfa_obc.h"

#include "alfa_rgb.h"
#include "alfa_rtc.h"
#include "alfa_sram.h"
#include "alfa_flash.h"
#include "alfa_uart.h"

void Alfa_Init(void)
{
    Alfa_RGB_Init();
    Alfa_RTC_Init();
    Alfa_SRAM_Init();
    Alfa_FLASH_Init();
    Alfa_UART_Init();
}
