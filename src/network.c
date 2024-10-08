// //--------------------- CLIENTE -----------------------------------
// //Criacao do socket AF_INET indica que é IPv4, SOCK_STREAM indica que é orientado a conexão (TCP).
// //c_socket = socket(AF_INET, SOCK_STREAM, 0);
// //if(c_socket == -1){ O socket eh um inteiro
// //    printf("Erro ao criar o socket\n");
// //    exit(EXIT_FAILURE);
// //}

// //Se conceta ao endereco de ip coletado em get loby code
// // s_addr eh uma struct sockaddr_in s_addr; ela esta definidad em <arpa/inet.h>
// //while(1){
// //    char ip[16] = {0};
// //    if(get_lobby_code(ip)){
// //        s_addr.sin_family = AF_INET;  Define o tipo de protocolo (IPv4).
// //        s_addr.sin_port = htons(8080);  Define a porta (8080).
// //        s_addr.sin_addr.s_addr = inet_addr(ip);  Converte o IP em uma estrutura adequada para a comunicação de rede
// //
// //        if(connect(c_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) != -1){ Tenta conectar o socket ao servidor
// //            break;
// //        } else {
// //            printf("Nao foi possivel conectar ao servidor. Tente novamente.\n");
// //        }
// //    }
// //}

// /// FUncao para receber as informacoes do jogo
// // recv_last_play(int c_socket, lp *last_play)

// /// FUncao para enviar as informarcoes do jogo
// //  send_last_play(int c_socket, lp *last_play)

// //-------------------------- SERVIDOR ------------------------------

// //server
// //int s_socket, c_sockets[game->p_num];  define os sockets
// //trata o jogo

// //Enderecamento do socket
// //struct sockaddr_in s_addr, c_addr;
// //s_addr.sin_family = AF_INET;
// //s_addr.sin_port = htons(8080);
// //s_addr.sin_addr.s_addr = INADDR_ANY;

// //Asocia o endereco de ip ao socket criado, faz o bind
// //if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1){
// //    printf("Erro! Falha ao fazer o bind\n");
// //    exit(EXIT_FAILURE);
// //}

// //Coloca o servidor em modo de escurta
// //if(listen(s_socket, game->p_num - 1) == -1){
// //    printf("Erro! Falha ao escutar uma conexao\n");
// //    exit(EXIT_FAILURE);
// //}

// // Cria uma lista de eventos em poolfd
// //struct pollfd poll_fds[game->p_num];
// //poll_fds[0].fd = s_socket; poll_fds é um array de estruturas pollfd que o poll() usará para monitorar eventos em vários descritores de arquivos, como o socket do servidor.
// //poll_fds[0].events = POLLIN; POLLIN indica que o poll() observará quando houver dados prontos para serem lidos (novas conexões).

// // thread_info eh uma struct com as informacoes da thread

// // declara as threads do host e dos clientes
// //Ele cria pthreads
// //pthread_t h_thread;
// //pthread_t c_threads[game->p_num - 1];
// //
// //// inicializa as informacoes passadas para as threads
// //thread_info ht_info;
// //ht_info.game = game;
// //ht_info.socket_id = -1;
// //thread_info ct_info[game->p_num - 1];
// //for(int i = 0; i < game->p_num - 1; i++){
// //    ct_info[i].game = game;
// //}
// //
// //ht_info.player_id = 0;
// //Aqui ele pega a thread junto com a struct que armazena as informacoes da threadh e as cria!
// //pthread_create(&h_thread, NULL, host_handler, &ht_info);

