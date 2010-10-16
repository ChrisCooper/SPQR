#include "Definitions.h" //All structure definitions, #defines, #includes, and ENUMs
#include "Globals.h"     //Includes all global variables declared as "extern"

/*EXTERNAL FUNCTIONS*/




extern BITMAP *makeBitmap(char *aFilename);
/*
Creates a bitmap form the file named "char *aFilename"
Shows error message on failure
*/

extern void drawGrid();
/*
Draws map and all peopl on the grid
*/

extern void drawHUD();
/*
Draws buttons and numerical information
*/

extern void saveGame();

extern void loadGame();

extern void emptyGame();
/* 
Destorys all PERSONs and map SQUAREs
*/

extern void initializeMapSquares();
/*
Mallocs mapWidth*mapWidth square, and initializes them to be EMPTY SQUAREs
*/

/*FUNCTION PROTOTYPES*/
void evaluateMouse();
/*
Updates mouse variables
*/

void createSquare(int createIndex, int x, int y);
/*
Creates a square of type "createIndex" on aMapSquare[x][y]
*/

void updateCamera();
/*
Moves the camera based on mose variables and keyboard input
*/

int getGridX(int x, int y);
/*
Converts a pixel location to grid coordinate
*/
 
int getGridY(int x, int y);
/*
Converts a pixel location to grid coordinate
*/
 
int buildingIDtoResource(int theNumber);
/*
Converts a bulding ID to a resource index
*/
 
void updatePeople();
/*
Searches for a destination for each person, makes people
drop off or pick up goods depending on their location.
*/
 
void setPath(PERSON *thePerson);
/*
Will do pathfinding in future hopefully
*/
 
PERSON *sendOccupant(int x, int y, int cargoIndex);
/*
Send a PERSON from aMapSquare[x][y] to pick up resource[cargoIndex]
*/
 
int setDestination(PERSON *thePerson);
/*
Finds the nearest producer of thePerson->cargoIndex to ick up their resource
*/
 
int findProducer(int index, int x, int y);
/*
Returns wether or not there is a nearby producer of resource[index]
*/
 
void sortPeople();
/*
Sorts the PERSONs in printing order
*/
 
void switchPeople(PERSON *first, PERSON *second);
/*
Used by sort to switch the location of two people in the linked list
*/
 
void movePeople();
/*
"walk"s the person one pixel closer to their destination
*/
 
void walk(PERSON *thePerson, int direction);
/*
Moves a person one pixel in "direction" (UP, DOWN, LEFT, or RIGHT)
*/
 
void searchButtons();
/*
Finds if the user is pressing any hotkeys or buttons on the HUD
*/
 
void checkHotkey(int key, int createIndex);
/*
Given a key, sets "placment" to "createIndex"
*/
 
void showInfo(int x, int y);
/*
Displays info about a square
*/
 
void showMenu();
/*
Displays the menu
*/
 
void checkThresholds();
/*
Checks if any houses need to send a PERSON, and sends them
*/
 
void makeTrades(PERSON *thePerson);
/*
Sees if the person is at their destintion, and if so, makes the
necessary trades with that square
*/
 
void updateResources();
/*
Increases each building's supply of each resource by its productions
rate, or decreases it by its consumption rate.
*/
 
void destroyPerson(PERSON *thePerson);
/*
Removes a PERSON from the linked list and "free()"s them
*/
 
void calculatePopulation();
/*
Calculates the total population and allocates employees
*/
 
void updateHousing();
/*
Checks if each house should upgrade or downgrade; upgrades
and downgrades the houses if necessary
*/
 
void calculatePercentages();
/*
Calculates how full of each resource a SQUARE is
*/
 
void upgradeHouse(int x, int y);
/*
Turns the house into a better class
*/
 
void downgradeHouse(int x, int y);
/*
Turns the house into a worse class
*/
 
int mouseIsOver(BUTTON * theButton);
/*
Given a BUTTON, returns whether or not the mouse is over that BUTTON
*/
 
void showOptions();
/*
Nothing... yet
*/
 
void updateMoney();
/*
Gives user revenue and takes away upkeep
*/
 
int calculateRevenue();
/*
Calculates how much tax the user receives
*/
 
int calculateSpending();
/*
Calculates how much upkeep the user must pay
*/
 

