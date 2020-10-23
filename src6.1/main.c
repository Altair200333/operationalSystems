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
void add(Array* a, size_t element);
void freeArray(Array* a);
void printLine(Array* arr, int strNumber, int file);

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

    size_t position = 0;
    char c;
    int res = read(file, &c, 1);

    while (res != 0)
    {
        if(c == '\n')
        {
            //printf("Add\n");
            add(arr, position+1);
        }
        ++position;
        res = read(file, &c, 1);
        printf("%c", c);
    }

    for(int i=0;i<arr->size;++i)
    {
        printf("%zu \n", arr->array[i]);
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
        int pos = 0;
        char buf[20];
        while(1)
        {
            pos = 0;
            for(int i=0;i<20;++i)
                buf[i] = '\0';

            char ch;
            while((ch = getchar()) != '\n')
            {
                buf[pos++] = ch;
            }
            int val = atoi(buf);
            if(val == -1)
                break;
            //printf("%d\n", val);
            printLine(arr, val, file);
        }
    }

    return 0;
}
void printLine(Array* arr, int strNumber, int file)
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
            lseek(file, i, SEEK_SET);
            char c;
            read(file, &c, 1);
            printf("%c", c);
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

void add(Array* a, size_t element)
{
    if (a->size == a->capacity) 
    {
        a->capacity *= 2;
        size_t* data = (size_t*)realloc(a->array, a->capacity * sizeof(size_t));
        if(data == NULL)
            return;
        a->array = data;
    }
    a->array[a->size++] = element;
}

void freeArray(Array* a) {
    free(a->array);
    a->array = NULL;
    a->size = a->capacity = 0;
}

