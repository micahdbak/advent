#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LINESIZE  256

int contains(char *line, char c)
{
	int length, i;

	length = strlen(line);

	for (i = 0; i < length; ++i)
		if (line[i] == c)
			// Character found.
			return 1;

	return 0;
}

int priority(int c)
{
	if (isalpha(c))
	{
		if (islower(c))
			return c - 'a' + 1;
		else
			return c - 'A' + 27;
	}

	return 0;
}

int main(void)
{
	FILE *file;
	int c,
	    length, i,
	    badge, value, sum = 0;
	char line[3][LINESIZE];

	file = fopen("day3_input.txt", "r");

	c = getc(file);

	for (; c != EOF; c = getc(file))
	{
		ungetc(c, file);

		// Read 3 lines.
		fscanf(file, "%s\n%s\n%s\n", line[0], line[1], line[2]);
		printf("Got lines:\n\t%s\n\t%s\n\t%s\n", line[0], line[1], line[2]);

		length = strlen(line[0]);

		for (i = 0; i < length; ++i)
			if (contains(line[1], line[0][i]) && contains(line[2], line[0][i]))
				badge = line[0][i];

		value = priority(badge);
		sum += value;

		printf("The badge is %c. Its value is %d. The sum is now %d.\n", badge, value, sum);
	}

	printf("The total sum is %d.\n", sum);

	return 0;
}
