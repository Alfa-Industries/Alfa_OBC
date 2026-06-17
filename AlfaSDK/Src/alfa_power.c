#include "alfa_power.h"

#include "alfa_config.h"
#include "alfa_board.h"

static volatile uint8_t alfa_stop_wakeup_flag = 0;

void Alfa_Power_Init(void)
{
    alfa_stop_wakeup_flag = 0;
}

void Alfa_Power_NormalMode(void)
{
    /*
     * For Sleep Mode, the MCU returns automatically to normal execution.
     * For Stop Mode, SystemClock_Config() is called after wakeup.
     */
}

void Alfa_Power_SleepMode(void)
{
#if ALFA_USE_IWDG
    HAL_IWDG_Refresh(&hiwdg);
#endif

    HAL_PWR_EnterSLEEPMode(
        PWR_MAINREGULATOR_ON,
        PWR_SLEEPENTRY_WFI
    );

#if ALFA_USE_IWDG
    HAL_IWDG_Refresh(&hiwdg);
#endif
}

void Alfa_Power_SleepMs(uint32_t ms)
{
    uint32_t start = HAL_GetTick();

    while ((HAL_GetTick() - start) < ms)
    {
        Alfa_Power_SleepMode();
    }
}



uint8_t Alfa_Power_IsStopWakeup(void)
{
    return alfa_stop_wakeup_flag;
}

void Alfa_Power_ClearStopWakeupFlag(void)
{
    alfa_stop_wakeup_flag = 0;
}

uint8_t Alfa_Power_StopForSeconds(uint32_t seconds)
{
#if !ALFA_USE_STOP_MODE

    (void)seconds;
    return 0;

#else

    RTC_TimeTypeDef current_time = {0};
    RTC_DateTypeDef current_date = {0};
    RTC_AlarmTypeDef alarm = {0};

    uint32_t total_seconds;
    uint32_t alarm_seconds;

    if (seconds == 0)
    {
        return 0;
    }

    if (seconds > ALFA_STOP_MAX_SEC)
    {
        return 0;
    }

#if ALFA_USE_IWDG
    HAL_IWDG_Refresh(&hiwdg);
#endif

    alfa_stop_wakeup_flag = 0;

    HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BIN);

    total_seconds =
        ((uint32_t)current_time.Hours * 3600UL) +
        ((uint32_t)current_time.Minutes * 60UL) +
        ((uint32_t)current_time.Seconds);

    alarm_seconds = (total_seconds + seconds) % 86400UL;

    alarm.AlarmTime.Hours = alarm_seconds / 3600UL;
    alarm.AlarmTime.Minutes = (alarm_seconds % 3600UL) / 60UL;
    alarm.AlarmTime.Seconds = alarm_seconds % 60UL;
    alarm.Alarm = RTC_ALARM_A;

    HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

    if (HAL_RTC_SetAlarm_IT(&hrtc, &alarm, RTC_FORMAT_BIN) != HAL_OK)
    {
        return 0;
    }

    HAL_SuspendTick();

    HAL_PWR_EnterSTOPMode(
        PWR_LOWPOWERREGULATOR_ON,
        PWR_STOPENTRY_WFI
    );

    SystemClock_Config();

    HAL_ResumeTick();

#if ALFA_USE_IWDG
    HAL_IWDG_Refresh(&hiwdg);
#endif

    HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

    return 1;

#endif
}
/*
 * This callback is called by HAL_RTC_AlarmIRQHandler()
 * from RTC_Alarm_IRQHandler().
 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc_ptr)
{
    (void)hrtc_ptr;

    alfa_stop_wakeup_flag = 1;
}
