#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <err.h>

#define CAPACITY 3
#define PROFSTR(professor) ((professor) ? "professor" : "student")

struct t_info {
	int id;
	int isProfesor;
};

pthread_mutex_t mutex;
pthread_cond_t cond;

int aforo = 0;
int contProfesor = 0;
int turnoN = 0;
int turnoProfesor = 0;
int tProfesor = 0;
int tNormal = 0;

void entra_estudiante(int id)
{
	printf("User %d (student) waiting on the queue.\n", id);
	int tN;
	pthread_mutex_lock(&mutex); 
	tN = tNormal++;
	while ((turnoN != tN) || (aforo == CAPACITY || contProfesor > 0)) {
		pthread_cond_wait(&cond, &mutex);
	}
	turnoN++;
	aforo++;
	printf("User %d (student) enters the library.\n", id);
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
}

void entra_profesor(int id)
{
	printf("User %d (professor) waiting on the queue.\n", id);
	int tP;
	pthread_mutex_lock(&mutex); 
	tP = tProfesor++;
	while ((turnoProfesor != tP) || (aforo == CAPACITY)) {
		pthread_cond_wait(&cond, &mutex);
	}
	turnoProfesor++;
	aforo++;
	contProfesor--;
	printf("User %d (professor) enters the library.\n", id);
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
}

void reading(int id, int isProfesor)
{
	printf("User %d (%s) is reading books for 2 seconds.\n", id, PROFSTR(isProfesor));
	sleep(2);
}

void exit_library(int id, int isProfesor)
{
	pthread_mutex_lock(&mutex);
	aforo--;
	printf("User %d (%s) leaves the library\n", id, PROFSTR(isProfesor));
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
}

void *client(void *arg)
{
	struct t_info *copia = (struct t_info *) arg;
	
	if (copia->isProfesor) {
		entra_profesor(copia->id);
	}	
	else {
		entra_estudiante(copia->id);
	}
	reading(copia->id, copia->isProfesor);
	exit_library(copia->id, copia->isProfesor);
	
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
	
	if (fscanf(file, "%d\n", &nClients) == 0)
		err(3,"The input file %s is empty",argv[1]);

	printf("%d nClients\n", nClients);
	struct t_info *param = malloc(sizeof(struct t_info) * nClients);
	if (!param) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	pthread_t *clients = malloc(sizeof(pthread_t) * nClients);
		if (!clients) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	int professor = 0;
	for (int i = 0; i < nClients; i++) {
		if (fscanf(file, "%d\n", &professor) == 0) {
			perror("fscanf");
			exit(EXIT_FAILURE);
		}
		param[i].id = i;
		param[i].isProfesor = professor;
		if (professor)
			++contProfesor;
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