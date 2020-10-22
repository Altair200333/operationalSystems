#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>


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
int kbhit();
Array* createArray(size_t initialSize);
void add(Array* a, size_t element);
void freeArray(Array* a);
void printLine(Array* arr, int strNumber, int file);
void cb()
{
 int c;
 while ((c = getch()) != '\n' && c != EOF) { }
}
int main()
{
    initscr();
    //-----------------------

    int file = open("f.txt", O_RDONLY);

    if (file == -1)
    {
        printw("File can't be read\n");
        refresh();
        endwin();
        getch();
        return 1;
    }

    Array* arr = createArray(10);
    if(arr == NULL)
    {
        printw("Memory allocation failed\n");
        refresh();
        endwin();
        getch();
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
        printw("%c", c);
        refresh();
    }

    for(int i=0;i<arr->size;++i)
    {
        printw("%zu \n", arr->array[i]);
        refresh();
    }

    //----------------------

    printw("You have 5 seconds to start input\n");
    refresh();

    bool oot = false;

    double start = dtime();
    while(! kbhit())
    {
	    if(dtime() - start>5000)
        {
            printw("Time ran out\n");
            refresh();
            oot = true;
            break;
        }
    }
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
            while((ch = getch()) != '\n' && ch != EOF)
            {
                buf[pos++] = ch;
            }

            int val = atoi(buf);
            if(val == -1)
                break;
            printw("%d\n", val);
            refresh();
            printLine(arr, val, file);
        }
    }

    printw("press any key to exit\n");
    refresh();
    getch();
    endwin();

    return 0;
}
void printLine(Array* arr, int strNumber, int file)
{
    if(strNumber<0 || strNumber >= arr->size)
    {
            printw("Out of range\n");
            refresh();
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
            printw("%c", c);
            refresh();
        }
    }
}
int kbhit()    /* comment */
{
    int ch, r;

    // turn off getch() blocking and echo
    nodelay(stdscr, TRUE);
    noecho();

    // check for input
    ch = getch();
    if( ch == ERR)      // no input
       r = FALSE;
    else                // input
    {
       r = TRUE;
       ungetch(ch);
    }

    // restore block and echo
    echo();
    nodelay(stdscr, FALSE);
    return r;
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

