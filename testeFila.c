#include<stdlib.h>
 #include<stdio.h>
 #include<string.h>
 #include<unistd.h>
 #include<sys/types.h>
 #include<sys/ipc.h>
 #include<sys/msg.h>
 struct my_msg{
         long int msg_type;
         char some_text[51];
 };
 int main()
 {
    int running=1;
    int msgid;
    struct my_msg some_data;
    long int msg_to_rec=0;
    msgid=msgget((key_t)14534,0666|IPC_CREAT);
    while(running < 10){
        msgrcv(msgid,(void *)&some_data,51,msg_to_rec,0);
        running++;
        printf("Data received: %s\n",some_data.some_text); 
    }
    msgctl(msgid,IPC_RMID,0); 
 } 