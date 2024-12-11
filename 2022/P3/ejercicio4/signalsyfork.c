#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


/*programa que temporiza la ejecución de un proceso hijo */

pid_t pid = -1;

void sig_handler(int signum){
  if(signum == SIGALRM && pid >= 0){         //signal handler for SIGALRM
    kill(pid, SIGKILL);
  } 
}


int main(int argc, char **argv)
{

	pid = fork();
	int status;

	
	if(pid == 0){
		//sleep(6);
		execvp(argv[1], &argv[1]);
		exit(-1);
	}

	else{	
		struct sigaction sa;

		sa.sa_handler = &sig_handler;
				
		//sigaction(SIGINT, &sa, NULL);
		sigaction(SIGALRM, &sa, NULL);		
		alarm(5);

		while(wait(&status) != pid);
	}

	if(WIFEXITED(status))
		printf("Mi hijo ha terminado su ejecucion correctamente");
	
	else if(WIFSIGNALED(status)){
		printf("Mi hijo ha muerto");
	}
	printf("\n");
		
	return 0;
}
