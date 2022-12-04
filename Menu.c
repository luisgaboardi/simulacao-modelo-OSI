#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>   

#define LSIZ 1000
#define RSIZ 100

struct my_msg{  
    long int msg_type;  
    char messageBody[51];  
};

int menu();
void menuEnviarMensagem();
void enviarMensagem();
void receberMensagem();

int main(int argc, char const *argv[]) { return menu(); }

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
            menuEnviarMensagem();
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

void splice(char mensagem[], struct my_msg message, int msgid){
    int i = 0;
    while (i <= strlen(mensagem)){
        message.msg_type=1;  
        strncpy(message.messageBody, (mensagem)+i, 50);
        message.messageBody[50] = '\0';
        if(msgsnd(msgid,(void *)&message, 51,0)==-1) // msgsnd returns -1 if the message is not sent  
            {  
                printf("Msg not sent\n");  
            }  
        i+=51;
    }
}

void menuEnviarMensagem()
{
    system("clear");
    char caminhoNome[50], mensagem[3000] = "";
    printf("** Enviar mensagem **\n\n");
    printf("\nQual o caminho e nome do arquivo de texto que deseja enviar?\n> ");
    scanf("%s", caminhoNome);

    int msgid;  
    struct my_msg message;  

    msgid=msgget((key_t)14534,0666|IPC_CREAT);  
    if (msgid == -1) // -1 means the message queue is not created  
    {  
        printf("Error in creating queue\n");  
        exit(0);  
    }  
    
    // Ler o arquivo
    char line[RSIZ][LSIZ];
    int i = 0;
    FILE *ptr;
    ptr = fopen(caminhoNome, "r");
    if (NULL == ptr)
    {
        printf("Não foi possível ler o arquivo\n");
    }
    while (fgets(line[i], LSIZ, ptr))
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
    splice(mensagem, message, msgid);
    enviarMensagem();
}

void enviarMensagem()
{   
    char cmd[3000] = "";
    strcat(cmd, "gcc udpClient.c -o client && ./client ");
    strcat(cmd, "127.0.0.25 ");
    strcat(cmd, "5000");
    system(cmd);
}

void receberMensagem() {
    char cmd[100] = "";
    strcat(cmd, "gcc udpServer.c -o server && ./server ");
    strcat(cmd, "127.0.0.25 ");
    strcat(cmd, "5000");
    system(cmd);
}
