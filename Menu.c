#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAXSIZE 10000

struct my_msg
{
    long int msg_type;
    char messageBody[MAXSIZE];
};

int menu();
void menuEnviarMensagem();
void enviarMensagem();
void receberMensagem();

int main(int argc, char const *argv[])
{
    if (argc < 2) /* Se o tamanho do quadro não for passado */
    {
        printf("uso correto: %s  <tam do quadro>\n", argv[0]);
        exit(1);
    }
    int tamQuadro = atoi(argv[1]);
    return menu(tamQuadro);
}

int menu(int tamQuadro)
{
    int opcao = 0;
    while (opcao != 3)
    {
        system("clear");
        printf("** MENU **\n\n");
        printf("(1) Enviar uma mensagem\n");
        printf("(2) Receber uma mensagem\n");
        printf("(3) Sair\n\n> ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            menuEnviarMensagem(tamQuadro);
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

void splice(char mensagem[], struct my_msg message, int msgid, int tamQuadro)
{
    int i = 0;
    int IDpacote = 0;
    char stringID[12];
    char mensagemPacote[tamQuadro];
    while (i <= strlen(mensagem))
    {
        message.msg_type = 1;
        sprintf(stringID, "%.3d", IDpacote);
        strncpy(mensagemPacote, (mensagem) + i, tamQuadro);
        strcpy(message.messageBody, stringID);
        strcat(message.messageBody, mensagemPacote);
        message.messageBody[tamQuadro] = '\0';
        if (msgsnd(msgid, (void *)&message, tamQuadro + 1, 0) == -1) // msgsnd returns -1 if the message is not sent
        {
            printf("Msg not sent\n");
        }
        i += tamQuadro + 1;
        IDpacote++;
    }
    struct my_msg finalMessage;
    strcpy(finalMessage.messageBody, "roger roger");
    finalMessage.msg_type = 1;
    msgsnd(msgid, (void *)&finalMessage, tamQuadro + 1, 0);
    IDpacote = 0;
}

void menuEnviarMensagem(int tamQuadro)
{
    system("clear");
    char caminhoNome[100], mensagem[MAXSIZE] = "";
    printf("** Enviar mensagem **\n\n");
    printf("\nQual o caminho e nome do arquivo de texto que deseja enviar?\n> ");
    scanf("%s", caminhoNome);

    int msgid;
    struct my_msg message;

    msgid = msgget((key_t)14534, 0666 | IPC_CREAT);
    if (msgid == -1) // -1 means the message queue is not created
    {
        printf("Error in creating queue\n");
        exit(0);
    }

    // Ler o arquivo
    char line[1000][1000];
    int i = 0;
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
        strcat(mensagem, line[i]);
    }
    fclose(ptr);
    // Gera identificador final
    splice(mensagem, message, msgid, tamQuadro);
    enviarMensagem();
}

void enviarMensagem()
{
    char cmd[100] = "";
    strcat(cmd, "gcc udpClient.c -o client && ./client ");
    strcat(cmd, "127.0.0.25 ");
    strcat(cmd, "5000");
    system(cmd);
}

void receberMensagem()
{
    char cmd[100] = "";
    strcat(cmd, "gcc udpServer.c -o server && ./server ");
    strcat(cmd, "127.0.0.25 ");
    strcat(cmd, "5000");
    system(cmd);
}
