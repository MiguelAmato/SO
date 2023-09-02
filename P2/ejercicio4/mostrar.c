#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

void mostrar(int fd, int size, int n, bool eOpt) {
	char* buffer = malloc(sizeof(char) * size);
	int offset;
	if (eOpt == false)  
		offset = lseek(fd, n, SEEK_SET);
	else 
		offset = lseek(fd, -n, SEEK_END);
	pread(fd, buffer, size, offset);
	write(1, buffer, size);
	free(buffer);
}

int main(int argc, char *argv[])
{	
	FILE* file = NULL;
	int n =  0;

	if (argc < 2) {
		fprintf(stderr,"Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}

	int opt;
	bool eOpt = false;

	while((opt = getopt(argc, argv, "n:e")) != -1) {
		switch(opt) {
			case 'n':
				if (optarg != NULL)
					n = atoi(optarg);
				break;
			case 'e':
				eOpt = true;
				break;
			default:
				break;
		}
	}
	
	int fd;

	if ((fd = open(argv[optind], O_RDONLY)) == -1)
		fprintf(stderr,"The input file %s could not be opened", argv[optind]);

	struct stat info;
	int stat = fstat(fd, &info); 

	

	mostrar(fd, info.st_size, n, eOpt);

	close(fd);

	return 0;
}

