#include <stdio.h>
#include <stdlib.h>

// Okay. Look. It was late at night. My computer has enough memory. I just made the grid really large for part 2. I'm sorry.

#undef PART2

#define SAND_START  500
#define ROOM  10

#define SOLID(T)  (T == ROCK || T == SAND)
#define FREE(T)   (T != ROCK && T != SAND)

enum type
{
	ROCK,
	SAND,
	AIR,
	FALL
};

void makeline(enum type **matrix, int x, int y, int xt, int yt)
{
	int min, max,
	    i;

	// horiz
	if (y == yt)
	{
		min = x < xt ? x : xt;
		max = x > xt ? x : xt;

		for (i = min; i <= max; ++i)
			matrix[i][y] = ROCK;
	// vert
	} else {
		min = y < yt ? y : yt;
		max = y > yt ? y : yt;

		for (i = min; i <= max; ++i)
			matrix[x][i] = ROCK;
	}
}

#define VOIDED  0
#define SANDED  1

int makesand(enum type **matrix, int x, int y, int w, int h)
{
	for (; y <= h - 1; ++y)
	{
	#ifdef PART2
		if (y == 0 && SOLID(matrix[x][y]))
			break;
	#endif

		if (SOLID(matrix[x][y]))
		{
			if (x == 0 || x == w - 1)
			{
				printf("Oops! Sand is going outside of the map at (%d,%d).\n", x, y);
				matrix[x][y - 1] = SAND;

				return SANDED;
			}

			if (FREE(matrix[x - 1][y]))
				--x;
			else
			if (FREE(matrix[x + 1][y]))
				++x;
			else {
				--y;

				matrix[x][y] = SAND;

				return SANDED;
			}
		} else
			matrix[x][y] = FALL;
	}

	return VOIDED;
}

int main(void)
{
	FILE *input;
	int c,
	    x, y, xt, yt,
	    l, r, h,
	    w,
	    sandx, sand;
	enum type **matrix;

	input = fopen("day14_input.txt", "r");

	c = getc(input);

	l = 500;
	r = 0;
	h = 0;

	for (; c != EOF; c = getc(input))
	{
		ungetc(c, input);

		// read an x-y-pair
		fscanf(input, "%d,%d", &x, &y);

		if (x < l)  l = x;
		if (x > r)  r = x;
		if (y > h)  h = y;

		printf("Got coordinate (%d,%d)\n", x, y);

		c = getc(input);

		if (c == '\n')
			continue;
		else
			fscanf(input, "-> ");
	}

	printf("Leftmost: %d, Rightmost: %d, Deepest: %d\n", l, r, h);

	#ifndef PART2
	w = r - l + 1 + (ROOM * 2);
	l -= ROOM;
	#endif
	++h;
	#ifdef PART2
	h += 2;
	w = r - l;
	l -= 2 * w;
	w *= 6;
	w += (ROOM * 2);
	l -= ROOM;
	#endif

	sandx = SAND_START - l;

	matrix = calloc(w, sizeof(enum type *));

	for (x = 0; x < w; ++x)
	{
		matrix[x] = calloc(h, sizeof(enum type));

		for (y = 0; y < h - 1; ++y)
			matrix[x][y] = AIR;

	#ifdef PART2
		matrix[x][h - 1] = ROCK;
	#else
		matrix[x][h - 1] = AIR;
	#endif
	}

	fseek(input, 0, SEEK_SET);

	fscanf(input, "%d,%d -> ", &x, &y);

	x -= l;

	c = getc(input);

	for (; c != EOF; c = getc(input))
	{
		ungetc(c, input);

		fscanf(input, "%d,%d", &xt, &yt);

		xt -= l;

		printf("Got (%d,%d) -> (%d,%d)\n", x, y, xt, yt);

		makeline(matrix, x, y, xt, yt);

		c = getc(input);

		if (c == '\n')
		{
			fscanf(input, "%d,%d -> ", &x, &y);

			x -= l;

			continue;
		} else {
			fscanf(input, "-> ");

			x = xt;
			y = yt;
		}
	}

	sand = 0;

	while (makesand(matrix, sandx, 0, w, h) != VOIDED)
		++sand;

	for (y = 0; y < h; ++y)
	{
		for (x = 0; x < w; ++x)
			switch (matrix[x][y])
			{
			case ROCK:  putc('#', stdout);  break;
			case SAND:  putc('O', stdout);  break;
			case FALL:  putc('~', stdout);  break;
			default:    putc('.', stdout);  break;
			}

		putc('\n', stdout);
	}

	printf("%d units of sand came to rest.\n", sand);

	return EXIT_SUCCESS;
}
