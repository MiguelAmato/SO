#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct t_info {
    int thread_num;
    char priority;
};

void *thread_usuario(void *arg) {
    struct t_info *args = (struct t_info *)arg;
    int thread_num = args->thread_num;
	char priority = args->priority;
    free(args);
    pthread_t thread_id = pthread_self();
    printf("ID: %lu, Number: %d, Priority: %c\n", thread_id, thread_num, priority);
    return NULL;
}

int main(int argc, char *argv[]) {
    int num_threads = 5;
    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++) {
        struct t_info *args = malloc(sizeof(struct t_info));
        if (args == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        args->thread_num = i;
        args->priority = (i % 2 == 0) ? 'P' : 'N';

        if (pthread_create(&threads[i], NULL, thread_usuario, args) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

