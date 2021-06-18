#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>


/*
requestType docs
1-create table
2-drop table
3-insert into
4-create db
5-select
*/ 

// #define PORT 8080
struct args{
    char* command[100];
    int wordCounter;
};

typedef struct commOut{
    long msg_type;
    int commandType; 
    char msg[100];
}CommOut;

void toDB(struct commOut sent,int iter,struct sockaddr_in server,int sockid){

    int connect_status = connect(sockid, (struct sockaddr *)&server, sizeof(server));
    int send_status = send(sockid, (void *)&sent, sizeof(sent), 0);
    printf("send with status %d with iter %d\n", send_status,iter);
    close(sockid);
}
int i=2;
int iter = 0;

void *comm_out_callback(void *vargp,struct sockaddr_in server,int sockid){

    CommOut *out = malloc(sizeof(CommOut));
    free(out);
    // printf("%s ini callback",((struct args*)vargp)->command);
    char* commandSplit;
    char tmp1[20] = "\0",tmp2[20] = "\0",temporary[100]="\0";
    strncpy(temporary,((struct args*)vargp)->command,sizeof(temporary));
    commandSplit = strtok(((struct args*)vargp)->command," ");
    strncpy(tmp1,commandSplit,sizeof(commandSplit));
    commandSplit =strtok(NULL," ");
    strncpy(tmp2,commandSplit,sizeof(commandSplit));
    strncpy(out->msg,temporary,sizeof(temporary));
    if(strcmp(tmp1,"CREATE") == 0 && strcmp(tmp2,"TABLE")==0){
        iter++;
        out->commandType = 1;
        printf("HELLOW1\n");
        if(fork()==0)
            toDB(*out,iter,server,sockid);
    }else if(strcmp(tmp1,"DROP") == 0 && strcmp(tmp2,"TABLE")==0){
        out->commandType = 2;
        printf("HELLOW2\n");
        if(fork()==0)
            toDB(*out,iter,server,sockid);
    }else if(strcmp(tmp1,"INSERT")==0&&strcmp(tmp2,"INTO")==0){
        out->commandType = 3;
        printf("HELLOW3\n");
        if(fork()==0)
            toDB(*out,iter,server,sockid);
    }else if(strcmp(tmp1,"CREATE")==0,strcmp(tmp2,"DATABASE")==0){
         out->commandType = 4;
        printf("HELLOW4\n");
        if(fork()==0)
            toDB(*out,iter,server,sockid);
    }else if(strcmp(tmp1,"SELECT")==0&& strcmp(tmp2,"*")==0){
        out->commandType = 5;
        printf("HELLOW4\n");
        if(fork()==0)
            toDB(*out,iter,server,sockid);
    }
    else
        printf("ERROR NIH\n");  
}

int main(int argc, char *argv[]){
    char input[100];
    int command = 1,n,sockid;
    struct args *Req = (struct args *)malloc(sizeof(struct args));
    struct sockaddr_in server;
    
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = 8080;
    while(1){
        n=0;
        while((input[n++] = getchar()) != '\n');
        strncpy(Req->command, input,sizeof(input)-1);
        comm_out_callback((void * )Req,server,sockid);
    }
    return 0;
    
}