#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

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

int main() {
    char input[] = "   ls   -l   -t ";
    char *argv[10]; // Suficiente espacio para hasta 10 argumentos

    printf("Entrada original: '%s'\n", input);

    // Contar argumentos
    int argc = setargs(input, NULL);
    printf("Número de argumentos: %d\n", argc);

    // Extraer argumentos
    setargs(input, argv);

    // Mostrar argumentos
    for (int i = 0; i < argc; ++i) {
        printf("argv[%d] = '%s'\n", i, argv[i]);
    }

    return 0;
}