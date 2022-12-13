#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define PART2

struct node
{
	enum type { LIST, VALUE, EMPTY } type;

	struct node *list;
	int value;

	struct node *next;
};

void printnode(struct node *node);

void readnode(FILE *file, struct node *node)
{
	int c;

	c = getc(file);

	node->type = EMPTY;
	node->list = NULL;
	node->value = 0;

	for (; c != ']'; c = getc(file))
	{
		if (c == '[')
		{
			node->type = LIST;
			node->list = malloc(sizeof(struct node));

			readnode(file, node->list);
		} else
		// check if c is the start of a value
		if (isdigit(c))
		{
			node->type = VALUE;
			node->list = NULL;

			// ensure that this digit is read
			ungetc(c, file);

			fscanf(file, "%d", &node->value);
		}

		c = getc(file);

		// more nodes
		if (c == ',')
		{
			node->next = malloc(sizeof(struct node));
			node = node->next;

			node->type = EMPTY;
			node->list = NULL;
			node->value = 0;
		} else
			ungetc(c, file);
	}

	node->next = NULL;
}

#define RIGHT_ORDER  0
#define WRONG_ORDER  1
#define UNDECIDED    2

int compare(struct node *left, struct node *right)
{
	int ret, i;

compare_start:

	ret = UNDECIDED;

	if (left->type != right->type)
	{
		if (left->type == EMPTY)
			return RIGHT_ORDER;
		else
		if (right->type == EMPTY)
			return WRONG_ORDER;

		struct node node;

		node.type = LIST;
		node.list = malloc(sizeof(struct node));

		node.list->type = VALUE;
		node.list->list = NULL;
		node.list->next = NULL;

		if (left->type == VALUE && right->type == LIST)
		{
			node.list->value = left->value;
			node.next = left->next;

			left = &node;
		} else {
			node.list->value = right->value;
			node.next = right->next;

			right = &node;
		}

		ret = compare(left, right);

		free(node.list);

		return ret;
	}

	for (;;)
	{
		// left->type == right->type
		if (left->type == LIST)
			ret = compare(left->list, right->list);
		else
		if (left->type == VALUE)
			ret = left->value < right->value ?
			      RIGHT_ORDER // left is less than right
			    : left->value > right->value ?
			      WRONG_ORDER // left is greater than right
			    : UNDECIDED;  // left is equal to right

		if (ret == UNDECIDED)
		{
			left = left->next;
			right = right->next;

			if (left == NULL && right == NULL)
				break;

			if (left == NULL)
			{
				ret = RIGHT_ORDER;

				break;
			}

			if (right == NULL)
			{
				ret = WRONG_ORDER;

				break;
			}

			goto compare_start;
		} else
			break;
	}

	return ret;
}

void printnode(struct node *node)
{
	if (node == NULL)
		return;

	putc('[', stdout);

	for (; node != NULL; node = node->next)
	{
		if (node->type == LIST)
			printnode(node->list);
		else
		if (node->type == VALUE)
			printf("%d", node->value);
		else
		if (node->type == EMPTY)
			printf("empty");

		if (node->next != NULL)
			putc(',', stdout);
	}

	putc(']', stdout);
}

void freenode(struct node *node)
{
	struct node *next;

	if (node == NULL)
		return;

	next = node;

	do {
		node = next;
		next = node->next;

		if (node->type == LIST)
			freenode(node->list);

		free(node);
	}
	while (next != NULL);
}

#define LEFT   0
#define RIGHT  1
#define END    2

#ifdef PART2

struct tree
{
	struct node *content;
	struct tree *lth, *gth;
};

void branch(struct tree *tree, struct node *node)
{
	switch (compare(tree->content, node))
	{
	// node > content
	case RIGHT_ORDER:
		if (tree->gth == NULL)
		{
			tree->gth = malloc(sizeof(struct tree));
			tree = tree->gth;

			tree->content = node;
			tree->lth = tree->gth = NULL;
		} else
			branch(tree->gth, node);

		break;
	// node < content
	case WRONG_ORDER:
		if (tree->lth == NULL)
		{
			tree->lth = malloc(sizeof(struct tree));
			tree = tree->lth;

			tree->content = node;
			tree->lth = tree->gth = NULL;
		} else
			branch(tree->lth, node);

		break;
	// content == node
	default:

		break;
	}
}

void printtree(struct tree *tree)
{
	static int index = 1;

	if (tree->lth != NULL)
		printtree(tree->lth);

	printf("%d\t", index++);
	printnode(tree->content);
	putc('\n', stdout);

	if (tree->gth != NULL)
		printtree(tree->gth);
}

#endif

int main(void)
{
#ifndef PART2
	struct node node[2];
#else
	struct node *node[2];
#endif
	FILE *input;
	int c, i, index, sum;
#ifdef PART2
	struct tree *tree;

	tree = NULL;

	node[LEFT] = malloc(sizeof(struct node));
	node[RIGHT] = malloc(sizeof(struct node));
#endif

	input = fopen("day13_input.txt", "r");

	c = getc(input);

	for (i = LEFT, index = 1, sum = 0; c != EOF; c = getc(input))
	{
		if (c == '\n')
		{
			++i;

			if (i == END)
			{
			#ifndef PART2
				// compare LEFT and RIGHT nodes
				switch (compare(&node[LEFT], &node[RIGHT]))
				{
				case RIGHT_ORDER:
					printf("Packet %d is in the right order.\n", index);

					sum += index;

					break;
				case WRONG_ORDER:
					printf("Packet %d is in the wrong order.\n", index);

					break;
				default:

					break;
				}
			#else
				// first packet
				if (tree == NULL)
				{
					tree = malloc(sizeof(struct tree));
					tree->content = node[LEFT];
					tree->lth = tree->gth = NULL;

					branch(tree, node[RIGHT]);
				} else {
					branch(tree, node[LEFT]);
					branch(tree, node[RIGHT]);
				}
			#endif

				++index;

			#ifndef PART2
				printnode(&node[LEFT]);
				putc('\n', stdout);
				printnode(&node[RIGHT]);
				putc('\n', stdout);

				freenode(node[LEFT].list);
				freenode(node[LEFT].next);

				freenode(node[RIGHT].list);
				freenode(node[RIGHT].next);
			#else

				node[LEFT] = malloc(sizeof(struct node));
				node[RIGHT] = malloc(sizeof(struct node));
			#endif

				// skip the double space
				c = getc(input);

				// begin reading node from the left
				i = LEFT;
			}

			continue;
		}

	#ifndef PART2
		if (c == '[')
			readnode(input, &node[i]);
		else {
			node[i].type = VALUE;
			node[i].list = NULL;
			node[i].next = NULL;

			// ensure that this digit is read
			ungetc(c, input);

			fscanf(input, "%d", &node[i].value);
		}
	#else
		if (c == '[')
			readnode(input, node[i]);
		else {
			node[i]->type = VALUE;
			node[i]->list = NULL;
			node[i]->next = NULL;

			// ensure that this digit is read
			ungetc(c, input);

			fscanf(input, "%d", &node[i]->value);
		}
	#endif
	}

#ifndef PART2
	printf("The sum of the packet indices that are in the right order is %d.\n", sum);
#else
	printtree(tree);
#endif

	return EXIT_SUCCESS;
}
