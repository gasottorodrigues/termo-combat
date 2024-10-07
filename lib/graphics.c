#include "../include/graphics.h"

void showMainMenu()
{
    printf("=========== TERMO COMBAT ===========");

    printf("\n %d - Criar uma sala\n", CMD_HOST);
    printf("\n %d - Entrar em uma sala\n", CMD_JOIN);
    printf("\n %d - Como jogar\n", CMD_TUT);
    printf("\n\n %d - Sair do jogo\n\n", CMD_EXIT);
}