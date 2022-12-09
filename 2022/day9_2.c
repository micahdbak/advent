#include <stdio.h>
#include <stdlib.h>

#define NUM_KNOTS  10

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

void updatePos(struct coord *pos, struct coord *head)
{
	int dx, dy;

	if (pos == NULL)
		return;

	dx = head->x - pos->x;
	dy = head->y - pos->y;

	// diagonal motion and in some direction it is moving >1
	if ((dx != 0 && dy != 0)
	 && (abs(dx) > 1 || abs(dy) > 1))
	{
		pos->x += sign(dx);
		pos->y += sign(dy);
	} else {
		if (abs(dx) > 1)
			pos->x += sign(dx);
		if (abs(dy) > 1)
			pos->y += sign(dy);
	}

	updatePos(pos->next, pos);
}

struct coord *makePos(struct coord *node, int n)
{
	int i;

	node->x = 0;
	node->y = 0;

	for (i = 1; i < n; ++i)
	{
		node->next = malloc(sizeof(struct coord));
		node = node->next;

		node->x = 0;
		node->y = 0;
	}

	node->next = NULL;

	// return the tail
	return node;
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
	struct coord coords, head, *tail, *node;
	
	file = fopen("day9_input.txt", "r");

	coords.x = 0;
	coords.y = 0;
	coords.next = NULL;

	tail = makePos(&head, NUM_KNOTS);

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
				head.y -= sign(amount);
	
				break;
			case 'D':
				head.y += sign(amount);

				break;
			case 'L':
				head.x -= sign(amount);
	
				break;
			case 'R':
				head.x += sign(amount);
	
				break;
			}
	
			updatePos(head.next, &head);
			pushCoord(&coords, tail->x, tail->y);
		}

		for (j = -12; j < 12; ++j)
		{
			for (k = -12; k < 12; ++k)
			{
				node = &head;

				for (i = 0; node != NULL; node = node->next, ++i)
				{
					if (k == node->x && j == node->y)
					{
						if (i == 0)
							putc('H', stdout);
						else
							putc(i + '0', stdout);

						break;
					}
				}

				if (node == NULL)
					putc('.', stdout);
			}

			putc('\n', stdout);
		}

		putc('\n', stdout);
	}

	for (i = 0, node = &coords; node != NULL; node = node->next)
	{
		printf("Point: %d %d\n", node->x, node->y);
		++i;
	}

	printf("The tail visited %d points.\n", i);

	return 0;
}
