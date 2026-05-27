#include "alfa_delay.h"

#include "stm32f1xx_hal.h"

void Alfa_DelayMs(uint32_t ms)
{
    HAL_Delay(ms);
}
