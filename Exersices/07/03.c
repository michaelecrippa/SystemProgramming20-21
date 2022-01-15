#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handle(int param) {
    const char* message = "Child finished!";
    printf("%s\n", message);
}

int main(int argc, char const *argv[]) {
    struct sigaction act = { &handle,0,0,0 };

    sigaction(SIGCHLD, &act, NULL);

    pid_t fork_result = fork();
    if (fork_result < 0) {
        perror("fork");
        return 1;
    } else if (fork_result == 0) { //child
        //Execute some action in the child
        for (size_t i = 0; i < 10000; i++)
        {
            ;
        }
        
    } else { //parent
        pause(); // wait for the child to finish
    }

    return 0;
}
