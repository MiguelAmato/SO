#include <stdio.h>
#include <string.h>

/********* TAREAS ************
 * Compila y ejecuta el código. Responde a las siguientes cuestiones:
 * 1. El código contiene un error. ¿Se manifiesta en compilación o en ejecución? ¿Por qué se produce?
 * Soluciona el error comentando la(s) línea(s) afectadas. Vuelve a compilar y ejecutar.
 * 2. ¿En qué dirección está la letra 'B' de la cadena Bonjour? ¿Y la de la la letra 'j'?
 * 3. Tras la asignación p=msg2;, ¿cómo podemos recuperar la dirección de la cadena "Bonjour"?
 * 4. ¿Por qué la longitud de las cadenas p y msg2 es 2 tras la línea 30?
 *    Se asignan 3 bytes a 'p' que modifican a ambos, pero luego la longitud es sólo 2.
 * 5. ¿Por qué strlen() devuelve un valor diferente a sizeof()?
 * 
 * 1. Ejecución, en la linea 35 al no definir el caracter de null msg no sabe cuando termina el array de caracteres. A la hora
 * 	  de mostrar el msg, mostrará el contenido sobreescrito y los 8 caracteres que contienen basura.
 * 2. En la direccion que apunta p.
 * 3. Habria que guardar antes de sobreescribir donde esta la direccion de la B ya que si no lo guardas se pierde.
 * 4. Porque se ha asignado el final de la cadena de caracteres en la pos 2, como lo dos apuntan a lo mismo la longitud es igual.
 * 5. Porque strlen devuelve la longitud hasta donde este el caracter null y sizeof devuelve lo que ocupa el array en memoria.  
 ************** */
int main()
{
	char msg[10]; /* array of 10 chars */
	char *p;      /* pointer to a char */
	char msg2[28] = "Hello";  /* msg2 = 'H' 'e' 'l' 'l' 'o' '\0' */

	p   = "Bonjour";
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("dir de msg: %p, dir de p: %p, dir de msg2: %p\n", msg, p, msg2);

	p = msg2;
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("dir de msg: %p, dir de p: %p, dir de msg2: %p\n", msg, p, msg2);

	p[0] = 'H', p[1] = 'i', p[2] = '\0';
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("msg len: %lu p len %lu msg2 len %lu\n", strlen(msg), strlen(p), strlen(msg2));
	printf("msg size: %lu p size %lu msg2 size %lu\n", sizeof(msg), sizeof(p), sizeof(msg2));

	msg[0] = 'B', msg[1] = 'y', msg[2] = '\0';
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
}