// //while(!start){
// //		// Aceitar uma conexao
// //		int num_events = poll(poll_fds, game->p_num + 1, 200);
// //        	if (num_events == -1) {
// //            		perror("poll");
// //            		exit(EXIT_FAILURE);
// //        	}
// //
// //        	// Confere se o socket que esta ouvindo recebeu uma requisicao
// //        	if ((poll_fds[0].revents & POLLIN) && game->p_connected < game->p_num - 1) {
// //
// //            		// Aceita uma nova conexao com um cliente
// //            		c_sockets[game->p_connected] = accept(s_socket, (struct sockaddr *)&c_addr, &c_len);
// //            		if (c_sockets[game->p_connected] == -1) {
// //                		printf("Erro ao aceitar a conexao\n");
// //            		} else {
// //                		printf("\nJogador conectado!\n");
// //				printf("\nPara iniciar o jogo, aperte 0\n");
// //
// //                		// Cria uma nova thread para lidar com o cliente
// //				ct_info[game->p_connected].player_id = game->p_connected + 1;
// //				ct_info[game->p_connected].socket_id = c_sockets[game->p_connected];
// //                		if (pthread_create(&c_threads[game->p_connected], NULL, client_handler, &(ct_info[game->p_connected])) != 0) {
// //                    			perror("Erro ao criar a thread\n");
// //					exit(-1);
// //                		} else {
// //                    			(game->p_connected)++;
// //                		}
// //            		}
// //		}
// //	}
// //
// //	// enquanto o jogo ainda nao acabou
// //	while(!game->g_ended){
// //		// entra na zona critica
// //		pthread_mutex_lock(&mutex);
// //		// se a informacao ainda nao tiver sido processada
// //		if(!processed){
// //
// //			// Faz as verificacoes relativas a jogada atual
// //			if(win(game, last_play.symb)){        // se ganhou
// //
// //			pthread_cond_broadcast(&cond); 					// avisa os clientes que as informacoes foram processadas
// //		}
// //		pthread_mutex_unlock(&mutex);
// //		// saindo da zona critica
// //	}
// //
// //	// espera todas as threads terminarem o processamento
// //	pthread_join(h_thread, NULL);
// //	for(int i = 0; i <  game->p_connected; i++){
// //		pthread_join(c_threads[i], NULL);
// //	}
// //
// //	// fecha os sockets
// //	for(int i = 0; i < game->p_connected; i++){
// //		close(c_sockets[i]);
// //	}
// //	close(s_socket);
// //	pthread_cond_destroy(&cond);
// //
// //}

// //// Thread do host
// //void *host_handler(void *arg){
// //	thread_info *h_info = ((thread_info *) arg);   				// informacoes basicas do host
// //
// //	// atribui dentro da thread o simbolo do host
// //	h_info->game->p_list[0].simb = (33 + h_info->player_id);
// //	printf("\nSeu caracter eh: '%c'\n\n", h_info->game->p_list[0].simb);
// //
// //
// //	// Espera o jogador digitar '0' pra comecar o jogo
// //	printf("\nPara iniciar o jogo, aperte 0\n");
// //	while(1){
// //		int aux;
// //		scanf("%d", &aux);
// //		if(!aux){
// //			start = 1;
// //			break;
// //		}
// //	}
// //
// //	// Logica de controle da thread
// //	// Enquanto o jogo nao terminar
// //	while(!last_play.end){
// //		// entra na zona critica
// //		pthread_mutex_lock(&mutex);
// //
// //		// mostra as informacoes do jogador
// //		mostra(h_info->game);
// //
// //		// O tabuleiro eh exibido enquanto nao for a vez do jogador jogar
// //		while((h_info->game->next_player != h_info->player_id || !processed) && last_play.end == 0){
// //			// poem em espera pela condicao
// //			pthread_cond_wait(&cond, &mutex);
// //
// //			// assim que volta da espera, exibe a atualizacao do tabuleiro
// //			mostra(h_info->game);
// //		}
// //		// se o jogo tiver acabado
// //		if(last_play.end != 0){
// //			// libera o mutex e sai do loop
// //			pthread_mutex_unlock(&mutex);
// //			break;
// //		}
// //
// //		// Avisa o usuario que eh a vez dele jogar
// //		printf("Eh sua vez! Digite sua jogada no formato 'N N', linha por coluna. Exemplo: 5 3\n");
// //
// //		// Recebe a jogada do usuario (jah validada)
// //		int row, col;
// //		while(1){
// //			scanf("%d %d", &row, &col);
// //			// testa se a jogada eh valida
// //			if(row <= h_info->game->b_size && col <= h_info->game->b_size && col > 0 && row > 0 && h_info->game->board[row - 1][col - 1] == '-'){
// //				break;
// //			}
// //			printf("Jogada invalida!\n");
// //		}
// //		// Aplica a jogada
// //		jogapessoa(h_info->game, h_info->game->p_list[h_info->player_id].simb, row, col);
// //
// //		// atualiza a ultima jogada
// //		last_play.col = col;
// //		last_play.row = row;
// //		last_play.symb = h_info->game->p_list[h_info->player_id].simb;
// //
// //		h_info->game->next_player = (h_info->game->next_player + 1) % (h_info->game->p_connected + 1);  	// atualiza o proximo jogador
// //		last_play.next_player = h_info->game->next_player;
// //
// //		// define como nao processado
// //		processed = 0;
// //
// //		pthread_mutex_unlock(&mutex);
// //		// sai da regiao critica
// //	}
// //
// //	// Jogo acabou
// //	printf("\nFim de jogo!\n");
// //
// //	mostra(h_info->game);
// //
// //	switch(last_play.end){
// //		case 1:
// //                         if(last_play.symb == h_info->game->p_list[0].simb){
// //				printf("\n Voce venceu! Parabens!\n");
// //			 }else{
// //				 printf("\nVoce perdeu! Jogador %c venceu!\n", last_play.symb);
// //			 }
// //			break;
// //		case -1:
// //			printf("\nEmpate!\n");
// //			break;
// //	}
// //
// //	return NULL;
// //}

