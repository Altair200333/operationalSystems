#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxLength  5

typedef struct 
{
	char* string;
	struct List* next;
}List;

List* createList()
{
	List* list = (List*)malloc(sizeof(List));
	if (list == NULL)
		return list;

	list->string = NULL;
	list->next = NULL;
	return list;
}
int append(char* string, List** list)
{
	if(*list == NULL)
	{
		*list = createList();
		if(*list == NULL)
			return -1;
		(*list)->string = string;
	}
	else
	{
		List* current = *list;
		while (current->next != NULL)
		{
			current = (List*)current->next;
		}
		List* newItem = createList();
		if (newItem == NULL)
			return -1;

		newItem->string = string;
		current->next = (struct List*)newItem;
	}
	return 0;

}
void dispose(List* list)
{
	List* current = list;
	while (current != NULL)
	{
		if (current->string != NULL)
			free(current->string);
		List* prev = current;
		current = (List*)current->next;
		free(prev);
	}
}
int main()
{
	char text[maxLength];

	List* list = NULL;

	printf("Type words, when you are done type '.' :\n");
	while (*(fgets(text, maxLength, stdin)) != '.')
	{
		size_t length = strlen(text);
		length += text[length-1] != '\n';

		char* truncated = (char*)malloc(length);
		if (truncated != NULL)
		{
			memcpy(truncated, text, length);
			truncated[length-1] = 0;
			if(append(truncated, &list)!=0)
			{
				printf("Allocation failed\n");
				dispose(list);
				return 1;
			}
		}
		else
		{
		  dispose(list);
		  return 1;
		}
	}

	List* current = list;
	while(current != NULL)
	{
		printf("%s\n", (char*)current->string);
		current = (List*)current->next;
	}
	dispose(list);

	return 1;
}

