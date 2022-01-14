#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX 10

//Да се напише програма на C, която създава сегмент обща памет с размер int инициализиран с 0
//поражда процес. 
//Двата процеса едновременно, през различен интервал от време, увеличават числото, докато стане 100
//извеждат на екрана номера на процеса си и новата стойност.

int main(int argc, char const *argv[]) {
    int id = shmget((key_t)IPC_PRIVATE, sizeof(int), IPC_CREAT | 0644);
    if(id == -1) {
        perror("shmget");
        return 1;
    }
    int* mem = (int*)shmat(id, NULL, 0);
    
    if((int)mem == -1) {
        perror("attach");
        return 1;
    }
    *mem = 0;

    int fork_res = fork();

    if(fork_res == -1) {
        perror("fork");
        return 1;
    } else if(fork_res == 0) { //child
        sleep(1);

        while(*mem < MAX) {
            (*mem)++;
            sleep(1);
        }

        printf("%d %d\n", getpid(), *mem);

        if(shmdt(mem) == -1) {
            perror("dt");
            return 1;
        }
    } else { //parent
        sleep(2);

        while(*mem < MAX) {
            (*mem)++;
            sleep(2);
        }

        printf("%d %d\n", getpid(), *mem);

        if(shmdt(mem) == -1) {
            perror("dt");
            return 1;
        }

        if(shmctl(id, IPC_RMID, NULL) == -1) {
            perror("rmid");
            return 1;
        }
    }

    return 0;
}