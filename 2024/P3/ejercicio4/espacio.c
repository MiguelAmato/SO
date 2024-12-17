#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

/* Forward declaration */
int get_size_dir(char *fname, size_t blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(size_t blocks)
{
	return blocks / 2;
}


/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t blocks)
{
	DIR *dir = opendir(dname);
	if (!dir) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}
	int total = blocks / 2;
	struct stat sb;
	struct dirent * dire = readdir(dir);
	while (dire) {
		if (strcmp(dire->d_name, ".") != 0 && strcmp(dire->d_name, "..") != 0) {
			if (lstat(dire->d_name, &sb)) {
				perror("lstat");
				exit(EXIT_FAILURE);
			}
			if (S_ISDIR(sb.st_mode)) 
				total += get_size_dir(dire->d_name, sb.st_blocks);
			else
				total += get_size(sb.st_blocks);
		}
		dire = readdir(dir);
	}
	return total;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	// Controlamos que los argumentos sean correctos.
	if (argc < 2) {
		fprintf(stderr, "Usage: %s [files].\n", argv[0]);
		return 0;
	}
	struct stat sb;
	// Para cada argumento se hace una llamada a lstat y se verifica que tipo
	// de fichero es para calcular su tamaño
	for (int i = 1; i < argc; ++i) {
		// Abrimos el archivo y manejamos el posible error (?)
		// Manejamos el error de lstat
		if (lstat(argv[i], &sb) == -1) {
			perror("lstat");
			exit(EXIT_FAILURE);
		}
		if (S_ISDIR(sb.st_mode)) 
			printf("%dK %s\n", get_size_dir(argv[i], sb.st_blocks), argv[i]);
		else
			printf("%dK %s\n", get_size(sb.st_blocks), argv[i]);
	}

	return 0;
}
