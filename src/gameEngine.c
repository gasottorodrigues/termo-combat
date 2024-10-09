#include "../include/gameEngine.h"

/**
 * @brief Configura o estado inicial do jogo.
 *
 * Aloca e inicializa a estrutura de estado do jogo com os parâmetros fornecidos, como a pontuação necessária para vencer,
 * o tamanho da palavra, o número de jogadores e o número de tentativas por rodada.
 *
 * @param scoreToWin Pontuação necessária para vencer o jogo.
 * @param wordSize Tamanho da palavra a ser adivinhada.
 * @param nPlayers Número de jogadores na partida.
 * @param triesPerRound Número de tentativas permitidas por rodada.
 * @return Ponteiro para a estrutura de estado do jogo inicializada.
 */
gameStateType *game_setupGameState(int scoreToWin, int wordSize, int nPlayers, int triesPerRound)
{
    gameStateType *newGame = (gameStateType *)malloc(sizeof(gameStateType));

    newGame->scoreToWin = scoreToWin;
    newGame->wordSize = wordSize;
    newGame->nPlayers = nPlayers;
    newGame->triesPerRound = triesPerRound;

    newGame->isMatchRunning = false;
    newGame->isRoundRunnig = false;
    newGame->isWordSet = false;
    newGame->isInScoreCalc = true;
    newGame->currRound = 0;
    newGame->currWriterId = 1;

    newGame->playersScore = (int *)calloc(nPlayers, sizeof(int));
    newGame->word = (char *)malloc((wordSize + 1) * sizeof(char));

    for (int i = 0; i < wordSize; i++)
    {
        newGame->word[i] = '-';
    }
    newGame->word[wordSize] = '\0';

    return newGame;
}

/**
 * @brief Libera a memória associada ao estado do jogo.
 *
 * Libera a memória alocada para o estado do jogo, incluindo a pontuação dos jogadores e a palavra.
 *
 * @param state Ponteiro para a estrutura de estado do jogo a ser liberada.
 */
void game_freeGameState(gameStateType *state)
{
    if (state != NULL)
    {
        free(state->playersScore);
        free(state->word);

        free(state);
    }
}
/**
 * @brief Permite ao jogador escolher uma palavra para a rodada.
 *
 * Solicita ao jogador que escolha uma palavra de um tamanho específico. A palavra é convertida para letras maiúsculas
 * e armazenada no estado do jogo.
 *
 * @param state Ponteiro para a estrutura de estado do jogo.
 */
void game_chooseWord(gameStateType *state)
{
    char input[state->wordSize + 1];
    bool valid = false;

    while (!valid)
    {
        printf(">Escolha uma palavra de %d letras: ", state->wordSize);

        // Limitar o número de caracteres lidos para evitar buffer overflow
        scanf("%s", input);
        getchar(); // Consumir o caractere de nova linha deixado pelo scanf

        if (strlen(input) != state->wordSize)
        {
            printf("Sua palavra está errada. Tente novamente.\n");
        }
        else
        {
            valid = true;
        }
    }

    // Converter a palavra para letras maiúsculas
    for (int i = 0; i < state->wordSize; i++)
    {
        input[i] = toupper(input[i]);
    }

    strncpy(state->word, input, state->wordSize);
    state->word[state->wordSize] = '\0';
}

/**
 * @brief Valida a tentativa de adivinhação de um jogador.
 *
 * Compara a palavra adivinhada com a palavra correta e retorna um resultado indicando se a adivinhação foi correta,
 * além de fornecer um feedback visual sobre as letras corretas e suas posições.
 *
 * @param word Palavra correta.
 * @param guess Palavra adivinhada pelo jogador.
 * @return Estrutura contendo o resultado da adivinhação e um feedback visual.
 */
