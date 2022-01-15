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

    int msg_id = msgget(key, IPC_CREAT | 0644);

    if(msg_id == -1) {
        perror("msgget");
        return 1;
    }

    return 0;
}
