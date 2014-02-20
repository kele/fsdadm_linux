#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
    if (argc != 3) {
        puts("Usage: read granularity file\n");
        return -1;
    }

    int granularity = atoi(argv[1]);

    int fd = open(argv[2], O_RDONLY);
    if (fd == -1) {
        puts("Bad file.");
        return -2;
    }

    char buf[10];
    size_t b;
    while (b = read(fd, buf, 10)) {
        printf("read %zu bytes\n", b); 
    }

    close(fd);
}
