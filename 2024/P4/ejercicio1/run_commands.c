#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <getopt.h>

pid_t launch_command(char** argv) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(argv[0], argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return pid;
}

char **parse_command(const char *cmd, int* argc) {
    size_t argv_size = 10;
    const char *end;
    size_t arg_len; 
    int arg_count = 0;
    const char *start = cmd;
    char **argv = malloc(argv_size * sizeof(char *));
    if (argv == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (*start && isspace(*start)) start++;

    while (*start) {
        if (arg_count >= argv_size - 1) {
            argv_size *= 2;
            argv = realloc(argv, argv_size * sizeof(char *));
            if (argv == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        end = start;
        while (*end && !isspace(*end)) end++;

        arg_len = end - start;
        argv[arg_count] = malloc(arg_len + 1);
        if (argv[arg_count] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(argv[arg_count], start, arg_len);
        argv[arg_count][arg_len] = '\0';
        arg_count++;

        start = end;
        while (*start && isspace(*start)) start++;
    }

    argv[arg_count] = NULL;
    (*argc) = arg_count;
    return argv;
}

void execute_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    pid_t pids[100];
    int command_count = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        int cmd_argc;
        char** cmd_argv = parse_command(buffer, &cmd_argc);
        if (cmd_argv[0] == NULL) {
            free(cmd_argv);
            continue;
        }

        pids[command_count] = launch_command(cmd_argv);
        printf("@@ Command #%d launched (pid: %d)\n", command_count + 1, pids[command_count]);

        int status;
        waitpid(pids[command_count], &status, 0);
        printf("@@ Command #%d terminated (pid: %d, status: %d)\n", command_count + 1, pids[command_count], WEXITSTATUS(status));

        for (int i = 0; i < cmd_argc; i++) {
            free(cmd_argv[i]);
        }
        free(cmd_argv);

        command_count++;
        if (command_count >= 100) {
            fprintf(stderr, "Too many commands in file.\n");
            break;
        }
    }


    fclose(file);
}

int main(int argc, char *argv[]) {
    int opt;
    char* command = NULL;
    char* filename = NULL;

    while ((opt = getopt(argc, argv, "x:s:b")) != -1) {
        switch (opt) {
        case 'x':
            command = optarg;
            break;
        case 's':
            filename = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s [-x command] [-s file] [-b]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (command) {
        int cmd_argc;
        char** cmd_argv = parse_command(command, &cmd_argc);
        pid_t pid = launch_command(cmd_argv);
        int status;
        waitpid(pid, &status, 0);

        for (int i = 0; i < cmd_argc; i++) {
            free(cmd_argv[i]);
        }
        free(cmd_argv);
    } else if (filename) {
        execute_file(filename);
    } else {
        fprintf(stderr, "Usage: %s [-x command] [-s file] [-b]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
