#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char const *argv[]) {
    if(argc < 2) {
        perror("Missing args");
        return 1;
    }

    key_t key = (key_t)atoi(argv[1]);

    int msg_id = msgget(key, 0);

    if(msg_id == -1) {
        perror("Error creating queue");
        return 1;
    }

    struct msqid_ds buff;
    if(msgctl(msg_id, IPC_STAT, &buff) == -1) {
        perror("ipcstat");
        return 1;
    }

    printf("%d\n", buff.msg_perm.gid);
    printf("%d\n", buff.msg_perm.uid);
    printf("%d\n", buff.msg_perm.cgid);
    printf("%d\n", buff.msg_perm.cuid);
    printf("%d\n", buff.msg_perm.mode);

    if(msgctl(msg_id, IPC_RMID, &buff) == -1) {
        perror("rmid");
        return 1;
    }

    return 0;
}
