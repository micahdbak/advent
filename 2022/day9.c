#include <stdio.h>
#include <stdlib.h>

int abs(int a)
{
	if (a < 0)
		return -1 * a;
	else
		return a;
}

int sign(int a)
{
	if (a < 0)
		return -1;
	else
		return 1;
}

int min(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}

int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

struct coord
{
	int x, y;
	struct coord *next;
};

void pushCoord(struct coord *coord, int x, int y)
{
	struct coord *node, *prev;

	node = coord;

	for (; node != NULL; node = node->next)
	{
		// same coordinate
		if (x == node->x && y == node->y)
			return;

		prev = node;
	}

	prev->next = malloc(sizeof(struct coord));

	node = prev->next;
	node->x = x;
	node->y = y;
	node->next = NULL;
}

int main(void)
{
	FILE *file;
	int x, y,
	    X, Y,
	    dx, dy,
	    c,
	    i, j, k,
	    amount;
	char direction;
	struct coord coords, *node;
	
	file = fopen("day9_input.txt", "r");

	coords.x = 0;
	coords.y = 0;
	coords.next = NULL;

	c = getc(file);

	for (x = 0, y = 0, X = 0, Y = 0; c != EOF; c = getc(file))
	{
		ungetc(c, file);

		fscanf(file, "%c %d\n", &direction, &amount);

		for (i = 0; i < amount; ++i)
		{
			switch (direction)
			{
			case 'U':
				y -= sign(amount);
	
				break;
			case 'D':
				y += sign(amount);

				break;
			case 'L':
				x -= sign(amount);
	
				break;
			case 'R':
				x += sign(amount);
	
				break;
			}
	
			dx = x - X;
			dy = y - Y;

			// diagonal motion and in some direction it is moving >1
			if ((dx != 0 && dy != 0)
			 && (abs(dx) > 1 || abs(dy) > 1))
			{
				X += sign(dx);
				Y += sign(dy);
			} else {
				if (abs(dx) > 1)
					X += sign(dx);
				if (abs(dy) > 1)
					Y += sign(dy);
			}

			/*
			for (j = -6; j < 1; ++j)
			{
				for (k = -1; k < 6; ++k)
				{
					if (x == k && y == j)
						putc('H', stdout);
					else
					if (X == k && Y == j)
						putc('T', stdout);
					else
						putc('.', stdout);
				}

				putc('\n', stdout);
			}

			putc('\n', stdout);
			*/

			pushCoord(&coords, X, Y);
		}
	}

	for (i = 0, node = &coords; node != NULL; node = node->next)
	{
		printf("Point: %d %d\n", node->x, node->y);
		++i;
	}

	printf("The tail visited %d points.\n", i);

	return 0;
}
