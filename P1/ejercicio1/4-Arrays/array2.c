#include <stdio.h>

/************ TAREAS
 * Compila y ejecuta el código. Contesta a las cuestiones:
 *  1. ¿La copia del array se realiza correctamente? ¿Por qué?
 *  2. Si no es correcto, escribe un código que sí realice la copia correctamente.
 *  3. Descomenta la llamada a la fución "tmo" en la función main(). Compila de nuevo y ejecuta.
 *  4. El problema que se produce, ¿es de compilación o de ejecución? ¿Por qué se produce?
 *  5. Encuentra un valor de MAXVALID superior a 4 con el que no se dé el problema. ¿Se está escribiendo
 *     más allá del tamaño del array? Si es así, ¿por qué funciona el código?
 * 
 * 1. No, ya que en C al no existir el paso por referencia al hacer dst = src no estas cambiando el valor original del puntero, 
 * 	  por lo que no copia.
 * 2. Hecho
 * 3. Hecho
 * 4. Ejecucion, porque estas sobreescribiendo valores de la pila que tienen contenido.
 * 5. El MAXVALID superior es el 6, porque hasta el 6 no hay valores en la pila que se usen, a partir de ahi ya sobreescribes valores usados
*******************/

#define N 10
#define MAXELEM 5000
#define MAXVALID 7

// funcion que imprime por pantalla el conteniod del array v de tam. size
void imprimeArray(int v[],int size)
{
	int i;
	printf("-------------------\n");
	for (i=0;i<size;i++) {
		printf("%d ", v[i]);
	}
	printf("\n\n");
}

// funcion que copia el contenido de un array en otro
void copyArray(int src[],int dst[],int size)
{
	// Lo que no comprendo es porque si descomento esta linea no copia bien el array teniendo lo de abajo 
	//dst = src;
	
	int i;
	for(i = 0; i < size; i++) {
		dst[i] = src[i];
	}
}

void tmo()
{
	int x = -1;
	int a[4] = {0,1,2,3};
	int b = 10000;
	int c = -1;
	int i;

	for  (i=4;i<MAXVALID;i++) // El valor maximo es 6
		a[i]=i;

	printf(" x %d b %d c %d\n", x,b,c);
}


int main()
{
	int A[N] = {4,3,8,5,6,9,0,1,7,2};
	int B[N];

	tmo();
	copyArray(A,B,N);
	imprimeArray(B,N);
}
