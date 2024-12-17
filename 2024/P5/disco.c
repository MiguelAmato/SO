#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <err.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "vip" : "not vip")

struct t_info {
	int id;
	int isvip;
};

pthread_mutex_t mutex;
pthread_cond_t cond;

int contClients = 0;
int contVip = 0;
int turnoN = 0;
int turnoVip = 0;
int tVip = 0;
int tNormal = 0;

void enter_normal_client(int id)
{
	printf("Client %d is waiting\n", id);
	int tN;
	pthread_mutex_lock(&mutex); 
	tN = tNormal++;
	while ((turnoN != tN) || (contClients == CAPACITY || contVip > 0)) {
		pthread_cond_wait(&cond, &mutex);
	}
	turnoN++;
	contClients++;
	printf("Client %d enters the disco (normal client)\n", id);
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
}

void enter_vip_client(int id)
{
	printf("VIP Client %d is waiting\n", id);
	int tV;
	pthread_mutex_lock(&mutex); 
	tV = tVip++;
	while ((turnoVip != tV) || (contClients == CAPACITY)) {
		pthread_cond_wait(&cond, &mutex);
	}
	turnoVip++;
	contClients++;
	contVip--;
	printf("VIP client %d enters the disco\n", id);
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip)
{
	pthread_mutex_lock(&mutex);
	contClients--;
	printf("Client %2d (%s) leaves the disco\n", id, VIPSTR(isvip));
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
}

void *client(void *arg)
{
	struct t_info *copia = (struct t_info *) arg;
	if (copia->isvip) {
		enter_vip_client(copia->id);
	}	
	else {
		enter_normal_client(copia->id);
	}
	dance(copia->id, copia->isvip);
	disco_exit(copia->id, copia->isvip);
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

	int vclient;
	int nClients;
	
	if ((nClients = (int)fgetc(file)) == EOF)
		err(3,"The input file %s is empty",argv[1]);
	nClients -= '0';

	struct t_info *param = malloc(sizeof(struct t_info) * nClients);
	pthread_t *clients = malloc(sizeof(pthread_t) * nClients);

	char aux;

	for (int i = 0; i < nClients; i++) {
		aux = fgetc(file);
		param[i].id = i;
		if ((vclient = (int)fgetc(file)) == EOF)
			err(4,"The input file %s has less clients than specified",argv[1]);
		vclient -= '0';
		param[i].isvip = vclient;
		if (vclient) 
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