/****************************************/
/* FGA/EngSofrware/FRC - udpServer.c    */
/* Prof. Fernando W. Cruz               */
/****************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdlib.h> 

int main(int argc, char *argv[])
{
  int MAX_MSG = 100;
  int socketDescriptor;
  struct sockaddr_in clientAddress;  /* Vai conter identificacao do cliente */
  socklen_t clientAdressSize;
  struct sockaddr_in serverAddress; /* Vai conter identificacao do servidor local */
  char msg[MAX_MSG];          /* Buffer que armazena os dados que chegaram via rede */

  if (argc < 3) /* Se o IP e a porta não foram passados (qtd de argumento inferior a 3) */
  {
    printf("Digite IP e Porta para este servidor\n");
    exit(1);
  }
  /* Criacao do socket UDP */
  socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0); /* AF_INET = utiliza IPV4, SOCK_DGRAM = utiliza UDP, 0 = utiliza protocolo IP */ 
  if (socketDescriptor < 0)
  {
    printf("%s: nao pode abrir o socket \n", argv[0]);
    exit(1);
  }

  /* Preenchendo informacoes sobre o servidor */
  serverAddress.sin_family = AF_INET; /* Define protocolo IPV4 */
  serverAddress.sin_addr.s_addr = inet_addr(argv[1]); /* Define o endereço IP */
  serverAddress.sin_port = htons(atoi(argv[2])); /* Define a porta */

  /* Fazendo um bind na porta local do servidor */
  if (bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) /* Se não foi possível linkar um endereço ao socket */
  {
    printf("%s: nao pode fazer bind na porta %s \n", argv[0], argv[2]);
    exit(1);
  }

  printf("%s: esperando por dados no IP: %s, porta UDP numero: %s\n", argv[0], argv[1], argv[2]); // Server, endereço ip e porta

  /* Este servidor entra num loop infinito esperando dados de clientes */
  while (1)
  {

    /* inicia o buffer */
    memset(msg, 0x0, MAX_MSG);
    clientAdressSize = sizeof(clientAddress);
    /* recebe a mensagem  */
    if (recvfrom(socketDescriptor, msg, MAX_MSG, 0, (struct sockaddr *)&clientAddress, &clientAdressSize) < 0) /* se não recebe a mensagem  */
    {
      printf("%s: nao pode receber dados\n", argv[0]);
      continue;
    }

    /* imprime a mensagem recebida na tela do usuario */
    printf("\nMENSAGEM RECEBIDA\n\n");
    printf("Informações do Servidor:\n");
    printf("   Protolo de rede: UDP\n");
    printf("   Endereço de IP : %s\n", inet_ntoa(serverAddress.sin_addr));
    printf("   Porta          : %u\n", ntohs(serverAddress.sin_port));

    printf("\nInformações do Cliente:\n");
    printf("   Endereço de IP : %s\n", inet_ntoa(clientAddress.sin_addr));
    printf("   Porta          : %u\n", ntohs(clientAddress.sin_port));
    printf("   Mensagem       : %s\n",  msg);
    printf("\n--------------------------------------\n");

  } /* fim do while */
  return 0;

} /* fim do programa */
