#include "../include/server.h"

void *srv_hostHandler(void *args)
{

    threadArgsType *threadArgs = (threadArgsType *)args;
    int playerId = threadArgs->playerId;
    lobbyConfigState *lobby = threadArgs->lobby;
    playerGuessesResultType *playerLastguess = threadArgs->guess;
    gameStateType *state = lobby->state;

    while (!state->isMatchRunning)
        ;

    while (state->isMatchRunning)
    {
        while (!state->isRoundRunnig)
            ;

        if (state->currWriterId == playerId)
        {
            game_chooseWord(state);
            pthread_mutex_lock(&lobby->lobbyMutex);
            state->isWordSet = true;
            pthread_mutex_unlock(&lobby->lobbyMutex);

            while (state->isInScoreCalc)
                ;
            int winner = game_isGameEnded(state);
            if (winner)
            {
                printf("host:Temos um vencedor!\n");
            }
        }
        else
        {
            while (!state->isWordSet)
                ;

            playerGuessesResultType guess = game_tryGuesses(state);
            if (guess.correctAns)
            {
                printf("Boa!\n");
            }
            else
            {
                printf("Suas chances acabaram. Que pena...\n");
            }

            playerLastguess->correctAns = guess.correctAns;
            playerLastguess->numOfTries = guess.numOfTries;
            threadArgs->guessesReceived = true;

            while (state->isInScoreCalc)
                ;

            int winner = game_isGameEnded(state);
            if (winner)
            {
                printf("host:Temos um vencedor!\n");
            }
        }
    }

    printf("thread %d: encerrando atividades.\n", playerId);

    pthread_exit(NULL);
    return NULL;
}

void *srv_clientSocketHandler(void *args)
{
    threadArgsType *threadArgs = (threadArgsType *)args;
    int clientSocket = threadArgs->cliSocket;
    int playerId = threadArgs->playerId;
    playerGuessesResultType *playerLastguess = threadArgs->guess;
    lobbyConfigState *lobby = threadArgs->lobby;
    gameStateType *state = lobby->state;

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    printf("thread %d: aguardando inicio da partida.\n", playerId);
    while (!state->isMatchRunning)
        ;

    while (state->isMatchRunning)
    {
        printf("thread %d: aguardando inicio do round.\n", playerId);
        while (!state->isRoundRunnig)
            ;

        if (state->currWriterId == playerId)
        {
            printf("client: lendo resposta do player %d\n", playerId);
            bytesRead = recv(clientSocket, &buffer, sizeof(state->word), 0);
            if (bytesRead == 0)
            {
                // TODO: usuario desconectou
                gx_printError("Jogador se desconectou.\n");
                srv_closeConnection(clientSocket);

                pthread_mutex_lock(&lobby->lobbyMutex);
                lobby->clientSocket[playerId - 2] = NO_CONN;
                lobby->nConn--;
                pthread_mutex_unlock(&lobby->lobbyMutex);

                pthread_exit(NULL);
            }

            if (bytesRead < 0)
            {
                // TODO: problema na comunicação
                gx_printError("problema na thread. Problema ao receber do socket.\n");
                srv_closeConnection(clientSocket);

                pthread_mutex_lock(&lobby->lobbyMutex);
                lobby->clientSocket[playerId - 2] = NO_CONN;
                lobby->nConn--;
                pthread_mutex_unlock(&lobby->lobbyMutex);

                pthread_exit(NULL);
            }

            printf("thread %d: palavra recebida %s \n", playerId, buffer);
            pthread_mutex_lock(&lobby->lobbyMutex);
            strncpy(state->word, buffer, state->wordSize);
            state->word[state->wordSize] = '\0';
            state->isWordSet = true;
            pthread_mutex_unlock(&lobby->lobbyMutex);

            while (state->isInScoreCalc)
                ;
        }
        else
        {
            printf("thread %d: aguardando palavra\n", playerId);
            while (!state->isWordSet)
                ;

            printf("thread %d: palavra recebida %s\n", playerId, state->word);
            send(clientSocket, state->word, state->wordSize + 1 * sizeof(char), 0);

            printf("thread %d: aguardando resposta\n", playerId);
            playerGuessesResultType guess;
            bytesRead = recv(clientSocket, &guess, sizeof(playerGuessesResultType), 0);
            if (bytesRead == 0)
            {
                // TODO: usuario desconectou
                gx_printError("Jogador se desconectou.\n");
                srv_closeConnection(clientSocket);

                pthread_mutex_lock(&lobby->lobbyMutex);
                lobby->clientSocket[playerId - 2] = NO_CONN;
                lobby->nConn--;
                pthread_mutex_unlock(&lobby->lobbyMutex);

                pthread_exit(NULL);
            }

            if (bytesRead < 0)
            {
                // TODO: problema na comunicação
                gx_printError("problema na thread. Problema ao receber do socket.\n");
                srv_closeConnection(clientSocket);

                pthread_mutex_lock(&lobby->lobbyMutex);
                lobby->clientSocket[playerId - 2] = NO_CONN;
                lobby->nConn--;
                pthread_mutex_unlock(&lobby->lobbyMutex);

                pthread_exit(NULL);
            }

            playerLastguess->correctAns = guess.correctAns;
            playerLastguess->numOfTries = guess.numOfTries;

            threadArgs->guessesReceived = true;

            while (state->isInScoreCalc)
                ;
        }
    }

    pthread_exit(NULL);
    return NULL;
}

