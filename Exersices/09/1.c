#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

//Да се напише програма на C, която създава опашка, изпраща съобщение в нея, 
//след това го прочита и го извежда на стандартния изход.
struct msgbuf { // must be declared
    long mtype;
    char mtext[1024];
};

int main(int argc, char const *argv[]) {
    int id = msgget((key_t)IPC_PRIVATE, IPC_CREAT | 0644);

    if(id < 0) {
        perror("Error creating msg");
        return 0;
    }

    struct msgbuf buf;
    buf.mtype = 1;

    sprintf(buf.mtext, "Some message");

    if(msgsnd(id, &buf, 1024, 0) < 0) {
        perror("Error sending message!");
        return 0;
    }
    buf.mtext[0]= '\0';  

    if(msgrcv(id, &buf, 1024, 1, 0) < 0) {
        perror("Error receiving message!");
        return 0;
    }

    printf("%s", buf.mtext);

    if(msgctl(id, IPC_RMID, NULL) < 0) {
        perror("Error closing the queue!");
        return 0;
    } 

    return 0;
}
