#include <stdio.h>
#include <stdlib.h>

/********* TAREAS ************
 * Compila y ejecuta el código. Responde a las siguientes cuestiones:
 *  1. ¿Por qué cambia el valor de ptr[13] tras la asignación ptr = &c;?
 *  2. El código tiene (al menos) un error. ¿Se manifiesta en compilación o en ejecución? ¿Por qué?
 *  3. ¿Qué ocurre con la zona reservada por malloc() tras a asignación ptr = &c;? ¿Cómo se puede acceder a ella?
 *     ¿Cómo se puede liberar dicha zona?
 * 
 * 	1. Porque has dejado de apuntar a la direccion de memoria (del array) y ahora estas apuntando a la de c, ptr[13] en ese caso seria
 * 	   13 direcciones de memoria despues que la direccion de memoria de c.	
 * 	2. Ejecución, estas liberando memoria que no es la que reservaste en el malloc
 * 	3. Que no es liberada y hay fugas de memoria, si no guardas anteriormente en un puntero la direccion reservada con el malloc antes
 * 	   de cambiar el ptr no puedes acceder a ella. Si se quisiera liberar habria que guardar la direccion a la memoria reservada.
 ************/

int nelem;
int c;

int main(void)
{
	int *ptr;
	int i;

	c = 37;
	nelem = 127;
	ptr = (int*) malloc(nelem * sizeof(int));
	for (i=0; i < nelem; i++)
		ptr[i] = i;

	printf("ptr[0]= %d ptr[13]=%d \n", ptr[0], ptr[13]);

	// int *aux = ptr;
	ptr = &c;
	printf("ptr[0]= %d ptr[13]=%d \n", ptr[0], ptr[13]);

	free(ptr); // Esto da error
	// free(aux); // En cambio si guardamos en un aux antes de hacer el cambio, libera correctamente.

}

