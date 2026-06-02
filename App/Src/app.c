#include "app.h"

#include "alfa_uart.h"
#include "alfa_temp.h"
#include "alfa_delay.h"
#include "alfa_rgb.h"

void App_Setup(void)
{
    Alfa_SerialPrintf("\r\nAlfa OBC Temperature Test\r\n");
}

void App_Loop(void)
{
    uint16_t raw;
    float voltage;
    float temp;

    raw = Alfa_Temp_ReadRaw();
    voltage = Alfa_Temp_ReadVoltage();
    temp = Alfa_Temp_ReadMCU();

    Alfa_SerialPrintf(
        "RAW=%u VSENSE=%.3f V TEMP=%.2f C\r\n",
        raw,
        voltage,
        temp
    );

    if (Alfa_Temp_IsSafe(temp))
    {
        Alfa_RGB_SetGreen();
    }
    else
    {
        Alfa_RGB_SetRed();
    }

    Alfa_DelayMs(1000);
}
