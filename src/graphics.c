#include "../include/graphics.h"

const char *green = "\033[0;32m";
const char *yellow = "\033[0;33m";
const char *red = "\033[0;31m";
const char *reset = "\033[0m";

void gx_printError(char *err)
{
    printf("%sERRO:%s%s\n", red, err, reset);
}

// void gx_mainMenu()
// {
//     printf("=========== TERMO COMBAT ===========\n\n");

//     printf("%d - Criar uma sala\n", CMD_HOST);
//     printf("%d - Entrar em uma sala\n", CMD_JOIN);
//     printf("%d - Como jogar\n\n", CMD_TUT);
//     printf("%d - Sair do jogo\n\n", CMD_EXIT);
// }

// void gx_exit()
// {
//     printf("\nObrigado por jogar. Encerrando...\n");
// }

// void gx_setWord(const char player)
// {
//     printf("> Players %c | Escolha a palavra de 5 letras para seu adversário adivinhar: ", player);
// }

// void gx_setWordError()
// {
//     printf("> Erro: A palavra deve ter exatamente 5 letras. Tente novamente.\n");
// }

// void gx_setGuess(const char player, const int try)
// {
//     printf("> Turno do Player %c | Tentativa %d: ", player, try);
// }

void gx_showGuessResult(const char *guess, const char *result)
{
    printf("Resultado: ");
    for (int i = 0; i < 5; i++)
    {
        switch (result[i])
        {
        case 'o':
            printf("%s%c%s", green, guess[i], reset);
            break;
        case '-':
            printf("%s%c%s", yellow, guess[i], reset);
            break;
        case 'x':
            printf("%s%c%s", red, guess[i], reset);
            break;
        default:
            printf("%c", guess[i]);
            break;
        }
    }
    printf("\n");
}

// void gx_startRound(const int roundCounter)
// {
//     printf("COMEÇANDO A RODADA %d\n", roundCounter);
// }

// void gx_endRound(const char player)
// {
//     printf("> Fim da rodada. O Player %c venceu!\n", player);
// }

// void gx_startMatch()
// {
//     printf("COMEÇANDO A PARTIDA\n");
// }

// void gx_endMatch(const char player)
// {
//     printf("PARTIDA FINALIZADA! O PLAYER %c VENCEU A MD3!\n", player);
//     printf("> Pressione qualquer tecla para voltar ao menu.\n");
//     getchar();
// }
// void gx_Score(const int p1Score, const int p2Score)
// {
//     printf("> Pontuação - Player 1: %d | Player 2: %d\n", p1Score, p2Score);
// }
// void gx_createdLobby()
// {
//     printf("> Sala criada com sucesso!\n");
// }