// //// Thread do cliente
// //void *client_handler(void *arg){
// //	// Informacoes passadas pela thread
// //	thread_info *c_info = ((thread_info *) arg);
// //
// //	// atribui um simbolo ao jogador
// //	c_info->game->p_list[c_info->player_id].simb = (char) (33 + c_info->player_id);
// //
// //	// atualiza a estrutura de dados basica a ser passada ao cliente
// //	client_basic_info.player_id = c_info->player_id;
// //	client_basic_info.player_symbol = (33 + c_info->player_id);
// //	printf("%d\n", c_info->player_id);
// //
// //	// envia as informacoes basicas
// //	if(!send_basic_info(c_info->socket_id, &client_basic_info)){
// //		printf("Erro de comunicacao!\n");
// //		exit(-1);
// //	}
// //	// espera o comeco do jogo
// //	while(!start);
// //	// inicializa a ultima jogada
// //	if(!send_last_play(c_info->socket_id, &last_play)){
// //		printf("Erro de comunicacao!\n");
// //		exit(-1);
// //	}
// //
// //	// logica de controle da comunicacao
// //	// enquanto nao acaba o jogo
// //	while(!last_play.end){
// //		//Entra na regiao critica
// //		pthread_mutex_lock(&mutex);
// //
// //		// Envia o tabuleiro atualizado
// //		if(!send_last_play(c_info->socket_id, &last_play)){
// //			printf("Erro de comunicacao!\n");
// //			exit(-1);
// //		}
// //
// //		// O tabuleiro eh exibido enquanto nao for a vez do jogador jogar
// //		while((c_info->game->next_player != c_info->player_id || !processed) && !last_play.end){
// //
// //			// Espera a condicao ser atingida
// //			pthread_cond_wait(&cond, &mutex);
// //
// //			// Envia a ultima jogada ao cliente
// //			if(!send_last_play(c_info->socket_id, &last_play)){
// //				printf("Erro de comunicacao!\n");
// //				exit(-1);
// //			}
// //		}
// //		// Confere se o jogo nao acabou enquanto esperava pela condicao
// //		if(last_play.end != 0){
// //			// Libera o mutex
// //			pthread_mutex_unlock(&mutex);
// //			break;
// //		}
// //
// //		// Recebe a jogada do usuario (jah validada)
// //		if(!recv_last_play(c_info->socket_id, &last_play)){
// //			printf("Erro de comunicacao!\n");
// //			exit(-1);
// //		}
// //
// //		// Aplica a jogada
// //		jogapessoa(c_info->game, c_info->game->p_list[c_info->player_id].simb, last_play.row, last_play.col);
// //
// //		c_info->game->next_player = (last_play.next_player + 1) % (c_info->game->p_connected + 1);  	// atualiza o proximo jogador
// //		last_play.next_player = c_info->game->next_player;
// //
// //		// Define o tabuleiro como nao processado
// //		processed = 0;
// //
// //		pthread_mutex_unlock(&mutex);
// //		// Saindo da regiao critica
// //	}
// //
// //	// Envia o ultimo tabuleiro atualizado
// //	if(!send_last_play(c_info->socket_id, &last_play)){
// //		printf("Erro de comunicacao!\n");
// //		exit(-1);
// //	}
// //	return 0;
// //}

// // -------------------- CODIGO SERVER GPT ---------------------
// // server.c
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <poll.h>

// #define PORT 8080
// #define MAX_CLIENTS 10
// #define BUFFER_SIZE 1024

// int main() {
//     int server_socket, client_socket, max_clients = MAX_CLIENTS;
//     struct sockaddr_in server_addr, client_addr;
//     socklen_t client_addr_len = sizeof(client_addr);
//     struct pollfd poll_fds[MAX_CLIENTS];
//     char buffer[BUFFER_SIZE];

//     // Criação do socket do servidor
//     server_socket = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_socket == -1) {
//         perror("Erro ao criar o socket");
//         exit(EXIT_FAILURE);
//     }

//     // Configuração do endereço do servidor
//     memset(&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = INADDR_ANY;
//     server_addr.sin_port = htons(PORT);

//     // Bind do servidor
//     if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
//         perror("Erro no bind");
//         close(server_socket);
//         exit(EXIT_FAILURE);
//     }

