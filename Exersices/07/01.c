#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handler(int param) {
    ;
}

int main(int argc, char const *argv[]) {
    struct sigaction act = { &handler,0,0,0 }; // or SIGNIGN instead of handler

    sigaction(SIGPIPE, &act, NULL);

    int p[2];
    if (pipe(p) == -1) { perror("pipe"); return 1; }

    if (close(p[0]) == -1) { perror("close"); return 1; }

    char buffer[100] = "Message!\n";
    if(write(p[1], buffer, strlen(buffer) + 1) == -1) {
        perror("write pipe");
        return 1;
    }
    
    write(1, buffer, strlen(buffer) + 1);

    return 0;
}
