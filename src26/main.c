#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define BUFS 256

bool closePipe(FILE* pipe)
{
    int status = pclose(pipe);
    if(status == -1)
    {
        printf("Failed to pclose\n");
        return false;
    }
    else
    {
         if (WIFEXITED(status))
         {
            printf("Child terminated normally, status = %d\n", WEXITSTATUS(status));
         }
         else if (WIFSIGNALED(status))
         {
            printf("Killed by signal with number %d\n", WTERMSIG(status));
         }
         else if (WIFSTOPPED(status))
         {
            printf("stopped by signal %d\n", WSTOPSIG(status));
         }
    }
    return true;
}
int main(int argc, char* argv[])
{
    char str[BUFS] = "echo text to send to child";

    FILE *pipe = popen(str, "r");
    if (pipe == NULL)
    {
        printf("Failed to open pipe\n");
        return 1;
    }

    char buf[BUFS];
    int length = fread(buf, 1, BUFS, pipe);
    if (length == -1)
    {
        printf("Failed to read from pipe\n");
        closePipe(pipe);
        return 1;
    }
    buf[length - 1] = '\0';
    closePipe(pipe);

    for(int i=0; i<length; ++i)
    {
        buf[i] = toupper(buf[i]);
    }

    printf("Uppercase: %s\n", buf);
    return 0;
}
