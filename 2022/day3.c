#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define LINESIZE  512

int contains(char *line, int c, int lbound, int rbound)
{
	int i;

	for (i = lbound; i < rbound; ++i)
		if (c == line[i])
			return 1;

	return 0;
}

int priority(int c)
{
	if (isalpha(c) && islower(c))
		return (c - 'a') + 1;

	if (isalpha(c) && isupper(c))
		return (c - 'A') + 27;

	// Should never happen..?
	return 0;
}

void alphasort(char *line)
{
	int i, length, repeat;
	char temp;

	length = strlen(line);
	repeat = 1;

	while (repeat)
	{
		repeat = 0;

		for (i = 0; i < length; ++i)
			if (line[i] < line[i - 1])
			{
				temp = line[i];
				line[i] = line[i - 1];
				line[i - 1] = temp;

				repeat = 1;
			}
	}
}

void removeDups(char *line)
{
	int i, j, length, c;
	char *old;

	length = strlen(line);
	old = calloc(length, sizeof(char));
	strcpy(old, line);

	j = 0;
	line[j] = old[0];

	for (i = 1; i < length; ++i)
	{
		if (old[i] != old[i - 1])
			line[++j] = old[i];
	}

	free(old);
	line[++j] = '\0';
}

int main(void)
{
	FILE *file;
	char line[3][LINESIZE], group[LINESIZE];
	int c, i, length, count, repeat, value, sum = 0;

	file = fopen("day3_input.txt", "r");

	c = getc(file);

	for (; c != EOF; c = getc(file))
	{
		ungetc(c, file);
		fscanf(file, "%s\n", line[0]);
		fscanf(file, "%s\n", line[1]);
		fscanf(file, "%s\n", line[2]);

		/*
		for (i = 0; i < lbound; ++i)
		{
			if (contains(line, line[i], lbound, rbound))
			{
				repeat = line[i];
				value = priority(repeat);
				sum += value;

				break;
			}
		}
		*/

		alphasort(line[0]);
		removeDups(line[0]);

		alphasort(line[1]);
		removeDups(line[1]);

		alphasort(line[2]);
		removeDups(line[2]);

		strcpy(group, line[0]);

		length = strlen(line[0]);
		strcpy(&group[length], line[1]);

		length += strlen(line[1]);
		strcpy(&group[length], line[2]);

		alphasort(group);
		length = strlen(group);

		count = 1;

		for (i = 1; i < length; ++i)
		{
			if (group[i] == group[i - 1])
				++count;
			else {
				if (count == 3)
					repeat = group[i - 1];

				count = 1;
			}
		}

		value = priority(repeat);

		printf("Sorted line: %s\nRpeating character is %c.\nValue is %d.\n", group, repeat, value);

		sum += value;
	}

	printf("The total sum is %d.\n", sum);

	return EXIT_SUCCESS;
}
