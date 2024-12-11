#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX 10
#define MAX_P 5

pthread_mutex_t mutex;
pthread_cond_t condF, condP;

int comida = MAX;
int palillos = MAX_P;

void cogerPalillosMonitor(int i) {
    pthread_mutex_lock(&mutex);
    while() 
}

void dejarPalillosMonitor(int i) {

}

int comer(int i) {
    printf("Filosofo %d comiendo.\n",i);
    sleep((rand() % 3) + 1); 
    comida--;   
}

void pensar(int i) {
    printf("Filosofo %d pensando.\n",i);
    sleep((rand() % 3) + 1);
}

void *filosofo (int i) {
    while(comida > 0) {
        pensar(i);
        pthread_mutex_lock(&mutex);
        cogerPalillosMonitor(i);
        comer(i);
        dejarPalillosMonitor(i);
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[]) {
    pthread_mutex_init(&mutex, NULL);

    pthread_t f1,f2,f3,f4,f5;
    pthread_create(&f1, NULL, filosofo, 1);
    pthread_create(&f2, NULL, filosofo, 2);
    pthread_create(&f3, NULL, filosofo, 3);
    pthread_create(&f4, NULL, filosofo, 4);
    pthread_create(&f5, NULL, filosofo, 5);

    pthread_join(&f1, NULL);
    pthread_join(&f2, NULL);
    pthread_join(&f3, NULL);
    pthread_join(&f4, NULL);
    pthread_join(&f5, NULL);

    pthread_mutex_destroy(&mutex);
}