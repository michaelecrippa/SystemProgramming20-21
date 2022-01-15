#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//Да се напише програма на C, която създава сегмент обща памет,
//прочита информацията за права и собственост и ги извежда на стандартния изход. 
//Променя собствеността и правата на достъп и отново извежда информацията.
//Накрая изтрива общата памет.
int main(int argc, char const *argv[]) {
    int id = shmget((pid_t)IPC_PRIVATE, 1024, IPC_CREAT | 0644);

    struct shmid_ds buff;

    if(shmctl(id, IPC_STAT, &buff) == -1) {
        perror("Error reading");
        return 1;
    }

    printf("%d\n", buff.shm_perm.uid);
    printf("%d\n", buff.shm_perm.cuid);
    printf("%d\n", buff.shm_perm.gid);

    if(shmctl(id, IPC_SET, &buff) == -1) {
        perror("Error setting!");
        return 1;
    }

    buff.shm_perm.uid = 1001;
    buff.shm_perm.cuid = 1001;
    buff.shm_perm.gid = 444;
    buff.shm_perm.cgid = 444;

    printf("%d\n", buff.shm_perm.uid);
    printf("%d\n", buff.shm_perm.cuid);
    printf("%d\n", buff.shm_perm.gid);

    if(shmctl(id, IPC_RMID, &buff) == -1) {
        perror("rmid");
        return 1;
    }

    return 0;
}