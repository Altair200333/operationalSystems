#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define path "test.txt"

int try()
{
	printf("The real user ID is %d\n", getuid());
        printf("The effective user ID is %d\n", geteuid());

	FILE* f = fopen(path, "r");
	if(f == NULL)
	{
		perror("File pointer = NULL\n");	
		return -1;
	}
	else
	{
		printf("file opened successfully\n");
		fclose(f);
		return 0;
	}
}
int main()
{
	int res = try();
	if (res == 0)
	{
		int res = setuid(getuid());
		if(res != 0)
			printf("Failed to set effective uid\n");
		try();
	}

	return 0;
}

