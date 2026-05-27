#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

void Error_Handler(void);

#define nCS_FLASH_Pin GPIO_PIN_4
#define nCS_FLASH_GPIO_Port GPIOA
#define red_Pin GPIO_PIN_6
#define red_GPIO_Port GPIOC
#define blue_Pin GPIO_PIN_7
#define blue_GPIO_Port GPIOC
#define green_Pin GPIO_PIN_8
#define green_GPIO_Port GPIOC
#define nCS_RAM_Pin GPIO_PIN_15
#define nCS_RAM_GPIO_Port GPIOA
#define SCK_FLASH_Pin GPIO_PIN_10
#define SCK_FLASH_GPIO_Port GPIOC
#define MISO_FLASH_Pin GPIO_PIN_11
#define MISO_FLASH_GPIO_Port GPIOC
#define MOSI_FLASH_Pin GPIO_PIN_12
#define MOSI_FLASH_GPIO_Port GPIOC
#define SCK_SRAM_Pin GPIO_PIN_3
#define SCK_SRAM_GPIO_Port GPIOB
#define MISO_SRAM_Pin GPIO_PIN_4
#define MISO_SRAM_GPIO_Port GPIOB
#define MOSI_SRAM_Pin GPIO_PIN_5
#define MOSI_SRAM_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif
