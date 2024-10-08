#include "../include/gameEngine.h"

gameStateType *egn_setGameState()
{
    gameStateType *newGame = (gameStateType *)malloc(sizeof(gameStateType));

    newGame->playerTurn = '1';

    memset(newGame->p1Word, ' ', sizeof(newGame->p1Word));
    memset(newGame->p1CurrGuess, ' ', sizeof(newGame->p1CurrGuess));
    newGame->p1Tries = 0;
    newGame->p1Score = 0;

    memset(newGame->p2Word, ' ', sizeof(newGame->p2Word));
    memset(newGame->p2CurrGuess, ' ', sizeof(newGame->p2CurrGuess));
    newGame->p2Tries = 0;
    newGame->p2Score = 0;

    newGame->isMatchRunning = false;
    newGame->isRoundRunning = false;

    return newGame;
}

void egn_endGame(gameStateType *state)
{
    if (state != NULL)
    {
        free(state);
        state = NULL;
    }
}

void egn_setWord(gameStateType *state, char player)
{
    char input[6];
    bool valid = false;

    while (!valid)
    {

        gx_setWord(player);
        scanf("%5s", input);
        getchar();

        if (strlen(input) != 5)
        {
            gx_setWordError();
        }
        else
        {
            valid = true;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        input[i] = toupper(input[i]);
    }

    if (player == '1')
    {
        strncpy(state->p2Word, input, 5);
        return;
    }

    strncpy(state->p1Word, input, 5);
}

void egn_startMatch(gameStateType *state)
{
    gx_startMatch();
    state->isMatchRunning = true;
    while (state->isMatchRunning)
    {
        gx_Score(state->p1Score, state->p2Score);
        egn_setWord(state, '1');
        egn_setWord(state, '2');

        egn_roundLooping(state);

        if (state->p1Score == 2)
        {
            gx_endMatch('1');
            state->isMatchRunning = false;
        }
        else if (state->p2Score == 2)
        {
            gx_endMatch('2');
            state->isMatchRunning = false;
        }
    }
}

void egn_setGuess(gameStateType *state)
{
    char input[6];
    bool valid = false;

    while (!valid)
    {
        state->playerTurn == '1' ? gx_setGuess(state->playerTurn, state->p1Tries + 1) : gx_setGuess(state->playerTurn, state->p2Tries + 1);

        scanf("%5s", input);
        getchar();

        if (strlen(input) != 5)
        {
            gx_setWordError();
        }
        else
        {
            valid = true;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        input[i] = toupper(input[i]);
    }

    state->playerTurn == '1' ? strncpy(state->p1CurrGuess, input, 5) : strncpy(state->p2CurrGuess, input, 5);
}

guessResult egn_validateGuess(gameStateType *state)
{
    guessResult result;

    char wordCopy[5];
    char currentGuess[5];

    if (state->playerTurn == '1')
    {
        strcpy(wordCopy, state->p1Word);
        strcpy(currentGuess, state->p1CurrGuess);
    }
    else
    {
        strcpy(wordCopy, state->p2Word);
        strcpy(currentGuess, state->p2CurrGuess);
    }

    result.isRightGuess = true;
    for (int i = 0; i < 5; i++)
    {
        result.word[i] = 'x';
    }

    for (int i = 0; i < 5; i++)
    {
        if (currentGuess[i] == wordCopy[i])
        {
            result.word[i] = 'o';
            wordCopy[i] = '-';
            currentGuess[i] = '.';
        }
        else
        {
            result.isRightGuess = false;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        if (result.word[i] == 'o')
        {
            continue;
        }
        for (int j = 0; j < 5; j++)
        {
            if (currentGuess[i] == wordCopy[j])
            {
                result.word[i] = '-';
                wordCopy[j] = '-';
                break;
            }
        }
    }

    return result;
}

void egn_roundLooping(gameStateType *state)
{
    gx_startRound(state->p1Score + state->p2Score);
    state->isRoundRunning = true;
    while (state->isRoundRunning)
    {
        egn_setGuess(state);
        guessResult guessRes = egn_validateGuess(state);

        state->playerTurn == '1' ? gx_showGuessResult(state->p1CurrGuess, guessRes.word) : gx_showGuessResult(state->p2CurrGuess, guessRes.word);

        if (guessRes.isRightGuess)
        {
            state->isRoundRunning = false;
            state->playerTurn == '1' ? state->p1Score++ : state->p2Score++;
            gx_endRound(state->playerTurn);
        }
        else
        {
            if (state->playerTurn == '1')
            {
                state->p1Tries++;
                state->playerTurn = '2';
            }
            else
            {
                state->p2Tries++;
                state->playerTurn = '1';
            }
        }
    }
}