/*  getInput()

Read in all OBJECTs from a file and initialize them

*/
void getInput()
{
	evaluateMouse();
	updateCamera();
   searchButtons();

	if (mouseClick[0] && money >= anObject[placement].cost)
	{
		if (mouseGridX >= 0 && mouseGridY >= 0)
		{
			if (aMapSquare[mouseGridX][mouseGridY].objectType == EMPTY)
			{
				for(int i = 0; i < NUMRESOURCES; i++)
				{
					if(aMapSquare[mouseGridX][mouseGridY].walker[i] != NULL)
					{
						destroyPerson(aMapSquare[mouseGridX][mouseGridY].walker[i]);
					}
				}
				createSquare(placement, mouseGridX, mouseGridY);
				money -= anObject[placement].cost;
			}
			else if(placement == EMPTY)
			{
				if(aMapSquare[mouseGridX][mouseGridY].objectType >= FARM && aMapSquare[mouseGridX][mouseGridY].objectType <= ACADEMY)
				{
            	totalProducers[buildingIDtoResource(aMapSquare[mouseGridX][mouseGridY].objectType)]--;
				}
            createSquare(placement, mouseGridX, mouseGridY);
				money -= anObject[placement].cost;
			}
		}
	}
	if (mouseClick[1])
	{
      if (mouseGridX >= 0 && mouseGridY >= 0)
		{
      	showInfo(mouseGridX, mouseGridY);
		}
	}
}
/*  getInput()

Sets the global variables pertaining to the mouse

*/
void evaluateMouse()
{
	mouseX = mouse_x;
	mouseY = mouse_y;
	for (int i = 0; i < 2; i++)
	{
		if ((!mouseClick[i]) && (!mouseDown[i]) && (mouse_b & (i+1)))
		{
         mouseClick[i] = TRUE;
			mouseDown[i] = TRUE;
		}
      else if (mouse_b & (i+1))
		{
			mouseClick[i] = FALSE;
			mouseDown[i] = TRUE;
		}
      else if (!(mouse_b & (i+1)))
		{
			mouseClick[i] = FALSE;
			mouseDown[i] = FALSE;
		}
		mouseGridX = getGridX(mouseX, mouseY);
		mouseGridY = getGridY(mouseX, mouseY);
	}
	selectionX = mouseGridX;
	selectionY = mouseGridY;
}

/*  createSquare(int createIndex, int x, int y)

Changes square(x,y) to type "createIndex"

*/
void createSquare(int createIndex, int x, int y)
{
	stop_sample(buildingSound[createIndex]);
	play_sample(buildingSound[createIndex], 255, 0, 1000, 0);

   aMapSquare[x][y].objectType = createIndex;
   aMapSquare[x][y].variety = rand() % (anObject[createIndex].varieties);
	aMapSquare[x][y].supplyBarOffset = rand() % 20 - 10;

	if (createIndex == TENT)
	{
		aMapSquare[x][y].people = (int)(rand() % (anObject[createIndex].maxPeople) + (anObject[createIndex].maxPeople/2));
	}

	if (aMapSquare[x][y].people > anObject[createIndex].maxPeople)
	{
      aMapSquare[x][y].people = anObject[createIndex].maxPeople;
	}
	for (int i = 0; i < NUMRESOURCES; i++)
   {
		aMapSquare[x][y].walker[i] = NULL;
      if (aMapSquare[x][y].currentSupply[i] > anObject[createIndex].maxSupply[i])
		{
      	aMapSquare[x][y].currentSupply[i] = anObject[createIndex].maxSupply[i];
		}
   }
	aMapSquare[x][y].picture = anObject[createIndex].picture[aMapSquare[x][y].variety];
	if(placement >= FARM && placement <= ACADEMY)
	{
   	totalProducers[buildingIDtoResource(createIndex)]++;
	}

}


/*
Moves the camera based on mose variables and keyboard input
*/
void updateCamera()
{
    if (mouseX < 10)
       cameraX += 6;
    if (key[KEY_LEFT])
       cameraX += 9;

    if (mouseY < 10)
       cameraY += 6;
    if (key[KEY_UP])
       cameraY += 9;

    if (mouseX > 1014)
       cameraX -= 6;
    if (key[KEY_RIGHT])
       cameraX -= 9;

    if (mouseY > 758)
       cameraY -= 6;
    if (key[KEY_DOWN])
       cameraY -= 9;

	if (cameraX > squareWidth * mapWidth/2)
	{
      cameraX = squareWidth * mapWidth/2;
	}
	else if (cameraX - 850 < -(squareWidth * mapWidth/2))
	{
      cameraX = -(squareWidth * mapWidth/2) + 850;
	}

	if (cameraY > squareWidth/2 * mapWidth/2)
	{
      cameraY = squareWidth/2 * mapWidth/2;
	}
	else if (cameraY < -(squareWidth/2 * mapWidth/2))
	{
      cameraY = -(squareWidth/2 * mapWidth/2);
	}
}

/*
Converts a pixel location to grid coordinate
*/
int getGridX(int x, int y)
{
    x -= cameraX;
    y -= cameraY;
    int a;
    int SW = squareWidth/2;
    x -= SW;
    for (int i = 0; i < mapWidth; i++)
    {
        a = i*SW;
        if ((x/2+y <= a+SW) &&(x/2 + y >= a))
        {
           return i;
        }
    }
    return -1;
}

/*
Converts a pixel location to grid coordinate
*/
int getGridY(int x, int y)
{
    x -= cameraX;
    y -= cameraY;
    int a;
    int SW = squareWidth/2;
    x -= SW;

    for (int i = 0; i < mapWidth; i++)
    {
        a = i*SW;
        if ((y-x/2 <= a+SW) && (y-x/2 >= a))
        {
           return i;
        }
    }
    return -1;
}

