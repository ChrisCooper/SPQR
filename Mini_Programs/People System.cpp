#include <stdio.h>
#include <stdlib.h>

#define PERSON struct person

PERSON{
       int gridX, gridY;
		PERSON *next, *previous;
};

void sendOccupant(int x, int y, int cargoIndex);
void sortPeople();

PERSON *personHeader;
int squareWidth = 30;

int main ()
{
}

void sendOccupant(int x, int y, int cargoIndex)
{
	PERSON *newPerson;

	newPerson = (PERSON*)malloc(sizeof(PERSON));
	newPerson->gridX = x;

	newPerson->previous = NULL;
	newPerson->next = personHeader;
	personHeader = newPerson;
}

void sortPeople()
{
	PERSON *tempHeader = personHeader;
	PERSON *bestChoice = personHeader;
	PERSON *checker = personHeader;

	while (checker != NULL)
	{
		if(checker->gridY < bestChoice->gridY)
		{
			if (checker->gridX < bestChoice->gridX)
			{
				bestChoice = checker;
			}
		}
		checker = checker->next;
	}

	if (bestChoice == tempHeader)
	{
		tempHeader = tempHeader->next;
	}

	bestChoice->previous->next = bestChoice->next;
	bestChoice->next->previous = bestChoice->previous;
	bestChoice->previous = tempHeader->previous;
	tempHeader->previous->next = bestChoice;
	bestChoice->next = tempHeader->previous;
	bestChoice->next = tempHeader;


	while (tempHeader != NULL)
	{
		bestChoice = tempHeader;
		checker = tempHeader;
		while (checker != NULL)
		{
			if(checker->gridY < bestChoice->gridY)
			{
				if (checker->gridX < bestChoice->gridX)
				{
					bestChoice = checker;
				}
			}
			checker = checker->next;
		}
		bestChoice->previous->next = bestChoice->next;
		bestChoice->next->previous = bestChoice->previous;
		bestChoice->previous = tempHeader->previous;
		tempHeader->previous->next = bestChoice;
		bestChoice->next = tempHeader->previous;
		bestChoice->next = tempHeader;

	}
}
