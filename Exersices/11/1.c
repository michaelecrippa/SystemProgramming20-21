#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

//Да се напише програма на C, която създава семафор,
//инициализира го с 1, 
//след това прочита стойността, 
//извежда я на екрана 
//унищожава семафора.

union semun {
    int val; /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    unsigned short *array; /* array for GETALL & SETALL */
};

int main(int argc, char const *argv[]) {
    int id = semget((key_t)IPC_PRIVATE, 1, IPC_CREAT | 0644);
    if(id == -1) {
        perror("get");
        return 0;
    }

    union semun semopts;    
    semopts.val = 1; /*initial sem value*/

    if(semctl(id, 0, SETVAL, semopts) == -1) {
        perror("sem");
        return 1;
    }

    int res = semctl(id, 0, GETVAL);
    if(res == -1) {
        perror("get val");
        return 1;
    }

    printf("%d\n", res);

    if(semctl(id, 0, IPC_RMID) == -1) {
        perror("rmid");
        return 1;
    }

    return 0;
}