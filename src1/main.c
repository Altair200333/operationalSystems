#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ulimit.h>
#include <sys/resource.h>

void printUserAndGroupIds()
{
  printf("real user ID: %d, effective user ID %d\n", getuid(), geteuid());
  printf("real group ID: %d, effective group ID %d\n", getgid(), getegid());
}
void setLeader(char* arg)
{
  int proc = atoi(arg);
  printf("setting process %d to be leader of a new group\n", proc);
  int res = setpgid(proc, proc);
  if(res == 0)
    printf("succeed\n");
  else
    printf("failed\n");
}
void processId()
{
  printf("current process ID: %d\n", getpid());
  printf("ID of the parent process: %d\n", getppid());
  printf("process group ID: %d\n", getgid());
}
void printUlimit()
{
  long res = ulimit(UL_GETFSIZE, 0);
  printf("ulimit: %d\n", res);
}
void setUlimit(char* arg)
{
  long limit = atol(arg);
  if(limit == 0)
    printf("failed to recognise argument: %s\n", arg);
  else
  {
    long res = ulimit(UL_SETFSIZE, limit);
    if(res > 0)
      printf("ulimit successfully set to %d\n", limit);
    else
      printf("failed to set ulimit\n");
  }
}
void printDir()
{
  char cwd[255];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
   }
}
void printEnv(char** envp)
{
  for (char **env = envp; *env != 0; env++)
  {
    char *thisEnv = *env;
    printf("%s\n", thisEnv);    
  }
}
void setEnv(char* val)
{
  int res = putenv(val);
  if(res == 0)
    printf("successfully set: %s code %d\n", val, res);
  else
    printf("failed to set: %s\n", val);
}
void coreFileSize()
{
  struct rlimit limit;
  int res = getrlimit(RLIMIT_CORE, &limit);
  if(res == -1)
   printf("failed to get core-file size limit\n");
  else
   printf("core-file limits (bytes):\n  soft: %ju\n  hard: %ju\n", (uintmax_t)limit.rlim_cur, (uintmax_t)limit.rlim_max);
}
void setCoreFileSize(char* arg)
{
  long lim = atol(arg);
  struct rlimit newLim = {.rlim_cur = lim, .rlim_max = lim};
  int res  = setrlimit(RLIMIT_CORE, &newLim);
  if(res == -1)
    printf("failed to set core file size limit to %ld\n", lim);
  else
    printf("core-file isze limit set to %ld\n", lim);
}
int main (int argc, char **argv, char **envp)
{
 if(argc == 1)
  printf("no arguments passed\n");
 int c;
 while(optind < argc)
  {
   if((c = getopt (argc, argv, "is:puU:dvV:cC:")) != -1)
    switch (c)
      {
      case 'i':
        printUserAndGroupIds();
        break;
      case 's':
        setLeader(optarg);
        break;
      case 'p':
        processId();
	break;
      case 'u':
        printUlimit();
        break;
      case 'U':
        setUlimit(optarg);
        break;
      case 'd':
        printDir();
        break;
      case 'v':
        printEnv(envp);
        break;
      case 'V':
        setEnv(optarg);
        break;
      case 'c':
        coreFileSize();
        break;
      case 'C':
        setCoreFileSize(optarg);
        break;
      case '?':
        printf("unknown argument \'%c\'\n", optopt);
        return 1;
      default:
        abort ();
      }
    else
     {
        printf("argument %s is not availible\n", argv[optind]);
        optind++;
     }
  }
 return 0;
}
