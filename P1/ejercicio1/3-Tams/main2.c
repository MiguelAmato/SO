#include <stdio.h>


/******** TAREAS *****
 * Compila el código y trata de ejecutarlo. Contesta a las cuestiones
 * 1. ¿Tenemos un problema de compilación o de ejecución?
 * 2. ¿Por qué se da el problema?. Soluciónalo, compila y ejecuta de nuevo.
 * 3. a,b,c, y x están declaradas de forma consecutiva. ¿Sus direcciones son concesutivas?
 * 4. ¿Qué significa el modificar "%lu" en printf()?
 * 5. ¿A qué dirección apunta "pc"? ¿Coincide con la de alguna variable anteriormente declarada? Si es así,
 * ¿Coinciden los tamaños de ambas?
 * 6. ¿Coincide el valor del tamaño de "array1" con el número de elementos del array? ¿Por qué?
 * 7. ¿Coinciden las direcciones a la que apunta cadena1 con la de cadena2?
 * 8. ¿Por qué los tamaños (según sizeof()) de cadena1 y cadena2 son diferentes?
 * 
 * 1. El problema es de compilación
 * 
 * 2. El problema se da al intentar establecer el tamaño del array con una variable
 * 
 * 3. No están en direcciones consecutivas
 * 
 * 4. Significa que se está estableciendo el formato como long unsigned
 * 
 * 5. El puntero pc apunta a la dirección de memoria de la variable x. Sus tamaños no coinciden porque x se refiere a un char y pc a una direccion de memoria
 * 
 * 6. El array ocupa 40 bytes (ARRAY_SIZE * Tamaño de un int), osea que no coincide el número de elementos con el tamaño
 * 
 * 7. No coinciden
 * 
 * 8. Porque el tamaño de cadena1 es el tamaño de una dirección de memoria y el tamaño de cadena2 es el de un array de 21 char
***************/

#define ARRAY_SIZE  10
#define ARRAY2_SIZE 7

int a;
unsigned long b = 8;
short c;
char x;
char* pc;

int array1[ARRAY_SIZE];
int array2[ARRAY2_SIZE];

char* cadena1 = "CADENA DE CARACTERES";
char cadena2[] = "CADENA DE CARACTERES";
int main()
{
	pc = &x;
	a = 16;
	printf("Direccion de a: %p Tam: %lu \n",&a,sizeof(a));
	printf("Direccion de b: %p Tam: %lu \n",&b,sizeof(b));
	printf("Direccion de c: %p Tam: %lu \n",&c,sizeof(c));
	printf("Direccion de x: %p Tam: %lu \n",&x,sizeof(x));
	printf("Direccion de pc: %p Direccion a la que apunta pc: %p Tam: %lu \n",
			&pc, pc, sizeof(pc));
	printf("Direccion de array: %p Direccion del elem 0: %p Tam de array: %lu \n",
			array1, &array1[0], sizeof(array1));
	printf("Direccion de cadena1: %p Direccion a la que apunta: %p Tam: %lu \n",
			&cadena1, cadena1, sizeof(cadena1));
	printf("Direccion de cadena2: %p Direccion a la que apunta: %p Tam: %lu \n",
			&cadena2, cadena2, sizeof(cadena2));
    return 0;
}
