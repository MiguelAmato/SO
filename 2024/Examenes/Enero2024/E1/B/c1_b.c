#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
       

#define MAX 500

/*
● Si la función se invoca con argv = NULL, entonces sólo contará el número de subcadenas separadas por
 espacios en blanco, y no modificará la cadena apuntada por args.

 ● Si la función se invoca con argv != NULL, además de lo anterior la función sustituirá en args todos los
 espacios por ‘\0’, y copiará en argv[i] la dirección de comienzo de la i-ésima subcadena separada por
 espacios de args.

 ● Lafunción ignorará los espacios al comienzo de la cadena apuntada por args. (HECHO)
 Se usará la función de la biblioteca estándar isspace para comprobar si un carácter
*/

char *trim(char *str) {
	while(isspace(*str))
		++str;
	if (*str == '\0')
		return str;
	char *end = str + strlen(str) - 1;
	while(end > str && isspace(*end))
		--end;
	*(end + 1) = '\0';
	return str;  
}

int setargs(char *args, char *argv[]) {
 	args = trim(args); 
    int count = 0;     
    if (argv == NULL) {
        for (int i = 0; args[i] != '\0'; ++i) {
            if (!isspace(args[i]) && (i == 0 || isspace(args[i - 1])))
                ++count;
        }
    } else {
        int i = 0;
        while (*args != '\0') {
            while (isspace(*args))
                ++args;
            if (*args != '\0') {
                argv[i++] = args;
                ++count;
            }
            while (*args != '\0' && !isspace(*args))
                ++args;
            if (*args != '\0') {
                *args = '\0';
                ++args;
            }
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>.\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	FILE *file = fopen(argv[1], "r");
	if (!file) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	char **cargv;
	char buff[MAX];
	int size = 0;
	while (fgets(buff, MAX, file) != NULL) {
		buff[strcspn(buff, "\n")] = '\0';
		size = setargs(buff, NULL);
		// reservamos la cantidad de comandos 
		cargv = malloc((sizeof(char *)) * (size + 1));
		if (!cargv) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		cargv[size] = NULL;
		setargs(buff, cargv);
		pid_t cpid = fork();
		if (cpid == -1) {
			free(cargv);
			perror("fork");
			exit(EXIT_FAILURE);
		}
		// Hijo encargado de ejecutar los comandos.
		if (cpid == 0) {
           	execvp(cargv[0], cargv);
            perror("execvp");
            free(cargv);
            exit(EXIT_FAILURE);
		}
		else {
			int wstatus;
			waitpid(cpid, &wstatus, 0);
			free(cargv);
		}
	}
	fclose(file);
    return EXIT_SUCCESS;
}