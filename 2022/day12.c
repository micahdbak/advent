#include <stdio.h>
#include <stdlib.h>

#define PART2

#define WIDTH   64
#define HEIGHT  41

struct node
{
	int weight;
};

struct node **neighbours(struct node *graph, struct node *node)
{
	/* The most neighbours a node can have is 8. Allow
	 * buffer to have 9 elements so the last can be
	 * equal to NULL, terminating the sequence. */
	struct node **buffer;

	buffer = calloc(9, sizeof(struct node *));

	/* node [i]ndex, [x]-pos, [y]-pos,
	 * [n]umber of neighbours */
	int i, x, y,
	    j, k, l;

	/* Because [node] is in [graph], then it is
	 * some [&graph[i]] where [i] is unknown.
	 * Given that
	 *     &arr[i] = arr + i,
	 * then
	 *     &graph[i] - graph
	 *   = (graph + i) - graph = i */
	i = node - graph;

	// find x-pos and y-pos of this node given [i]
	x = i % WIDTH;
	y = i / WIDTH;

	j = 0;

	for (k = y - 1; k <= y + 1; ++k)
		for (l = x - 1; l <= x + 1; ++l)
		{
			// check if node isn't eligible
			if ((k == y && l == x)  // same node
			 || (k != y && l != x)  // diagonal node
			 || k < 0               // negative y-pos
			 || l < 0               // negative x-pos
			 || k >= HEIGHT         // y-pos too large
			 || l >= WIDTH          // x-pos too large
			 // weight difference too large
			 || graph[(k * WIDTH) + l].weight > node->weight + 1)
				continue;

			buffer[j++] = &graph[(k * WIDTH) + l];
		}

	buffer[j] = NULL;

	return buffer;
}

struct node **solve(struct node *graph, struct node *start)
{
	struct node **prev;

	prev = calloc(WIDTH * HEIGHT, sizeof(struct node *));

	struct node *queue[WIDTH * HEIGHT],
		    *node, **bours, **next;
	int visited[WIDTH * HEIGHT], head, tail, i;

	head = 0;
	tail = 1;

	// first node in queue
	queue[head] = start;

	// initialize [visited] to 0
	for (i = 0; i < WIDTH * HEIGHT; ++i)
		visited[i] = 0;

	// mark the starting node as visited
	visited[start - graph] = 1;

	// initialize [prev] to NULL
	for (i = 0; i < WIDTH * HEIGHT; ++i)
		prev[i] = NULL;

	while (head != tail)
	{
		node = queue[head++];
		bours = neighbours(graph, node);

		next = bours;

		for (; *next != NULL; ++next)
			if (!visited[*next - graph])
			{
				queue[tail++] = *next;
				visited[*next - graph] = 1;
				prev[*next - graph] = node;
			}

		// free [bours]
		free(bours);
	}

	return prev;
}

struct node **makePath(struct node *graph, struct node *start, struct node *end, struct node **prev)
{
	struct node **path;

	path = calloc(WIDTH * HEIGHT, sizeof(struct node *));

	struct node *node;
	int i, j;

	i = 0;

	for (node = end; node != NULL; node = prev[node - graph])
		path[i++] = node;

	// path is reversed
	
	path[i] = NULL;

	// reverse the path
	for (j = i - 1; j >= i / 2; --j)
	{
		node = path[(i - 1) - j];
		path[(i - 1) - j] = path[j];
		path[j] = node;
	}

	return path;
}

int pathlen(struct node **path)
{
	int len;

	len = 0;

	for (; *path != NULL; ++path)
		++len;

	// this is how AoC measures len
	--len;

	return len;
}

int main(void)
{
	FILE *input;
	struct node graph[WIDTH * HEIGHT],
	            *start, *end,
		    **prev,
		    **path, **shortest,
		    **node;
#ifdef PART2
	struct node *anode;
#endif
	int c, i, x, y,
	    len, shortestlen;

	input = fopen("day12_input.txt", "r");

	c = getc(input);

	// read weight of each node from input file
	for (x = 0, y = 0; c != EOF; c = getc(input))
	{
		if (c == '\n')
		{
			x = 0;
			++y;

			continue;
		}

		if (c == 'E')
		{
			graph[(y * WIDTH) + x].weight = 'z' - 'a';
			end = &graph[(y * WIDTH) + x];
		} else
		if (c == 'S')
		{
			graph[(y * WIDTH) + x].weight = 'a' - 'a';
			start = &graph[(y * WIDTH) + x];
		} else
			graph[(y * WIDTH) + x].weight = c - 'a';

		++x;
	}

	fclose(input);

#ifndef PART2
	prev = solve(graph, start);
#endif

#ifndef PART2
	path = makePath(graph, start, end, prev);

	printf("There are %d nodes in the path.\n", pathlen(path));
#else
	shortestlen = WIDTH * HEIGHT;

	// ugly brute forcing... but it works!
	for (i = 0; i < WIDTH * HEIGHT; ++i)
		if (graph[i].weight == 0)
		{
			anode = &graph[i];

			prev = solve(graph, anode);
			path = makePath(graph, anode, end, prev);
			len = pathlen(path);

			if (len < shortestlen && len > 0)
			{
				free(shortest);
				shortest = path;
				shortestlen = len;
			} else
				free(path);

			free(prev);
		}

	path = shortest;

	printf("There are %d nodes in the shortest path.\n", pathlen(path));
#endif
	node = path;

	for (; *node != NULL; ++node)
		(*node)->weight = -1;

	for (y = 0; y < HEIGHT; ++y)
	{
		for (x = 0; x < WIDTH; ++x)
		{
			if (graph[(y * WIDTH) + x].weight == -1)
				printf("*");
			else
				printf("%c", 'a' + graph[(y * WIDTH) + x].weight);
		}

		putc('\n', stdout);
	}

	// clean up
	free(path);
#ifndef PART2
	free(prev);
#endif

	return EXIT_SUCCESS;
}
