#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int  main()
{
     int res = putenv("TZ=America/Los_Angeles");
     if (res != 0)
     {
          //handle
       printf("Error\n");
     }


     time_t now;
     time( &now );
     char* p1 = ctime(&now);
     printf("%s\n", p1);

     sleep(1);

     time(&now);
     char* p2 = ctime(&now);
     printf("%s\n", p2);
     printf("%p - %p\n", p1, p2);

     return 0;
}
