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

    while (inExec)
    {
        gx_mainMenu();
        scanf("%d", &cmd);
        switch (cmd)
        {
        case CMD_EXIT:
            inExec = false;
            break;
        case CMD_HOST:
            int score, wSize, nPlayers, tries;
            gx_createLobby(&score, &wSize, &nPlayers, &tries);
            lobbyConfigState *lobby = srv_setupLobby(score, wSize, nPlayers, tries);
            printf("Servidor criado com sucesso!\n");
            threadArgsType *threadVars = srv_waitForPlayers(lobby);
            srv_gameLoop(lobby, threadVars);
            printf("Digite qualquer tecla para continuar.\n");
            getchar();
            srv_closeLobby(lobby);
            break;

        case CMD_JOIN:
            char ip[16];
            printf("Insira o IP da sala: ");
            scanf("%16s", ip);

            printf("Tentando entrar...\n");
            clientBufferType *connData = cli_setupConnBuffer();
            int cliSocket = cli_connectToLobby(ip, connData);
            printf("Voce entrou na partida!\n");

            cli_playerHandler(cliSocket, connData);
            srv_closeConnection(cliSocket);
            cli_freeConnBuffer(connData);
            break;

        default:
            break;
        }
    }

    // int buffer;
    // printf("1- servidor | 2- cliente: ");
    // scanf("%d", &buffer);

    // if (buffer == 1)
    // {
    //     lobbyConfigState *lobby = srv_setupLobby(10, 5, 3, 4);
    //     printf("Aguardando conexao de todos os jogadores...\n");
    //     threadArgsType *threadVars = srv_waitForPlayers(lobby);
    //     return 0;
    // }
    // else
    // {
    //     char ip[16];
    //     printf("IP: ");
    //     scanf("%16s", ip);

    //     clientBufferType *connData = cli_setupConnBuffer();
    //     int cliSocket = cli_connectToLobby(ip, connData);
    //     printf("Conexao realizada com sucesso.\n");

    //     cli_playerHandler(cliSocket, connData);
    //     srv_closeConnection(cliSocket);
    //     cli_freeConnBuffer(connData);

    //     return 0;
    // }

    // int cmd;
    // bool inExec = true;
    // gameStateType *state = egn_setGameState();
    // connInfoType *conn;

    // while (inExec)
    // {
    //     gx_mainMenu();
    //     scanf("%d", &cmd);
    //     int buff;

    //     switch (cmd)
    //     {
    //     case CMD_EXIT:
    //         // egn_endGame(state);
    //         inExec = false;
    //         break;
    //     case CMD_HOST:
    //         conn = srv_setupConnection();
    //         gx_createdLobby();
    //         srv_waitForConnection(conn);
    //         break;

    //     case CMD_JOIN:
    //         int cliConn = cli_connectToLobby();
    //         scanf("%d", &buff);
    //         srv_closeConnection(cliConn);
    //         break;

    //     case CMD_TUT:
    //         break;

    //     default:
    //         break;
    //     }
    // }

    // srv_closeConnection(conn->serverSocket);
    // srv_closeConnection(conn->clientSocket);
    // gx_exit();
}