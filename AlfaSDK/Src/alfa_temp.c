#include "alfa_temp.h"

#include "alfa_board.h"

/*
 * STM32F1 typical values.
 * These values are approximate and may vary between MCUs.
 */
#define ALFA_ADC_RESOLUTION_COUNTS      4095.0f
#define ALFA_ADC_VDDA                   3.3f

#define ALFA_TEMP_V25                   1.43f
#define ALFA_TEMP_AVG_SLOPE             0.0043f

#define ALFA_TEMP_SAFE_MIN_C            -20.0f
#define ALFA_TEMP_SAFE_MAX_C             85.0f

void Alfa_Temp_Init(void)
{
    /*
     * ADC1 ya fue inicializado desde Alfa_Board_Init().
     */
}

uint16_t Alfa_Temp_ReadRaw(void)
{
    uint32_t sum = 0;

    for(uint8_t i=0;i<32;i++)
    {
        HAL_ADC_Start(&hadc1);

        HAL_ADC_PollForConversion(
            &hadc1,
            HAL_MAX_DELAY
        );

        sum += HAL_ADC_GetValue(&hadc1);

        HAL_ADC_Stop(&hadc1);
    }

    return (uint16_t)(sum / 32);
}

float Alfa_Temp_ReadVoltage(void)
{
    uint16_t raw = Alfa_Temp_ReadRaw();

    return ((float)raw * ALFA_ADC_VDDA) / ALFA_ADC_RESOLUTION_COUNTS;
}

float Alfa_Temp_ReadMCU(void)
{
    float voltage = Alfa_Temp_ReadVoltage();

    /*
     * Formula:
     * Temperature = ((V25 - Vsense) / AvgSlope) + 25
     */
    return ((ALFA_TEMP_V25 - voltage) / ALFA_TEMP_AVG_SLOPE) + 25.0f;
}

uint8_t Alfa_Temp_IsSafe(float temp_c)
{
    if (temp_c < ALFA_TEMP_SAFE_MIN_C)
    {
        return 0;
    }

    if (temp_c > ALFA_TEMP_SAFE_MAX_C)
    {
        return 0;
    }

    return 1;
}
