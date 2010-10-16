#include <stdio.h>

int main()
{
	int repeat = 1;
	int population = 0;
	int supplyFor = 0;
	char resource[12][25] = {"Food", "Water", "Entertainment", "Religion", "Pottery", "Barber", "Furniture", "Wine", "Bathing", "Slaves", "Olive_Oil", "Education", };
	float maxMultiplier[12] = {10, 15, 7, 7, 6, 4, 3, 8, 6, 1.2, 5, 8};

	while(repeat)
	{
		printf("Pop? ");
		scanf("%d", &population);
	
		for (int i = 0; i < 12; i++)
		{
			printf("%s: %d\n", resource[i], (int)(population * maxMultiplier[i]));
		}
		printf("\n Again? (0 or other)  --->");
		scanf("%d", &repeat);
	}
	return 0;
}
