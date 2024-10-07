#include "../include/graphics.h"

void gx_mainMenu()
{
    printf("=========== TERMO COMBAT ===========\n\n");

    printf("%d - Criar uma sala\n", CMD_HOST);
    printf("%d - Entrar em uma sala\n", CMD_JOIN);
    printf("%d - Como jogar\n\n", CMD_TUT);
    printf("%d - Sair do jogo\n\n", CMD_EXIT);
}

void gx_exit()
{
    print("\nObrigado por jogar. Encerrando...\n");
}