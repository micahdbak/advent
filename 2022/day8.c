#include <stdio.h>

#define MAP_WIDTH   99
#define MAP_HEIGHT  99

int main(void)
{
	FILE *input;
	int c,
	    x, y,
	    tree[MAP_HEIGHT][MAP_WIDTH],
	/*  visible, */ // part a
	    i,
	    visL, visR,
	    visU, visD,
	    scenic, scenicMax;

	input = fopen("day8_input.txt", "r");

	c = getc(input);

	// for every character in the file
	for (x = 0, y = 0; c != EOF; c = getc(input))
	{
		// if there is a new line, increment our row and set the column back to zero
		if (c == '\n')
		{
			x = 0;
			++y;

			continue;
		}

		// upload the value of this tree into our matrix
		tree[y][x++] = c - '0';
	}

	// part a
	// initialize the number of visible trees to zero
	// visible = 0;
	
	// part b
	scenicMax = 0;

	// for every tree in the matrix
	for (y = 0; y < MAP_HEIGHT; ++y)
	{
		for (x = 0; x < MAP_WIDTH; ++x)
		{
			printf("%d", tree[y][x]);

			// part a
			/*
			// edge tree; automatically visible
			if (x == 0 || y == 0 || x == MAP_WIDTH - 1 || y == MAP_HEIGHT - 1)
			{
				++visible;

				continue;
			}

			// initialize visibility checks
			visL = 1;
			visR = 1;
			visU = 1;
			visD = 1;

			// for every tree in this row
			for (i = 0; i < MAP_WIDTH; ++i)
			{
				// don't check the tree at x
				if (i == x)
					continue;

				// check if a tree is larger or equal to this tree
				if (tree[y][i] >= tree[y][x])
				{
					// not visible from the left
					if (i < x)
					{
						visL = 0;

						// skip i to x
						i = x;
					} else {
					// not visible from the right
						visR = 0;

						break;
					}
				}
			}

			// for every tree in this column
			for (i = 0; i < MAP_HEIGHT; ++i)
			{
				// don't check the tree at y
				if (i == y)
					continue;

				// check if a tree is larger or equal to this tree
				if (tree[i][x] >= tree[y][x])
				{
					// not visible from the top
					if (i < y)
					{
						visU = 0;

						// skip i to y
						i = y;
					} else {
					// not visible from the bottom
						visD = 0;

						break;
					}
				}
			}

			if (visL || visR || visU || visD)
				++visible;
			*/

			// part b

			// check left
			for (visL = 0, i = x - 1; i >= 0; --i)
			{
				++visL;

				// view is blocked
				if (tree[y][i] >= tree[y][x])
					break;
			}

			// check right
			for (visR = 0, i = x + 1; i < MAP_WIDTH; ++i)
			{
				++visR;

				// view is blocked
				if (tree[y][i] >= tree[y][x])
					break;
			}

			// check up
			for (visU = 0, i = y - 1; i >= 0; --i)
			{
				++visU;

				// view is blocked
				if (tree[i][x] >= tree[y][x])
					break;
			}

			// check down
			for (visD = 0, i = y + 1; i < MAP_HEIGHT; ++i)
			{
				++visD;

				// view is blocked
				if (tree[i][x] >= tree[y][x])
					break;
			}

			scenic = visL * visR * visU * visD;

			if (scenic > scenicMax)
				scenicMax = scenic;
		}
		putc('\n', stdout);
	}

	// part a
	// printf("There are %d trees visible from the edge in this matrix.\n", visible);
	
	// part b
	printf("The highest scenic value is %d.\n", scenicMax);

	return 0;
}
