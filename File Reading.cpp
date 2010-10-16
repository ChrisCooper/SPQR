#include "Definitions.h" //All structure definitions, #defines, #includes, and ENUMs
#include "Globals.h"     //Includes all global variables declared as "extern"


extern BITMAP *makeBitmap(char *aFilename);
/*
Creates a bitmap form the file named "char *aFilename"
Shows error message on failure
*/

void readingErrorCheck(int actual, int errorCheck, char* message);
/*
Checks if file was read properly
*/

void saveGame();

void loadGame();

extern void showLoadingBar(char* stage);
/*
Fills the loading bar 1 more space out of "loadingPhases" and prints:
	" Now loading "(char *stage)" "
*/

extern void evaluateMouse();
/*
Updates mouse variables
*/

extern int mouseIsOver(BUTTON* theButtone);
/*
Given a BUTTON, returns whether or not the mouse is over that BUTTON
*/

extern void emptyGame();
/*
Destroys the map (aMapSquare[widht][width]) as well as all people
*/

extern void destroyObjects();
/*
Destory all OBJECTs
*/

extern void initializeMapSquares();
/*
Mallocs aMapSquare[width][width] and creats EMPTY squares on all of them
*/

extern void sendOccupant(int x, int y, int Index);
/*
Sends a PERSON from aMapSquare[x][y] to pick up resource[Index]
*/

extern int buildingIDtoResource(int);
/*
Converts a building index to a resource index
*/

extern void initializeAllObjects();
/*
Loads all BITMAPs, SAMPLEs, and initializes other variables used in the actual game
*/

extern void SPQR_Game();
/*
Plays the actual game
*/