lobbyConfigState *srv_setupLobby(int scoreToWin, int wordSize, int nPlayers, int triesPerRound)
{
    int serverSocket;
    struct sockaddr_in address;
    int opt = 1;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == ERROR)
    {
        gx_printError("nao foi possivel criar o socket.");
        exit(EXIT_FAILURE);
    }

    // Configurar opções do socket
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        gx_printError("nao foi possivel definir as opcoes de socket.");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) == ERROR)
    {
        gx_printError("falha ao realizar o bind do servidor.");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, BACKLOG) == ERROR)
    {
        gx_printError("falha ao escutar a porta");
        exit(EXIT_FAILURE);
    }

    lobbyConfigState *conn = (lobbyConfigState *)malloc(sizeof(lobbyConfigState));
    conn->serverSocket = serverSocket;
    conn->clientSocket = (int *)malloc((nPlayers - 1) * sizeof(int));

    for (int i = 0; i < (nPlayers - 1); i++)
    {
        conn->clientSocket[i] = NO_CONN;
    }

    conn->nConn = 0;
    conn->state = game_setupGameState(scoreToWin, wordSize, nPlayers, triesPerRound);

    pthread_mutex_init(&conn->lobbyMutex, NULL);

    return conn;
}

void srv_closeLobby(lobbyConfigState *lobby)
{
    for (int i = 0; i < lobby->state->nPlayers; i++)
    {
        if (lobby->clientSocket[i] != NO_CONN)
            srv_closeConnection(lobby->clientSocket[i]);
    }

    srv_closeConnection(lobby->serverSocket);
    srv_freeLobby(lobby);
}

void srv_freeLobby(lobbyConfigState *lobby)
{
    if (lobby != NULL)
    {
        pthread_mutex_destroy(&lobby->lobbyMutex);
        free(lobby->clientSocket);
        game_freeGameState(lobby->state);
        free(lobby);
    }
}

void srv_closeConnection(int connRef)
{
    close(connRef);
}

void srv_broadcast(lobbyConfigState *lobby, void *data, long size)
{
    for (int i = 0; i < lobby->nConn; i++)
    {
        if (lobby->clientSocket[i] != NO_CONN)
        {
            // Tenta enviar os dados com a flag MSG_NOSIGNAL para evitar SIGPIPE
            ssize_t result = send(lobby->clientSocket[i], data, size, MSG_NOSIGNAL);

            // Se o envio falhar, trata a desconexão
            if (result == -1)
            {
                perror("Erro ao enviar dados para o cliente");
                printf("Cliente no socket %d desconectado.\n", lobby->clientSocket[i]);

                srv_closeConnection(lobby->clientSocket[i]);
                lobby->clientSocket[i] = NO_CONN;
            }
        }
    }
}

void srv_createClientThreads(lobbyConfigState *lobby, pthread_t *threads, threadArgsType *threadArgs)
{
    for (int i = 1; i <= lobby->nConn; i++)
    {
        threadArgs[i].cliSocket = lobby->clientSocket[i - 1];
        threadArgs[i].playerId = i + 1;
        threadArgs[i].guessesReceived = false;
        threadArgs[i].lobby = lobby;
        threadArgs[i].guess = (playerGuessesResultType *)malloc(sizeof(playerGuessesResultType));

        if (pthread_create(&(threads[i]), NULL, srv_clientSocketHandler, (void *)&(threadArgs[i])) != 0)
        {
            gx_printError("nao foi possivel criar a thread para cliente.");
            exit(EXIT_FAILURE);
        }
    }
}

void srv_createHostThread(lobbyConfigState *lobby, pthread_t *threads, threadArgsType *threadArgs)
{
    threadArgs[0].cliSocket = NO_CONN;
    threadArgs[0].playerId = 1;
    threadArgs[0].guessesReceived = false;
    threadArgs[0].lobby = lobby;
    threadArgs[0].guess = (playerGuessesResultType *)malloc(sizeof(playerGuessesResultType));

    if (pthread_create(&(threads[0]), NULL, srv_hostHandler, (void *)&(threadArgs[0])) != 0)
    {
        gx_printError("nao foi possivel criar a thread para cliente.");
        exit(EXIT_FAILURE);
    }
}

