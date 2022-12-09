#include <stdio.h>

int main(void)
{
	FILE *file;
	int l1, r1, l2, r2, c, count = 0;

	file = fopen("day4_input.txt", "r");

	c = getc(file);

	for (; c != EOF; c = getc(file))
	{
		ungetc(c, file);

		fscanf(file, "%d-%d,%d-%d\n", &l1, &r1, &l2, &r2);

		if ((l2 >= l1 && l2 <= r1)
		 || (l1 >= l2 && l1 <= r2))
		{
			printf("The range from %d-%d overlaps with %d-%d!\n", l1, r1, l2, r2);
			++count;
		}
	}

	printf("Your count is: %d.\n", count);

	return 0;
}
