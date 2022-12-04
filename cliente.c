#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LSIZ 128
#define RSIZ 10

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
        system("clear");
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

void menuEnviarMensagem()
{
    system("clear");
    char caminhoNome[50], mensagem[3000] = "";
    printf("** Enviar mensagem **\n\n");
    printf("\nQual o caminho e nome do arquivo de texto que deseja enviar?\n> ");
    scanf("%s", caminhoNome);

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
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    int tot = i;
    for (i = 0; i < tot; ++i)
    {
        strcat(mensagem, line[i]);
    }

    // Definir tamanho do pacote

    enviarMensagem(mensagem);
}

void enviarMensagem(char mensagem[3000])
{
    char cmd[3000] = "";
    strcat(cmd, "gcc udpClient.c -o client && ./client ");
    strcat(cmd, "127.0.0.25 ");
    strcat(cmd, "5000 \"");
    strcat(cmd, mensagem);
    strcat(cmd, "\"");
    system(cmd);
}

void receberMensagem() {
    char cmd[100] = "";
    strcat(cmd, "gcc udpServer.c -o server && ./server ");
    strcat(cmd, "127.0.0.25 ");
    strcat(cmd, "5000");
    system(cmd);
}
