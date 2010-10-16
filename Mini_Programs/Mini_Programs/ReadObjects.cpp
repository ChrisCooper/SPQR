void loadObjects();
void printObject(int index);

int main()
{
	loadObjects();\
	for (int i = 0; i < 17; i ++)
	{
		printObject(i);
	}
	getchar();
}

void loadObjects()
{
	FILE *objectLibrary = fopen ("Text/ObjectLibrary.txt", "r");
	int totalBuildings = 0;
	int objectIndex = 0;
	fscanf(objectLibrary, "Objects in Total: %d\n\n", &totalBuildings);
	for (int objectIndex = 0; objectIndex < totalBuildings; objectIndex ++)
	{
		fscanf(objectLibrary, "Name: %s\n", theBuilding[objectIndex].name);
		fscanf(objectLibrary, "Blurb: %s\n", theBuilding[objectIndex].blurb);
		fscanf(objectLibrary, "Cost: %d\n", &theBuilding[objectIndex].cost);
		fscanf(objectLibrary, "Varieties: %d\n", &theBuilding[objectIndex].varieties);
		fscanf(objectLibrary, "Max People: %d\n", &theBuilding[objectIndex].maxPeople);
		fscanf(objectLibrary, "Production Rates: ", NULL);
		for (int i = 0; i < 12; i ++)
		{
			fscanf(objectLibrary, "%d", &theBuilding[objectIndex].productionRate[i]);
		}
		fscanf(objectLibrary, "\n", NULL);
		fscanf(objectLibrary, "Consumption Rates: ", NULL);
		for (int i = 0; i < 12; i ++)
		{
			fscanf(objectLibrary, "%d", &theBuilding[objectIndex].consumptionRate[i]);
		}
		fscanf(objectLibrary, "\n", NULL);
		fscanf(objectLibrary, "Max Supply: ", NULL);
		for (int i = 0; i < 12; i ++)
		{
			fscanf(objectLibrary, "%d", &theBuilding[objectIndex].maxSupply[i]);
		}
		fscanf(objectLibrary, "\n", NULL);
		fscanf(objectLibrary, "Supply Threshold: ", NULL);
		for (int i = 0; i < 12; i ++)
		{
			fscanf(objectLibrary, "%d", &theBuilding[objectIndex].supplyThreshold[i]);
		}
		fscanf(objectLibrary, "\n", NULL);
		fscanf(objectLibrary, "Print  Offset: %d\n", &theBuilding[objectIndex].printOffset);
		for (int i = 0; i < 16; i ++)
		{
			fscanf(objectLibrary, "Picture: %s\n", theBuilding[objectIndex].bitmap[i]);
		}
		fscanf(objectLibrary, "Products: ", NULL);
		for (int i = 0; i < 16; i++)
		{
			int aResourceBool;
			fscanf(objectLibrary, "%d ", &aResourceBool);
			for (int j = 0; j < i; j++)
			{
				aResourceBool *= 2;
			}
			theBuilding[objectIndex].products |= aResourceBool;
		}
	}
	fclose(objectLibrary);
}

void printObject(int index)
{
	printf("Name: %s\n", theBuilding[index].name);
	printf("Blurb: %s\n", theBuilding[index].blurb);
	printf("Cost: %d\n", theBuilding[index].cost);
	printf("Varieties: %d\n", theBuilding[index].varieties);
	printf("Max People: %d\n", theBuilding[index].maxPeople);
	printf("Production Rates: ");
	for (int i = 0; i < 12; i ++)
	{
		printf("%d ", theBuilding[index].productionRate[i]);

	}
	printf("\n");
	printf("Consumption Rates: ");
	for (int i = 0; i < 12; i ++)
	{
		printf("%d ", theBuilding[index].consumptionRate[i]);

	}
	printf("\n");
	printf("Max Supplies Rates: ");
	for (int i = 0; i < 12; i ++)
	{
		printf("%d ", theBuilding[index].maxSupply[i]);

	}
	printf("\n");
	printf("Supply Thresholds: ");
	for (int i = 0; i < 12; i ++)
	{
		printf("%d ", theBuilding[index].supplyThreshold[i]);

	}
	printf("\n");
	printf("Print Offset: %d\n", theBuilding[index].printOffset);
	for (int i = 0; i < 16; i ++)
	{
		printf("Picture %d: %s\n", i, theBuilding[index].bitmap[i]);
	}
	printf("Product: %d\n", theBuilding[index].products);
}
