#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
//Да се напише програма на C, която синхронизира два родствени процеса чрез сигнали. 
//Двата процеса пишат текст на стандартния изход в следния ред - единия, другия, отново първия. 
//Процеса който пише пръв, изчаква няколко секунди преди да пише.
void handle(int param) {
    printf("second\n");
}

void handle1(int param) {
    printf("third\n");
}

int main(int argc, char** argv) {
    int fork_res = fork();

    if(fork_res == -1) {
        perror("fork");
        return 1;
    } else if(fork_res == 0) { //child
        struct sigaction act = { &handle1, 0, 0, 0};
        sigaction(SIGTERM ,&act , NULL);

        sleep(2);
        printf("first\n");
        int ppid = getppid();
        if(ppid == -1) {
            perror("ppid");
            return 1;
        }

        if(kill(ppid, SIGTERM) == -1) {
            perror("kill");
            return 1;
        }
        
        pause();
    } else { //parent
        struct sigaction act = { &handle, 0, 0, 0};
        sigaction(SIGTERM ,&act , NULL);

        pause();
        if(kill(fork_res, SIGTERM) == -1 ) {
            perror("kill");
            return 1;
        }
    }

    return 0;
}