#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <err.h>

#define CAPACITY 5
#define VIPSTR(vip) (vip == 1 ? "vip" : ( vip == 2 ? "special" : "normal"))

struct t_info {
    int id;
    int status;
};

pthread_mutex_t mutex;
pthread_cond_t cond;

int contSpecials = 0;
int contNonSpecials = 0;
int contClients = 0;
int contVip = 0;
int turnoN = 0;
int turnoVip = 0;
int turnoSpecial = 0;
int tVip = 0;
int tNormal = 0;
int tSpecial = 0;

void enter_special_client(int id) {
    printf("Client %d (special) is waiting\n", id);
    int tS;
    pthread_mutex_lock(&mutex);
    tS = tSpecial++;
    while ((turnoSpecial != tS) || (contClients == CAPACITY || contNonSpecials > 0)) {
        pthread_cond_wait(&cond, &mutex);
    }
    turnoSpecial++;
    contClients++;
	contSpecials--;
    printf("Client %d (special) enters the disco \n", id);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void enter_normal_client(int id) {
    printf("Client %d (normal) is waiting\n", id);
    int tN;
    pthread_mutex_lock(&mutex);
    tN = tNormal++;
    while ((turnoN != tN) || (contClients == CAPACITY || contVip > 0 || contSpecials > 0)) {
        pthread_cond_wait(&cond, &mutex);
    }
    turnoN++;
    contClients++;
    contNonSpecials++;
    printf("Client %d (normal) enters the disco \n", id);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void enter_vip_client(int id) {
    printf("Client %d (vip) is waiting\n", id);
    int tV;
    pthread_mutex_lock(&mutex);
    tV = tVip++;
    while ((turnoVip != tV) || (contClients == CAPACITY || contSpecials > 0)) {
        pthread_cond_wait(&cond, &mutex);
    }
    turnoVip++;
    contClients++;
    contNonSpecials++;
	contVip--;
    printf("Client %d (vip) enters the disco\n", id);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void dance(int id, int status) {
    printf("Client %d (%s) dancing in disco\n", id, VIPSTR(status));
    sleep((rand() % 3) + 1);
}

void disco_exit(int id, int status) {
    pthread_mutex_lock(&mutex);
    contClients--;
    if (status != 2)
        contNonSpecials--;
    printf("Client %d (%s) leaves the disco\n", id, VIPSTR(status));
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void *client(void *arg) {
    struct t_info *copia = (struct t_info *)arg;
    if (copia->status == 2) {
        enter_special_client(copia->id);
    } else if (copia->status == 1) {
        enter_vip_client(copia->id);
    } else {
        enter_normal_client(copia->id);
    }
    dance(copia->id, copia->status);
    disco_exit(copia->id, copia->status);
}

int main(int argc, char *argv[])
{
	
	if (argc != 2) {
		printf("Usage: %s <file name>\n", argv[0]);
		return 1;
	}
	FILE* file;
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	int nClients;
	
	if ((nClients = (int)fgetc(file)) == EOF)
		err(3,"The input file %s is empty",argv[1]);
	nClients -= '0';

	struct t_info *param = malloc(sizeof(struct t_info) * nClients);
	if (!param) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	pthread_t *clients = malloc(sizeof(pthread_t) * nClients);
	if (!clients) {
		free(param);
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	char aux;
	int status;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);


	for (int i = 0; i < nClients; i++) {
		aux = fgetc(file);
		param[i].id = i;
		if ((status = (int)fgetc(file)) == EOF)
			err(4,"The input file %s has less clients than specified",argv[1]);
		status -= '0';
		param[i].status = status;
		if (status == 2)
			contSpecials++;
		else if (status == 1)
			contVip++;
		pthread_create(&clients[i], NULL, client, &param[i]);
	}

	for (int i = 0; i < nClients; i++) 
		pthread_join(clients[i], NULL);

	free(param);
	free(clients);

	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
	
	return 0;
}