//     // Escutar conexões
//     if (listen(server_socket, max_clients) < 0) {
//         perror("Erro no listen");
//         close(server_socket);
//         exit(EXIT_FAILURE);
//     }

//     printf("Servidor iniciado e esperando por conexões...\n");

//     // Inicializar a lista de sockets de cliente
//     for (int i = 0; i < max_clients; i++) {
//         poll_fds[i].fd = -1;  // Inicializa o socket como não válido
//     }

//     // Adiciona o socket do servidor na lista de eventos de poll
//     poll_fds[0].fd = server_socket;
//     poll_fds[0].events = POLLIN;

//     while (1) {
//         // Monitorar os sockets
//         int activity = poll(poll_fds, max_clients, -1);
//         if (activity < 0) {
//             perror("Erro no poll");
//             close(server_socket);
//             exit(EXIT_FAILURE);
//         }

//         // Verificar se há uma nova conexão
//         if (poll_fds[0].revents & POLLIN) {
//             client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
//             if (client_socket < 0) {
//                 perror("Erro no accept");
//                 close(server_socket);
//                 exit(EXIT_FAILURE);
//             }

//             printf("Novo cliente conectado: %d\n", client_socket);

//             // Adicionar novo cliente à lista de clientes
//             for (int i = 1; i < max_clients; i++) {
//                 if (poll_fds[i].fd == -1) {
//                     poll_fds[i].fd = client_socket;
//                     poll_fds[i].events = POLLIN;
//                     break;
//                 }
//             }
//         }

//         // Verificar cada cliente para ver se há uma mensagem
//         for (int i = 1; i < max_clients; i++) {
//             if (poll_fds[i].fd != -1 && poll_fds[i].revents & POLLIN) {
//                 memset(buffer, 0, BUFFER_SIZE);
//                 int bytes_read = read(poll_fds[i].fd, buffer, sizeof(buffer));

//                 // Se a conexão foi encerrada
//                 if (bytes_read == 0) {
//                     printf("Cliente %d desconectou\n", poll_fds[i].fd);
//                     close(poll_fds[i].fd);
//                     poll_fds[i].fd = -1;
//                 } else {
//                     printf("Mensagem do cliente %d: %s\n", poll_fds[i].fd, buffer);

//                     // Enviar a mensagem para todos os outros clientes
//                     for (int j = 1; j < max_clients; j++) {
//                         if (poll_fds[j].fd != -1 && poll_fds[j].fd != poll_fds[i].fd) {
//                             write(poll_fds[j].fd, buffer, strlen(buffer));
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     close(server_socket);
//     return 0;
// }

// // ------------------------- CODIGO CLIENT GPT ---------------------------------------------

// // client.c
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <poll.h>

// #define PORT 8080
// #define BUFFER_SIZE 1024

// int main() {
//     int client_socket;
//     struct sockaddr_in server_addr;
//     char buffer[BUFFER_SIZE];
//     struct pollfd poll_fds[2];  // 0 = stdin, 1 = socket

//     // Criação do socket
//     client_socket = socket(AF_INET, SOCK_STREAM, 0);
//     if (client_socket == -1) {
//         perror("Erro ao criar o socket");
//         exit(EXIT_FAILURE);
//     }

//     // Configuração do endereço do servidor
//     memset(&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(PORT);

//     if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
//         perror("Endereço inválido");
//         exit(EXIT_FAILURE);
//     }

//     // Conectar ao servidor
//     if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
//         perror("Erro ao conectar ao servidor");
//         close(client_socket);
//         exit(EXIT_FAILURE);
//     }

//     printf("Conectado ao servidor!\n");

//     // Configurar o poll para stdin e socket
//     poll_fds[0].fd = 0;  // Entrada padrão (teclado)
//     poll_fds[0].events = POLLIN;

//     poll_fds[1].fd = client_socket;  // Socket do cliente
//     poll_fds[1].events = POLLIN;

//     while (1) {
//         // Esperar por eventos
//         poll(poll_fds, 2, -1);

//         // Se uma mensagem foi digitada no teclado
//         if (poll_fds[0].revents & POLLIN) {
//             memset(buffer, 0, BUFFER_SIZE);
//             read(0, buffer, BUFFER_SIZE);
//             write(client_socket, buffer, strlen(buffer));
//         }

//         // Se uma mensagem foi recebida do servidor
//         if (poll_fds[1].revents & POLLIN) {
//             memset(buffer, 0, BUFFER_SIZE);
//             int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
//             if (bytes_read == 0) {
//                 printf("Servidor desconectado\n");
//                 break;
//             }
//             printf("Mensagem recebida: %s\n", buffer);
//         }
//     }

//     close(client_socket);
//     return 0;
// }
