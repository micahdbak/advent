#include <stdio.h>
#include <stdlib.h>

#undef PART2

#ifndef PART2
#define TARGET_Y  2000000
#else
#define MAX_Y  4000000
#define MAX_X  4000000
#endif

struct sensor
{
	int x, y,
	    bx, by,
	    distance;

	struct sensor *next;
};

struct range
{
	int lx, rx,
	    marked;

	struct exclude
	{
		int x;
		struct exclude *next;
	} *exclude;

	struct range *next;
};

#define abs(X)                         ( (X) < 0 ? -1 * (X) : (X) )
#define manhattan(X_1, Y_1, X_2, Y_2)  ( abs((X_1) - (X_2)) + abs((Y_1) - (Y_2)) )
#define min(X_1, X_2)                  ( (X_1) <= (X_2) ? (X_1) : (X_2) )
#define max(X_1, X_2)                  ( (X_1) >= (X_2) ? (X_1) : (X_2) )

void push_exclude(struct range *node, int x)
{
	struct exclude *exnode;

	if (node->exclude == NULL)
	{
		node->exclude = malloc(sizeof(struct exclude));
		exnode = node->exclude;
	} else {
		for (exnode = node->exclude;; exnode = exnode->next)
		{
			if (exnode->x == x)
				return;

			if (exnode->next == NULL)
				break;
		}

		exnode->next = malloc(sizeof(struct exclude));
		exnode = exnode->next;
	}

	exnode->next = NULL;
	exnode->x = x;
}

int is_exclude(struct range *node, int x)
{
	struct exclude *exnode;

	for (exnode = node->exclude; exnode != NULL; exnode = exnode->next)
		if (exnode->x == x)
			return 1;

	return 0;
}

void remove_node(struct range *start, struct range *goal)
{
	struct range *node;

	for (node = start;; node = node->next)
	{
		if (node->next == goal)
			break;

		// goal is not in start
		if (node == NULL)
			return;
	}

	node->next = goal->next;
}

void free_node(struct range *node)
{
	struct exclude *exnode, *exnext;

	for (exnode = node->exclude; exnode != NULL; exnode = exnext)
	{
		exnext = exnode->next;

		free(exnode);
	}

	free(node);
}

struct range *cut_range(struct range *start, struct range *cut)
{
	struct range *node, *next, *end, *push;

	for (end = start; end->next != NULL; end = end->next)
		;

	push = end;

	/* Note: end->next will change as this loops.
	 * This is no matter, because (node) should be
	 * (end) at some point, but not what proceeds
	 * (end), NULL or a previously cut (node). */
	for (node = start; node != end->next; node = next)
	{
		next = node->next;

		if (cut->lx <= node->lx && cut->rx >= node->rx)
		// this range is entirely cut
		{
			if (node == start)
				start = node->next;
			else
				remove_node(start, node);

			node->exclude = NULL;
			free_node(node);
		} else
		if (cut->lx > node->lx && cut->rx >= node->rx)
		// this range is reduced to the left of (cut)
			node->rx = cut->lx - 1;
		else
		if (cut->rx < node->rx && cut->lx <= node->lx)
		// this range is reduced to the right of (cut)
			node->lx = cut->rx + 1;
		else
		if (cut->lx > node->lx && cut->rx < node->rx)
		// (cut) is inside of this range
		{
			// (push) will be the right side of the split ranges

			push->next = malloc(sizeof(struct range));
			push = push->next;
			
			push->lx = cut->rx + 1;
			push->rx = node->rx;

			// (node) is the left side
			node->rx = cut->lx - 1;
		}
	}

	return start;
}

