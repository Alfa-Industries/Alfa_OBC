#include "app.h"

#include "alfa_uart.h"
#include "alfa_delay.h"

void App_Setup(void)
{
    Alfa_DelayMs(1000);

    Alfa_SerialPrintf("\r\n");
    Alfa_SerialPrintf("Alfa OBC VCP Ready\r\n");

}

void App_Loop(void)
{
    Alfa_SerialPrintf("USB CDC VCP heartbeat\r\n");
    Alfa_DelayMs(1000);
}
