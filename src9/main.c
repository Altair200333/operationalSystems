#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    char* path = "f.txt";

    pid_t child = fork(); //Zero: Returned to the newly created child process; Positive value: Returned to parent or caller. The value contains process ID of newly created child process;

    if (child == -1)
    {
        printf("Failed to fork child process\n");
        return 1;
    }
    int status;
    if (child == 0) //this branch runs in child process
    {
        execl("/bin/cat", "cat", path, (char*)NULL);
        return 0;
    }
    else //this branch runs in parent process
    {
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
                printf("Child terminated normally, status=%d\n", WEXITSTATUS(status));
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
