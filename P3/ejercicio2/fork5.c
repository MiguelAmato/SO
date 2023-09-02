#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* programa que crea N hijos siguiendo un grado de dependencias. Usa las
 * versiones de execl y execlp */


int main(int argc, char **argv)
{

	pid_t pid[9];

	printf("P0\n");	

	// El main es P0

	pid[1] = fork();  // Aqui se crea P1

	if(pid[1] == 0){
		execlp("echo","echo","P1",NULL);
		exit(-1);
	}

	waitpid(pid[1],NULL,0);

	pid[2] = fork();

	if(pid[2] == 0){
		execl("/bin/echo","echo","P2",NULL);
		exit(-1);
	}

	pid[5] = fork();

	if(pid[5] == 0){
		execlp("echo","echo","P5",NULL);
		exit(-1);
	}

	pid[7] = fork();

	if(pid[7] == 0){
		execlp("echo","echo","P7",NULL);
		exit(-1);
	}

	waitpid(pid[2],NULL,0);

	pid[3] = fork();

	if(pid[3] == 0){
		execlp("echo","echo","P3",NULL);
		exit(-1);
	}

	pid[4] = fork();

	if(pid[4] == 0){
		execl("/bin/echo","echo","P4",NULL);
		exit(-1);
	}

	waitpid(pid[4],NULL,0);
	waitpid(pid[5],NULL,0);

	pid[6] = fork();

	if(pid[6] == 0){
		execl("/bin/echo","echo","P6",NULL);
		exit(-1);
	}

	waitpid(pid[3],NULL,0);
	waitpid(pid[6],NULL,0);
	waitpid(pid[7],NULL,0);


	pid[8] = fork();

	if(pid[8] == 0){
		execl("/bin/echo","echo","P8",NULL);
		exit(-1);
	}

	return 0;
}
