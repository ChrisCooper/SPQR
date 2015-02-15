#include "Definitions.h" //All structure definitions, #defines, #includes, and ENUMs
#include "Globals.h"     //Includes all global variables declared as "extern"

void drawGrid ();                                     //Draws map and people
void drawHistogram(int x, int y);
void drawPerson(PERSON *thePerson);
void drawHUD();
int placeOnGridX(int x, int y);                       //Converts a grid coordinate to pixels
int placeOnGridY(int x, int y);                       //Converts a grid coordinate to pixels
int placementToButton();                              //Converts the int "placement" to button number

extern int getGridX(int x, int y);                    //Converts a pixel location to grid coordinate
extern int getGridY(int x, int y);                    //Converts a pixel location to grid coordinate
extern int buildingIDtoResource(int theNumber);       //Converts a building index to resource index (0-12)

/*  gameVisuals()

Draws the map, people, HUD, and mouse, in that order.

*/
void gameVisuals()
{      
   drawGrid ();                                          //Draws map and people
	if (mouseGridX >= 0 && mouseGridY >= 0)
	{
		//Draw the map highlighter
 		draw_sprite(screenBuffer, selector, placeOnGridX(mouseGridX, mouseGridY),placeOnGridY(mouseGridX, mouseGridY));
	}
   drawHUD();
	draw_sprite(screenBuffer, mousePic, mouseX, mouseY);

	blit(screenBuffer, screen, 0,0,0,0,1024,768);         //Blit the buffer
   clear_bitmap(screenBuffer);
}

/*  drawGrid()

Draws the map, and people.

*/
void drawGrid ()
{
	PERSON *thePerson = personHeader;
	int y;
	int x;
	int i;
   for (y = 0; y < mapWidth; y++)
   {
        for (x = 0; x < mapWidth; x++)
        {
				//Draw the squares
        		draw_sprite(screenBuffer, aMapSquare[x][y].picture, placeOnGridX(x, y), placeOnGridY(x, y) - anObject[aMapSquare[x][y].objectType].printOffset);
            drawHistogram(x,y);

				/*	Drawing people
				Because people are always moving, ther can be no fixed order for printing them.
				Instead, they are constantly sorted, and once sorted, this loop prints them.
				They are sorted by gridX and gridY, in the same order as the squares.
				Thus, while stepping though the squares, you can step through the list of people
				and print all of them until  you reach the next square
				*/
				while (thePerson != NULL && thePerson->gridY == y && thePerson->gridX == x)
				{
               drawPerson(thePerson);
					thePerson = thePerson->next;
    			}
			}
   }
	if(showPersonInfo)
	{
		for(i = 0; i < NUMRESOURCES; i++)
		{
			if (selectionX != -1 && selectionY != -1){
				if (aMapSquare[selectionX][selectionY].walker[i] != NULL)
				{
					thePerson = aMapSquare[selectionX][selectionY].walker[i];
					line(screenBuffer, thePerson->x + cameraX, thePerson->y + cameraY - 15, placeOnGridX(selectionX, selectionY) + squareWidth/2,  placeOnGridY(selectionX, selectionY) + squareWidth/4, resourceColour[i]);
				}
			}
		}
	}
}

/*Converts a grid coordinate to pixels*/
int placeOnGridX(int x, int y)
{
	return (int)(0.5*squareWidth*(x-y) + cameraX);
}
/*Converts a grid coordinate to pixels*/
int placeOnGridY(int x, int y)
{
	return (int)(0.25*squareWidth*(x+y) + cameraY);
}

