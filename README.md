# Termo Combat - Jogo Online utilizando Sockets em C
## Desenvolvedores:

- Gabriel Sotto Rodrigues - 11800952
- Henrique Carobolante Parro - 11917987

## Sistema Operacional:

 - Linux Ubuntu LTS 24.04.1 (Utilizando Windows Subsistem Linux)


## Descrição

**Termo Combat** é um jogo multiplayer baseado em turnos, onde os jogadores competem para adivinhar palavras. Um jogador é designado como o "escolhedor" da palavra em cada rodada, enquanto os outros jogadores tentam adivinhar a palavra correta. O jogo continua até que um jogador atinja a pontuação necessária para vencer.

O jogo é dividido em duas partes principais:
- **Servidor (Host)**: Um jogador cria uma sala (lobby) e aguarda que outros jogadores se conectem.
- **Cliente (Jogador)**: Os outros jogadores se conectam ao servidor e participam da partida.

## Funcionalidades

- **Multiplayer**: Suporte para múltiplos jogadores em uma partida.
- **Escolha de palavras**: Um jogador escolhe a palavra em cada rodada, enquanto os outros tentam adivinhar.
- **Pontuação**: Os jogadores ganham pontos com base no número de tentativas corretas e erradas.
- **Interface de texto**: O jogo é jogado em um terminal, com feedback visual para as tentativas de adivinhação.

## Estrutura do Projeto

- **src/**: Contém os arquivos fonte do projeto.
- **include/**: Contém os arquivos de cabeçalho (.h) com as definições de tipos e funções.
- **build/**: Diretório onde os arquivos objeto (.o) serão gerados.
- **bin/**: Diretório onde o executável final será gerado.
- **Makefile**: Arquivo de automação para compilar o projeto.

## Dependências

- **GCC**: O projeto é compilado usando o GCC (GNU Compiler Collection).
- **Bibliotecas padrão do C**: O projeto utiliza bibliotecas padrão como `stdio.h`, `stdlib.h`, `string.h`, `pthread.h`, entre outras.

## Compilação

Para compilar o projeto, siga os passos abaixo:

1. Certifique-se de que você tem o GCC instalado. Você pode verificar isso executando o comando:

   ```bash
   gcc --version
   ```

2. Clone o repositório ou baixe os arquivos do projeto.

3. Navegue até o diretório raiz do projeto e execute o comando `make` para compilar o projeto:

   ```bash
   make
   ```

   Isso irá compilar todos os arquivos fonte e gerar o executável no diretório `bin/` com o nome `app`.

4. Se desejar limpar os arquivos objeto e o executável, execute:

   ```bash
   make clean
   ```

## Execução

### Como Servidor (Host)

1. Para iniciar o jogo como servidor (host), execute o comando abaixo:

   ```bash
   ./bin/app
   ```

2. No menu principal, escolha a opção para **Criar uma sala**. Você será solicitado a fornecer algumas configurações, como:
   - Pontuação necessária para vencer.
   - Tamanho das palavras.
   - Número de tentativas por rodada.
   - Número de jogadores.

3. Após configurar a sala, o servidor aguardará que os outros jogadores se conectem.

### Como Cliente (Jogador)

1. Para se conectar a uma sala existente, execute o comando abaixo:

   ```bash
   ./bin/app
   ```

2. No menu principal, escolha a opção para **Entrar em uma sala**. Você será solicitado a fornecer o endereço IP do servidor (host).

3. Após se conectar, você participará da partida e poderá começar a adivinhar as palavras.

## Estrutura do Código

### Principais Arquivos

- **server.c**: Contém a lógica do servidor, incluindo a criação de salas, gerenciamento de conexões e controle do fluxo do jogo.
- **client.c**: Contém a lógica do cliente, incluindo a conexão ao servidor e a interação do jogador com o jogo.
- **gameEngine.c**: Contém a lógica do jogo, como validação de tentativas, cálculo de pontuação e controle de rodadas.
- **graphics.c**: Contém funções para exibir menus e resultados no terminal.
- **main.c**: Ponto de entrada do programa, responsável por exibir o menu principal e iniciar o servidor ou cliente com base na escolha do usuário.

### Tipos e Estruturas

O projeto utiliza várias estruturas de dados para gerenciar o estado do jogo, como:

- **gameStateType**: Armazena o estado atual do jogo, incluindo a pontuação dos jogadores, a palavra atual e o número de tentativas.
- **lobbyConfigState**: Armazena as informações do lobby, como o número de jogadores conectados e os sockets de comunicação.
- **clientBufferType**: Armazena as informações de conexão do cliente, como o ID do jogador e o estado do jogo.

## Contribuição

Se você deseja contribuir com o projeto, sinta-se à vontade para abrir um pull request ou relatar problemas na seção de issues.

## Licença

Este projeto está licenciado sob a [GNU General Public License](LICENSE).

