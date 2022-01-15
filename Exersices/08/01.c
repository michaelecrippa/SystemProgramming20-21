#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char const *argv[]) {
    int id = msgget((key_t)IPC_PRIVATE, IPC_CREAT | 0644);
    if (id == -1) {
        perror("msggget");
        return 1;
    }

    struct msqid_ds buf;
    if (msgctl(id, IPC_STAT, &buf) == -1) {
        perror("msgctl");
        return 1;
    }
    printf("%s\n", "Printing permissions");
    printf("%d\n", buf.msg_perm.uid);
    printf("%d\n", buf.msg_perm.gid);
    printf("%d\n", buf.msg_perm.cgid);
    printf("%d\n", buf.msg_perm.cuid);
    printf("%o\n", buf.msg_perm.mode);

    //update permissions
    buf.msg_perm.gid = 1001;
    buf.msg_perm.uid = 1001;
    buf.msg_perm.mode = 0444;

    if (msgctl(id, IPC_SET, &buf) == -1) {
        perror("msgctl");
        return 1;
    }

    printf("%s\n", "Printing permissions");
    printf("%d\n", buf.msg_perm.uid);
    printf("%d\n", buf.msg_perm.gid);
    printf("%o\n", buf.msg_perm.mode);

    if(msgctl(id, IPC_RMID, NULL) == -1) {
        perror("rmid");
        return 1;
    }

    return 0;
}
