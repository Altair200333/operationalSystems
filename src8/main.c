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
        printf("File can't be read\n");
        return 1;
    }

    return 0;
}