/*  loadAllObjects()

Read in all OBJECTs from a file and initialize them

*/
void loadAllObjects()
{
	FILE *objectLibrary = fopen ("Text/ObjectLibrary.txt", "r");

	//The current OBJECT index being loaded
	int objectIndex = 0;

	//Used for error checking
	int returnCode = 0;

	//Total OBJECTs
	returnCode = fscanf(objectLibrary, "Objects in Total: %d\n\n", &totalBuildings);
	//readingErrorCheck(returnCode, 1, "Error! While reading in \"totalBuildings\"");

	//Malloc the array of objects, based on how big "totalBuildings" is
	anObject = (OBJECT*)malloc(totalBuildings * sizeof(OBJECT));

	for (int objectIndex = 0; objectIndex < totalBuildings; objectIndex ++)
	{

		//Name
		returnCode = fscanf(objectLibrary, "Name: %s\n", anObject[objectIndex].name);
		//readingErrorCheck(returnCode, 1, "Error! While reading in \"Name\"");

		//Show the loading screen and the loading bar
		draw_sprite(screenBuffer, loadBackground, 0, 0);
		rectfill(screenBuffer, 209, 524, 209 + (int)(((float)((float)loadingPhase)/(float)(loadingPhases)) * (810 - 209)), 531, makecol(255, 0, 0));
		textprintf_centre_ex(screenBuffer, font15, 509, 482, 0, -1, "Now loading \"%s\"", anObject[objectIndex].name);
		textprintf_centre_ex(screenBuffer, font15, 510, 480, makecol(255, 255, 255), -1, "Now loading \"%s\"", anObject[objectIndex].name);
		draw_sprite(screenBuffer, mousePic, mouseX, mouseY);
		blit(screenBuffer, screen, 0,0,0,0,1024,768);
   	clear_bitmap(screenBuffer);

		//Blurb
		returnCode = fscanf(objectLibrary, "Blurb: %s\n", anObject[objectIndex].blurb);
		//readingErrorCheck(returnCode, 1, "Error! While reading in \"Blurb\"");

		//Cost
		returnCode = fscanf(objectLibrary, "Cost: %d\n", &anObject[objectIndex].cost);
		//readingErrorCheck(returnCode, 1, "Error! While reading in \"Cost\"");

		//Varieties
		returnCode = fscanf(objectLibrary, "Varieties: %d\n", &anObject[objectIndex].varieties);

		//maxPeople
		returnCode = fscanf(objectLibrary, "Max People: %d\n", &anObject[objectIndex].maxPeople);

		//Advance cursor to actual data
		returnCode |= fscanf(objectLibrary, "Production Rates: ", NULL);

		//Read NUMRESOURCES production rates
		for (int i = 0; i < NUMRESOURCES; i ++)
		{
			fscanf(objectLibrary, "%d", &anObject[objectIndex].productionRate[i]);
		}
		//Advance cursor to actual data
		returnCode |= fscanf(objectLibrary, "\n", NULL);
		//Advance cursor to actual data
		returnCode |= fscanf(objectLibrary, "Consumption Rates: ", NULL);

		//Read NUMRESOURCES consumption rates
		for (int i = 0; i < NUMRESOURCES; i ++)
		{
			fscanf(objectLibrary, "%d", &anObject[objectIndex].consumptionRate[i]);
		}
		//Advance cursor to actual data
		fscanf(objectLibrary, "\n", NULL);
		//Advance cursor to actual data
		fscanf(objectLibrary, "Max Supply: ", NULL);

		//Read NUMRESOURCES max supplies
		for (int i = 0; i < NUMRESOURCES; i ++)
		{
			fscanf(objectLibrary, "%d", &anObject[objectIndex].maxSupply[i]);
		}
		//Advance cursor to actual data
		fscanf(objectLibrary, "\n", NULL);
		//Advance cursor to actual data
		fscanf(objectLibrary, "Supply Threshold: ", NULL);

		//Read NUMRESOURCES supply thresholds
		for (int i = 0; i < NUMRESOURCES; i ++)
		{
			fscanf(objectLibrary, "%d", &anObject[objectIndex].supplyThreshold[i]);
		}

		//Advance cursor to actual data
		fscanf(objectLibrary, "\n", NULL);

		//Print offset
		fscanf(objectLibrary, "Print  Offset: %d\n", &anObject[objectIndex].printOffset);

		/*Loading Images*/

		//Temporary filename
      char aFileName[50];
		//Add the prefix
		char aPrefix[] = "Images/";

		//Read in MAXPICS filenames, and load that picture into anObject[currentObject].picture[i]
		for (int i = 0; i < MAXPICS; i ++)
		{
			strcpy(aPrefix, "Images/");
			fscanf(objectLibrary, "Picture: %s\n", aFileName);
			strcat(aPrefix, aFileName);
			anObject[objectIndex].picture[i] = makeBitmap(aPrefix);
		}

		/*Loading products*/
      //int products is an int where each byte acts as a boolean representing
		//whether or not this OBJECT produces a certain resource
		fscanf(objectLibrary, "Products: ", NULL);
		for (int i = 0; i < NUMRESOURCES; i++)
		{
			int aResourceBool;
			fscanf(objectLibrary, "%d ", &aResourceBool);

			//Move to the current bool/byte
			aResourceBool = aResourceBool << i;
			anObject[objectIndex].products |= aResourceBool;
		}
      loadingPhase++;
	}
	fclose(objectLibrary);
}


/*  readingErrorCheck(int actual, int errorCheck, char* message)

Checks if the correct number of items were read in

*/
void readingErrorCheck(int actual, int errorCheck, char* message)
{
	if (actual != errorCheck)
	{
		set_gfx_mode(GFX_TEXT, 1024,768,0,0);
		allegro_message(message);
		exit(1);
	}
}

