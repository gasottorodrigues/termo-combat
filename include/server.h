#ifndef SERVER_H
#define SERVER_H

#include "../include/constants.h"
#include "../include/graphics.h"
#include "../include/gameEngine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <pthread.h>

typedef struct
{
    int serverSocket;

    int *clientSocket;
    int nConn;

    gameStateType *state;

    pthread_mutex_t lobbyMutex;

} lobbyConfigState;

typedef struct
{
    int playerId;
    int scoreToWin;
    int wordSize;
    int nPlayers;
    int triesPerRound;
} matchConfigType;

typedef struct
{
    int playerId;
    gameStateType *state;
} clientBufferType;

typedef struct
{
    int cliSocket;
    int playerId;
    bool guessesReceived;

    playerGuessesResultType *guess;
    lobbyConfigState *lobby;
} threadArgsType;

lobbyConfigState *srv_setupLobby(int scoreToWin, int wordSize, int nPlayers, int triesPerRound);
threadArgsType *srv_waitForPlayers(lobbyConfigState *lobby);
void srv_closeLobby(lobbyConfigState *lobby);
void srv_roundHandler(lobbyConfigState *lobby);
void srv_gameLoop(lobbyConfigState *lobby, threadArgsType *threadVars);

void srv_freeLobby(lobbyConfigState *lobby);
void srv_closeConnection(int connRef);
#endif