#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "key.h"

#define BUFSIZE 16

struct msgbuf {
    long mtype;
    char mtext[BUFSIZE];
};
//Да се напишат 2 програми на C, които комуникират по между си чрез обща опашка, чийто ключ е дефиниран в общ заглавен файл. 
//Единия процес чете съдържанието на файл, чието име е подадено като параметър в командния му ред и го праща в опашката. 
//Другия процес получава съобщения от опашката и ги извежда на стандартния изход. (Стартирайте получателя преди изпращача)
int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char buf[BUFSIZE];

    int fd = open(argv[1], O_RDONLY | O_CREAT, 0777);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    int msgid = msgget((key_t)KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    struct msgbuf msgbuf;
    msgbuf.mtype = 1;

    int bytes_read = 0;
    while ((bytes_read = read(fd, buf, BUFSIZE)) > 0) {
        strncpy(msgbuf.mtext, buf, bytes_read);
        if (msgsnd(msgid, &msgbuf, bytes_read, 0) == -1) {
            perror("msgsnd");
            return 1;
        }
    }

    return 0;
}
