#include <stdio.h>
#include <stdlib.h>

void main()
{
	int i;

	char str[20];
	char *pt = NULL;
	pt = malloc(20);

	char *start = pt;

	for (i = 'a'; i < 'j'; i++)
	{
		*pt = i;
		pt++;
	}
	*pt = '\0';

	pt = start;

	for (i = 0; *pt != '\0'; i++)
	{
		str[i] = *pt;
		pt++;
	}
	str[i] = '\0';

	pt = start;

	printf("Init test. String is: %s\n", str);



	printf("pt+1: %c\n", (*pt)++);

	printf("Elem: %c\n", *pt);

}