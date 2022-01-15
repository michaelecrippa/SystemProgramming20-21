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
struct msgbuf { // must be delared
    long mtype;
    char mtext[1024];
};

int main(int argc, char const *argv[]) {
    int id = msgget((key_t)IPC_PRIVATE, IPC_CREAT | 0644);

    if(id == -1) {
        perror("ipc_creat");
        return 1;
    }

    struct msgbuf buf;
    buf.mtype = 1;

    strcpy(buf.mtext, "Some message");

    if(msgsnd(id, &buf, 1024, 1) == -1) {
        perror("send");
        return 1;
    }
    
    strcpy(buf.mtext, "Change message");

    if(msgrcv(id, &buf, 1024, 1, 0) < 0) {
        perror("receive");
        return 1;
    }

    printf("%s\n", buf.mtext);

    if(msgctl(id, IPC_RMID, NULL) == -1) {
        perror("RMID");
        return 1;
    } 

    return 0;
}