/*  saveGame()

Saves the game

*/
void saveGame()
{
	//Make sure the file exists
	FILE *aFile = fopen("Text/All_Saves.SPQR", "a");
	fclose(aFile);


	aFile = fopen("Text/All_Saves.SPQR", "r");

	//An array of possible save file names
	char gameFile[50][20];

	//The current name to save the game as
	char saveName[20] = "";

	int i = 0;
	int files = 0;
	int startFile = 0;
	int length = 0;
	BITMAP *background = makeBitmap("Images/SaveBackground.gif");
	BITMAP *aGameButton = makeBitmap("Images/GameSlot.gif");
	BITMAP *upArrow = makeBitmap("Images/UpArrow.gif");
	BITMAP *downArrow = makeBitmap("Images/DownArrow.gif");
	BITMAP *backArrow = makeBitmap("Images/Back Arrow.gif");
	BUTTON okButton;
	okButton.left = 269;
	okButton.top = 689;
	okButton.width = 289;
	okButton.height = 48;
	BUTTON scrollButton[2];
	scrollButton[0].left = 485;
	scrollButton[0].top = 45;
	scrollButton[0].width = 49;
	scrollButton[0].height = 50;
	scrollButton[1].left = 485;
	scrollButton[1].top = 316;
	scrollButton[1].width = 49;
	scrollButton[1].height = 50;
	BUTTON backButton;
	backButton.left = 40;
	backButton.top = 700;
	backButton.width = 49;
	backButton.height = 49;

	//Detemine the number of saveGame files
	while(fscanf(aFile, "%s", gameFile[i]) > 0)
	{
		i++;
		files++;
	}
	clear_keybuf();
	/*Get input and display save screen*/
	while((!key[KEY_ENTER]) || length == 0)
	{
		draw_sprite(screenBuffer, background, 0, 0);

		//Draw the 6 selected saveGame files
		for(i = startFile; i < MIN(files + startFile, 6 + startFile); i++)
		{
         draw_sprite(screenBuffer, aGameButton, 361, 100 + (i - startFile) * 36);
			textprintf_centre_ex(screenBuffer, font15, 508,110 + (i - startFile) * 36, makecol(0, 0, 0), -1, "%s.SPQR", gameFile[i]);
         textprintf_centre_ex(screenBuffer, font15, 510, 108 + (i - startFile) * 36, makecol(255, 255, 255), -1, "%s.SPQR", gameFile[i]);
		}
      textprintf_centre_ex(screenBuffer, font15, 510, 600, makecol(255, 255, 255), -1, "%s", saveName);

		//If there are more files above, draw buttons
		if (files > 6 && startFile != 0)
		{
         draw_sprite(screenBuffer, upArrow, scrollButton[0].left, scrollButton[0].top);
		}
		//If there are more files above, draw buttons
		if (files > 6 && startFile != files - 6)
		{
         draw_sprite(screenBuffer, downArrow, scrollButton[1].left, scrollButton[1].top);
		}
		draw_sprite(screenBuffer, backArrow, backButton.left, backButton.top);

		//Get keyboard input
		if (keypressed())
		{
			char aLetter[2] = "a";
			aLetter[0] = (readkey() & 0xff);
			if (((aLetter[0] >= 'A' && aLetter[0] <= 'Z') || (aLetter[0] >= 'a' && aLetter[0] <= 'z') || aLetter[0] == '_') && length < 20)
			{
				strcat(saveName, aLetter);
				length++;
			}
			else if (key[KEY_BACKSPACE] && length > 0)
			{
				saveName[length - 1] = '\0';
				length--;
			}
		}

		//Select a game if the user clicks one
		if (mouseClick[0] && mouseY < (105 + (MIN(files, 6) * 36)) && mouseY > 100)
		{
			if (mouseX > 361 && mouseX < 661)
			{
				strcpy(saveName, gameFile[(mouseY - 105) / 36 + startFile]);
				length = strlen(saveName);
			}
		}

		//If there is valid input and the user presses the next button, continue
   	if (mouseClick[0] && mouseIsOver(&okButton) && length > 0)
		{
 			break;
		}

		//To scroll up
		if (mouseClick[0] && mouseIsOver(&scrollButton[0]) && startFile > 0)
		{
         startFile--;
		}

		//To scroll down
		if (mouseClick[0] && mouseIsOver(&scrollButton[1]) && startFile < files - 6)
		{
         startFile++;
		}

		//Back Button
		if (mouseClick[0] && mouseIsOver(&backButton))
		{
         return;
		}
		evaluateMouse();
     	draw_sprite(screenBuffer, mousePic, mouseX, mouseY);
		blit(screenBuffer, screen, 0,0,0,0,1024,768);
	 	clear_bitmap(screenBuffer);
	}
	fclose(aFile);

	//Add the save file name to the master list
	aFile = fopen("Text/All_Saves.SPQR", "a");
	fprintf(aFile, "%s\n", saveName);
	fclose(aFile);

	char fileName[35] = "Text/";
	strcat(fileName, saveName);
	strcat(fileName, ".SPRQ");

	aFile = fopen(fileName, "w");
	//Add info to the individual save file
	fprintf(aFile, "%d\n", mapWidth);
	fprintf(aFile, "%d\n", money);
	fprintf(aFile, "%d\n", cameraX);
	fprintf(aFile, "%d\n", cameraY);
	for (i = 0; i < NUMRESOURCES; i++)
	{
     	fprintf(aFile, "\n%d ", totalProducers[i]);
	}
	int x;
	for (int y = 0; y < mapWidth; y++)
	{
		for (x = 0; x < mapWidth; x++)
		{
			fprintf(aFile, "\n%d ", aMapSquare[x][y].objectType);
			fprintf(aFile, "%d ", aMapSquare[x][y].variety);
			fprintf(aFile, "%d ", aMapSquare[x][y].people);
			for (i = 0; i < NUMRESOURCES; i ++)
			{
				fprintf(aFile, "%d ", aMapSquare[x][y].currentSupply[i]);
			}
		}
	}

	//Add info about people
	fprintf(aFile, "%d\n", totalWalkers);
	PERSON *thePerson = personHeader;
	while (thePerson != NULL)
	{
		fprintf(aFile, "%d ", thePerson->cargo);
		fprintf(aFile, "%d ", thePerson->cargoIndex);
		fprintf(aFile, "%d ", thePerson->x);
		fprintf(aFile, "%d ", thePerson->y);
		fprintf(aFile, "%d ", thePerson->homeX);
		fprintf(aFile, "%d ", thePerson->homeY);
		fprintf(aFile, "%d\n", thePerson->homeComing);
		thePerson = thePerson->next;
	}

	fclose(aFile);
	destroy_bitmap(background);
	destroy_bitmap(aGameButton);
	return;
}

