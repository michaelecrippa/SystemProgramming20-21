#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SIZE 1024
//Да се напише програма на C,
//която създава сегмент обща памет и записва текст в него. 
//Друга програма присъединява същия сегмент, прочита текста, извежда го на екрана и унищожава сегмента.
int main(int argc, char const *argv[]) {

    int id = shmget((key_t)123, SIZE, IPC_CREAT | 0644);

    if(id == -1 ) {
        perror("create shm");
        return 0;
    }

    char* shm_ptr = (char*)shmat(id, NULL, 0);
    if((int)shm_ptr == -1) {
        perror("attach");
        return 0;
    }

    char buff [100] = "Message";

    memmove(shm_ptr, buff, strlen(buff));

    if(shmdt(shm_ptr) == -1 ) {
        perror("dt");
        return 0;
    }

    return 0;
}
