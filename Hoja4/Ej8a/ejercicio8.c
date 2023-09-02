#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX 5

struct infoCliente {
    int id;
    int dinero;
};

pthread_mutex_t mutex;
pthread_cond_t cond;

int turno = 0;
int ticket = 0;

int ocupados = 0;
int pumps[2] = {0,0};

int getUnusedPump() {
    int t;
    int ret;
    pthread_mutex_lock(&mutex);
    t = ticket++;
    while((turno != t) || ocupados == 2)  
        pthread_cond_wait(&cond, &mutex);
    turno++;
    if (pumps[0] == 0)
        ret = 0;
    else if (pumps[1] == 0)
        ret = 1;
    pumps[ret]++;
    ocupados++;
	pthread_mutex_unlock(&mutex);
    return ret;
}

void pumpFuel(int pump, int id, int dinero) {
    printf("Cliente %d usando el surtidor %d\n", id, pump + 1);
    sleep(dinero);
}

void releasePump(int pump, int id) {
    pthread_mutex_lock(&mutex);
    printf("Cliente %d deja el surtidor %d\n", id, pump + 1);
    pumps[pump]--;
    ocupados--;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void* cliente(void *arg) {
    struct infoCliente *copia = (struct infoCliente *) arg; 
    int pump;
    pump = getUnusedPump();
    /* Uso del surtidor */
    pumpFuel(pump, copia->id, copia->dinero);
    /* Deja de usar el surtidor */
    releasePump(pump, copia->id);
}

int main(int argc, char* argv[]) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    struct infoCliente info[MAX];
    pthread_t clientes[MAX];

    for(int i = 0; i < MAX; i++) {
        info[i].id = i;
        info[i].dinero = (rand() % 3) + 1;
        pthread_create(&clientes[i], NULL, cliente, &info[i]);
    }

    for (int i = 0; i < MAX; i++) 
		pthread_join(clientes[i], NULL);

    pthread_cond_destroy(&cond);    
    pthread_mutex_destroy(&mutex);
}