#ifndef ENGINE_H
#define ENGINE_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/constants.h"
#include "../include/graphics.h"

typedef struct
{
    bool isMatchRunning;
    bool isRoundRunning;

    char playerTurn;

    int p1Score;
    int p2Score;

    char p1Word[5];
    char p1CurrGuess[5];
    int p1Tries;

    char p2Word[5];
    char p2CurrGuess[5];
    int p2Tries;

} gameStateType;

typedef struct
{
    bool isRightGuess;
    char word[6];
} guessResult;

gameStateType *egn_setGameState();

void egn_setWord(gameStateType *state, char player);
void egn_printGameState(const gameStateType *state);

void egn_startMatch(gameStateType *state);
void egn_roundLooping(gameStateType *state);
void egn_endGame(gameStateType *state);

#endif