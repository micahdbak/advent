#include <stdio.h>

int noRepeat(char *arr, int len)
{
	int i, j;

	for (i = 0; i < len; ++i)
		for (j = 0; j < len; ++j)
			if (i != j && arr[i] == arr[j])
				return 0;

	return 1;
}

#define LENGTH 14

int main(void)
{
	FILE *file;
	int c, i, j;
	char four[LENGTH + 1];

	file = fopen("day6_input.txt", "r");

	four[LENGTH] = '\0';

	for (i = 2; i <= LENGTH; ++i)
		four[LENGTH - i] = getc(file);

	c = getc(file);

	for (i = LENGTH; c != EOF; c = getc(file))
	{
		for (j = 1; j < LENGTH; ++j)
			four[LENGTH - j] = four[LENGTH - j - 1];

		four[0] = c;

		printf("The four characters being compared are: %s\n", four);

		if (noRepeat(four, LENGTH))
			break;

		++i;
	}

	printf("%d is the character where the message begins.\n", i);

	return 0;
}
