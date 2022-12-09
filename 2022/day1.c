#include <stdio.h>
#include <ctype.h>

#define PART2

int main(void)
{
	int c,
	    cal, maxCal = 0,
	#ifdef PART2
	    maxCal2 = 0, maxCal3 = 0,
	#endif
	    item;
	char proc;
	FILE *file;

	file = fopen("day1_input.txt", "r");

	while ((c = getc(file)) != EOF)
	{
		if (c == '\n')
		{
			if (cal > maxCal)
			{
				if (maxCal > maxCal2)
				{
					if (maxCal2 > maxCal3)
						maxCal3 = maxCal2;

					maxCal2 = maxCal;
				}

				maxCal = cal;
		#ifdef PART2
			} else
			if (cal > maxCal2)
			{
				if (maxCal2 > maxCal3)
					maxCal3 = maxCal2;

				maxCal2 = cal;
			} else
			if (cal > maxCal3)
				maxCal3 = cal;
		#endif

			printf("Got new elf. Calories was %d.\n", cal);

			cal = 0;

			continue;
		}

		ungetc(c, file);

		fscanf(file, "%d", &item);
		cal += item;

		/* Skip new line character. */
		c = getc(file);
	}

#ifdef PART2
	printf("The three maxes were %d, %d, and %d. Their sum is %d.\n", maxCal, maxCal2, maxCal3, maxCal + maxCal2 + maxCal3);
#else
	printf("The max was %d.\n", maxCal);
#endif

	return 0;
}
