#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxLength 255

typedef struct 
{
	char** strings;
	size_t count;
	size_t allocated;
}List;
void append(char* string, List* list)
{
	if(list == NULL)
		return;
	if(list->count >= list->allocated)
	{
		char** newArray = (char**)realloc(list->strings, list->allocated * 2 * sizeof(char*));
		
		if(newArray==NULL)
			return;
		
		list->allocated *= 2;
		list->strings = newArray;
	}
	list->strings[list->count++] = string;

}
List* createList()
{
	List* list = (List*)malloc(sizeof(List));
	if (list == NULL)
		return list;
	list->allocated = 5;
	list->count = 0;
	list->strings = (char**)malloc(sizeof(char*) * list->allocated);
	return list;
}
int main()
{
	char text[maxLength];
	
	List* list = createList();
	if (list == NULL)
		return 0;

	printf("Type words, when you are done type '.' :\n");
	while (*(fgets(text, maxLength, stdin)) != '.')
	{	
		const size_t length = strlen(text);
	
		char* truncated = (char*)malloc(length);
		if (truncated != NULL)
		{
			memcpy(truncated, text, length);
			truncated[length-1] = 0;
			append(truncated, list);
		}
	}
	for(size_t i=0; i<list->count;++i)
	{
		printf("%s\n", (char*)list->strings[i]);
	}
	for(int i=0; i<list->count; ++i)
	{
		free(list->strings[i]);
	}
	free(list->strings);
	free(list);
	return 1;
}

