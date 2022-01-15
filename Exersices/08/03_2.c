#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(int argc, char const *argv[]) {
    if(argc < 2) {
        perror("Missing arg!");
        return 1;
    }

    int key = atoi(argv[1]);

    int sem_id = semget((key_t)key, 0 , 0);

    if(sem_id == -1) {
        perror("semget");
        return 1;
    }

    printf("%d\n", sem_id);

    if(semctl(sem_id, 1, IPC_RMID) == -1) {
        perror("semrm");
        return 1;
    }

    return 0;
}