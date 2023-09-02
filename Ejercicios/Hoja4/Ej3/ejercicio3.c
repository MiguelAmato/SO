#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct sem_t {
    int valor;
    pthread_mutex_t mutex;
    pthread_cond_t cond; // ????
};

int espera = 0;

void init(struct sem_t* sem, int valor) {
    sem->valor = valor;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
}

void destroy(struct sem_t* sem) {
    pthread_mutex_destroy(&(sem->mutex));
    pthread_cond_destroy(&(sem->cond));
}

void wait(struct sem_t* sem) {
    pthread_mutex_lock(&(sem->mutex));
    if (sem->valor == 0)
        while(sem->valor == 0) 
            pthread_cond_wait(&(sem->cond), &(sem->mutex));
    sem->valor = sem->valor - 1;
    pthread_mutex_unlock(&(sem->mutex));
}

void post(struct sem_t* sem) {
    pthread_mutex_lock(&(sem->mutex));
    sem->valor = sem->valor + 1;
    if (sem->valor == 1) 
        pthread_cond_signal(&(sem->cond));
    pthread_mutex_unlock(&(sem->mutex));
}

// ---------- PARA PROBAR ---------------

#define MAX 100

// Variables que comparten
int n = 0;

// Semaforos uno para el numero y otro para 
struct sem_t par, impar, print;

void *contarPar(void *arg) {
    while(n < MAX) {
        wait(&par);
        n++;
        post(&print);
    }
}

void *contarImpar(void *arg) {
    while(n < MAX) {
        wait(&impar);
        n++;
        post(&print);
    }
}

void *mostrarNum(void *arg) {
    while(n < MAX) {
        wait(&print);
        printf("%d ", n);
        if(n%2 != 0) 
            post(&par);
        else
            post(&impar);
    }
}

int main(int argc, char *argv[])
{
    init(&par, 0);
    init(&impar, 1);
    init(&print, 0);

    pthread_t h1, h2, h3;
    pthread_create(&h1, NULL, contarPar, NULL);
    pthread_create(&h2, NULL, contarImpar, NULL);
    pthread_create(&h3, NULL, mostrarNum, NULL);

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);

    printf("\n");

    destroy(&par);
    destroy(&impar);
    destroy(&print);
}