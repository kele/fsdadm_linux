#include <errno.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/fsdadm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DRIVER_PATH "/dev/fsdadm"
int open_driver()
{
    return open(DRIVER_PATH, O_RDWR);
}

int install_hook(const char *filename)
{
    struct fsdadm_ioc_hook io;
    int err;
    int drv;
    int mnt;

    if ((drv = open_driver()) == -1)
        return errno;

    if ((mnt = open(filename, O_RDONLY)) == -1) {
        err = errno;
        close(drv);
        return err;
    }

    io.io_fd = mnt;
    io.io_type = FSDADM_TYPE_READLESS;
    io.io_params.readless.probability = 50;
    io.io_params.readless.range[0] = 1;
    io.io_params.readless.range[1] = 10;
    if (ioctl(drv, FSDADM_IOC_INSTALL, &io) == -1) {
        err = errno;
        close(mnt);
        close(drv);
        return err;
    }

    printf("Installed hook of id: %u\n", (unsigned)io.io_id);
    
    close(mnt);
    close(drv);
    return 0;
}

int remove_hook(uint64_t id)
{
    int err;
    int drv;

    if ((drv = open_driver()) == -1)
        return errno;


    if (ioctl(drv, FSDADM_IOC_REMOVE, &id) == -1) {
        err = errno;
        close(drv);
        return err;
    }
    
    close(drv);
    return 0;
}

int main(int argc, char *argv[])
{
    int err;

    if (argc != 3) {
        puts("Usage:\n"
            "\tfsdadm i filename\n"
            "\tfsdadm r id\n\n");
        return 1;
    }

    switch (argv[1][0]) {
    case 'i':
        err = install_hook(argv[2]);    
        break;
    case 'r':
        err = remove_hook(atoi(argv[2]));
        break;
    }

    if (err) {
        printf("Error: %s\n", strerror(err));
        return -1;
    }

    return 0;
}
