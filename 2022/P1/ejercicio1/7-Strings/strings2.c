#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/********* TAREAS ************
 * Compila y ejecuta el código. Responde a las siguientes cuestiones:
 * 1. El código de "copia" no funciona. ¿Por qué?
 * 2. Usa ahora la función copia2() (descomenta la línea correspondiente). ¿Funciona la copia?
 * 3. Propón una implementación correcta de la copia.
 * 4. ¿Qué hace la funcióbn "mod"? ¿Por qué funciona?
 * 5. Descomenta la última llamada a la función "mod". Compila y ejecuta. ¿Por qué se produce el error?
 * 
 * 1. Ya que en C al no existir el paso por referencia al hacer dst = src no estas cambiando el valor original del puntero, 
 * 	  por lo que no copia.
 * 2. Copia2 no copia el valor de org a dst, lo que hace es hacer que dst apunte a lo que apunte org.
 * 3. Hecho (funcion copiaCorrecta)
 * 4. Modifica el valor ascii de los caracteres de la cadena, funciona porque hay margen para modifcar, si se usase otro caracter
 * 	 quizas de excepcion.
 * 5. Se produce una excepcion porque no reservas memoria en cad1
 ************** */

void copiaCorrecta(char* org, char* dst, int size) {
	int i;

	for(i = 0; i < size; i++) {
		*dst = *org;
		dst++;
		org++;
	}
}

void copia2(char* org, char** dst)
{
	*dst = org;
}

void copia(char* org, char* dst)
{
	dst = org;
}

void mod(char* org, char* dst)
{
	int i;

	for (i=0;i<strlen(org);i++)
		dst[i] = org[i] - 32;
	dst[i] = '\0';
}

int main()
{
	char* cad1 = "original";
	char* cad2 = "otra";
	char cad3[32];

	//copia(cad1,cad2);
	//copia2(cad1, &cad2);

	cad2 = malloc(sizeof(char) * strlen(cad1));
	copiaCorrecta(cad1, cad2, strlen(cad1));

	printf("cad1 %s cad2 %s\n", cad1, cad2);

	mod(cad1, cad3);
	printf("cad1 %s cad3 %s\n", cad1,cad3);

	cad1 = malloc(sizeof(char) * strlen(cad1));
	mod(cad1, cad1);

	free(cad1);
	free(cad2);
	return 0;
}

