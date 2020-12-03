#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stropts.h>
#include <sys/select.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

double dtime()
{
   struct timeval  tv;
   gettimeofday(&tv, NULL);

   double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
   return time_in_mill;
}
typedef struct {
    size_t* array;
    size_t size;
    size_t capacity;
} Array;

Array* createArray(size_t initialSize);
bool add(Array* a, size_t element);
void freeArray(Array* a);
void printLine(Array* arr, int strNumber, char* buffer);
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
        getchar();
        return 1;
    }

    Array* arr = createArray(10);
    if(arr == NULL)
    {
        printf("Memory allocation failed\n");
        getchar();
        return 1;
    }

    off_t seekPos = lseek(file, 0, SEEK_END);
    if (seekPos == (off_t)-1)
    {
        printf("Failed to seek file\n");
        closeF(file);
        freeArray(arr);
        return 1;
    }

    int len = (int)seekPos + 1;
    lseek(file, 0, SEEK_SET);

    char* buffer = (char*)mmap(0, len, PROT_READ, MAP_SHARED, file, 0);
    if(buffer == MAP_FAILED)
    {
        printf("Unable to allocate buffer to save file in\n");
        closeF(file);
        freeArray(arr);
        return 1;
    }
    for(int i = 0; i < len; ++i)
    {
        if(buffer[i] == '\n')
        {
            if(!add(arr,i+1))
            {
                printf("Memory allocation in list failed\n");
                closeF(file);
                freeArray(arr);
                return 1;
            }
        }
    }

    //----------------------

    printf("You have 5 seconds to start input\n");

    bool oot = false;
    //---

    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    /* Wait up to five seconds. */

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    retval = select(1, &rfds, NULL, NULL, &tv);
    if(retval == 0)
    {
        printf("Out of time\n");
        oot = true;
    }
    //---
    if(oot == false)
    {
        bool exit = false;
        while (!exit)
        {
            int strNumber;
            if(scanf("%d", &strNumber) != 1)
            {
                int c;
                while((c = getchar()) != '\n' && c != EOF);
                printf("Incorrect input\n");
                continue;
            }
            if (strNumber == -1)
                exit = true;
            else
                printLine(arr, strNumber, buffer);
        }
    }
    munmap(buffer, len);
    freeArray(arr);
    closeF(file);
    return 0;
}
void printLine(Array* arr, int strNumber, char* buffer)
{
    if(strNumber<0 || strNumber >= arr->size)
    {
            printf("Out of range\n");
    }
    else
    {
        size_t start;
        size_t end;
        if(strNumber == 0)
        {
            start = 0;
            end = arr->array[0];
        }
        else
        {
            start = arr->array[strNumber - 1];
            end = arr->array[strNumber];
        }
        for(int i=start;i<end;++i)
        {
            printf("%c", buffer[i]);
        }
    }
}

Array* createArray(size_t initialSize)
{
    Array* a = (Array*)malloc(sizeof(Array));
    if (a == NULL)
        return a;
    a->array = (size_t*)malloc(initialSize * sizeof(size_t));
    a->size = 0;
    a->capacity = initialSize;

    return a;
}

bool add(Array* a, size_t element)
{
    if (a->size == a->capacity) 
    {
        a->capacity *= 2;
        size_t* data = (size_t*)realloc(a->array, a->capacity * sizeof(size_t));
        if(data == NULL)
            return false;
        a->array = data;
    }
    a->array[a->size++] = element;
    return true;
}

void freeArray(Array* a) {
    free(a->array);
    a->array = NULL;
    a->size = a->capacity = 0;
}

