#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>

#define M 10

int finish = 0;
sem_t * sem_salvajes; 
sem_t* sem_cocineros; 
sem_t* sem_serv;
int* caldero;

void putServingsInPot(int servings) {
    printf("Cocinero sirve %d raciones en el caldero\n", M);
    *caldero = M;
}

void cook(void) {
    while(!finish) {
        sem_wait(sem_cocineros);
        if(finish) 
            break;
        putServingsInPot(M);
        sem_post(sem_salvajes);
    }
}

void handler(int signo) {
    finish = 1;
}

int main(int argc, char* argv[]) {
    printf("PID: %d\n", getpid());
    int shd = shm_open("/CALDERO", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shd, sizeof(int));
    caldero = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shd, 0); 
    sem_cocineros = sem_open("/COCINERO", O_CREAT, 0700, 0);
    sem_serv = sem_open("/SERVIR", O_CREAT, 0700, 1);
    sem_salvajes = sem_open("/SALVAJES", O_CREAT, 0700, 0);
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = handler;
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    cook();
    sem_close(sem_cocineros);
    sem_unlink("/COCINERO");
    sem_close(sem_salvajes);
    sem_unlink("/SALVAJES");
    sem_close(sem_serv);
    sem_unlink("/SERVIR");
    shm_unlink("/CALDERO");
    munmap(caldero, sizeof(int));
    return 0;
}