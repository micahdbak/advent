#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#undef PART2

#define NUM_MONKEY  8
#ifdef PART2
#define NUM_ROUNDS  10000
#else
#define NUM_ROUNDS  20
#endif

struct monkey
{
	struct item
	{
		unsigned long long worry;
		struct item *next;
	} *item;
	enum operation
	{
		OP_ADD,
		OP_MUL,
		OP_SQR
	} operation;
	int op_val, div, ints;
	struct monkey *ifTrue, *ifFalse;
};

void throwItem(struct monkey *from, struct monkey *to)
{
	struct item *node;

	if (to->item == NULL)
	{
		to->item = from->item;
		node = to->item;

		from->item = node->next;
		node->next = NULL;

		return;
	}

	node = to->item;

	for (; node->next != NULL; node = node->next)
		;

	node->next = from->item;
	node = node->next;

	from->item = node->next;
	node->next = NULL;
}

int main(void)
{
	FILE *input;
	struct monkey monkey[NUM_MONKEY];
	struct item *node;
	int c, i, j;
#ifdef PART2
	int common_divisor = 1;
#endif
	char ch;

	input = fopen("day11_input.txt", "r");

	c = getc(input);

	for (; c != EOF; c = getc(input))
	{
		ungetc(c, input);

		fscanf(input, "Monkey %d:\n  Starting items:", &i);
		printf("Got monkey %d with items ", i);

		monkey[i].item = malloc(sizeof(struct item));
		node = monkey[i].item;

		for (;;)
		{
			fscanf(input, " %llu%c", &node->worry, &ch);
			printf("%llu, ", node->worry);

			if (ch == ',')
			{
				node->next = malloc(sizeof(struct item));
				node = node->next;
			} else {
				node->next = NULL;

				break;
			}
		}

		putc('\n', stdout);

		fscanf(input, "  Operation: new = old %c ", &ch);

		switch (ch)
		{
		case '+':
			monkey[i].operation = OP_ADD;

			fscanf(input, "%d", &monkey[i].op_val);

			break;
		case '*':
			monkey[i].operation = OP_MUL;

			if (!isdigit(c = getc(input)))
			{
				monkey[i].operation = OP_SQR;	

				for (; c != '\n'; c = getc(input))
					;
			} else {
				ungetc(c, input);

				fscanf(input, "%d", &monkey[i].op_val);
			}

			break;
		}

		printf("Monkey operation %d with value %d.\n", monkey[i].operation, monkey[i].op_val);

		fscanf(input, "  Test: divisible by %d\n", &monkey[i].div);
	#ifdef PART2
		common_divisor *= monkey[i].div;
	#endif

		fscanf(input, "    If true: throw to monkey %d\n", &j);
		printf("True goes to %d, ", j);
		monkey[i].ifTrue = &monkey[j];

		fscanf(input, "    If false: throw to monkey %d\n", &j);
		printf("False goes to %d.\n", j);
		monkey[i].ifFalse = &monkey[j];

		putc('\n', stdout);

		monkey[i].ints = 0;
	}

	// rounds
	for (i = 0; i < NUM_ROUNDS; ++i)
	{
		printf("\nRound %d:\n", i);

		for (j = 0; j < NUM_MONKEY; ++j)
			while (monkey[j].item != NULL)
			{
				switch (monkey[j].operation)
				{
				case OP_ADD:
					monkey[j].item->worry += monkey[j].op_val;

					break;
				case OP_MUL:
					monkey[j].item->worry *= monkey[j].op_val;

					break;
				case OP_SQR:
					monkey[j].item->worry *= monkey[j].item->worry;

					break;
				}

			#ifndef PART2
				monkey[j].item->worry /= 3;
			#else
				monkey[j].item->worry %= common_divisor;
			#endif

				if ((monkey[j].item->worry % monkey[j].div) == 0)
				{
					// true
					throwItem(&monkey[j], monkey[j].ifTrue);
				} else
					// false
					throwItem(&monkey[j], monkey[j].ifFalse);

				++monkey[j].ints;
			}
		
		for (j = 0; j < NUM_MONKEY; ++j)
		{
			printf("Monkey %d has items: ", j);

			node = monkey[j].item;

			for (; node != NULL; node = node->next)
				printf("%llu, ", node->worry);

			putc('\n', stdout);
		}
	}

	for (i = 0; i < NUM_MONKEY; ++i)
		printf("Monkey %d has interacted %d times.\n", i, monkey[i].ints);

	return EXIT_SUCCESS;
}