threadArgsType *srv_waitForPlayers(lobbyConfigState *lobby)
{
    matchConfigType match;
    match.playerId = -1;
    match.scoreToWin = lobby->state->scoreToWin;
    match.wordSize = lobby->state->wordSize;
    match.nPlayers = lobby->state->nPlayers;
    match.triesPerRound = lobby->state->triesPerRound;

    int newSocket;
    int addrlen = sizeof(struct sockaddr_in);
    struct sockaddr_in address;

    while (lobby->nConn < (lobby->state->nPlayers - 1))
    {
        newSocket = accept(lobby->serverSocket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (newSocket == ERROR)
        {

            gx_printError("falha ao aceitar nova conexao.");
            srv_closeLobby(lobby);
            exit(EXIT_FAILURE);
        }
        else
        {
            match.playerId = lobby->nConn + 2;
            send(newSocket, &match, sizeof(matchConfigType), 0);

            printf("Jogador conectado!\n");
            lobby->clientSocket[lobby->nConn] = newSocket;
            lobby->nConn++;
        }
    }

    printf("Gerando lista de threads..\n");
    int threadListSize = lobby->nConn + 1;
    pthread_t *threads = (pthread_t *)malloc(threadListSize * sizeof(pthread_t));
    if (threads == NULL)
    {
        gx_printError("Falha ao alocar memória para threads.");
        exit(EXIT_FAILURE);
    }

    threadArgsType *threadArgs = (threadArgsType *)malloc(threadListSize * sizeof(threadArgsType));
    if (threadArgs == NULL)
    {
        gx_printError("Falha ao alocar memória para argumentos das threads.");
        free(threads);
        exit(EXIT_FAILURE);
    }

    srv_createClientThreads(lobby, threads, threadArgs);
    srv_createHostThread(lobby, threads, threadArgs);

    return threadArgs;
}

void printIntList(int *list, int size)
{
    printf("[");
    for (int i = 0; i < size; i++)
    {
        printf("%d", list[i]);
        if (i < size - 1)
        {
            printf(", "); // Adiciona uma vírgula e espaço entre os elementos
        }
    }
    printf("]\n");
}

void srv_gameLoop(lobbyConfigState *lobby, threadArgsType *threadVars)
{
    gameStateType *state = lobby->state;

    state->isMatchRunning = true;
    srv_broadcast(lobby, &(state->isMatchRunning), sizeof(bool));

    while (state->isMatchRunning)
    {
        bool startRound = true;
        srv_broadcast(lobby, &startRound, sizeof(bool));
        lobby->state->isRoundRunnig = true;
        state->isInScoreCalc = true;

        printf("server: aguardando escolha da palavra...\n");
        while (!state->isWordSet)
            ;
        // srv_broadcast(lobby, &(lobby->state->isRoundRunnig), sizeof(bool));
        while (state->isRoundRunnig)
        {
            int totalRecv = 0;
            for (int i = 0; i < lobby->state->nPlayers; i++)
            {
                if (lobby->clientSocket[i] == NO_CONN)
                {
                    continue;
                }

                if (threadVars[i].guessesReceived)
                    totalRecv++;
            }

            if (totalRecv == lobby->nConn)
            {
                state->isRoundRunnig = false;
            }
        }

        pthread_mutex_lock(&lobby->lobbyMutex);
        printf("server: calculando pontuacao.\n");
        int totalTries = 0;
        int totalWrong = 0;
        for (int i = 0; i < state->nPlayers; i++)
        {
            if (state->currWriterId == i + 1)
            {
                continue;
            }

            if (threadVars[i].guessesReceived)
            {
                game_calculateRoundPoints(state, i, threadVars[i].guess->numOfTries, threadVars[i].guess->correctAns);
                if (threadVars[i].guess->correctAns)
                {
                    totalTries += threadVars[i].guess->numOfTries;
                }
                else
                {
                    totalWrong++;
                }

                threadVars[i].guessesReceived = false;
                threadVars[i].guess->numOfTries = 0;
                threadVars[i].guess->correctAns = false;
            }
        }
        game_calculateWriterPoints(state, totalTries, totalWrong);
        printf("server: Pontuação calculada.\n");
        state->isRoundRunnig = false;
        state->isWordSet = false;
        state->isInScoreCalc = false;
        pthread_mutex_unlock(&lobby->lobbyMutex);

        srv_broadcast(lobby, lobby->state->playersScore, lobby->state->nPlayers * sizeof(int));

        int winner = game_isGameEnded(state);
        srv_broadcast(lobby, &winner, sizeof(int));

        if (!winner)
        {
            game_setNextWriter(state);
            printf("server: Proximo leitor definido: %d\n.", state->currWriterId);
        }
        else
        {
            state->isMatchRunning = false;
        }
    }
}
