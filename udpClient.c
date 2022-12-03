/***********************************/
/* FGA/EngSofware/FRC  udpClient.c */
/* Prof. Fernando W Cruz           */
/***********************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <stdlib.h>

#define MAX_MSG 100

int main(int argc, char *argv[])
{
  int socketDescriptor;
  struct sockaddr_in ClientAddress;  /* dados do cliente local   */
  struct sockaddr_in serverAddress; /* dados do servidor remoto */

  /* confere o numero de argumentos passados para o programa */
  if (argc < 3) /* Se o IP e a porta não foram passados (qtd de argumento inferior a 3) */
  {
    printf("uso correto: %s <ip_do_servidor> <porta_do_servidor> <dado1> ... <dadoN> \n", argv[0]);
    exit(1);
  }

  /* Preenchendo as informacoes de identificacao do remoto */
  serverAddress.sin_family = AF_INET; /* Protocolo IPV4 */
  serverAddress.sin_addr.s_addr = inet_addr(argv[1]); /* O endereço IP */
  serverAddress.sin_port = htons(atoi(argv[2])); /* A porta */

  /* Preenchendo as informacoes de identificacao do cliente */
  ClientAddress.sin_family = AF_INET; /* Define protocolo IPV4 */
  ClientAddress.sin_addr.s_addr = htonl(INADDR_ANY); /* Define o endereço IP */
  ClientAddress.sin_port = htons(0); /* usa porta livre entre (1024-5000)*/

  /* Criando um socket. Nesse momento a variavel       */
  /* socketDescriptor contém apenas dados sobre familia e protocolo  */
  socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0); /* AF_INET = utiliza IPV4, SOCK_DGRAM = utiliza UDP, 0 = utiliza protocolo IP */ 
  if (socketDescriptor < 0)
  {
    printf("%s: nao pode abrir o socket \n", argv[0]);
    exit(1);
  }

  /* Relacionando o socket socketDescriptor com a estrutura ClientAddress */
  /* Depois do bind, socketDescriptor faz referencia a protocolo local, ip local e porta local */
  if (bind(socketDescriptor, (struct sockaddr *)&ClientAddress, sizeof(ClientAddress)) < 0)
  {
    printf("%s: nao pode fazer um bind da porta\n", argv[0]);
    exit(1);
  }

  printf("Informações para envio de mensagem:\n\n");
    printf("   Protolo de rede         : UDP\n");
    printf("   Endereço de IP (Cliente): %s\n", inet_ntoa(ClientAddress.sin_addr));
    printf("   Porta          (Cliente): %u\n", ntohs(ClientAddress.sin_port));
    printf("-------------------------------------------------\n");
    printf("   Endereço de IP (Cliente): %s\n", argv[1]);
    printf("   Porta          (Cliente): %s\n", argv[2]);

  /* Enviando um pacote para cada parâmetro informado */
  for (int param = 3; param < argc; param++)
  {
    if (sendto(socketDescriptor, argv[param], strlen(argv[param]), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
      printf("%s: nao pode enviar dados %d \n", argv[0], param - 1);
      close(socketDescriptor);
      exit(1);
    }
    printf("Enviando parametro %d: %s\n", param - 2, argv[param]);
  } /* fim do for (laco) */
  return 1;
} /* fim do programa */
