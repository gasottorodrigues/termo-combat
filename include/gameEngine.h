#ifndef ENGINE_H
#define ENGINE_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/constants.h"
#include "../include/graphics.h"

typedef struct
{
    int scoreToWin;
    int wordSize;
    int nPlayers;
    int triesPerRound;

    bool isMatchRunning;
    bool isRoundRunnig;
    bool isWordSet;
    bool isInScoreCalc;

    int currRound;
    int currWriterId;
    int *playersScore;

    char *word;
} gameStateType;

typedef struct
{
    int numOfTries;
    bool correctAns;
} playerGuessesResultType;

typedef struct
{
    bool result;
    char *visualRes;
} guessResult;

gameStateType *game_setupGameState(int scoreToWin, int wordSize, int nPlayers, int triesPerRound);
void game_freeGameState(gameStateType *state);
void game_chooseWord(gameStateType *state);

playerGuessesResultType game_tryGuesses(gameStateType *state);
void game_calculateRoundPoints(gameStateType *state, int playerStateId, int numOfTries, bool correctAns);
void game_calculateWriterPoints(gameStateType *state, int numOfCorrectTries, int totalWrongs);
int game_isGameEnded(gameStateType *state);
void game_setNextWriter(gameStateType *state);
#endif