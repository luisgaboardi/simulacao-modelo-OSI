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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

struct my_msg
{
  long int msgID;
  char messageBody[10000];
};
#define MAX_MSG 100000

int main(int argc, char *argv[])
{
  int sockerDescriptorClient, sockerDescriptorServer;
  struct sockaddr_in ClientAddress; /* dados do cliente local   */
  struct sockaddr_in serverAddress; /* dados do servidor remoto */

  /* confere o numero de argumentos passados para o programa */
  if (argc < 3) /* Se o IP e a porta não foram passados (qtd de argumento inferior a 3) */
  {
    printf("uso correto: %s <ip_do_servidor> <porta_do_servidor> <dado1> ... <dadoN> \n", argv[0]);
    exit(1);
  }

  /* Preenchendo as informacoes de identificacao do remoto */
  serverAddress.sin_family = AF_INET;                 /* Protocolo IPV4 */
  serverAddress.sin_addr.s_addr = inet_addr(argv[1]); /* O endereço IP */
  serverAddress.sin_port = htons(atoi(argv[2]));      /* A porta */

  /* Preenchendo as informacoes de identificacao do cliente */
  ClientAddress.sin_family = AF_INET;                /* Define protocolo IPV4 */
  ClientAddress.sin_addr.s_addr = htonl(0x7f000001); /* Define o endereço IP */
  ClientAddress.sin_port = htons(4000);              /* Define porta */

  /* Criando um socket. Nesse momento a variavel       */
  /* sockerDescriptorClient contém apenas dados sobre familia e protocolo  */
  sockerDescriptorClient = socket(AF_INET, SOCK_DGRAM, 0); /* AF_INET = utiliza IPV4, SOCK_DGRAM = utiliza UDP, 0 = utiliza protocolo IP */
  if (sockerDescriptorClient < 0)
  {
    printf("%s: nao pode abrir o socket \n", argv[0]);
    exit(1);
  }
  /* Relacionando o socket sockerDescriptorClient com a estrutura ClientAddress */
  /* Depois do bind, sockerDescriptorClient faz referencia a protocolo local, ip local e porta local */
  if (bind(sockerDescriptorClient, (struct sockaddr *)&ClientAddress, sizeof(ClientAddress)) < 0)
  {
    printf("%s: nao pode fazer um bind da porta\n", argv[0]);
    exit(1);
  }

  int msgid;
  struct my_msg message;
  char ACK[3];
  long int msg_to_rec = 0;
  msgid = msgget((key_t)12345, 0666 | IPC_CREAT);
  int flagRecebido = 1;
  /* Enviando um pacote para cada parâmetro informado */
  while (1)
  {
    if (flagRecebido)
    {
      msgrcv(msgid, (void *)&message, MAX_MSG, msg_to_rec, 0);
      if (strcmp(message.messageBody, "roger roger") == 0)
      {
        sendto(sockerDescriptorClient, message.messageBody, strlen(message.messageBody), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        return 1;
      }
      printf("Quadro Enviado!\n");
    }
    if (sendto(sockerDescriptorClient, message.messageBody, strlen(message.messageBody), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
      printf("%s: nao pode enviar dados %s \n", argv[0], message.messageBody);
      close(sockerDescriptorClient);
      exit(1);
    }

    clock_t before = clock();
    socklen_t serverAdressSize = sizeof(serverAddress);
    flagRecebido = 0;
    while (flagRecebido == 0) // enquanto não recebe confirmação
    {
      before = clock();
      if (recvfrom(sockerDescriptorClient, ACK, MAX_MSG, 0, (struct sockaddr *)&serverAddress, &serverAdressSize) >= 0)
      {
        if (strcmp(ACK, "OK") == 0 && (clock() - before) < 25)
        {
          flagRecebido = 1;
          break;
        }
        else
        {
          printf("ERROR: Servidor não confirmou recebimento a tempo.\n\n");
          sendto(sockerDescriptorClient, message.messageBody, strlen(message.messageBody), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        }
      }
    }
  }
  return 1;
} /* fim do programa */
