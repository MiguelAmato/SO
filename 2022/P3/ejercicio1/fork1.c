#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/*programa que crea dos hijos: uno no cambia de ejecutable y otro si */

int main(int argc, char **argv)
{
	pid_t pid;
	pid = fork();
	int status;
	
	if(pid == 0){
		printf("Soy hijo:\n\tMi padre es %d \n\tYo soy %d\n",getppid(),getpid());
	}
	else{
		printf("Soy padre:\n\tMi hijo es %d \n\tYo soy %d\n",pid,getpid());
	}

	waitpid(pid,&status,0);

	pid = fork();


	if (pid == 0) {
		printf("Soy hijo:\n\tmi padre es %d, y yo soy %d\n", getppid(), getpid());
		execvp(argv[1], &argv[1]);
		exit(-1);
	}
	else {
		printf("Soy padre: \n\tmi ultimo hijo es %d, y yo soy %d\n", pid, getpid());
	}

	return 0;
}
