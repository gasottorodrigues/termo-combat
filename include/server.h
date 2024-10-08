#ifndef SERVER_H
#define SERVER_H

#include "../include/constants.h"
#include "../include/graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct
{
    int serverSocket;
    int clientSocket;
    bool hasConnection;

} connInfoType;

connInfoType *srv_setupConnection();
int srv_waitForConnection(connInfoType *conn);
void srv_closeConnection(int connRef);

#endif