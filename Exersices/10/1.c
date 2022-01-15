#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SIZE 1024
//Да се напише програма на C, която създава сегмент обща памет,
//записва текст в него, 
//след това го прочита,
//извежда го на екрана 
//и унищожава сегмента.
int main(int argc, char const *argv[]) {

    int id = shmget((key_t)IPC_PRIVATE, SIZE, IPC_CREAT | 0644);

    if(id == -1) {
        perror("ipc_creat");
        return 1;
    }

    char* shm_ptr = (char*)shmat(id, NULL, 0);
    if((int)shm_ptr == -1) {
        perror("attach");
        return 1;
    }

    char buff [100] = "Message";

    memmove(shm_ptr, buff, strlen(buff));

    printf("%s\n", shm_ptr);

    if(shmdt(shm_ptr) == -1) {
        perror("shmdt");
        return 1;
    }

    if(shmctl(id, IPC_RMID, NULL) == -1) {
        perror("rmid");
        return 1;
    }

    return 0;
}