/*
Converts a pixel location to grid coordinate
*/
int getPersonGridX(int x, int y)
{
    int a;
    int SW = squareWidth/2;
    x -= SW;
    for (int i = 0; i < mapWidth; i++)
    {
        a = i*SW;
        if ((x/2+y <= a+SW) &&(x/2 + y >= a))
        {
           return i;
        }
    }
    return -1;
}

/*
Converts a pixel location to grid coordinate
*/
int getPersonGridY(int x, int y)
{
    int a;
    int SW = squareWidth/2;
    x -= SW;

    for (int i = 0; i < mapWidth; i++)
    {
        a = i*SW;
        if ((y-x/2 <= a+SW) && (y-x/2 >= a))
        {
           return i;
        }
    }
    return -1;
}

/*
Searches for a destination for each person, makes people
drop off or pick up goods depending on their location.
*/
 
void updatePeople()
{
	PERSON *thePerson = personHeader;
	while (thePerson != NULL)
	{
		thePerson->gridX = getPersonGridX(thePerson->x, thePerson->y);
		thePerson->gridY = getPersonGridY(thePerson->x, thePerson->y);
		if (setDestination(thePerson))
		{
			thePerson->hasDestination = 1;
		}
		else
		{
			thePerson->hasDestination = 0;
		}
		setPath(thePerson);
		makeTrades(thePerson);
		thePerson = thePerson->next;
	}

}

/*
Send a PERSON from aMapSquare[x][y] to pick up resource[cargoIndex]
*/
PERSON *sendOccupant(int x, int y, int cargoIndex)
{
	PERSON *newPerson;

	newPerson = (PERSON*)malloc(sizeof(PERSON));
	newPerson->x = (int)(0.5*squareWidth*(x-y)+ 0.5*squareWidth);
	newPerson->y = (int)(0.25*squareWidth*(x+y) + 0.25*squareWidth);
	newPerson->height = 0;
	newPerson->gridX = x;
	newPerson->gridY = y;
	newPerson->homeX = x;
	newPerson->homeY = y;
	if (setDestination(newPerson))
	{
		newPerson->hasDestination = 1;
	}
	else
	{
		newPerson->hasDestination = 0;
	}
	for (int i = 0; i < 6; i ++)
	{
		newPerson->picture[i] = personPic[i];
	}
	newPerson->cargo = 0;
	newPerson->cargoIndex = cargoIndex;
   newPerson->animationCounter = 0;
   newPerson->homeComing = 0;
	newPerson->ID = IDcounter;
	IDcounter++;
	newPerson->previous = NULL;
	newPerson->next = personHeader;
	if (newPerson->next != NULL)
	{
      	newPerson->next->previous = newPerson;
	}
	personHeader = newPerson;
	fprintf(infoFile, "Created Person: %d\n", newPerson->ID);
	fflush(infoFile);
   sortPeople();
   totalWalkers++;



	return newPerson;
}

/*Pathfinding! (not)*/
void setPath(PERSON *thePerson)
{
   thePerson->nextX = thePerson->destinX;
   thePerson->nextY = thePerson->destinY;
}

/*
Sorts the PERSONs in printing order
*/
void sortPeople()
{
	PERSON *	personA = personHeader;
	PERSON *personB;

	int totalPeople = 0;
	int j;

	while (personA != NULL)
	{
		totalPeople++;
		personA = personA->next;
	}

	if (personHeader != NULL && personHeader->next != NULL)
	{
		for (int i = totalPeople -1; i > 0; i--)
		{
			personA = personHeader;
			personB = personHeader->next;
			for (j = 0; j < i; j++)
			{
				if (personA->gridY > personB->gridY)
				{
					switchPeople(personA, personB);
					personB = personA->next;
				}
				else if (personA->gridY == personB->gridY && personA->gridX > personB->gridX)
				{
   				switchPeople(personA, personB);
					personB = personA->next;
				}
				else
				{
         		personA = personA->next;
					personB = personB->next;
				}
			}
		}
	}
}

/*
Changes everyone's frame number
*/
void animatePeople()
{
	PERSON *thePerson = personHeader;
	while (thePerson != NULL)
	{
		thePerson->animationCounter += rand() % 2;
		if (thePerson->animationCounter >= ANIMATIONFRAMES)
		{
         thePerson->animationCounter = 0;
		}
		thePerson = thePerson->next;
	}
}

