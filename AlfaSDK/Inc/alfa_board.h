#ifndef ALFA_BOARD_H
#define ALFA_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern IWDG_HandleTypeDef hiwdg;
extern RTC_HandleTypeDef hrtc;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;
extern UART_HandleTypeDef huart1;

void Alfa_Board_Init(void);
void SystemClock_Config(void);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* ALFA_BOARD_H */
