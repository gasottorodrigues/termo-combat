#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "../include/constants.h"
#include "../include/graphics.h"
#include "../include/gameEngine.h"
#include "../include/server.h"
#include "../include/client.h"

int main(int argc, char *argv[])
{
    int cmd;
    bool inExec = true;
    // gameStateType *state;
    connInfoType *conn;

    while (inExec)
    {
        gx_mainMenu();
        scanf("%d", &cmd);
        int buff;

        switch (cmd)
        {
        case CMD_EXIT:
            // egn_endGame(state);
            inExec = false;
            break;
        case CMD_HOST:
            conn = srv_setupConnection();
            gx_createdLobby();
            srv_waitForConnection(conn);
            srv_closeConnection(conn->serverSocket);
            srv_closeConnection(conn->clientSocket);
            break;

        case CMD_JOIN:
            int cliConn = cli_connectToLobby();
            scanf("%d", &buff);
            srv_closeConnection(cliConn);
            break;

        case CMD_TUT:
            break;

        default:
            break;
        }
    }

    gx_exit();
}