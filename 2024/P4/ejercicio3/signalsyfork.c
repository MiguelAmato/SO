#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

/*programa que temporiza la ejecución de un proceso hijo */

pid_t child_pid;

void handle_alarm(int sig) {
    if (child_pid > 0) {
        printf("Parent: Timeout reached. Sending SIGKILL to child (PID: %d).\n", child_pid);
        kill(child_pid, SIGKILL);
    }
}

void handle_sigint(int sig) {
    printf("Parent: Ignoring SIGINT.\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // SIGALRM handler
    struct sigaction sa;
    sa.sa_handler = handle_alarm;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // SIGINT (ignore) handler
    struct sigaction sa_ignore;
    sa_ignore.sa_handler = handle_sigint;
    sa_ignore.sa_flags = 0;
    sigemptyset(&sa_ignore.sa_mask);

    if (sigaction(SIGINT, &sa_ignore, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        alarm(5);

        int status;
        pid_t finished_pid = wait(&status);

        if (finished_pid == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            printf("Parent: Child (PID: %d) exited %d.\n", finished_pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Parent: Child (PID: %d) signal %d.\n", finished_pid, WTERMSIG(status));
        }
    }

    return 0;
}