/*
"Walk()"s PERSONs to the destination
*/
void movePeople()
{
	PERSON *thePerson = personHeader;
	while (thePerson != NULL)
	{
   	if(rand() % 2)
		{
			thePerson->height += 5;
		}
		else
		{
			thePerson->height -= 5;
		}
		if(thePerson->homeComing)
		{
	     if (thePerson->gridX > thePerson->nextX)
	     {
	        walk(thePerson, LEFT);
	     }
	     if (thePerson->gridX < thePerson->nextX)
	     {
	        walk(thePerson, RIGHT);
	     }
	     if (thePerson->gridY > thePerson->nextY)
	     {
	        walk(thePerson, UP);
	     }
	     if(thePerson->gridY < thePerson->nextY)
	     {
	        walk(thePerson, DOWN);
	     }
		}
		else
		{
			if(thePerson->hasDestination)
			{
		     if (thePerson->gridX > thePerson->nextX)
		     {
		        walk(thePerson, LEFT);
		     }
		     if (thePerson->gridX < thePerson->nextX)
		     {
		        walk(thePerson, RIGHT);
		     }
		     if (thePerson->gridY > thePerson->nextY)
		     {
		        walk(thePerson, UP);
		     }
		     if(thePerson->gridY < thePerson->nextY)
		     {
		        walk(thePerson, DOWN);
		     }
			}
		}
	  thePerson = thePerson->next;
	}
}

/*
Moves a PERSON one pixel
*/
void walk(PERSON *thePerson, int direction)
{
     if (direction == UP)
     {
        thePerson->x += 2;
        thePerson->y -= 1;
     }
     if (direction == DOWN)
     {
        thePerson->x -= 2;
        thePerson->y += 1;
     }
     if (direction == LEFT)
     {
        thePerson->x -= 2;
        thePerson->y -= 1;
     }
     if (direction == RIGHT)
     {
        thePerson->x += 2;
        thePerson->y += 1;
     }
}

/*
Used by sort to switch the location of two people in the linked list
*/
void switchPeople(PERSON *first, PERSON *second)
{
	PERSON * A = first->previous;
	PERSON * B = first;
	PERSON * C = second;
	PERSON * D = second->next;

	C->next = B;
	B->next = D;
	B->previous = C;
	C->previous = A;

	if (A == NULL)
	{
		personHeader = C;
	}
   
	else
	{
		A->next = C;
	}
	if (D != NULL)
	{
		D->previous = B;
	}
}

/*
Finds the nearest producer of thePerson->cargoIndex to ick up their resource
*/
 
int setDestination(PERSON *thePerson)
{
	thePerson->destinX = thePerson->gridX;
	thePerson->destinY = thePerson->gridY;
	if (!(thePerson->homeComing))
	{
	  int x = thePerson->gridX;
	  int y = thePerson->gridY;
	  int modifier = 1;
	  int distance = 0;
	  int notFound = 1;
	  int i = 0;

	   while (notFound && (distance < 30))
	   {
	        modifier = -modifier;
	        distance = distance +1;
	       if (x < mapWidth && x>= 0 && y < mapWidth && y >= 0)
	       {
	               if ((anObject[aMapSquare[x][y].objectType].products & ((1 <<(thePerson->cargoIndex)))) && aMapSquare[x][y].currentSupply[thePerson->cargoIndex] > 2000)
	               {
	                  thePerson->destinX = x;
	                  thePerson->destinY = y;
	                  notFound = 0;
							return 1;
	               }
         }
	        for (i = 0; i < distance; i++)
	        {
	            x += modifier;
	            if (x < mapWidth && x>= 0 && y < mapWidth && y >= 0)
	            {
	               if ((anObject[aMapSquare[x][y].objectType].products & ((1 << (thePerson->cargoIndex)))) && aMapSquare[x][y].currentSupply[thePerson->cargoIndex] > 2000)
	               {
	                  thePerson->destinX = x;
	                  thePerson->destinY = y;
	                  notFound = 0;
							return 1;
	               }
	            }
	        }
	        for (i = 0; i < distance; i++)
	        {
	            y += modifier;
	            if (x < mapWidth && x>= 0 && y < mapWidth && y >= 0)
	            {
	               if ((anObject[aMapSquare[x][y].objectType].products & ((1 << (thePerson->cargoIndex)))) && aMapSquare[x][y].currentSupply[thePerson->cargoIndex] > 2000)
	               {
	                  thePerson->destinX = x;
	                  thePerson->destinY = y;
	                  notFound = 0;
							return 1;
	               }
	            }
	        }
	   }
	}
	else
	{
		thePerson->destinX = thePerson->homeX;
		thePerson->destinY = thePerson->homeY;
		return 0;
	}
}

