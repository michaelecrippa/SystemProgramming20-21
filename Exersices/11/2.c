#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

// Да се напише програма на C, която синхронизира 2 родствени процеса чрез семафор.
// Двата процеса работят едновременно и трябва да изведат текст на стандартния изход в определен ред.
// Единия чака няколко секунди и пише.
// Другия пише след него.

union semun {
    int val; /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    unsigned short *array; /* array for GETALL & SETALL */
};

int main(int argc, char const *argv[]) {
    int id = semget((key_t)123, 1, IPC_CREAT | 0644);

    if(id == -1){
        perror("sem get");
        return 1;
    }

    union semun sem;
    sem.val = 0;

    if(semctl(id, 0, SETVAL, sem) == -1) {
        perror("set val");
        return 1;
    }

    int fork_res = fork();

    if(fork_res == -1) {
        perror("fork");
        return 1;
    } else if(fork_res == 0) { //child
        sleep(2);
        sem.val = 1;
        printf("%s\n", "first");
        if(semctl(id, 0, SETVAL, sem)) {
            perror("set val child");
            return 1;
        }
    } else { //parent
        while(!semctl(id, 0, GETVAL, NULL)) {
            ;
        }
        printf("%s\n", "second");

        if(semctl(id, 0, IPC_RMID, NULL) == -1)  {
            perror("rmid");
            return 1;
        }
    }

    return 0;
}