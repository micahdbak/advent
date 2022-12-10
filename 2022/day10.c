#include <stdio.h>
#include <ctype.h>

void increment_cycle(int *cycle, int amount, int *value)
{
	int i;

	for (i = 0; i < amount; ++i)
	{
		if (*value - 1 <= *cycle % 40 && *value + 1 >= *cycle % 40)
			putc('#', stdout);
		else
			putc('.', stdout);

		if (((*cycle + 1) % 40) == 0)
			putc('\n', stdout);

		++(*cycle);
	}
}

int main(void)
{
	FILE *file;
	int c, i, cycle, n, value;
	char op[5];

	file = fopen("day10_input.txt", "r");

	c = getc(file);

	for (cycle = 0, value = 1; c != EOF; c = getc(file))
	{
		for (i = 0; !isspace(c); c = getc(file))
			op[i++] = c;

		op[i] = '\0';

		// noop
		if (c == '\n')
		{
			increment_cycle(&cycle, 1, &value);

			continue;
		}

		// addx

		fscanf(file, "%d\n", &n);

		increment_cycle(&cycle, 2, &value);

		value += n;
	}

	return 0;
}
