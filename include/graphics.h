#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include "../include/constants.h"

void gx_mainMenu();
void gx_exit();

void gx_setWord(const char player);
void gx_setWordError();

void gx_setGuess(const char player, const int try);
void gx_showGuessResult(const char *guess, const char *result);

void gx_startRound(const int roundCounter);
void gx_endRound(const char player);

void gx_startMatch();
void gx_endMatch(const char player);

void gx_Score(const int p1Score, const int p2Score);

#endif