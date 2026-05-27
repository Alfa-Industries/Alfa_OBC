#include "alfa_board.h"
#include "alfa_obc.h"
#include "app.h"

int main(void)
{
    Alfa_Board_Init();

    Alfa_Init();

    App_Setup();

    while (1)
    {
        App_Loop();
    }
}
