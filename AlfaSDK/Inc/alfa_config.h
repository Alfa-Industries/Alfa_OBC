#ifndef ALFA_CONFIG_H
#define ALFA_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ==============================
 * AlfaSDK Feature Configuration
 * ==============================
 */

/*
 * Independent Watchdog
 *
 * 0 = Disabled
 * 1 = Enabled
 */
#define ALFA_USE_IWDG                 0

/*
 * Stop Mode Support
 *
 * 0 = Disabled
 * 1 = Enabled
 */
#define ALFA_USE_STOP_MODE            1

/*
 * IWDG timeout used by the current CubeMX configuration:
 *
 * Prescaler = 256
 * Reload    = 4095
 * LSI       ≈ 40 kHz
 *
 * Timeout ≈ 26 seconds
 */
#define ALFA_IWDG_TIMEOUT_SEC         26
#define ALFA_STOP_SAFE_MARGIN_SEC     2

#if ALFA_USE_IWDG
#define ALFA_STOP_MAX_SEC             (ALFA_IWDG_TIMEOUT_SEC - ALFA_STOP_SAFE_MARGIN_SEC)
#else
#define ALFA_STOP_MAX_SEC             3600
#endif

/*
 * Safe predefined stop intervals
 */
#define STOP_MODE_SEC_5               5
#define STOP_MODE_SEC_10              10
#define STOP_MODE_SEC_15              15
#define STOP_MODE_SEC_20              20

#if !ALFA_USE_IWDG
#define STOP_MODE_SEC_25              25
#define STOP_MODE_SEC_40              40
#define STOP_MODE_SEC_60              60
#endif

#ifdef __cplusplus
}
#endif

#endif /* ALFA_CONFIG_H */
