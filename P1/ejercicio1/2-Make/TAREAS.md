1. Examina el makefile, identifica las variables definidas, los objetivos
   (targets) y las regalas.
2. Ejecuta make en la linea de comandos y comprueba las ordenes que ejecuta para
   construir el proyecto.
3. Marca el fichero aux.c como modificado ejecutando touch aux.c. Después
   ejecuta de nuevo make. ¿Qué diferencia hay con la primera vez que lo
   ejecutaste? ¿Por qué?
4. Ejecuta la orden make clean. ¿Qué ha sucedido? Observa que el objetivo clean
   está marcado como phony en la orden .PHONY: clean. ¿por qué? Para comprobarlo
   puedes comentar dicha línea del makefile, compilar de nuevo haciendo make, y
   después crear un fichero en el mismo directorio que se llame clean, usando el
   comando touch clean. Ejecuta ahora make clean, ¿qué pasa?
5. Comenta la linea LIBS = -lm poniendo delante una almoadilla (#). Vuelve a
   contruir el proyecto ejecutando make (haz un clean antes si es necesario).
   ¿Qué sucede? ¿Qué etapa es la que da problemas?


1. Variables:
   CC: compilador gcc
   CFLAGS: Los flags del comando gcc
   LIBS: Las librerias enlazadas 

   Targets:
   all, clean, .PHONY, %o

   Reglas:
   %.o: %.c
	   $(CC) $(CFLAGS) -c -o $@ $<

   all : aux.o init.o aux.h
	   gcc $(CFLAGS) -o prueba aux.o init.o $(LIBS)

   clean:
	   -rm *.o prueba



2. Hecho, ejecuto la linea de orden del target all

3. Ahora se ejecuta la linea de orden de la regla implicita %o ya que en la lista de dependecias de esta te dice que si hay alguna           modificacion en el archicvo .c con el mismo nombre (en este caso, touch aux.c) se ejecuta dicha linea de orden, una vez ejecutada, vuelve a la regla del target all y ejecuta la linea de orden de la misma.

4. Al hacer el make clean se ha ejecutado su linea de orden en la que se eliminan todos los archivos .o y el archivo prueba.

   El objetivo .PHONY hace que siempre se ejecute el objetivo clean independientemente de si existe un archivo clean o no y si este existe, que se ejecute el objetivo independientemente de si se ha modificado o no. 

   Al crear un archivo clean, al no tener ninguna modificacion no se ejecuta la linea de orden del objetivo clean.

5. Como hemos comentado la variable LIBS se producen errores en la etapa de enlazado ya que se pretende utilizar funciones definidas en una libreria que no está enlazada