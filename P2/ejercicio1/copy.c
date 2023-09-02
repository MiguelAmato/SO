#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 512

void copy(int fdo, int fdd)
{
	char* buffer = malloc(sizeof(char) * BUFFER_SIZE);
	int ret = read(fdo, buffer, BUFFER_SIZE);
	while (ret == BUFFER_SIZE) {
		write(fdd, buffer, ret);
		ret = read(fdo, buffer, BUFFER_SIZE);
	}
	write(fdd, buffer, ret);
	free(buffer);
}

int main(int argc, char* argv[])
{	
	int origen, destino;

	if (argc!=3) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	if ((origen = open(argv[1], O_CREAT | O_RDONLY)) == -1)
		err(2,"The input file %s could not be opened",argv[1]);
	if ((destino = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC)) == -1)
		err(2,"The input file %s could not be opened",argv[2]);

	copy(origen, destino);

	close(origen);
	close(destino);

    return 0;
}