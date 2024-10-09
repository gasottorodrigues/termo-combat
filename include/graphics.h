#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include "../include/constants.h"

void gx_printError(char *err);

void gx_mainMenu();
void gx_createLobby(int *s, int *w, int *p, int *t);

void gx_startRound(int *points, int const nPlayers, int playerId);
void gx_showPoints(int *points, int const nPlayers);
void gx_showGuessResult(const char *guess, const char *result);

#endif