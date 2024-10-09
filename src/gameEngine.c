#include "../include/gameEngine.h"

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

void game_freeGameState(gameStateType *state)
{
    if (state != NULL)
    {
        free(state->playersScore);
        free(state->word);

        free(state);
    }
}

void game_chooseWord(gameStateType *state)
{
    char input[state->wordSize + 1];
    bool valid = false;

    while (!valid)
    {
        printf("Escolha uma palavra de %d letras: ", state->wordSize);

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

    // Copiar a palavra para o estado do jogo e garantir que seja terminada com '\0'
    strncpy(state->word, input, state->wordSize);
    state->word[state->wordSize] = '\0';
}

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

playerGuessesResultType game_tryGuesses(gameStateType *state)
{
    int maxTries = state->triesPerRound;
    playerGuessesResultType res;
    res.numOfTries = 0;
    res.correctAns = false;

    char input[state->wordSize];

    while (!res.correctAns && res.numOfTries <= maxTries)
    {
        res.numOfTries++;

        bool valid = false;
        while (!valid)
        {
            printf("Tentativa %d! Tente adivinha a palavra: ", res.numOfTries);
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

void game_calculateRoundPoints(gameStateType *state, int playerStateId, int numOfTries, bool correctAns)
{
    int totalRoundPoints = correctAns ? state->triesPerRound - numOfTries + 1 : 0;
    state->playersScore[playerStateId] += totalRoundPoints;
}

void game_calculateWriterPoints(gameStateType *state, int numOfCorrectTries, int totalWrongs)
{
    int totalRoundPoints = totalWrongs * state->triesPerRound + numOfCorrectTries - 1;
    state->playersScore[state->currWriterId - 1] += totalRoundPoints;
}

int game_isGameEnded(gameStateType *state)
{
    for (int i = 0; i < state->nPlayers; i++)
    {
        if (state->playersScore[i] >= state->scoreToWin)
            return i + 1;
    }

    return 0;
}

void game_setNextWriter(gameStateType *state)
{
    state->currWriterId++;

    if (state->currWriterId > state->nPlayers)
    {
        state->currWriterId = 1;
    }
}