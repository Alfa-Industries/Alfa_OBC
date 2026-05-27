#include "alfa_rtc.h"
#include "alfa_board.h"

#define ALFA_RTC_BKP_MAGIC_REG   RTC_BKP_DR1

void Alfa_RTC_Init(void)
{
    /*
     * El RTC ya fue inicializado por Alfa_Board_Init().
     *
     * Aquí solo validamos si ya estaba configurado.
     */
}

uint8_t Alfa_RTC_IsConfigured(void)
{
    uint32_t value;

    value = HAL_RTCEx_BKUPRead(&hrtc,
                               ALFA_RTC_BKP_MAGIC_REG);

    return (value == ALFA_RTC_BKP_MAGIC_VALUE);
}

uint8_t Alfa_RTC_SetDateTime(uint8_t hours,
                             uint8_t minutes,
                             uint8_t seconds,
                             uint8_t weekday,
                             uint8_t month,
                             uint8_t date,
                             uint8_t year)
{
    RTC_TimeTypeDef rtc_time = {0};
    RTC_DateTypeDef rtc_date = {0};

    rtc_time.Hours = hours;
    rtc_time.Minutes = minutes;
    rtc_time.Seconds = seconds;

    rtc_date.WeekDay = weekday;
    rtc_date.Month = month;
    rtc_date.Date = date;
    rtc_date.Year = year;

    if (HAL_RTC_SetTime(&hrtc,
                        &rtc_time,
                        RTC_FORMAT_BIN) != HAL_OK)
    {
        return 0;
    }

    if (HAL_RTC_SetDate(&hrtc,
                        &rtc_date,
                        RTC_FORMAT_BIN) != HAL_OK)
    {
        return 0;
    }

    /*
     * Guardamos marca para indicar
     * que el RTC ya fue configurado.
     */
    HAL_RTCEx_BKUPWrite(&hrtc,
                        ALFA_RTC_BKP_MAGIC_REG,
                        ALFA_RTC_BKP_MAGIC_VALUE);

    return 1;
}

uint8_t Alfa_RTC_SetDefaultIfNeeded(void)
{
    if (Alfa_RTC_IsConfigured())
    {
        return 1;
    }

    /*
     * Fecha default:
     * 01/01/2026 12:00:00
     */
    return Alfa_RTC_SetDateTime(
                12,                     /* hours */
                0,                      /* minutes */
                0,                      /* seconds */
                RTC_WEEKDAY_MONDAY,
                RTC_MONTH_JANUARY,
                1,                      /* date */
                26                      /* year */
           );
}

void Alfa_RTC_GetTime(RTC_TimeTypeDef *time)
{
    if (time == 0)
    {
        return;
    }

    HAL_RTC_GetTime(&hrtc,
                    time,
                    RTC_FORMAT_BIN);
}

void Alfa_RTC_GetDate(RTC_DateTypeDef *date)
{
    if (date == 0)
    {
        return;
    }

    /*
     * IMPORTANTE:
     * En STM32F1 se recomienda leer
     * la fecha después de la hora
     * para desbloquear shadow registers.
     */
    HAL_RTC_GetDate(&hrtc,
                    date,
                    RTC_FORMAT_BIN);
}

uint32_t Alfa_RTC_GetBackupRegister(void)
{
    return HAL_RTCEx_BKUPRead(&hrtc,
                              ALFA_RTC_BKP_MAGIC_REG);
}