guessResult game_validateGuess(char *word, char *guess)
{
    guessResult retval;
    int wordLen = strlen(word);

    // Aloca memória para o visualRes, incluindo o caractere nulo no final
    retval.visualRes = (char *)malloc((wordLen + 1) * sizeof(char));

    // Inicializa o resultado como verdadeiro, mas será ajustado se houver diferenças
    retval.result = true;

    // Arrays para marcar letras já usadas
    bool *usedInWord = (bool *)malloc((wordLen + 1) * sizeof(bool));
    ;                                                                 // Para marcar letras já usadas em 'word'
    bool *usedInGuess = (bool *)malloc((wordLen + 1) * sizeof(bool)); // Para marcar letras já usadas em 'guess'

    // Primeira passagem: verifica as correspondências exatas (letras na posição correta)
    for (int i = 0; i < wordLen; i++)
    {
        if (guess[i] == word[i])
        {
            retval.visualRes[i] = 'o'; // Letra correta na posição correta
            usedInWord[i] = true;      // Marca a letra como usada em 'word'
            usedInGuess[i] = true;     // Marca a letra como usada em 'guess'
        }
        else
        {
            retval.result = false; // Se houver qualquer diferença, o resultado é falso
        }
    }

    // Segunda passagem: verifica as correspondências parciais (letras corretas na posição errada)
    for (int i = 0; i < wordLen; i++)
    {
        if (!usedInGuess[i]) // Se a letra ainda não foi usada
        {
            bool found = false;
            for (int j = 0; j < wordLen; j++)
            {
                if (!usedInWord[j] && guess[i] == word[j])
                {
                    retval.visualRes[i] = '-'; // Letra correta na posição errada
                    usedInWord[j] = true;      // Marca a letra como usada em 'word'
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                retval.visualRes[i] = 'x'; // Letra não encontrada em 'word'
            }
        }
    }

    free(usedInGuess);
    free(usedInWord);

    // Adiciona o caractere nulo ao final da string visualRes
    retval.visualRes[wordLen] = '\0';

    return retval;
}
/**
 * @brief Gerencia as tentativas de adivinhação de um jogador.
 *
 * Permite que o jogador faça várias tentativas de adivinhar a palavra correta, até que ele acerte ou esgote o número
 * máximo de tentativas. Exibe o resultado de cada tentativa.
 *
 * @param state Ponteiro para a estrutura de estado do jogo.
 * @return Estrutura contendo o número de tentativas e se a adivinhação foi correta.
 */
playerGuessesResultType game_tryGuesses(gameStateType *state)
{
    int maxTries = state->triesPerRound;
    playerGuessesResultType res;
    res.numOfTries = 0;
    res.correctAns = false;

    char input[state->wordSize];

    while (!res.correctAns && res.numOfTries < maxTries)
    {
        res.numOfTries++;

        bool valid = false;
        while (!valid)
        {
            printf(">Tentativa %d! Tente adivinha a palavra: ", res.numOfTries);
            scanf("%5s", input);
            getchar();

            if (strlen(input) != state->wordSize)
            {
                printf("Tamanho errado. A palavra tem %d letras.\n", state->wordSize);
            }
            else
            {
                valid = true;
            }
        }

        guessResult guessResult = game_validateGuess(state->word, input);
        res.correctAns = guessResult.result;
        gx_showGuessResult(input, guessResult.visualRes);
        free(guessResult.visualRes);
    }

    return res;
}
/**
 * @brief Calcula os pontos de um jogador com base nas tentativas de adivinhação.
 *
 * Atualiza a pontuação de um jogador com base no número de tentativas feitas e se a adivinhação foi correta.
 *
 * @param state Ponteiro para a estrutura de estado do jogo.
 * @param playerStateId ID do jogador.
 * @param numOfTries Número de tentativas feitas pelo jogador.
 * @param correctAns Indica se a adivinhação foi correta.
 */
void game_calculateRoundPoints(gameStateType *state, int playerStateId, int numOfTries, bool correctAns)
{
    int totalRoundPoints = correctAns ? (state->triesPerRound - numOfTries) + 1 : 0;
    state->playersScore[playerStateId] += totalRoundPoints;
}
/**
 * @brief Calcula os pontos do jogador que escolheu a palavra.
 *
 * Atualiza a pontuação do jogador que escolheu a palavra com base no número de tentativas corretas e erradas feitas pelos outros jogadores.
 *
 * @param state Ponteiro para a estrutura de estado do jogo.
 * @param numOfCorrectTries Número de tentativas corretas feitas pelos outros jogadores.
 * @param totalWrongs Número total de tentativas erradas feitas pelos outros jogadores.
 */
void game_calculateWriterPoints(gameStateType *state, int numOfCorrectTries, int totalWrongs)
{
    int totalRoundPoints = (totalWrongs * state->triesPerRound);
    state->playersScore[state->currWriterId - 1] += totalRoundPoints;
}
/**
 * @brief Verifica se o jogo terminou.
 *
 * Verifica se algum jogador atingiu a pontuação necessária para vencer o jogo.
 *
 * @param state Ponteiro para a estrutura de estado do jogo.
 * @return O ID do jogador vencedor, ou 0 se o jogo ainda não terminou.
 */
int game_isGameEnded(gameStateType *state)
{
    for (int i = 0; i < state->nPlayers; i++)
    {
        if (state->playersScore[i] >= state->scoreToWin)
            return i + 1;
    }

    return 0;
}
/**
 * @brief Define o próximo jogador que escolherá a palavra.
 *
 * Atualiza o estado do jogo para definir o próximo jogador que escolherá a palavra na próxima rodada.
 *
 * @param state Ponteiro para a estrutura de estado do jogo.
 */
void game_setNextWriter(gameStateType *state)
{
    state->currWriterId++;

    if (state->currWriterId > state->nPlayers)
    {
        state->currWriterId = 1;
    }
}