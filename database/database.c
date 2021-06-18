#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/msg.h>        

typedef struct commIn{
    long msg_type;
    int commandType;
    char msg[100];
}commRecieved;

int open_record(char *filename){
   int fd;
   fd = open(filename, O_CREAT | O_APPEND | O_RDWR, 0644);
   if(fd == -1)
      perror("open_record");
   return fd;
}

void close_record(int fd){
    close(fd);
}

int insert_record(int fd, commRecieved *rec) {
    int ret;
    ret = write(fd, rec, sizeof(commRecieved));
    return ret;
}

int get_record(int fd, commRecieved *rec, int key)
{
    int ret;
    while( ( ret = read(fd, rec, sizeof(commRecieved)) ) != -1){
        if(ret == 0){
            memset(rec, 0, sizeof(commRecieved)); 
            break;
            return ret;
        }
        else if (key == rec->commandType)
            return ret;
    }
    memset(rec, 0, sizeof(commRecieved)); 
    return ret;
}


//delete function
int delete_record(int fd, int key){
    int ret;
    commRecieved rec;
    off_t pos;
    pos = lseek(fd, 0, SEEK_SET);
    while( ( ret = read(fd, &rec, sizeof(commRecieved)) ) != -1){
            if(ret == 0){
                return ret;
            }
            else if (key == rec.commandType){
                lseek(fd, pos, SEEK_SET);
                rec.commandType = 0;
                ret = write(fd, &rec, sizeof(commRecieved));
                return ret;
            }
            pos = lseek(fd, 0, SEEK_CUR);
        }
    return ret;
}

int main(int argc, char *argv[]) {
    int fd;
    commRecieved Recieved;
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    msgrcv(msgid, &Recieved, sizeof(Recieved), 1, 0);
    printf(Recieved.msg);

    msgctl(msgid, IPC_RMID, NULL);
        
    if(Recieved.commandType == 1){
        char *table = strtok(Recieved.msg," ");
        for(int i=0;i<2;i++)
            table = strtok(NULL," ");
        fd = open_record(table);
    }else if(Recieved.commandType == 2){
        char *table = strtok(Recieved.msg," ");
        for(int i=0;i<2;i++)
            table = strtok(NULL," ");
        fd = open_record(table);
        delete_record(fd,&Recieved);
    }else if(Recieved.commandType == 3){
        char *table = strtok(Recieved.msg," ");
        for(int i=0;i<2;i++)
            table = strtok(NULL," ");
        fd = open_record(table);
        insert_record(fd,&Recieved);
    }else if(Recieved.commandType == 4){
        char *dir = strtok(Recieved.msg," ");
        for(int i=0;i<2;i++)
            dir = strtok(NULL," ");
        fd = open_record("DATA");
        mkdir(dir,0777);
    }

    //     /*print */
    //     if(argc > 2 && !strcmp(argv[1], "print")){
    //         get_record(fd, &rec, atoi(argv[2]));
    //         printf("key = %d\n", rec.key);
    //         printf("First = %s\n", rec.fname);
    //         printf("Last = %s\n", rec.lname);
    //         printf("Age = %d\n", rec.age);
    //     }
    // }


    return 0;
 } 