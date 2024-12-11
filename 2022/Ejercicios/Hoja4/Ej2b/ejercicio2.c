#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 100

// Variables que comparten
int n = 0;

// Semaforos uno para el numero y otro para 
sem_t par, impar, print;

void *contarPar(void *arg) {
    while(n < MAX) {
        sem_wait(&par);
        n++;
        sem_post(&print);
    }
}

void *contarImpar(void *arg) {
    while(n < MAX) {
        sem_wait(&impar);
        n++;
        sem_post(&print);
    }
}

void *mostrarNum(void *arg) {
    while(n < MAX) {
        sem_wait(&print);
        printf("%d ", n);
        if(n%2 != 0) 
            sem_post(&par);
        else
            sem_post(&impar);
    }
}

int main(int argc, char *argv[])
{
    sem_init(&par, 0, 0);
    sem_init(&impar, 0, 1);
    sem_init(&print, 0, 0);

    pthread_t h1, h2, h3;
    pthread_create(&h1, NULL, contarPar, NULL);
    pthread_create(&h2, NULL, contarImpar, NULL);
    pthread_create(&h3, NULL, mostrarNum, NULL);

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);

    printf("\n");

    sem_destroy(&par);
    sem_destroy(&impar);
    sem_destroy(&print);
}

