#include "../include/client.h"

clientBufferType *cli_setupConnBuffer()
{
    clientBufferType *connBuffer = (clientBufferType *)malloc(sizeof(clientBufferType));

    connBuffer->playerId = NO_CONN;
    connBuffer->state = NULL;

    return connBuffer;
}

void cli_freeConnBuffer(clientBufferType *connData)
{
    if (connData != NULL)
    {
        game_freeGameState(connData->state);
        free(connData);
    }
}

int cli_connectToLobby(char *ip, clientBufferType *connBuffer)
{

    int cliSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (cliSocket == ERROR)
    {
        gx_printError("nao foi possivel criar o socket.");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0)
    {
        gx_printError("endereco de ip nao suportado.");
        exit(EXIT_FAILURE);
    }

    printf("iniciando conexao...\n");
    int connStatus = connect(cliSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (connStatus == ERROR)
    {
        gx_printError("nao foi possivel conectar ao servidor.");
        exit(EXIT_FAILURE);
    }

    matchConfigType match;
    recv(cliSocket, &match, sizeof(matchConfigType), 0);

    connBuffer->playerId = match.playerId;
    connBuffer->state = game_setupGameState(match.scoreToWin, match.wordSize, match.nPlayers, match.triesPerRound);
    return cliSocket;
}

void cli_playerHandler(int socket, clientBufferType *connData)
{
    gameStateType *state = connData->state;
    printf("Aguardando inicio da partida...\n");
    recv(socket, &(connData->state->isMatchRunning), sizeof(bool), 0);

    printf("Partida Iniciada! Voce e o jogador %d\n", connData->playerId);
    while (state->isMatchRunning)
    {
        recv(socket, &(state->isRoundRunnig), sizeof(bool), 0);
        if (state->currWriterId == connData->playerId)
        {
            game_chooseWord(state);
            printf("Palavra definida: %s\n", state->word);
            state->isWordSet = true;
            send(socket, state->word, sizeof(state->word), 0);
        }
        else
        {
            printf("Aguardando palavra...\n");
            recv(socket, state->word, state->wordSize + 1 * sizeof(char), 0);
            printf("Palavra recebida! %s\n", state->word);

            playerGuessesResultType res = game_tryGuesses(state);
            if (res.correctAns)
            {
                printf("Boa!\n");
            }
            else
            {
                printf("Suas chances acabaram. Que pena...\n");
            }

            send(socket, &res, sizeof(playerGuessesResultType), 0);
        }

        printf("Espere até que todos tenham respondido...\n");

        recv(socket, state->playersScore, state->nPlayers * sizeof(int), 0);
        printf("Pontuacao recebida!");

        int winner;
        recv(socket, &winner, sizeof(int), 0);

        if (!winner)
        {
            game_setNextWriter(state);
        }
        else
        {
            state->isMatchRunning = false;
        }
    }
    printf("Temos um vencedor. Partida encerrada.\n");
}