/*
Finds if the user is pressing any hotkeys or buttons on the HUD
*/
void searchButtons()
{
	  checkHotkey(key[KEY_C], EMPTY);
//     checkHotkey(key[KEY_D], DEBRIS);
//     checkHotkey(key[KEY_R], ROAD);
//     checkHotkey(key[KEY_M], MARKET);
     checkHotkey(key[KEY_1], FARM);
     checkHotkey(key[KEY_2], FOUNTAIN);
     checkHotkey(key[KEY_3], THEATRE);
     checkHotkey(key[KEY_4], TEMPLE);
     checkHotkey(key[KEY_5], POTTERSHOP);
     checkHotkey(key[KEY_6], BARBERSHOP);
     checkHotkey(key[KEY_7], FURNITUREWORKSHOP);
     checkHotkey(key[KEY_8], WINEPRESS);
     checkHotkey(key[KEY_9], BATHHOUSE);
	  checkHotkey(key[KEY_0], SLAVETRADER);
     checkHotkey(key[KEY_MINUS], OLIVEPRESS);
     checkHotkey(key[KEY_EQUALS], ACADEMY);
     checkHotkey(key[KEY_H], TENT);
	if (key[KEY_ESC])
	{
		showMenu();
	}

		int nextKey = 0;
      if (keypressed()) // If the user hits the space key, fire missile
      {
                        nextKey = readkey();
      }
      else
      {
                        nextKey = -1;
      }

  	   switch ((nextKey & 0xff))
      {
             case 'p':
                 showPersonInfo ^= 1;
                 break;
             case 'b':
                 showBuildingInfo ^= 1;
                 break;
     }

	 if (mouseClick [0] && mouseIsOver(&menuButton))
	 {
				showMenu();
	 }
	 if (mouseClick [0] && mouseIsOver(&personButton))
	 {
				showPersonInfo ^= 1;
	 }
	 if (mouseClick [0] && mouseIsOver(&buildingButton))
	 {
				showBuildingInfo ^= 1;
	 }
     for (int i = 0; i < NUMHUDBUTTONS; i++)
     {
			if (mouseClick [0] && mouseIsOver(&resourceButton[i]))
			{
				if (i == EMPTY)
				{
					placement = EMPTY;
				}
				else if (i == 1)
				{
					placement = TENT;
				}
				else
				{
               placement = i + 2;
				}
			}
     }
		if (mouseX >= 931)
		{
			for (int i = 0; i < 2; i ++)
			{
				mouseClick[i] = 0;
			}
		}
		else if (mouseY <= 40)
		{
			for (int i = 0; i < 2; i ++)
			{
				mouseClick[i] = 0;
			}
		}
		else if (mouseX >= 750 && mouseY <= 201)
		{
			for (int i = 0; i < 2; i ++)
			{
				mouseClick[i] = 0;
			}
		}
	if (key[KEY_Z] && key[KEY_X])
	{
		money += 23;
	}
}

/*
Checks one hotkey
*/
void checkHotkey(int key, int createIndex)
{
	if (key)
   {
         placement = createIndex;
   }
}

/*
Shows info about one sqaure
*/
void showInfo(int x, int y)
{
     int inInfo = 1;
     BITMAP *popup = makeBitmap("Images/ResourceForm.bmp");
	  BUTTON backButton;
     backButton.left = 817;
     backButton.top = 100;
     backButton.width = 41;
     backButton.height = 33;
     int tY[4] = {455, 510, 570, 633};
     int tX[3] = {319, 559, 804};
     while (inInfo)
     {
        evaluateMouse();
		  updateCamera();
        drawGrid();
		  drawHUD();
        draw_sprite(screenBuffer, popup, 0, 0);

        textout_centre_ex(screenBuffer, font70, anObject[aMapSquare[x][y].objectType].name, 523, 112, makecol(215, 215,100), -1);
        textout_centre_ex(screenBuffer, font70, anObject[aMapSquare[x][y].objectType].name, 525, 108, makecol(255, 255,100), -1);

        textprintf_centre_ex(screenBuffer, font15, 210, 140, -1 /*makecol(215, 215, 185)*/, 0, "%d/%d", aMapSquare[x][y].people, anObject[aMapSquare[x][y].objectType].maxPeople);

        for (int i = 0; i < NUMRESOURCES; i++)
        {
            textprintf_centre_ex(screenBuffer, font15, tX[i/4], tY[i%4], -1/*makecol(215, 215, 185)*/, 0, "%d/%d", aMapSquare[x][y].currentSupply[i], anObject[aMapSquare[x][y].objectType].maxSupply[i]);
        }
        draw_sprite(screenBuffer, mousePic, mouse_x, mouse_y);
        blit(screenBuffer, screen, 0,0,0,0,1024,768);
        clear_bitmap(screenBuffer);
        if (key[KEY_SPACE] || (mouseClick[0] && mouseIsOver(&backButton)))
        {
				destroy_bitmap(popup);
           break;
        }
     }
}


/*
Given a BUTTON, returns whether or not the mouse is over that BUTTON
*/

int mouseIsOver(BUTTON * theButton)
{
	return (mouseX > theButton->left && mouseX < theButton->left + theButton->width && mouseY > theButton->top && mouseY < theButton->top + theButton->height);
}

/*
Checks if any houses need to send a PERSON, and sends them
*/
void checkThresholds()
{
	int x;
	int i;
	for (int y = 0; y < mapWidth; y++)
	{
		for (x = 0; x < mapWidth; x++)
		{
			if (aMapSquare[x][y].objectType >= TENT)
			{
				for (i = 0; i < NUMRESOURCES; i ++)
				{
					if ((aMapSquare[x][y].walker[i] == NULL) && aMapSquare[x][y].currentSupply[i] < anObject[aMapSquare[x][y].objectType].supplyThreshold[i])
					{
						if (findProducer(i, x, y))
						{
	                  aMapSquare[x][y].walker[i] = sendOccupant(x, y, i);
						}
					}
				}
			}
		}
	}
}

