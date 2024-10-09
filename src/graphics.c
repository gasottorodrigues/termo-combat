#include "../include/graphics.h"

const char *green = "\033[0;32m";
const char *yellow = "\033[0;33m";
const char *red = "\033[0;31m";
const char *reset = "\033[0m";

/**
 * @brief Exibe uma mensagem de erro formatada em vermelho.
 *
 * @param err String contendo a mensagem de erro a ser exibida.
 */
void gx_printError(char *err)
{
    printf("%sERRO:%s%s\n", red, err, reset);
}

/**
 * @brief Exibe o menu principal do jogo, com opções para criar sala, entrar em sala ou sair do jogo.
 */
void gx_mainMenu()
{
    system("clear");
    printf("=========== TERMO COMBAT ===========\n\n");

    printf("%d - Criar uma sala\n", CMD_HOST);
    printf("%d - Entrar em uma sala\n", CMD_JOIN);
    printf("%d - Sair do jogo\n\n", CMD_EXIT);

    printf("Acesse a opcao desejada: ");
}

/**
 * @brief Exibe a interface para criar uma sala (lobby) e coleta as configurações da sala.
 *
 * @param s Ponteiro para armazenar a pontuação necessária para ganhar.
 * @param w Ponteiro para armazenar o tamanho das palavras.
 * @param p Ponteiro para armazenar o número de jogadores.
 * @param t Ponteiro para armazenar o número de tentativas para adivinhar.
 */
void gx_createLobby(int *s, int *w, int *p, int *t)
{
    system("clear");
    printf("=========== TERMO COMBAT ===========\n\n");
    printf("Criando uma sala\n");
    printf(">Defina a pontuacao para ganhar: ");
    scanf("%d", s);
    printf(">Defina o tamanho das palavras: ");
    scanf("%d", w);
    printf(">Defina o numero de tentativas para adivinhar: ");
    scanf("%d", t);
    printf(">Defina o numero de jogadores: ");
    scanf("%d", p);
}

/**
 * @brief Exibe a interface de início de rodada, mostrando a pontuação atual de cada jogador.
 *
 * @param points Array contendo as pontuações dos jogadores.
 * @param nPlayers Número total de jogadores.
 * @param playerId ID do jogador atual.
 */
void gx_startRound(int *points, int const nPlayers, int playerId)
{
    system("clear");
    printf("=========== TERMO COMBAT ===========\n\n");
    printf("Voce e o jogador %d\n", playerId);
    printf("Pontuacao:\n");

    for (int i = 0; i < nPlayers; i++)
    {
        printf("P%d:%d\t", i + 1, points[i]);
    }
    printf("\n");
}

/**
 * @brief Exibe a pontuação de todos os jogadores no final de uma rodada.
 *
 * @param points Array contendo as pontuações dos jogadores.
 * @param nPlayers Número total de jogadores.
 */
void gx_showPoints(int *points, int const nPlayers)
{
    system("clear");
    printf("Pontuacao no fim da Rodada:\n");

    for (int i = 0; i < nPlayers; i++)
    {
        printf("P%d:%d\t", i + 1, points[i + 1]);
    }
    printf("\n");
}

/**
 * @brief Exibe o resultado da tentativa de adivinhar uma palavra, com cores indicando o acerto.
 *
 * @param guess String contendo a palavra adivinhada.
 * @param result String contendo o resultado da adivinhação ('o' para correto, '-' para posição incorreta, 'x' para incorreto).
 */
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
