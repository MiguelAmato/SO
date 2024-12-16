#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <err.h>

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor
 *
 * The loadstr() function must allocate memory from the heap to store
 * the contents of the string read from the FILE.
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc())
 *
 * Returns: !=NULL if success, NULL if error
 */
#include <stdio.h>
#include <stdlib.h>

char *loadstr(FILE *file) {
    char c;
    int length = 0;
    long start_pos = ftell(file);
	printf("Posición inicial: %ld\n", start_pos);
    while (fread(&c, 1, 1, file) == 1 && c != '\0') {
        ++length;
	}
	printf("Length calculado: %d\n", length);
    if (feof(file) || ferror(file))
        return NULL;
    if (fseek(file, start_pos, SEEK_SET) == -1)
        return NULL;
    char *ret = malloc(length + 1);
    if (!ret)
        return NULL;
    fread(ret, 1, length + 1, file);
	ret[length] = '\0';
    return ret;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <output file>\n", argv[0]);
        return 0;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    char *line = loadstr(file);
	printf("%s\n", line);
    while (line) {
        printf("%s\n", line);
        free(line);
        line = loadstr(file);
    }
    fclose(file);
    return 0;
}

