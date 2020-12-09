#include <sys/types.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void closeF(int fd)
{
    int ret = close(fd);
    if(ret == -1)
    {
        printf("Could not close file\n");
    }
}

int main()
{
    int file = open("f.txt", O_RDONLY);

    if (file == -1)
    {
        printf("File can't be opened\n");
        return 1;
    }

    if (flock(file, LOCK_EX | LOCK_NB) == -1 && errno == EAGAIN)
    {
        while (flock(file, LOCK_EX) == -1 && errno == EINTR)
            printf("attempt to open\n");

        printf("Failed to lock file\n");
        closeF(file);
        return 1;
    }

    int sysRes = system("nano f.txt");

    if (sysRes == -1)
    {
        printf("Failed to execute child process\n");
        closeF(file);
        flock(file, LOCK_UN);
        return 1;
    }

    return 0;
}