int main(void)
{
	FILE *input;
	int c,
#ifdef PART2
	    TARGET_Y,
	    finished = 0,
#else
	    count,
#endif
	    x, y, dy,
	    intersection;
	struct sensor *sensor, *snode;
	struct range *range, *node, *next,
#ifdef PART2
	             *possible,
#endif
	             *compile, *cnode, *cnext;
	struct exclude *exnode, *exnext;

	input = fopen("day15_input.txt", "r");

	sensor = malloc(sizeof(struct sensor));
	snode = sensor;

	c = getc(input);

	for (;;)
	{
		ungetc(c, input);

		fscanf(input, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &snode->x, &snode->y, &snode->bx, &snode->by);

		snode->distance = manhattan(snode->x, snode->y, snode->bx, snode->by);

		printf("Read sensor from file at (%d,%d), beacon at (%d,%d), distance %d.\n", snode->x, snode->y, snode->bx, snode->by, snode->distance);

		c = getc(input);

		if (c == EOF)
			break;

		snode->next = malloc(sizeof(struct sensor));
		snode = snode->next;
	}
	
	snode->next = NULL;

	fclose(input);

#ifdef PART2
	for (TARGET_Y = 0; TARGET_Y <= MAX_Y && !finished; ++TARGET_Y)
	{
	
	printf("\nTESTING FOR TARGET Y %d\n", TARGET_Y);
#endif

	range = malloc(sizeof(struct range));
	node = range;
	node->exclude = NULL;

	for (snode = sensor; snode != NULL; snode = snode->next)
	{
		y = TARGET_Y;
		dy = abs(y - snode->y);

		// the range of this sensor does not impact the target y.
		if (dy > snode->distance)
		{
			if (snode->next == NULL)
				break;

			continue;
		}

		node->lx = snode->x - (snode->distance - dy);
		node->rx = snode->x + (snode->distance - dy);

	#ifndef PART2
		printf("Got capable sensor for y=%d. Source is (%d,%d). Beacon is (%d,%d). Distance to beacon is %d. lx=%d, rx=%d.\n",
		       TARGET_Y, snode->x, snode->y, snode->bx, snode->by, snode->distance, node->lx, node->rx);
	#endif

		if (snode->y == y)
			push_exclude(node, snode->x);

		if (snode->by == y)
			push_exclude(node, snode->bx);

	#ifndef PART2
		printf("\tGot range from %d to %d%s", node->lx, node->rx, node->exclude == NULL ? "." : ", with something at");

		for (exnode = node->exclude; exnode != NULL; exnode = exnode->next)
			printf(", %d", exnode->x);

		printf("\n\n");
	#endif

		if (snode->next == NULL)
			break;

		node->next = malloc(sizeof(struct range));
		node = node->next;
		node->exclude = NULL;
	}

#ifndef PART2
	printf("Considered all sensors.\n");
#endif

	node->next = NULL;

	compile = malloc(sizeof(struct range));

	// initialize (compile) to be (range)
	compile->lx = range->lx;
	compile->rx = range->rx;
	compile->exclude = NULL;
	compile->next = NULL;

	// initialize (compile->exclude) to be (range->exclude)
	for (exnode = range->exclude; exnode != NULL; exnode = exnode->next)
		push_exclude(compile, exnode->x);

	// skip (range), because (compile) is (range)
	for (node = range->next; node != NULL; node = next)
	{
		next = node->next;

		intersection = 0;

		for (cnode = compile;; cnode = cnode->next)
		{
			if ((node->rx <= cnode->rx && node->rx >= cnode->lx)
			 || (node->lx <= cnode->rx && node->lx >= cnode->lx))
			// intersection
			{
				++intersection;

				if (node->lx < cnode->lx)
					cnode->lx = node->lx;
				if (node->rx > cnode->rx)
					cnode->rx = node->rx;

				for (exnode = node->exclude; exnode != NULL; exnode = exnode->next)
					push_exclude(cnode, exnode->x);

				// break loop; no need duplicating this range into another compiled node
				break;
			}

			if (cnode->next == NULL)
				break;
		}

		if (intersection == 0)
		// none of the ranges intersect with a compiled range
		{
			cnode->next = malloc(sizeof(struct range));
			cnode = cnode->next;

			// initialize this new compiled node to be (node)
			cnode->lx = node->lx;
			cnode->rx = node->rx;
			cnode->exclude = NULL;
			cnode->next = NULL;

			// initialize (cnode->exclude) to be (node->exclude)
			for (exnode = node->exclude; exnode != NULL; exnode = exnode->next)
				push_exclude(cnode, exnode->x);
		}

		// free (node)

		for (exnode = node->exclude; exnode != NULL; exnode = exnext)
		{
			exnext = exnode->next;
			free(exnode);
		}

		free(node);
	}

	// (range) is free'd. (compile) is the compiled version of it.

	// resolve intersections in (compile)
	do {
		intersection = 0;

		for (cnode = compile; cnode != NULL; cnode = cnode->next)
		{
			for (node = compile; node != NULL; node = next)
			{
				next = node->next;

				if (node == cnode)
					continue;

				if ((node->rx <= cnode->rx && node->rx >= cnode->lx)
				 || (node->lx <= cnode->rx && node->lx >= cnode->lx))
				// intersection
				{
					++intersection;

					// combine (node) into (cnode)

					if (node->lx < cnode->lx)
						cnode->lx = node->lx;
					if (node->rx > cnode->rx)
						cnode->rx = node->rx;

					// transfer and free all exclude nodes
					for (exnode = node->exclude; exnode != NULL; exnode = exnext)
					{
						exnext = exnode->next;

						push_exclude(cnode, exnode->x);

						free(exnode);
					}

					// remove (node) from (compile)

					if (node == compile)
						compile = node->next;
					else
						remove_node(compile, node);

					free(node);
				}
			}
		}
	}
	while (intersection);

#ifdef PART2
	possible = malloc(sizeof(struct range));

	possible->lx = 0;
	possible->rx = MAX_X;

	possible->next = NULL;
#else
	count = 0;
#endif

	for (cnode = compile; cnode != NULL; cnode = cnext)
	{
		cnext = cnode->next;

	#ifdef PART2
		printf("Got compiled range from %d to %d.\n", cnode->lx, cnode->rx);

		possible = cut_range(possible, cnode);

		if (possible == NULL)
		{
			printf("\tNo possible places for a beacon here.\n");

			break;
		}

		for (node = possible; node != NULL; node = node->next)
			printf("\tPossible range from %d to %d.\n", node->lx, node->rx);
	#else
		printf("Got compiled range from %d to %d%s", cnode->lx, cnode->rx, cnode->exclude == NULL ? "." : ", with something at");

		count += cnode->rx - cnode->lx + 1;

		for (exnode = cnode->exclude; exnode != NULL; exnode = exnext)
		{
			exnext = exnode->next;

			printf(", %d", exnode->x), --count;

			free(exnode);
		}
	#endif

		putc('\n', stdout);

		free(cnode);
	}

#ifndef PART2
	putc('\n', stdout);

	printf("The number of impossible spaces at y=%d is %d.\n", TARGET_Y, count);
#else
	if (possible != NULL)
	{
		printf("!!\tAbove are available beacon coordinates!\n");

		finished = 1;
	}

	for (node = possible; node != NULL; node = next)
	{
		next = node->next;

		node->exclude = NULL;
		free_node(node);
	}

	}
#endif

	fclose(input);

	return EXIT_SUCCESS;
}
