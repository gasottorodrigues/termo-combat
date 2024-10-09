#ifndef CLIENT_H
#define CLIENT_H

#include "../include/constants.h"
#include "../include/graphics.h"
#include "../include/server.h"

clientBufferType *cli_setupConnBuffer();
int cli_connectToLobby(char *ip, clientBufferType *connBuffer);
void cli_playerHandler(int socket, clientBufferType *connData);
void cli_freeConnBuffer(clientBufferType *connData);

#endif