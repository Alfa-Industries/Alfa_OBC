#include "alfa_rgb.h"
#include "alfa_board.h"

void Alfa_RGB_Init(void)
{
    Alfa_RGB_Off();
}

void Alfa_RGB_SetGreen(void)
{
    Alfa_RGB_Off();

    HAL_GPIO_WritePin(red_GPIO_Port,
                      red_Pin,
                      GPIO_PIN_RESET);
}

void Alfa_RGB_SetRed(void)
{
    Alfa_RGB_Off();

    HAL_GPIO_WritePin(green_GPIO_Port,
                      green_Pin,
                      GPIO_PIN_RESET);
}

void Alfa_RGB_SetBlue(void)
{
    Alfa_RGB_Off();

    HAL_GPIO_WritePin(blue_GPIO_Port,
                      blue_Pin,
                      GPIO_PIN_RESET);
}

void Alfa_RGB_Off(void)
{
    HAL_GPIO_WritePin(GPIOC,
                      red_Pin |
                      green_Pin |
                      blue_Pin,
                      GPIO_PIN_SET);
}

void Alfa_RGB_ToggleRed(void)
{
    HAL_GPIO_TogglePin(red_GPIO_Port, red_Pin);
}

void Alfa_RGB_ToggleGreen(void)
{
    HAL_GPIO_TogglePin(green_GPIO_Port, green_Pin);
}

void Alfa_RGB_ToggleBlue(void)
{
    HAL_GPIO_TogglePin(blue_GPIO_Port, blue_Pin);
}
