#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

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
    char str[100] = "echo text to send to child";

    FILE *pipe = popen(str, "r");

    closePipe(pipe);
    return 0;
}
