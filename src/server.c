#include "../include/server.h"

void srv_closeConnection(int connRef)
{
    close(connRef);
}

connInfoType *srv_setupConnection()
{
    int serverFd;
    struct sockaddr_in address;
    int opt = 1;

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configurar opções do socket
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, BACKLOG) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    connInfoType *conn = (connInfoType *)malloc(sizeof(connInfoType));
    conn->serverSocket = serverFd;
    conn->clientSocket = -1;
    conn->hasConnection = false;

    return conn;
}

int srv_waitForConnection(connInfoType *conn)
{
    printf("Aguardando conexão de jogador...\n");
    int newSocket;
    int addrlen = sizeof(struct sockaddr_in);
    struct sockaddr_in address;

    while (!conn->hasConnection)
    {
        if ((newSocket = accept(conn->serverSocket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) == ERROR)
        {
            perror("accept");
            srv_closeConnection(conn->serverSocket);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Jogador conectado!\n");
            conn->clientSocket = newSocket;
            conn->hasConnection = true;
        }
    }

    return SUCCESS;
}