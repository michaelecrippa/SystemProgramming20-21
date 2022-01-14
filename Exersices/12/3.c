#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

//Да се напише програма на C, която създава сегмент обща памет с размер int, 
//инициализиран с 0 и поражда процес.
//Двата процеса, едновременно, в продължение на еднакъв интервал от време (например 5 секунди) променят числото, 
//като единия процес го увеличава с 1, а другия го намалява с 1. 
//Накрая и двата процеса извеждат стойността му.

int main(int argc, char** argv) {
    int id = shmget((key_t)1234, sizeof(int), IPC_CREAT | 0644);

    int* mem = (int*)shmat(id, NULL, 0);
    *mem = 0;

    int fork_res = fork();

    if(fork_res == -1) {
        perror("fork");
        return 1;
    } else if(fork_res == 0) { //child
        for (size_t i = 0; i < 3; i++)
        {
            sleep(1);
            (*mem)++;
        }
        printf("CHILD -> %d\n", *mem);

        if(shmdt(mem) == -1) {
            perror("detach");
            return 1;
        }
    } else { //parent
        for (size_t i = 0; i < 3; i++)
        {
            sleep(1);
            (*mem)--;
        }
        printf("PARENT -> %d\n", *mem);

        if(shmdt(mem) == -1) {
            perror("detach");
            return 1;
        }

        if(shmctl(id, IPC_RMID, NULL) == -1) {
            perror("rmid");
            return 1;
        }
    }

    return 0;
}