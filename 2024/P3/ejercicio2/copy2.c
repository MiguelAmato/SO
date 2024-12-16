#include <stdio.h>
#include <stdlib.h>
// includes añadidos para: open, read, write, close [man 2 open]
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// [man lstat]
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <sys/sysmacros.h>

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

void copy_link(char *orig, char *dest)
{
	struct stat sb;
	// Controlamos el error de lstat
	if (lstat(orig, &sb) == -1) {
		perror("lstat");
		exit(EXIT_FAILURE);
    }
	// Se reserva el tamaño del archivo en memoria + 1 que es el caracter nulo 
	char *buf = malloc(sizeof(char) * (sb.st_size + 1));
	// Controlamos el error del malloc
	if (!buf) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if (readlink(orig, buf, sb.st_size + 1) == -1) {
		perror("readlink");
		free(buf);
		exit(EXIT_FAILURE);
	}
	// Añadimos el caracter nulo al final del archivo
	buf[sb.st_size] = '\0';

	// Eliminar el archivo de destino si ya existe
    if (lstat(dest, &sb) == 0) {  // Si el archivo de destino existe
        if (unlink(dest) == -1) {  // Eliminar el archivo
            perror("unlink");
            free(buf);
            exit(EXIT_FAILURE);
        }
    }
	if (symlink(buf, dest) == -1) {
		perror("symlink");
		free(buf);
		exit(EXIT_FAILURE);
	}
	free(buf);
}

/* PARA CREAR UN ENLACE SIMBOLICO!
 * ln -s archivo mylink
 */

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
	struct stat statbuf;
	if (lstat(argv[1], &statbuf) == -1) {
		fprintf(stderr, "fstat function failed\n");
		// Cerramos los fd
		close(fdo);
		close(fdd);
		return 0;
	}
	if (S_ISREG(statbuf.st_mode)) {
		// Archivo regular
		copy(fdo, fdd);
	} else if (S_ISLNK(statbuf.st_mode)) {
		// Enlace simbólico
		copy_link(argv[1], argv[2]);
	} else {
		fprintf(stderr, "Unknown file\n");
	}

	// Cerramos los fd
	close(fdo);
	close(fdd);
	return 0;
}
