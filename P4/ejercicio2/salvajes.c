#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define NUMITER 3

sem_t* sem_salvajes;
sem_t* sem_cocineros; 
sem_t* sem_serv;
int* caldero;

void getServingsFromPot(void) {
    (*caldero)--;
}

void eat(void) {
    unsigned long id = (unsigned long) getpid();
    printf("Salvaje %lu esta comiendo\n", id);
    sleep(rand() % 5);
}

void savages(void) {
    int i = 0;
    while(i < NUMITER) {
        sem_wait(sem_serv);
        if(*caldero == 0) {
            sem_post(sem_cocineros);
            sem_wait(sem_salvajes);
        }
        getServingsFromPot(); 
        sem_post(sem_serv);
        eat();
        i++;
    }
}

int main(int argc, char* argv[]) {
    int shd = shm_open("/CALDERO", O_RDWR, S_IRUSR | S_IWUSR);
    if(shd == -1) {
        printf("ERROR: Ejecuta primero el programa cocinero.\n");
        exit(EXIT_FAILURE);
    }
    caldero = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shd, 0);
    sem_cocineros = sem_open("/COCINERO", 0);
    sem_serv = sem_open("/SERVIR", 0);
    sem_salvajes = sem_open("/SALVAJES", 0);
    savages();
    sem_close(sem_cocineros);
    sem_close(sem_salvajes);
    sem_close(sem_serv);
    munmap(caldero, sizeof(int));
    return 0;
}