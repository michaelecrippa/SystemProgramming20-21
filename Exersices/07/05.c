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

void handle_alarm(int param) {
    printf("%ld\n", counter);

    alarm(1);
}

void handle_int(int param) {
    counter = 0;
}

int main(int argc, char const *argv[]) {
    struct sigaction act = { handle_alarm, 0, 0, 0 };
    struct sigaction act2 = { handle_int, 0, 0, 0 };

    sigaction(SIGALRM, &act, NULL);
    sigaction(SIGINT, &act2, NULL);

    while (1) {
        counter++;

        if (alarmed == 0) {
            alarm(1);
            alarmed = 1;
        }
    }

    return 0;
}
