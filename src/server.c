#include "../include/server.h"

/**
 * @brief Função que gerencia o host (jogador principal) durante a partida.
 *
 * Esta função é executada em uma thread separada para o host. Ela controla o fluxo da partida para o host,
 * incluindo a escolha de palavras e a espera pelas respostas dos outros jogadores.
 *
 * @param args Argumentos da thread, que incluem o estado do lobby e o ID do jogador.
 * @return NULL
 */
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

        gx_startRound(state->playersScore, state->nPlayers, playerId);
        if (state->currWriterId == playerId)
        {

            printf("Voce escolhe a palavra nessa rodada.\n");
            game_chooseWord(state);
            pthread_mutex_lock(&lobby->lobbyMutex);
            state->isWordSet = true;
            pthread_mutex_unlock(&lobby->lobbyMutex);

            printf("Sua palavra foi definida\n");
            printf("Aguardando respostas dos usuários...\n");
            while (state->isInScoreCalc)
                ;
        }
        else
        {
            printf("O jogador %d esta escolhendo a palavra...\n", state->currWriterId);
            while (!state->isWordSet)
                ;

            printf("Palavra definida! hora de adivinhar.\n");
            playerGuessesResultType guess = game_tryGuesses(state);
            if (guess.correctAns)
            {
                printf("Boa! Agora vamos esperar os demais jogadores...\n");
            }
            else
            {
                printf("Que pena :( Agora vamos esperar os demais jogadores...\n");
            }

            playerLastguess->correctAns = guess.correctAns;
            playerLastguess->numOfTries = guess.numOfTries;
            threadArgs->guessesReceived = true;

            while (state->isInScoreCalc)
                ;
        }

        printf("Respostas recebidas!\n");
        gx_showPoints(state->playersScore, state->nPlayers);
        int winner = game_isGameEnded(state);
        if (winner)
        {
            printf("Jogador %d venceu a partida!\n", winner);
        }
    }

    pthread_exit(NULL);
    return NULL;
}

/**
 * @brief Função que gerencia a comunicação com um cliente (jogador) durante a partida.
 *
 * Esta função é executada em uma thread separada para cada cliente. Ela lida com a comunicação via socket,
 * recebendo e enviando dados entre o servidor e o cliente, como a palavra escolhida e as tentativas de adivinhação.
 *
 * @param args Argumentos da thread, que incluem o socket do cliente e o estado do lobby.
 * @return NULL
 */
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

    // printf removido
    while (!state->isMatchRunning)
        ;

    while (state->isMatchRunning)
    {
        // printf removido
        while (!state->isRoundRunnig)
            ;

        if (state->currWriterId == playerId)
        {
            // printf removido
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

            // printf removido
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
            // printf removido
            while (!state->isWordSet)
                ;

            // printf removido
            send(clientSocket, state->word, state->wordSize + 1 * sizeof(char), 0);

            // printf removido
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
/**
 * @brief Configura o lobby do servidor para uma nova partida.
 *
 * Inicializa o estado do lobby, configura o socket do servidor e prepara o ambiente para que os jogadores
 * possam se conectar e iniciar a partida.
 *
 * @param scoreToWin Pontuação necessária para vencer a partida.
 * @param wordSize Tamanho da palavra a ser adivinhada.
 * @param nPlayers Número de jogadores na partida.
 * @param triesPerRound Número de tentativas permitidas por rodada.
 * @return Ponteiro para a estrutura de configuração do lobby.
 */
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

/**
 * @brief Fecha o lobby e desconecta todos os jogadores.
 *
 * Fecha todas as conexões de socket dos jogadores e do servidor, e libera os recursos associados ao lobby.
 *
 * @param lobby Ponteiro para a estrutura de configuração do lobby.
 */
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
/**
 * @brief Libera a memória associada ao lobby.
 *
 * Libera os recursos alocados para o lobby, incluindo o estado do jogo, os sockets dos clientes e o mutex.
 *
 * @param lobby Ponteiro para a estrutura de configuração do lobby.
 */
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
/**
 * @brief Fecha uma conexão de socket.
 *
 * Fecha o socket especificado, encerrando a comunicação com o cliente ou servidor.
 *
 * @param connRef Referência para o socket a ser fechado.
 */
void srv_closeConnection(int connRef)
{
    close(connRef);
}
/**
 * @brief Envia dados para todos os clientes conectados no lobby.
 *
 * Envia os dados especificados para todos os clientes conectados, utilizando o socket de cada cliente.
 * Se a conexão com algum cliente falhar, o cliente é desconectado.
 *
 * @param lobby Ponteiro para a estrutura de configuração do lobby.
 * @param data Dados a serem enviados.
 * @param size Tamanho dos dados a serem enviados.
 */
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
                // printf removido

                srv_closeConnection(lobby->clientSocket[i]);
                lobby->clientSocket[i] = NO_CONN;
            }
        }
    }
}
/**
 * @brief Cria threads para gerenciar a comunicação com os clientes.
 *
 * Para cada cliente conectado, cria uma thread que gerencia a comunicação via socket, recebendo e enviando
 * dados durante a partida.
 *
 * @param lobby Ponteiro para a estrutura de configuração do lobby.
 * @param threads Array de threads a serem criadas.
 * @param threadArgs Argumentos para as threads, incluindo o socket do cliente e o estado do lobby.
 */
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

/**
 * @brief Cria uma thread para gerenciar o host (jogador principal).
 *
 * Cria uma thread separada para o host, que será responsável por escolher a palavra e gerenciar o fluxo da partida.
 *
 * @param lobby Ponteiro para a estrutura de configuração do lobby.
 * @param threads Array de threads a serem criadas.
 * @param threadArgs Argumentos para a thread do host, incluindo o estado do lobby.
 */
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

/**
 * @brief Aguarda a conexão de todos os jogadores no lobby.
 *
 * Aceita conexões de novos jogadores até que todos os jogadores necessários estejam conectados.
 * Após todos os jogadores entrarem, inicia a partida.
 *
 * @param lobby Ponteiro para a estrutura de configuração do lobby.
 * @return Ponteiro para os argumentos das threads criadas para cada jogador.
 */
threadArgsType *srv_waitForPlayers(lobbyConfigState *lobby)
{
    printf("Aguardando novos jogadores...\n");

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

            printf("Jogador %d conectado.\n", match.playerId);

            lobby->clientSocket[lobby->nConn] = newSocket;
            lobby->nConn++;
        }
    }
    printf("Todos os jogadores já entraram. Iniciando partida...\n");

    // printf removido
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

/**
 * @brief Loop principal do jogo, gerenciando as rodadas e o fluxo da partida.
 *
 * Controla o início e o fim de cada rodada, calcula os pontos dos jogadores e verifica se a partida terminou.
 * Envia atualizações para todos os jogadores conectados.
 *
 * @param lobby Ponteiro para a estrutura de configuração do lobby.
 * @param threadVars Argumentos das threads criadas para cada jogador.
 */
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

        // printf removido
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
        // printf removido
        int totalTries = 0;
        int totalWrong = 0;
        for (int i = 0; i < state->nPlayers; i++)
        {
            if (state->currWriterId == threadVars[i].playerId)
            {
                continue;
            }

            if (threadVars[i].guessesReceived)
            {
                game_calculateRoundPoints(state, threadVars[i].playerId - 1, threadVars[i].guess->numOfTries, threadVars[i].guess->correctAns);
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
            // printf removido
        }
        else
        {
            state->isMatchRunning = false;
        }
    }
}