void drawHistogram(int x, int y)
{
   //Make sure an actual resource building is selected
	if  (buildingIDtoResource(placement) >= 0)   			
	{
      //make sure you are printing a house
		if (aMapSquare[x][y].objectType >= TENT && showBuildingInfo)
																			
		{
         //If the house has a decent amount of the resource
			if (aMapSquare[x][y].currentSupply[buildingIDtoResource(placement)] > 250)
			{
					//Draw the bar graph
				   rectfill(screenBuffer, placeOnGridX(x, y) + squareWidth/2 - 5 + aMapSquare[x][y].supplyBarOffset, placeOnGridY(x, y) - 5, placeOnGridX(x, y) - 5 + squareWidth/2 + 10 + aMapSquare[x][y].supplyBarOffset, placeOnGridY(x, y) - 5 - 50, 0);
					rectfill(screenBuffer, placeOnGridX(x, y) + squareWidth/2 + 2 - 5 + aMapSquare[x][y].supplyBarOffset, placeOnGridY(x, y) - 5 - 2, placeOnGridX(x, y) - 5 + squareWidth/2 + 10 -2 + aMapSquare[x][y].supplyBarOffset, placeOnGridY(x, y) - 5 - (aMapSquare[x][y].percentageFull[buildingIDtoResource(placement)]/2) + 2, resourceColour[buildingIDtoResource(placement)]);
			}
		}
		//If you are printing a producer
		else if (aMapSquare[x][y].objectType >= FARM && showBuildingInfo && anObject[aMapSquare[x][y].objectType].productionRate[buildingIDtoResource(placement)] > 0)
		{
			//Draw a white bar graph
		   rectfill(screenBuffer, placeOnGridX(x, y) + squareWidth/2 - 5 + aMapSquare[x][y].supplyBarOffset, placeOnGridY(x, y) - 5, placeOnGridX(x, y) - 5 + squareWidth/2 + 10 + aMapSquare[x][y].supplyBarOffset, placeOnGridY(x, y) - 5 - 50, makecol(255, 255, 255));
		   rectfill(screenBuffer, placeOnGridX(x, y) + squareWidth/2 - 4 + aMapSquare[x][y].supplyBarOffset, placeOnGridY(x, y) - 6, placeOnGridX(x, y) - 6 + squareWidth/2 + 10 + aMapSquare[x][y].supplyBarOffset, placeOnGridY(x, y) - 4 - 50, 0);
			if ((aMapSquare[x][y].percentageFull[buildingIDtoResource(placement)]/2) > 2)
			{
				//Draw the graph
				rectfill(screenBuffer, placeOnGridX(x, y) + squareWidth/2 + 2 - 5 + aMapSquare[x][y].supplyBarOffset, placeOnGridY(x, y) - 5 - 2, placeOnGridX(x, y) - 5 + squareWidth/2 + 10 -2 + aMapSquare[x][y].supplyBarOffset, placeOnGridY(x, y) - 5 - (aMapSquare[x][y].percentageFull[buildingIDtoResource(placement)]/2) + 2, resourceColour[buildingIDtoResource(placement)]);
			}
		}
	}
}
/*  drawPerson(PERSON *thePerson)

Draws a single person, given a pointer to the person.

*/
void drawPerson(PERSON *thePerson)
{
	//Draw the person
	draw_sprite(screenBuffer, thePerson->picture[thePerson->animationCounter], thePerson->x - 12 + cameraX, thePerson->y - 28 + cameraY);

	//If showPersonInfo is true, write the person's cargo info
	if (showPersonInfo && buildingIDtoResource(placement) >= 0)
	{
		//If the person is carrying the selected resource
		if (thePerson->cargoIndex == buildingIDtoResource(placement))
		{
         
			draw_sprite(screenBuffer, resourceIcon[buildingIDtoResource(placement)], thePerson->x + cameraX, thePerson->y - 38 + cameraY);
			textprintf_centre_ex(screenBuffer, font, thePerson->x + cameraX, thePerson->y - 20 + cameraY, makecol(255, 255, 255), -1, "%d", thePerson->cargo);
		}
	}
}

/*  drawHUD()

Draws the entire HUD

*/
void drawHUD()
{
	//HUD picture
	draw_sprite(screenBuffer, HUD, 0, 0);

	/*PRINT TEXT INFO*/
	//Population
	textprintf_ex(screenBuffer, font15, 530, 14, makecol(255, 255, 255), 0, "%d", population);
	//Money
	textprintf_ex(screenBuffer, font15, 130, 14, makecol(255, 255, 255), 0, "%d", money);
	//Employees / Employees Needed
	textprintf_centre_ex(screenBuffer, font15, 890, 14, makecol(255, 255, 255), 0, "%d/%d", employees, employeesNeeded );

	//Highlight the selected resource button
	draw_sprite(screenBuffer, placmentOutline, resourceButton[placementToButton()].left - 4, resourceButton[placementToButton()].top - 4);
}

/*  placementToButton()

Converts an OBJECT index to a HUD-BUTTON index

*/
int placementToButton()
{
	if (placement > EMPTY)
	{
		if (placement == TENT)
		{
			return 1;
		}
		else
		{
			return (placement - 2);
		}
	}
	return EMPTY;
}
