#include "../include/client.h"

/**
 * @brief Configura e inicializa um buffer de conexão do cliente.
 *
 * Aloca memória para um buffer de conexão do cliente e inicializa
 * os campos `playerId` e `state` com valores padrão.
 *
 * @return Ponteiro para o buffer de conexão inicializado.
 */
clientBufferType *cli_setupConnBuffer()
{
    clientBufferType *connBuffer = (clientBufferType *)malloc(sizeof(clientBufferType));

    connBuffer->playerId = NO_CONN;
    connBuffer->state = NULL;

    return connBuffer;
}

/**
 * @brief Libera a memória associada ao buffer de conexão do cliente.
 *
 * Libera a memória alocada para o estado do jogo e o buffer de conexão
 * do cliente, se não forem nulos.
 *
 * @param connData Ponteiro para o buffer de conexão do cliente a ser liberado.
 */
void cli_freeConnBuffer(clientBufferType *connData)
{
    if (connData != NULL)
    {
        game_freeGameState(connData->state);
        free(connData);
    }
}

/**
 * @brief Conecta o cliente ao lobby do servidor.
 *
 * Estabelece uma conexão TCP com o servidor usando o endereço IP fornecido.
 * Recebe a configuração da partida e inicializa o estado do jogo no buffer
 * de conexão do cliente.
 *
 * @param ip Endereço IP do servidor.
 * @param connBuffer Ponteiro para o buffer de conexão do cliente.
 * @return O descritor de socket da conexão estabelecida.
 */
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

/**
 * @brief Gerencia a interação do jogador durante a partida.
 *
 * Controla o fluxo de uma partida, incluindo a escolha de palavras,
 * adivinhações e o envio/recebimento de dados entre o cliente e o servidor.
 * Atualiza o estado do jogo e exibe informações relevantes ao jogador.
 *
 * @param socket Descritor de socket da conexão com o servidor.
 * @param connData Ponteiro para o buffer de conexão do cliente.
 */
void cli_playerHandler(int socket, clientBufferType *connData)
{
    gameStateType *state = connData->state;
    recv(socket, &(connData->state->isMatchRunning), sizeof(bool), 0);

    printf("Voce e o jogador %d...\n", connData->playerId);
    printf("Aguardando o inicio da partida...\n");
    while (state->isMatchRunning)
    {
        gx_startRound(state->playersScore, state->nPlayers, connData->playerId);
        recv(socket, &(state->isRoundRunnig), sizeof(bool), 0);
        if (state->currWriterId == connData->playerId)
        {
            printf("Voce escolhe a palavra nessa rodada.\n");
            game_chooseWord(state);
            state->isWordSet = true;
            send(socket, state->word, sizeof(state->word), 0);

            printf("Sua palavra foi definida\n");
            printf("Aguardando respostas dos usuários...\n");
        }
        else
        {
            printf("O jogador %d esta escolhendo a palavra...\n", state->currWriterId);
            recv(socket, state->word, state->wordSize + 1 * sizeof(char), 0);

            printf("Palavra definida! hora de adivinhar.\n");
            playerGuessesResultType res = game_tryGuesses(state);
            if (res.correctAns)
            {
                printf("Boa! Agora vamos esperar os demais jogadores...\n");
            }
            else
            {
                printf("Que pena :( Agora vamos esperar os demais jogadores...\n");
            }

            send(socket, &res, sizeof(playerGuessesResultType), 0);
        }

        recv(socket, state->playersScore, state->nPlayers * sizeof(int), 0);
        printf("Respostas recebidas!\n");
        gx_showPoints(state->playersScore, state->nPlayers);

        int winner;
        recv(socket, &winner, sizeof(int), 0);

        if (!winner)
        {
            game_setNextWriter(state);
        }
        else
        {
            printf("Jogador %d venceu a partida!\n", winner);
            state->isMatchRunning = false;
        }
    }

    printf("Digite qualquer tecla para continuar.\n");
    getchar();
}
