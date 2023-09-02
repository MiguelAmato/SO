#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file =  NULL;
	char* c = malloc(sizeof(char));

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	while ((fread(c, sizeof(char), 1,file)) > 0) {
		// Print byte to stdout 
		
		if (*c==EOF){
			fclose(file);
			printf("cerro\n");
			err(3,"putc() failed!!");
		}
		else 
			fwrite(c, sizeof(char), 1, stdout);
	}

	free(c);
	fclose(file);
	return 0;
}


