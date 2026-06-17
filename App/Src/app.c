#include "app.h"

#include "alfa_uart.h"
#include "alfa_delay.h"

void App_Setup(void)
{
    Alfa_DelayMs(1000);

    Alfa_SerialPrint(
        "Hola Mundo\r\n Alfa OBC!!"
    );
}

void App_Loop(void)
{
}