/*
Sees if the person is at their destintion, and if so, makes the
necessary trades with that square
*/
void makeTrades(PERSON *thePerson)
{
	if (thePerson->homeComing)
	{
		if (thePerson->gridX == thePerson->homeX && thePerson->gridY == thePerson->homeY)
		{
			aMapSquare[thePerson->homeX][thePerson->homeY].currentSupply[thePerson->cargoIndex] += thePerson->cargo;
			if (aMapSquare[thePerson->homeX][thePerson->homeY].currentSupply[thePerson->cargoIndex] > anObject[aMapSquare[thePerson->homeX][thePerson->homeY].objectType].maxSupply[thePerson->cargoIndex])
			{
				aMapSquare[thePerson->homeX][thePerson->homeY].currentSupply[thePerson->cargoIndex] = anObject[aMapSquare[thePerson->homeX][thePerson->homeY].objectType].maxSupply[thePerson->cargoIndex];
			}
      	destroyPerson(thePerson);
		}
	}
	else
	{
      if (thePerson->gridX == thePerson->destinX && thePerson->gridY == thePerson->destinY && (thePerson->destinX != thePerson->homeX || thePerson->destinY != thePerson->homeY))
		{
         int difference =(anObject[aMapSquare[thePerson->homeX][thePerson->homeY].objectType].maxSupply[thePerson->cargoIndex] - aMapSquare[thePerson->homeX][thePerson->homeY].currentSupply[thePerson->cargoIndex] - thePerson->cargo);
			if (difference > aMapSquare[thePerson->gridX][thePerson->gridY].currentSupply[thePerson->cargoIndex])
			{
				difference = aMapSquare[thePerson->gridX][thePerson->gridY].currentSupply[thePerson->cargoIndex];
			}
			thePerson->cargo += difference;
			aMapSquare[thePerson->gridX][thePerson->gridY].currentSupply[thePerson->cargoIndex] -= difference;
			if (thePerson->cargo >= (anObject[aMapSquare[thePerson->homeX][thePerson->homeY].objectType].maxSupply[thePerson->cargoIndex] - aMapSquare[thePerson->homeX][thePerson->homeY].currentSupply[thePerson->cargoIndex]))
			{
				thePerson->homeComing = 1;
			}
		}
	}
}

/*
Increases each building's supply of each resource by its productions rate, or decreases it by its consumption
rate.
*/
void updateResources()
{
	 int i;
	 int x;
    for (int y = 0; y < mapWidth; y++)
    {
        for (x = 0; x < mapWidth; x++)
        {
   		 	if (aMapSquare[x][y].objectType >= FARM)
				{
	            for (i = 0; i < NUMRESOURCES; i++)
	            {
	                aMapSquare[x][y].currentSupply[i] -= (int)(((float)anObject[aMapSquare[x][y].objectType].consumptionRate[i])*(((float)aMapSquare[x][y].people)/((float)anObject[aMapSquare[x][y].objectType].maxPeople)));
	                if (aMapSquare[x][y].currentSupply[i] < 0)
	                {
	                   aMapSquare[x][y].currentSupply[i] = 0;
	                }

	                aMapSquare[x][y].currentSupply[i] += (int)(((float)anObject[aMapSquare[x][y].objectType].productionRate[i])*(((float)aMapSquare[x][y].people)/((float)anObject[aMapSquare[x][y].objectType].maxPeople)));
	                if (aMapSquare[x][y].currentSupply[i] > anObject[aMapSquare[x][y].objectType].maxSupply[i])
	                {
	                   aMapSquare[x][y].currentSupply[i] = anObject[aMapSquare[x][y].objectType].maxSupply[i];
	                }
					 }
            }
        }
    }
}

/*
Removes a PERSON from the linked list and "free()"s them
*/
void destroyPerson(PERSON *thePerson)
{
   totalWalkers--;
	aMapSquare[thePerson->homeX][thePerson->homeY].walker[thePerson->cargoIndex] = NULL;

	if (thePerson == personHeader)
	{
		personHeader = thePerson->next;
	}
	else
	{
		thePerson->previous->next = thePerson->next;
	}

	if (thePerson->next != NULL)
	{
		thePerson->next->previous = thePerson->previous;
	}
   fprintf(infoFile, "Destroyed Person: %d\n", thePerson->ID);
	free(thePerson);
}

