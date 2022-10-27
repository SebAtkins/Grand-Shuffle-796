#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define DECKSIZE 54
#define RUNS 2000000

int makeDeck ();

int main (int argc, char **argv) {
	clock_t start, end;
	int i, total = 0, runs, temp;
	double avg;

	// Seed rand
	srand((unsigned int)time(NULL));

	start = clock();

	#pragma omp parallel shared(total) private(temp)
	{
		temp = 0;
		#pragma omp for
			for (runs = 0; runs < RUNS; runs++) {
				temp += makeDeck();
			}
		#pragma omp critical
		{
			total += temp;
		}
	}
  
	end = clock();

  avg = (double)total / RUNS;

  printf("Average: %lf\n", avg);
  printf("Time: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);

	return 0;
}

int makeDeck () {
	int cards[DECKSIZE];
	int i, j, temp;

	// Define all cards in deck
	for (i = 0; i < (DECKSIZE - 2) / 4; i++) {
		for (j = 0; j < 4; j++)
			cards[i * 4 + j] = pow(2, i);
	}
  cards[52] = 4096 * 2;
  cards[53] = 4096 * 2;

  // Shuffle deck using Fisher-Yates
  for (i = DECKSIZE - 1; i > 0; i--) {
  	j = rand() % (i + 1);
  	temp = cards[j];
  	cards[j] = cards[i];
  	cards[i] = temp;
  }

  int goal = 8191;

	// Loop until a card of each rank
	while (goal != 0) {
		goal &= ~cards[i];
		i++;
	}

	return i;
}