/*  loadGame()

Loads the game

*/
void loadGame()
{
	//Make sure the file exists
	FILE *aFile = fopen("Text/All_Saves.SPQR", "a");
	fclose(aFile);

	aFile = fopen("Text/All_Saves.SPQR", "r");

	//An array of possible save file names
	char gameFile[100][50];

	//The current name to load
	char loadName[20] = "";

	int i = 0;
	int files = 0;
	int startFile = 0;
	int length = 0;
	int notValid = 1;
	BITMAP *background = makeBitmap("Images/LoadBackground.gif");
	BITMAP *aGameButton = makeBitmap("Images/GameSlot.gif");
	BITMAP *upArrow = makeBitmap("Images/UpArrow.gif");
	BITMAP *downArrow = makeBitmap("Images/DownArrow.gif");
	BITMAP *backArrow = makeBitmap("Images/Back Arrow.gif");
	BUTTON okButton;
	okButton.left = 269;
	okButton.top = 689;
	okButton.width = 289;
	okButton.height = 48;
	BUTTON backButton;
	backButton.left = 40;
	backButton.top = 700;
	backButton.width = 49;
	backButton.height = 49;
	BUTTON scrollButton[2];
	scrollButton[0].left = 485;
	scrollButton[0].top = 45;
	scrollButton[0].width = 49;
	scrollButton[0].height = 50;
	scrollButton[1].left = 485;
	scrollButton[1].top = 316;
	scrollButton[1].width = 49;
	scrollButton[1].height = 50;

	//Detemine the number of saveGame files
	while(fscanf(aFile, "%s", gameFile[i]) > 0)
	{
		i++;
		files++;
	}

	clear_keybuf();
	/*Get input and display load screen*/
	while(notValid)
	{
		draw_sprite(screenBuffer, background, 0, 0);

		//Draw the 6 selected saveGame files
		for(i = startFile; i < MIN(files + startFile, 6 + startFile); i++)
		{
         draw_sprite(screenBuffer, aGameButton, 361, 100 + (i - startFile) * 36);
			textprintf_centre_ex(screenBuffer, font15, 508,110 + (i - startFile) * 36, makecol(0, 0, 0), -1, "%s.SPQR", gameFile[i]);
         textprintf_centre_ex(screenBuffer, font15, 510, 108 + (i - startFile) * 36, makecol(255, 255, 255), -1, "%s.SPQR", gameFile[i]);
		}
      textprintf_centre_ex(screenBuffer, font15, 510, 590, makecol(255, 255, 255), -1, "%s", loadName);

		//If there are more files above, draw buttons
		if (files > 6 && startFile != 0)
		{
         draw_sprite(screenBuffer, upArrow, scrollButton[0].left, scrollButton[0].top);
		}
		//If there are more files above, draw buttons
		if (files > 6 && startFile != files - 6)
		{
         draw_sprite(screenBuffer, downArrow, scrollButton[1].left, scrollButton[1].top);
		}
		//draw_sprite(screenBuffer, backArrow, backButton.left, backButton.top);

		//Get keyboard input
		if (keypressed())
		{
			char aLetter[2] = "a";
			aLetter[0] = (readkey() & 0xff);
			if (((aLetter[0] >= 'A' && aLetter[0] <= 'Z') || (aLetter[0] >= 'a' && aLetter[0] <= 'z') || aLetter[0] == '_') && length < 20)
			{
				strcat(loadName, aLetter);
				length++;
			}
			else if (key[KEY_BACKSPACE] && length > 0)
			{
				loadName[length - 1] = '\0';
				length--;
			}
		}

		//Select a game if the user clicks one
		if (mouseClick[0] && mouseY < (105 + (MIN(files, 6) * 36)) && mouseY > 100)
		{
			if (mouseX > 361 && mouseX < 661)
			{
				strcpy(loadName, gameFile[(mouseY - 105) / 36 + startFile]);
				length = strlen(loadName);
			}
		}

		//To scroll up
		if (mouseClick[0] && mouseIsOver(&scrollButton[0]) && startFile > 0)
		{
         startFile--;
		}

		//To scroll down
		if (mouseClick[0] && mouseIsOver(&scrollButton[1]) && startFile < files - 6)
		{
         startFile++;
		}

		//Back Button
		/*if (mouseClick[0] && mouseIsOver(&backButton))
		{
         return;
		}*/

		//If user clicks "OK" and the specified file exists, continue
		if ((key[KEY_ENTER] || (mouseClick[0] && mouseIsOver(&okButton))) && length > 0)
		{
			for (i = 0; i < files; i ++)
			{
				if (!(strcmp(loadName, gameFile[i])));
				notValid = 0;
			}
		}
		evaluateMouse();
     	draw_sprite(screenBuffer, mousePic, mouseX, mouseY);
		blit(screenBuffer, screen, 0,0,0,0,1024,768);
	 	clear_bitmap(screenBuffer);
	}

	fclose(aFile);

	//If the game has not been initialized yet, do so
	if (!inGame)
	{
		initializeAllObjects();
	}
	//Create an empty map from the scanned mapWidth
	initializeMapSquares();

	char fileName[35] = "Text/";
	strcat(fileName, loadName);
	strcat(fileName, ".SPRQ");

	aFile = fopen(fileName, "r");
	//emptyGame();

	//Load all information from the file
	fscanf(aFile, "%d\n", &mapWidth);
	initializeMapSquares();
	fscanf(aFile, "%d\n", &money);
	fscanf(aFile, "%d\n", &cameraX);
	fscanf(aFile, "%d\n", &cameraY);
	for (i = 0; i < NUMRESOURCES; i++)
	{
     	fscanf(aFile, "\n%d ", &totalProducers[i]);
	}
	int x;
	for (int y = 0; y < mapWidth; y++)
	{
		for (x = 0; x < mapWidth; x++)
		{
			fscanf(aFile, "\n%d ", &aMapSquare[x][y].objectType);
			fscanf(aFile, "%d ", &aMapSquare[x][y].variety);
			aMapSquare[x][y].picture = anObject[aMapSquare[x][y].objectType].picture[aMapSquare[x][y].variety];
      	fscanf(aFile, "%d ", &aMapSquare[x][y].people);
			for (i = 0; i < NUMRESOURCES; i ++)
			{
				fscanf(aFile, "%d ", &aMapSquare[x][y].currentSupply[i]);
			}
		}
	}

/*	fscanf(aFile, "%d\n", &totalWalkers);
	for (i = 0; i < totalWalkers; i ++)
	{
		sendOccupant(0,0,FOOD);
      fscanf(aFile, "%d ", &(personHeader->cargo));
		fscanf(aFile, "%d ", &(personHeader->cargoIndex));
		fscanf(aFile, "%d ", &(personHeader->x));
		fscanf(aFile, "%d ", &(personHeader->y));
		fscanf(aFile, "%d ", &(personHeader->homeX));
		fscanf(aFile, "%d ", &(personHeader->homeY));
		fscanf(aFile, "%d\n", &(personHeader->homeComing));
	}*/


	destroy_bitmap(background);
	destroy_bitmap(aGameButton);
	if (!inGame)
	{
		SPQR_Game();
	}
	return;
}

