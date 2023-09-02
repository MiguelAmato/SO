#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX 100

// el mutex y las condiciones
pthread_mutex_t mutex;
pthread_cond_t turn, imp;

// Variables que comparten
int n = 0;
int imprime = 0;

void *contarPar(void *arg) {
    while(n < MAX) {
        pthread_mutex_lock(&mutex); // Bloqueamos mutex
        while(n % 2 == 0) // Hasta que no sea par y haya mostrado...
            pthread_cond_wait(&turn, &mutex);
        n++;
        imprime = 1;
        pthread_cond_signal(&imp);
        pthread_mutex_unlock(&mutex);
    }
}

void *contarImpar(void *arg) {
    while(n < MAX) {
        pthread_mutex_lock(&mutex); // Bloqueamos mutex
        while(n % 2 != 0) // Hasta que no sea impar y haya mostrado...
            pthread_cond_wait(&turn, &mutex);
        n++;
        imprime = 1;
        pthread_cond_signal(&imp);
        pthread_mutex_unlock(&mutex);
    }
}

void *mostrarNum(void *arg) {
    while(n < MAX) {
        pthread_mutex_lock(&mutex);
        while(imprime == 0)
            pthread_cond_wait(&imp, &mutex);
        printf("%d ", n);
        imprime = 0;
        pthread_cond_signal(&turn);
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&turn, NULL);
    pthread_cond_init(&imp, NULL);

    pthread_t h1, h2, h3;
    pthread_create(&h1, NULL, contarPar, NULL);
    pthread_create(&h2, NULL, contarImpar, NULL);
    pthread_create(&h3, NULL, mostrarNum, NULL);

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);

    printf("\n");

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&turn);
    pthread_cond_destroy(&imp);
}

