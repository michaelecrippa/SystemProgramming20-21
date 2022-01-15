#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

void handle_sigterm(int param) {
    char message[100] = "Handle message\n";
    write(1, message, strlen(message));
}

int main(int argc, char const *argv[]) {

    pid_t fork_result = fork();
    if (fork_result < 0) {
        perror("fork");
        return 1;
    } else if (fork_result == 0) { // child
        struct sigaction act1 = { handle_sigterm, 0, 0, 0 };

        sigaction(SIGTERM, &act1, NULL);

        for(;;) {}
    } else {    // fork_result > 0
        sleep(1);
        kill(fork_result, SIGTERM);
        sleep(1);
        kill(fork_result, SIGTERM);
    }

    return 0;
}
