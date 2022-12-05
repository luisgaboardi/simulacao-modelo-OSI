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

int getTamQuadro(){
    return tamQuadro;
}

int menu()
{
    int opcao = 0;
    while (opcao != 3)
    {
        //system("clear");
        printf("** MENU **\n\n");
        printf("(1) Enviar uma mensagem\n");
        printf("(2) Receber uma mensagem\n");
        printf("(3) Sair\n\n> ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            menuEnviarMensagens(tamQuadro);
            break;
        case 2:
            receberMensagem();
            break;
        default:
            break;
        }
    }
    return 0;
}

void menuEnviarMensagens()
{
    system("clear");

    char caminhoNome[100], mensagem[MAXSIZE] = "";
    printf("** Enviar mensagem **\n\n");
    printf("\nQual o caminho e nome do arquivo de texto que deseja enviar?\n> ");
    scanf("%s", caminhoNome);

    lerArquivo(caminhoNome);
    
    enviarQuadros();
}

void lerArquivo(char caminhoNome[])
{
    char line[1000][1000];
    char mensagem [100000];
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

void enviarQuadros()
{
    char cmd[100] = "";
    strcat(cmd, "gcc udpClient.c -o client && ./client ");
    strcat(cmd, "127.0.0.25 ");
    strcat(cmd, "5000");
    system(cmd);
}

void receberMensagem()
{    
    char temp[10];
    struct my_msg test;
    long int msg_to_rec = 0;

    system("clear");
    char cmd[100] = "";
    strcat(cmd, "gcc udpServer.c -o server && ./server ");
    strcat(cmd, "127.0.0.25 ");
    strcat(cmd, "5000 ");
    sprintf(temp, "%d", tamQuadro);
    strcat(cmd, temp);
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
