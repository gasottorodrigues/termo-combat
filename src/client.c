#include "../include/client.h"

int cli_connectToLobby()
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Criar socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
    {
        perror("Erro ao criar socket");
        return -1;
    }

    // Definir endereço do servidor
    serv_addr.sin_family = AF_INET;   // IPv4
    serv_addr.sin_port = htons(PORT); // Definir a porta 8080

    // Converter endereço IPv4 e IPv6 de texto para binário
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Endereço inválido/ Endereço não suportado");
        return -1;
    }

    // Conectar ao servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == ERROR)
    {
        perror("Erro ao conectar ao servidor");
        return -1;
    }

    return sock;
}