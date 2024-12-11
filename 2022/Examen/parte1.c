#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>

#define MAXPROC 16

struct task {
    int valid; // booleano, indica si la entrada es válida (1) o no (0)
    int next[MAXPROC]; // array de booleanos para sucesores: next[i]=1 si sale una arista hacia el nodo i
    int id; // identificador/nombre de la tarea, que corresponde a su posición en el array de nodos
};

struct task tasks[MAXPROC] = {0, {0}, 0};
int cont;

pthread_mutex_t mutex;
pthread_cond_t cond;

int read_graph(FILE* file, struct task tasks[]) {
    char basura;
    int ind;
    int n;
    int i = 0;
    int cont = 0;
    while ((ind = (int)getc(file) - '0') != EOF) {
        basura = getc(file);
        n = (int)getc(file) - '0';
        tasks[i].id = i;
        tasks[i].valid = 1;
        for(int j = 0; j < n; j++) {
            basura = getc(file);
            ind = (int)getc(file) - '0';
            tasks[ind].next[i] = 1;
        }
        basura = getc(file);
        i++;
        if (basura == EOF)
            break;
    }
    return i;
}

void print_graph(struct task tasks[], int n)
{
    int i,j;
    for (i = 0; i < n; i++) {
        if (!tasks[i].valid)
            continue;
        printf("%d: ", tasks[i].id);
        for (j = 0; j < n; j++)
            if (tasks[i].next[j])
                printf("%d ", j);
        printf("\n");
    }
}

int esperaPredecesores(int id) {
    int ret = 0;
    int i = 0;
    while (i < cont && ret == 0) {
        if (tasks[i].valid && tasks[i].next[id] == 1) {
            ret = 1;
        }
        ++i;
    }
    return ret;
}

void wait_for_predecesors(int id) {
    pthread_mutex_lock(&mutex);
    while(esperaPredecesores(id)) 
        pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);
}

void notify_successors(int id) {
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < cont; i++) {
        if (tasks[i].valid) {
            tasks[i].next[id] = 0;
        }
    }
    tasks[id].valid = 0;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void *task_body(void * arg) {
    int id = *(int *) arg;
    wait_for_predecesors(id); // bloqueante, el hilo espera a que sus predecesores le notifiquen su finalización
    printf("Task %d running\n", id); // cuerpo real de la tarea
    notify_successors(id); // sincronización, aviso a los sucesores de que esta tarea ha terminado
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    FILE* file;
    if((file = fopen(argv[1], "r")) == -1) {
        printf("%s could not be opened!\n", argv[1]);
        return 1;
    }

    // PARTE 1
    cont = read_graph(file, tasks);
    print_graph(tasks,cont);
    
    pthread_t threads[MAXPROC];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < cont; i++) {
        pthread_create(&threads[i], NULL, task_body, &tasks[i].id);
    }
    
    for (int i = 0; i < cont; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

}