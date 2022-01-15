#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

long counter = 0;
int alarmed = 0;

void handle(int param) {
    printf("%ld\n", counter);

    alarm(1); // call handle after 1 second again
}

int main(int argc, char const *argv[]) {
    struct sigaction act = { &handle,0,0,0 };

    sigaction(SIGALRM, &act, NULL);

    while (1) {
        counter++;

        if (alarmed == 0) {
            alarm(1);
            alarmed = 1;
        }
    }

    return 0;
}
