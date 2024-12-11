#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <dirent.h>

int get_size(char *fname, int blocks) // Quitar el fname que es irrelevante
{
	return (blocks / 2);
}

int get_size_dir(char *dname, int blocks)
{
	DIR* dir = opendir(dname);
	int suma = blocks / 2;
	struct dirent* d = readdir(dir);
	struct stat info;
	while(d != NULL) {
		if(strcmp(d->d_name,".") != 0 && strcmp(d->d_name,"..") != 0) {
			lstat(d->d_name, &info); 
			if (S_ISDIR(info.st_mode)) 
				suma += get_size_dir(d->d_name, (int)(info.st_blocks));
			else
				suma += get_size(d->d_name, (int)(info.st_blocks));
		}
		 d = readdir(dir);
	}
	return suma;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr,"Usage: %s <file_name1> <file_name2> ...\n", argv[0]);
		exit(1);
	}

	int i;
	struct stat info;

	for (i = 1; i < argc; ++i) {
		lstat(argv[i], &info); 
		if (S_ISDIR(info.st_mode)) 
			printf("%dK %s \n", get_size_dir(argv[i], info.st_blocks), argv[i]);
		else
			printf("%dK %s \n", get_size(argv[i], info.st_blocks), argv[i]);
	}

	return 0;
}