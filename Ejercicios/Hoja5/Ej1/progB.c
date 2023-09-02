//progB.c
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(void)
{
    char c = 'a';
    int fd = open("fichero.txt", O_WRONLY | O_CREAT | O_APPEND); // El append hace cositas
    if (fd == -1) {
        return -1;
    }
    // lseek(fd, 1024, SEEK_CUR);
    write(fd, &c, 1);
    return 0;
}
