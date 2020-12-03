#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include <sys/types.h>
#include "unistd.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

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
    if(a->array == NULL)
    {
        free(a);
        return NULL;
    }
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
        {
            a->capacity /= 2;
            return false;
        }
        a->array = data;
    }
    a->array[a->size++] = element;
    return true;
}

void freeArray(Array* a)
{
    free(a->array);
    a->array = NULL;
    a->size = a->capacity = 0;
}
void closeF(int fd)
{
    int ret = close(fd);
    if(ret == -1)
    {
        printf("Could not close file\n");
    }
}
bool tryReadFile(int file, char* buffer, size_t len)
{
    int attempts = 10;
    for(int i=0; i<attempts; ++i)
    {
        size_t readSize = read(file, buffer, len);
        if (readSize == -1 && errno != EINTR)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                printf("failed to read\n");
                return false;
            }
        }
        else if(readSize != -1)
            return true;
    }
    printf("Out of read attempts\n");
    return false;
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
        closeF(file);
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

    char* buffer = (char*)malloc(len);
    if(buffer == NULL)
    {
        printf("Unable to allocate buffer to save file in\n");
        closeF(file);
        freeArray(arr);
        return 1;
    }
    if (!tryReadFile(file, buffer, len))
    {
        closeF(file);
        freeArray(arr);
        free(buffer);
        return 1;
    }

    for(int i = 0; i < len; ++i)
    {
    	if(buffer[i] == '\n')
    	{
            if(!add(arr,i+1))
            {
                printf("failed to allocate array\n");
                closeF(file);
                freeArray(arr);
                free(buffer);
                return 1;
            }
    	}
    }
    free(buffer); //we don't need it anymore
    bool exit = false;
    while (!exit)
    {
        printf("Input string number (-1 to exit): ");
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
                lseek(file, start, SEEK_SET);
                char str[255];

                size_t readC = read(file, str, end - start);
                if (readC == -1 && errno != EINTR)
                {
                    printf("failed to read line\n");
                    freeArray(arr);
                    closeF(file);
                    return 0;
                }
                str[end - start - 1] = '\0';
                printf("%s\n", str);
            }
    	}
    }

    freeArray(arr);
    closeF(file);
    return 0;
}

