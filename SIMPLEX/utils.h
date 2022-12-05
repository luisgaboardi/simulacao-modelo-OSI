#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msg
{
    long int msg_type;
    char messageBody[10000];
};

int addFilaMensagens(char mensagem[], int tamQuadro, int key)
{
    int i = 0;
    int IDpacote = 0;
    char stringID[12] = " as";
    int tamHeader = 3;

    struct my_msg message;

    //Criar fila
    int msgid;

    msgid = msgget((key_t)key,0660 | IPC_CREAT);
    if (msgid == -1) // -1 means the message queue is not created
    {
        printf("Error in creating queue\n");
        exit(0);
    }
    char *mensagemPacote = (char *) calloc(tamQuadro, sizeof(char));
    while (i <= strlen(mensagem))
    {
        message.msg_type = 1;
        if (key == 12345){
            sprintf(stringID, "%.3d", IDpacote);
            strcpy(message.messageBody, stringID);
        }else if (i != 0){
            i+= tamHeader;
            strcpy(message.messageBody, "");
        }

        strncpy(mensagemPacote, (mensagem) + i, tamQuadro - tamHeader);
        strcat(message.messageBody, mensagemPacote);
        message.messageBody[tamQuadro] = '\0';
        if (msgsnd(msgid, (void *)&message, tamQuadro + 1, 0) == -1) // msgsnd returns -1 if the message is not sent
        {
            printf("Msg not sent\n");
        }
        i += tamQuadro - tamHeader;
        IDpacote++;
    }
    struct my_msg finalMessage;
    strcpy(finalMessage.messageBody, "roger roger");
    finalMessage.msg_type = 1;
    msgsnd(msgid, (void *)&finalMessage, tamQuadro + 1, 0);
    IDpacote = 0;

    return msgid;
}