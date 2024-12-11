#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5

struct t_info {
	int id;
	char prio;
};

void* thread_usuario(void* arg) {
	struct t_info *copia = (struct t_info *) arg;
	printf("ID: %d, prio: %c\n", copia->id, copia->prio);
	pthread_exit(0);
}


int main(int argc, char* argv[]) {
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	struct t_info* param;
	pthread_t* thid;
	
    param = malloc(sizeof(struct t_info)*N);
	thid = malloc(sizeof(pthread_t) * N);

	for(int i = 0; i < N; i ++) {
		
		param[i].id = i;
		if (i % 2 == 0)
			param[i].prio = 'P';
		else
			param[i].prio = 'N';
		pthread_create(&thid[i], &attr, thread_usuario, &param[i]);
	}
	for(int i = 0; i < N; i ++) {
		pthread_join(thid[i], NULL);
	}
	free(param);
	free(thid);
	return 0;
}