#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include <sys/types.h>
#include "unistd.h"
#include <sys/stat.h>
#include <fcntl.h>

typedef struct
{
    size_t* array;
    size_t size;
    size_t capacity;
} Array;

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

int main()
{
    int file = open("f.txt", O_RDONLY);

    if (file == -1)
    {
        printf("File can't be read\n");
        return 1;
    }

    Array* arr = createArray(10);
    if(arr == NULL)
    {
        printf("Memory allocation failed\n");
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
    }

    bool exit = false;
    while (!exit)
    {
        printf("Input string number (-1 to exit): ");
        int strNumber;
        if(scanf("%d", &strNumber) != 1)
	{
	    printf("Incorrect input\n");
	    continue;
	}
        if (strNumber == -1)
            exit = true;
    	else
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
    }

    freeArray(arr);
    close(file);
    return 0;
}

