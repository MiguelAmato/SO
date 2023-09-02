#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex; 
pthread_cond_t cond;

int contE = 0; 
int hayLector = 0;

int recurso = 0;

void *escritor(void* arg) {
    pthread_mutex_lock(&mutex);
    while(hayLector != 0) 
        pthread_cond_wait(&cond, &mutex);
    contE++; 
    recurso++;
    contE--;
    printf("Modifico el recurso: %d\n", recurso);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void *lector(void* arg) {
    while(contE > 0) 
        pthread_cond_wait(&cond, &mutex);
    printf("Leyendo...\n");
    hayLector++;
    printf("Lei el recurso: %d\n", recurso);
    hayLector--;
    pthread_cond_broadcast(&cond);
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
    pthread_join(h4, NULL);
    pthread_join(h5, NULL);
    

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    int y;
}