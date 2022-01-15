#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

struct msgbuf {
    long mtype;
    char mtext[16];
};

//Да се напише програма на C, която създава опашка и поражда процес. 
//Единия процес чете съдържанието на файл, чието име е подадено като параметър в командния ред и го праща в опашката. 
//Другия процес получава съобщения от опашката и ги извежда на стандартния изход.
int main(int argc, char const *argv[]) {
    int id = msgget((key_t)IPC_PRIVATE, IPC_CREAT | 0644);

    if(id == -1) {
        perror("ipc_crea");
        return 1;
    }

    int fork_res = fork();

    if(fork_res < 0) {
        perror("Fork");
        return 1;
    } else if(fork_res == 0) { //child
        if(argc < 2) {
            perror("Missing args!");
            return 1;
        }

        char * fileName = argv[1];
        int fid = open(fileName, O_RDONLY);

        if(fid == -1) {
            perror("File missing");
            return 1;
        }

        struct msgbuf buf;
        buf.mtype = 1;

        int counter = 0;
        char buff[1];
        while(read(fid, &buf, 1))
        {
            counter++;
            buf.mtext[counter] = buff[0];
        }

        if(msgsnd(id, &buf,buf.mtext, 0) == -1) {
            perror("Send");
            return 1;
        }       
    } else { //parent
        struct msgbuf buf;
        buf.mtype = 1;
        buf.mtext[0] = '\0';
        sleep(5);
        if(msgrcv(id, &buf, 1024, buf.mtype, 0) == -1) {
            perror("Rcv");
            return 1;
        }

        printf("%s\n", buf.mtext);
        if(msgctl(id, IPC_RMID, NULL) == -1) {
            perror("close");
            return 1;
        }
    }


    return 0;
}
