#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

//Да се напише програма на C, която синхронизира два родствени процеса чрез съобщения. 
//Двата процеса пишат текст на стандартния изход в следния ред - единия, другия, отново първия. 
//Процеса който пише пръв, изчаква няколко секунди преди да пише.
struct msgbuf {
  long mtype;
  char mtext[16];
};
int main (int argc, char** argv) {
    int id = msgget((key_t)1234, IPC_CREAT | 0644);

    if(id == -1) {
        perror("msgget");
        return 0;
    }
    int fork_res = fork();

    if(fork_res == -1) {
        perror("fork");
        return 1;
    } else if(fork_res == 0) { //child
        struct msgbuf buf;
        strcpy(buf.mtext, "Message");
        buf.mtype = 1;

        sleep(2);
        printf("first\n");
        if(msgsnd(id, &buf, strlen(buf.mtext), 0) == -1) {
            perror("snd child");
            return 1;
        }

        if(msgrcv(id, &buf, 16, 2, 0) == -1) {
            perror("rcv child");
            return 0;
        }
        printf("third\n");

        if(msgctl(id, IPC_RMID, NULL) == -1) {
            perror("rmid");
            return 1;
        }    
    } else { //parent
        struct msgbuf buf;

        if(msgrcv(id, &buf, 16, 1, 0) == -1) {
            perror("rcv parent");
            return 1;
        }

        buf.mtype = 2;
        printf("second\n");

        strcpy(buf.mtext, "Message1");
        if(msgsnd(id, &buf, strlen(buf.mtext), 0) == -1) {
            perror("snd parent");
            return 1;
        }   
    }

    return 0;
}
