#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "utils.h"

#define MAXSIZE 1000000

int tamQuadro;

int menu();
void menuEnviarMensagens();
void lerArquivo(char[]);
void enviarQuadros();
void receberMensagem();

int main(int argc, char const *argv[])
{
    if (argc < 2) /* Se o tamanho do quadro não for passado */
    {
        printf("uso correto: %s  <tam do quadro>\n", argv[0]);
        exit(1);
    }
    tamQuadro = atoi(argv[1]);
    return menu();
}

int menu()
{
    int opcao = 0;
    while (opcao != 2)
    {
        // system("clear");
        printf("** MENU **\n\n");
        printf("(1) Conectar\n");
        printf("(2) Sair\n\n> ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            menuEnviarMensagens(tamQuadro);
            break;
        default:
            break;
        }
    }
    return 0;
}

void menuEnviarMensagens()
{
    struct sockaddr_in serverAddress;
    int socketDescriptor;
    char ip1[20] = "127.0.0.25";
    char ip2[20] = "127.0.0.1";
    int port[2] = {5000, 4000};
    int flag_switch = 0;
    int user = 0;

    //system("clear");

    /* Criacao do socket UDP */
   socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0); /* AF_INET = utiliza IPV4, SOCK_DGRAM = utiliza UDP, 0 = utiliza protocolo IP */
    if (socketDescriptor < 0)
    {
        printf("nao pode abrir o socket \n");
        exit(1);
    }

    /* Preenchendo informacoes sobre o servidor */
    serverAddress.sin_family = AF_INET;                 /* Define protocolo IPV4 */
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Define o endereço IP */
    serverAddress.sin_port = htons(4000);      /* Define a porta */

    /* Fazendo um bind na porta local do servidor */
    if (bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) /* Se não foi possível linkar um endereço ao socket */
    {
        user = 1;
        flag_switch = 1;   
    }
    while (flag_switch != 2){
        if(flag_switch == 0){
            char caminhoNome[100], mensagem[MAXSIZE] = "";
            printf("** Enviar mensagem **\n\n");
            printf("\nQual o caminho e nome do arquivo de texto que deseja enviar?\n> ");
            scanf("%s", caminhoNome);

            lerArquivo(caminhoNome);
            close(socketDescriptor);
            if(user == 0){
                enviarQuadros(ip1, port[0]);
            }else{
                printf("AQUI?");
                enviarQuadros(ip2, port[1]);
            }
            flag_switch = 1;
        }else{
            if(user == 0){
                close(socketDescriptor);
                receberMensagem(ip2, port[1]);
            }else{
                close(socketDescriptor);
                receberMensagem(ip1, port[0]);
            }
            flag_switch = 0;
        }
    }
}
void lerArquivo(char caminhoNome[])
{
    char line[100][1000];
    char mensagem [10000];
    int i = 0;

    //Abre arquivo
    FILE *ptr;
    ptr = fopen(caminhoNome, "r");
    if (NULL == ptr)
    {
        printf("Não foi possível ler o arquivo\n");
    }
    while (fgets(line[i], 1000, ptr))
    {
        line[i][strlen(line[i])] = '\0';
        i++;
    }
    int tot = i;
    for (i = 0; i < tot; ++i)
    {
        if(i == 0){
            strcpy(mensagem, line[i]);
        }else{
            strcat(mensagem, line[i]);
        }
    }
    fclose(ptr);
    addFilaMensagens(mensagem, tamQuadro, 12345);
}

void enviarQuadros(char IP[], int port)
{   
    char temp[10];
    char cmd[100] = "";
    strcat(cmd, "gcc udpClient.c -o client && ./client ");
    strcat(cmd, IP);
    sprintf(temp, " %d", port);
    strcat(cmd, temp);
    system(cmd);
}

void receberMensagem(char IP[], int port)
{    
    struct my_msg test;
    long int msg_to_rec = 0;

    //system("clear");
    char new[10];
    char tam[10];
    char cmd[150] = "";
    strcat(cmd, "gcc udpServer.c -o server && ./server ");
    strcat(cmd, IP);
    sprintf(new, " %d", port);
    sprintf(tam, " %d", tamQuadro);
    strcat(cmd, new);
    strcat(cmd,tam);
    printf("%s\n", cmd);
    system(cmd);

    // //Criar fila
    int msgid;
    msgid = msgget((key_t)14435,0666 | IPC_CREAT);
    while(1){
        msgrcv(msgid, (void *)&test, MAXSIZE, msg_to_rec, 0);
        if(strcmp(test.messageBody, "roger roger") == 0){
            return;
        }
        printf("%s", test.messageBody);
    }
}