/*
Calculates the total population and allocates employees
*/
void calculatePopulation()
{
	population = 0;
	totalBuildings = 0;
   employeesNeeded = 0;
	int x;
	for (int y = 0; y < mapWidth; y ++)
	{
		for (x = 0; x < mapWidth; x ++)
		{
			if (aMapSquare[x][y].objectType >= TENT)
			{
				population += aMapSquare[x][y].people;

			}
			else if (aMapSquare[x][y].objectType >= FARM && aMapSquare[x][y].objectType <= ACADEMY)
			{
				totalBuildings++;
				employeesNeeded += anObject[aMapSquare[x][y].objectType].maxPeople;
			}
		}
	}
	employees = population / 3;
   int tempEmployees = employees;
	int tempBuildings = totalBuildings;
	if (totalBuildings > 0)
	{
		for (int y = 0; y < mapWidth; y ++)
		{
			for (x = 0; x < mapWidth; x ++)
			{
				if (aMapSquare[x][y].objectType >= FARM && aMapSquare[x][y].objectType <= ACADEMY)
				{
					if (tempBuildings != 0)
					{
						aMapSquare[x][y].people = (int)((tempEmployees)/tempBuildings);
						if (aMapSquare[x][y].people > anObject[aMapSquare[x][y].objectType].maxPeople)
						{
							tempEmployees += aMapSquare[x][y].people - anObject[aMapSquare[x][y].objectType].maxPeople;
		               aMapSquare[x][y].people = anObject[aMapSquare[x][y].objectType].maxPeople;
						}
						tempEmployees -= aMapSquare[x][y].people;
	               tempBuildings--;
					}
				}
			}
		}
	}
}

/*
Checks if each house should upgrade or downgrade; upgrades
and downgrades the houses if necessary
*/
void updateHousing()
{
	 int i;
	 int x;
	 int canUpgrade = 1;
	 int willDowngrade = 0;
    for (int y = 0; y < mapWidth; y++)
    {
        for (x = 0; x < mapWidth; x++)
        {
            if (aMapSquare[x][y].objectType >= TENT && aMapSquare[x][y].objectType < PALACE)
				{
					canUpgrade = 1;
	            for (i = 0; i < NUMRESOURCES; i++)
	            {
	                  canUpgrade &= (aMapSquare[x][y].currentSupply[i] > anObject[aMapSquare[x][y].objectType].supplyThreshold[i] || aMapSquare[x][y].currentSupply[i] > 0);
	            }
					if (canUpgrade)
					{
						upgradeHouse(x, y);
					}
				}
				if (aMapSquare[x][y].objectType > TENT && aMapSquare[x][y].objectType <= PALACE)
				{
					willDowngrade = 0;
					for (i = 0; i < (aMapSquare[x][y].objectType - TENT) * 3; i++)
	            {
	            	willDowngrade |= (aMapSquare[x][y].currentSupply[i] == 0);
	            }
					if (willDowngrade)
					{
						downgradeHouse(x, y);
					}
				}
        }
    }
}

/*
Turns the house into a better class
*/
void upgradeHouse(int x, int y)
{
   aMapSquare[x][y].objectType = aMapSquare[x][y].objectType + 1;
	aMapSquare[x][y].supplyBarOffset = rand() % 20 - 10;
   aMapSquare[x][y].variety = rand() % (anObject[aMapSquare[x][y].objectType].varieties);
	aMapSquare[x][y].people = (int)(rand() % (anObject[aMapSquare[x][y].objectType].maxPeople) + (anObject[aMapSquare[x][y].objectType].maxPeople/2));

	if (aMapSquare[x][y].people > anObject[aMapSquare[x][y].objectType].maxPeople)
	{
      aMapSquare[x][y].people = anObject[aMapSquare[x][y].objectType].maxPeople;
	}
	aMapSquare[x][y].picture = anObject[aMapSquare[x][y].objectType].picture[aMapSquare[x][y].variety];
}

/*
Turns the house into a worse class
*/
void downgradeHouse(int x, int y)
{
   aMapSquare[x][y].objectType = aMapSquare[x][y].objectType - 1;
   aMapSquare[x][y].variety = rand() % (anObject[aMapSquare[x][y].objectType].varieties);
	aMapSquare[x][y].supplyBarOffset = rand() % 20 - 10;

	aMapSquare[x][y].people = (int)(rand() % (anObject[aMapSquare[x][y].objectType].maxPeople) + (anObject[aMapSquare[x][y].objectType].maxPeople/2));

	if (aMapSquare[x][y].people > anObject[aMapSquare[x][y].objectType].maxPeople)
	{
      aMapSquare[x][y].people = anObject[aMapSquare[x][y].objectType].maxPeople;
	}
	for (int i = 0; i < NUMRESOURCES; i++)
   {
      if (aMapSquare[x][y].currentSupply[i] > anObject[aMapSquare[x][y].objectType].maxSupply[i])
		{
      	aMapSquare[x][y].currentSupply[i] = anObject[aMapSquare[x][y].objectType].maxSupply[i];
		}
   }
	aMapSquare[x][y].picture = anObject[aMapSquare[x][y].objectType].picture[aMapSquare[x][y].variety];
}

/*
Calculates how full of each resource a SQUARE is
*/
void calculatePercentages()
{
	int x;
	int i;
   for (int y = 0; y < mapWidth; y++)
   {
		for (x = 0; x < mapWidth; x++)
		{
			for (i = 0; i < NUMRESOURCES; i++)
	      {
				if (anObject[aMapSquare[x][y].objectType].maxSupply[i] != 0)
				{
					aMapSquare[x][y].percentageFull[i] = (int)((((float)aMapSquare[x][y].currentSupply[i])/((float)anObject[aMapSquare[x][y].objectType].maxSupply[i])) * 100);
				}
				else
				{
               aMapSquare[x][y].percentageFull[i] = 0;
				}
			}
		}
	}
}

