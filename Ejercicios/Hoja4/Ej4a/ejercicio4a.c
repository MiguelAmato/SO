#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex; 
pthread_cond_t cond;

int contL = 0; 
int contE = 0;

int recurso = 0;

void *escritor(void* arg) {
    pthread_mutex_lock(&mutex);
    while(contL > 0) 
        pthread_cond_wait(&cond, &mutex);
    contE = 1;
    recurso++;
    contE = 0;
    printf("Modifico el recurso: %d\n", recurso);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void *lector(void* arg) {
    pthread_mutex_lock(&mutex);
    while(contE > 0) 
        pthread_cond_wait(&cond, &mutex);
    contL++;
    printf("Leo el recurso: %d\n", recurso);
    contL--;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[]) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t h1, h2, h3, h4, h5;
    pthread_create(&h1, NULL, escritor, NULL);
    pthread_create(&h2, NULL, lector, NULL);
    pthread_create(&h4, NULL, escritor, NULL);
    pthread_create(&h3, NULL, lector, NULL);
    pthread_create(&h5, NULL, lector, NULL);

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}