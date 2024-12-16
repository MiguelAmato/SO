#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern char *optarg;
extern int optind, opterr, optopt;

void show_file(int fd) {
	char buffer;
    ssize_t bytes_read;
    while ((bytes_read = read(fd, &buffer, 1)) > 0) {
        if (write(1, &buffer, 1) == -1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1) {
		close(fd);
        perror("read");
    }
}

int main(int argc, char *argv[])
{
	int opt;
	// N: Tamaño del salto que tenemos que hacer, inicializado a 0
	// eOpt: Flag que indica si tenemos que leer al final o no
	int N = 0, eOpt = 0;
	while ((opt = getopt(argc, argv, "n:e")) != -1) {
		switch (opt)
		{
		case 'n':
			N = atoi(optarg);
			break;
		case 'e':
			eOpt = 1;
			break;
		default:
		    fprintf(stderr, "Usage: %s [-n N] [-e] <file>\n", argv[0]);
            exit(EXIT_FAILURE);
			break;
		}
	}
	if (optind >= argc) {
        fprintf(stderr, "Expected file name after options\n");
        exit(EXIT_FAILURE);
    }
	int fd = open(argv[optind], O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	// Desplazamiento del marcador de posición con lseek
    off_t file_size = lseek(fd, 0, SEEK_END); // Obtener el tamaño del archivo
    if (file_size == -1) {
        perror("lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (eOpt) {
        // Leer los últimos N bytes
        if (N > file_size) N = file_size; // Evitar desplazamiento fuera de los límites
        if (lseek(fd, -N, SEEK_END) == -1) {
            perror("lseek");
            close(fd);
            exit(EXIT_FAILURE);
        }
    } else {
        // Saltar los primeros N bytes
        if (N > file_size) N = file_size; // Evitar desplazamiento fuera de los límites
        if (lseek(fd, N, SEEK_SET) == -1) {
            perror("lseek");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    // Leer y escribir el contenido desde el marcador de posición hasta el final del archivo
	show_file(fd);

    // Cerrar el archivo
    close(fd);
    return 0;
}
