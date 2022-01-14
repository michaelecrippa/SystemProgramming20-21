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
    int msgid = msgget((key_t)KEY, 0);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    struct msgbuf msgbuf;

    int bytes_read = 0;
    while ((bytes_read = msgrcv(msgid, &msgbuf, BUFSIZE, 1, IPC_NOWAIT)) > 0) {
        write(STDOUT_FILENO, msgbuf.mtext, bytes_read);
    }

    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        return 1;
    }

    return 0;
}