/*
Shows the menu
*/
void showMenu()
{
	int inMenu = 1;
  BITMAP *popup = makeBitmap("Images/MenuForm.bmp");
  BUTTON menuButton[5];
	BUTTON backButton;
	for (int i = 0; i < 5; i ++)
	{
     menuButton[i].left = 406;
     menuButton[i].width = 237;
     menuButton[i].height = 58;
	}
     menuButton[0].top = 228;
     menuButton[1].top = 299;
     menuButton[2].top = 368;
     menuButton[3].top = 439;
     menuButton[4].top = 508;
     backButton.left = 634;
     backButton.top = 141;
     backButton.width = 31;
     backButton.height = 31;
  while (inMenu)
  {
     evaluateMouse();
	  updateCamera();
     drawGrid();
	  drawHUD();
     draw_sprite(screenBuffer, popup, 0, 0);

     draw_sprite(screenBuffer, mousePic, mouse_x, mouse_y);
     blit(screenBuffer, screen, 0,0,0,0,1024,768);
     clear_bitmap(screenBuffer);
		if (mouseClick[0])
		{
        if (mouseIsOver(&menuButton[0]))
        {
            mouseClick[0] = 0;
				money = 1000;
				emptyGame();
				initializeMapSquares();
           break;
        }
        if (mouseIsOver(&menuButton[1]))
        {
            mouseClick[0] = 0;
				saveGame();
            mouseClick[0] = 0;
           break;
        }
        if (mouseIsOver(&menuButton[2]))
        {
            mouseClick[0] = 0;
            emptyGame();
			  loadGame();
            mouseClick[0] = 0;
           break;
        }
        if (mouseIsOver(&menuButton[3]))
        {
            mouseClick[0] = 0;
				showOptions();
           break;
        }
        if (mouseIsOver(&menuButton[4]))
        {
            mouseClick[0] = 0;
				destroy_bitmap(popup);
				inGame = 0;
            exit(0);
        }
        if (mouseIsOver(&backButton))
        {
            mouseClick[0] = 0;
           return;
        }

		}
	}
}

void showOptions()
{
}

int resourceToBuildingID(int theNumber)
{
	return (theNumber + 4);
}
int buildingIDtoResource(int theNumber)
{
	if (theNumber >= FARM && theNumber <= ACADEMY)
	{
		return (theNumber - 4);
	}
	else
	{
      return -1;
	}
}

/*
Returns wether or not there is a nearby producer of resource[index]
*/
int findProducer(int index, int x, int y)
{
	  int modifier = 1;
	  int distance = 0;
	  int i = 0;
	   while (distance < 30)
	   {
	        modifier = -modifier;
	        distance = distance +1;
	       if (x < mapWidth && x>= 0 && y < mapWidth && y >= 0)
	       {
	               if ((anObject[aMapSquare[x][y].objectType].products & ((1 << index))) && aMapSquare[x][y].currentSupply[index] > 2000)
	               {
							return 1;
	               }
         }
	        for (i = 0; i < distance; i++)
	        {
	            x += modifier;
	            if (x < mapWidth && x>= 0 && y < mapWidth && y >= 0)
	            {
	               if ((anObject[aMapSquare[x][y].objectType].products & ((1 << index))) && aMapSquare[x][y].currentSupply[index] > 2000)
	               {
							return 1;
	               }
	            }
	        }
	        for (i = 0; i < distance; i++)
	        {
	            y += modifier;
	            if (x < mapWidth && x>= 0 && y < mapWidth && y >= 0)
	            {
	               if ((anObject[aMapSquare[x][y].objectType].products & ((1 << index))) && aMapSquare[x][y].currentSupply[index] > 2000)
	               {
							return 1;
	               }
	            }
	        }
	   }
	return 0;
}

/*
Gives user revenue and takes away upkeep
*/
void updateMoney()
{
	money -= calculateSpending();
	money += calculateRevenue();
	if (money < 0)
	{
		money = 0;
	}
}

/*
Calculates how much upkeep the user must pay
*/
int calculateSpending()
{
	float spent = 0;
	int x;
	for (int y = 0; y < mapWidth; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			if (aMapSquare[x][y].objectType >= FARM && aMapSquare[x][y].objectType <= ACADEMY)
			{
				spent += 0.2 * aMapSquare[x][y].objectType;
			}
		}
	}
	return ((int)spent);
}

/*
Calculates how much tax the user receives
*/
int calculateRevenue()
{
	float gained = 0;
	int x;
	for (int y = 0; y < mapWidth; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			if (aMapSquare[x][y].objectType >= TENT)
			{
            gained += 0.3 * ((aMapSquare[x][y].objectType - TENT) * (aMapSquare[x][y].objectType - TENT));
			}
		}
	}
	return ((int)gained);
}

 
