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
#include <fcntl.h>
#include <string.h>

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
    int file = open("f.txt", O_RDWR);

    if (file == -1)
    {
        printf("File can't be opened\n");
        return 1;
    }

    struct flock fl_info;
    memset(&fl_info, 0, sizeof(fl_info));

    fl_info.l_type = F_WRLCK;
    fl_info.l_whence = SEEK_SET;
    fl_info.l_start = 0;
    fl_info.l_len = 0;

    int ret = fcntl(file, F_GETLK, &fl_info);
    if(ret == -1)
    {
        printf("Failed to lock file\n");
        closeF(file);
        return 1;
    }
    if (fl_info.l_type != F_UNLCK) {
        printf("File is already locked by pid=%d\n", fl_info.l_pid);
        return 1;
    }
    fl_info.l_type = F_WRLCK;
    ret = fcntl(file, F_SETLK, &fl_info);

    if (ret == -1)
    {
        perror("fcntl(F_SETLK)");
        printf("Failed to lock file\n");
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
