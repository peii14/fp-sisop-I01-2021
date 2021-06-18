#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>


typedef struct commIn{
    long msg_type;
    int commandType;
    char msg[100];
}commRecieved;

void recieve(){
    struct sockaddr client_addr;
    int sockid;
    sockid = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = 8080;

    int bind_status = bind(sockid, (struct sockaddr *)&server, sizeof(server));

    int queue_limit = 50;
    listen(sockid, queue_limit);

    socklen_t addr_size = sizeof client_addr;
    int client_sockid = accept(sockid, (struct sockaddr *)&client_addr, &addr_size);

    commRecieved Recieved;
    int recv_status = recv(client_sockid, (commRecieved *)&Recieved, sizeof(Recieved), 0);

    printf("client msg %s \n", Recieved.msg);
    printf("client type %d\n", Recieved.commandType);
    close(sockid);
}
void sent(){
    key_t key = ftok("progfile", 65);
    int msgid;
    msgid = msgget(key, 0666 | IPC_CREAT);
    commRecieved Recieved;
    Recieved.msg_type = 1;
    printf("SENT");
    msgsnd(msgid, &Recieved, sizeof(Recieved), 0);

    msgctl(msgid, IPC_RMID, NULL);
}
int main(){
   while(1){
    recieve();
    sent();  
   }
}