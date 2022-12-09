#include <stdio.h>

#define YOU_LOSS  'X'
#define YOU_DRAW  'Y'
#define YOU_WIN   'Z'

#define OPP_ROCK      'A'
#define OPP_PAPER     'B'
#define OPP_SCISSORS  'C'

#define SCORE_ROCK      1
#define SCORE_PAPER     2
#define SCORE_SCISSORS  3

int main(void)
{
	FILE *file;
	int score, c;
	char opp, you;

	file = fopen("day2_input.txt", "r");

	c = getc(file);

	for (score = 0; c != EOF; c = getc(file))
	{
		ungetc(c, file);

		fscanf(file, "%c %c\n", &opp, &you);

		switch (you)
		{
		case YOU_LOSS:
			switch (opp)
			{
			case OPP_ROCK:
				score += SCORE_SCISSORS;

				break;
			case OPP_PAPER:
				score += SCORE_ROCK;

				break;
			case OPP_SCISSORS:
				score += SCORE_PAPER;

				break;
			}

			break;
		case YOU_DRAW:
			score += 3;

			switch (opp)
			{
			case OPP_ROCK:
				score += SCORE_ROCK;

				break;
			case OPP_PAPER:
				score += SCORE_PAPER;

				break;
			case OPP_SCISSORS:
				score += SCORE_SCISSORS;

				break;
			}

			break;
		case YOU_WIN:
			score += 6;

			switch (opp)
			{
			case OPP_ROCK:
				score += SCORE_PAPER;

				break;
			case OPP_PAPER:
				score += SCORE_SCISSORS;

				break;
			case OPP_SCISSORS:
				score += SCORE_ROCK;

				break;
			}

			break;
		}
	}

	printf("Your total score is %d.\n", score);

	return 0;
}
