#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>

void closePipe(int* pipes)
{
    if(close(pipes[0]) == -1)
    {
        printf("Failed to close pipe[0]\n");
    }
    if(close(pipes[1]) == -1)
    {
        printf("Failed to close pipe[1]\n");
    }
}
int main(int argc, char* argv[])
{
    char str[100] = "text to send to child";
    int pipes[2];
    if (pipe(pipes) == -1)
    {
        printf("Failed to pipe\n");
        return 1;
    }
    pid_t child = fork(); //Zero: Returned to the newly created child process; Positive value: Returned to parent or caller. The value contains process ID of newly created child process;

    if (child == -1)
    {
        printf("Failed to fork child process\n");
        closePipe(pipes);
        return 1;
    }
    int status;
    if (child == 0) //this branch runs in child process
    {
        if (read(pipes[0], str, 100) == -1)
        {
            printf("Failed to read from pipe\n");
            closePipe(pipes);
            return 1;
        }
        for (int i = 0; i < strlen(str); i++)
        {
            str[i] = (char)toupper(str[i]);
        }
        printf("Uppercase string: \"%s\" \n", str);
        closePipe(pipes);

        return 1;
    }
    else //this branch runs in parent process
    {
        if (write(pipes[1], str, strlen(str) + 1) == -1)
        {
            printf("Failed to write to pipe\n");
            closePipe(pipes);
            return 1;
        }
        printf("Sending \"%s\" \n", str);
        closePipe(pipes);

        do
        {
            //(*)The waitpid() system call : It suspends execution of the calling process until a child specified by pid argument has changed state.
            pid_t killedChildPid = waitpid(child, &status, 0);
            if(killedChildPid == -1)
            {
                printf("waitpid failde\n");
                return 0;
            }

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
            else if (WIFCONTINUED(status))
            {
                printf("Child process was resumed\n");
            }
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 0;
}
