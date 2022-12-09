#include <stdio.h>
#include <string.h>

#define PILESIZE  256

struct stack
{
	char pile[PILESIZE];
	int pos;
};

void moveBox(struct stack *from, struct stack *to)
{
	to->pile[++to->pos] = from->pile[from->pos--];
}

void moveBoxes(int amount, struct stack *from, struct stack *to)
{
	strcpy(&to->pile[to->pos + 1], &from->pile[from->pos - (amount - 1)]);

	to->pos += amount;
	from->pos -= amount;

	from->pile[from->pos + 1] = '\0';
}

void initPile(struct stack *stack, int length, char *init)
{
	stack->pos = length - 1;
	strcpy(stack->pile, init);
}

int main(void)
{
	struct stack stack[9];
	FILE *file;
	int c, i,
	    amount, from, to;

	initPile(&stack[0], 8, "HBVWNMLP");
	initPile(&stack[1], 3, "MQH");
	initPile(&stack[2], 8, "NDBGFQML");
	initPile(&stack[3], 7, "ZTFQMWG");
	initPile(&stack[4], 4, "MTHP");
	initPile(&stack[5], 8, "CBMJDHGT");
	initPile(&stack[6], 6, "MNBFVR");
	initPile(&stack[7], 7, "PLHMRGS");
	initPile(&stack[8], 5, "PDBCN");

	file = fopen("day5_input.txt", "r");

	c = getc(file);

	for (; c != EOF; c = getc(file))
	{
		ungetc(c, file);

		fscanf(file, "move %d from %d to %d\n", &amount, &from, &to);
		printf("Moving %d boxes from %d to %d.\n", amount, from, to);

		--from;
		--to;

		// Part 1
		/*
		for (i = 0; i < amount; ++i)
			moveBox(&stack[from], &stack[to]);
		*/
		moveBoxes(amount, &stack[from], &stack[to]);
	}

	for (i = 0; i < 9; ++i)
	{
		stack[i].pile[stack[i].pos + 1] = '\0';

		printf("Stack %d contents: %s\n", i + 1, stack[i].pile);
	}

	return 0;
}
