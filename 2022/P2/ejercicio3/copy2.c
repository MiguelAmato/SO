#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <time.h>

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

void copy2(char* fdo, char* fdd, int size) {
	char buffer[size + 1];
	int l;
	if ((l = readlink(fdo, buffer, sizeof(buffer) - 1)) != -1) 
		buffer[l] = '\0';
	struct stat info;
	lstat(buffer, &info);
	if (!S_ISREG(info.st_mode)) 
		err(2, "The input file %s is not a regular file", buffer);
	symlink(buffer, fdd);
}

int main(int argc, char* argv[])
{	
	int origen, destino;

	if (argc!=3) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}
	int eje = O_CREAT;
    eje = eje | O_RDONLY;
	if ((origen = open(argv[1], eje)) == -1)
		err(2,"The input file %s could not be opened",argv[1]);
	if ((destino = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC)) == -1) 
		err(2,"The input file %s could not be opened",argv[2]);

	struct stat info;
	int cosa = lstat(argv[1], &info); 

	if (S_ISREG(info.st_mode)) { // Booleano que te dice si es un regular file
		copy(origen, destino);
	}
	else if (S_ISLNK(info.st_mode)) { // Booleano que te dice si es un symbolic link
		copy2(argv[1], "copyLink", info.st_size);
	} 
	else {
		err(3,"The input file %s is not a regular file or symlink file", argv[1]);
	}

	close(origen);
	close(destino);

    return 0;
}