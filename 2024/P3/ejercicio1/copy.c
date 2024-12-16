#include <stdio.h>
#include <stdlib.h>
// includes añadidos para: open, read, write, close [man 2 open]
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// [man lstat]
#include <unistd.h>
#include <sys/stat.h>

#define MAX 512

void copy(int fdo, int fdd)
{
	char buffer[MAX];
	struct stat * statbuf = malloc(sizeof(struct stat));
	// Controlamos error en el fstat y el malloc
	if (!statbuf || fstat(fdo, statbuf) == -1) {
		fprintf(stderr, "fstat function failed\n");
		return;
	}
	// Verificamos cuantos bloques de 512 B hay en el archivo src
	int blocks = statbuf->st_size / MAX;
	int rd, wr;
	for(int i = 0; i < blocks; ++i) {
		rd = read(fdo, buffer, MAX);
		if (rd == -1) {
			fprintf(stderr, "read function failed\n");
			return;
		}
		wr = write(fdd, buffer, MAX);
	}
	if (statbuf->st_size % MAX != 0) {
		int aux_size = (statbuf->st_size - (blocks * MAX));
		rd = read(fdo, buffer, aux_size);
		if (rd == -1) {
			fprintf(stderr, "read function failed\n");
			return;
		}
		wr = write(fdd, buffer, aux_size);
	}
	// Liberamos la memoria reservada para la estructura
	free(statbuf);
}	

int main(int argc, char *argv[])
{
	// Si no llegan los argumentos esperados, informamos al usuario y salimos del programa
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <src file> <dest file>\n", argv[0]);
		return 0;
	}
	// Abrimos los archivos destino y fuente con sus respectivos flags
	int fdo = open(argv[1], O_RDONLY), fdd = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, 0644);
	// Manejo de error de open
	if (fdo == -1 && fdd == -1) {
		fprintf(stderr, "Files can't be opened\n");
		return 0;
	}
	// Llamamos a la funcion copy
	copy(fdo, fdd);
	// Cerramos los fd
	close(fdo);
	close(fdd);
	return 0;
}
