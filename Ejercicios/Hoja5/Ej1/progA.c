//progA.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(void)
{
    int i;
    int fd = open("fichero.txt", O_RDONLY);
    if (fd == -1) {
        return -1;
    }
    i = lseek( fd, 0, SEEK_END );
    printf(" %d\n", i);
    return 0;
}