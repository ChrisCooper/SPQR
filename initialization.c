#include "definitions.h" //All structure definitions, #defines, #includes, and ENUMs
#include "globals.h"     //Includes all global variables declared as "extern"

/*Timer functions*/
//This funtions all increment the specified long int
extern void incrementWalkTimer();
extern void incrementResourceTimer();
extern void incrementEconomyCounter();
extern void incrementUpdateCounter();
extern void incrementAnimateCounter();
extern void incrementFireCounter();
extern void incrementTimer();



/*OTHER EXTERNS*/


extern void createSquare(int createIndex, int x, int y);
/*
Creates a square of type "createIndex" on aMapSquare[x][y]
*/

extern void loadAllObjects();
/*
Read in all OBJECTs from a file and initialize them
*/

void initializeAllObjects();
/*
Loads all BITMAPs, SAMPLEs, and initializes other variables used in the actual game
*/

void initializeMapSquares();
/*
Mallocs mapWidth*mapWidth square, and initializes them to be EMPTY SQUAREs
*/

void showLoadingBar(char *stage);
/*
Show loading screen with loading bar and an indication of what is being loaded (char *stage)
*/

void loadEssentials();
/*
Loads support variables such as mousePic, screenBuffer, etc.
*/

/*These functions load a BITMAP, SAMPLE, or FONT and do their own error checking*/
extern BITMAP *makeBitmap(char *aFilename);
extern SAMPLE *makeSample(char *aFilename);
extern FONT *makeFont(char *aFilename);

/*  allegInitializations()

Initializes allegro and its associated packages (algif), as well as installing
hardware settings for mouse, keyboard, sound, timers, and the screen.
Autochecks for errors, and displays message and exits(1) on error.

*/
void allegInitializations()
{
	int error = 0;
	srand(time(0));
   error |= allegro_init ();
	algif_init ();

   	set_color_depth (16);
   	//set_gfx_mode(GFX_AUTODETECT_WINDOWED, 900,600,0,0);
   	set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1024,768,0,0);
   	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
   	install_keyboard ();
   	install_mouse();
   	error |= install_timer();
   	error |= install_int_ex(incrementTimer, BPS_TO_TIMER(60));
   	error |= install_int_ex(incrementUpdateCounter, BPS_TO_TIMER(4));
   	error |= install_int_ex(incrementAnimateCounter, BPS_TO_TIMER(8));
   	error |= install_int_ex(incrementEconomyCounter, BPM_TO_TIMER(60));
   	error |= install_int_ex(incrementFireCounter, BPS_TO_TIMER(12));
	if (error)
	{
		set_gfx_mode(GFX_TEXT, 1024,768,0,0);
		allegro_message("Allegro Failed to Initialize!");
		exit(1);
	}

}


/*  initializeAllObjects()

Loads all images, sounds, etc.
Operations are quite self-explanatory

*/
void initializeAllObjects()
{
	screenBuffer = create_bitmap(1024, 768);
	loadBackground = makeBitmap("Images/Loading Background.gif");
	mousePic = makeBitmap("Images/GameMouse.gif");
   font15 = makeFont("Text/Hand15.pcx");
	loadAllObjects();

   showLoadingBar("Building Sounds");

   buildingSound[EMPTY] = makeSample("Sound/Building/BUILD1.wav");
   buildingSound[ROAD] = makeSample("Sound/Building/BUILD1.wav");
   buildingSound[MARKET] = makeSample("Sound/Building/COIN.wav");
   buildingSound[FARM] = makeSample("Sound/Building/GRANARY1.wav");
   buildingSound[FOUNTAIN] = makeSample("Sound/Building/FOUNTAIN.wav");
   buildingSound[THEATRE] = makeSample("Sound/Building/ART_PIT.wav");
   buildingSound[TEMPLE] = makeSample("Sound/Building/ORACLE.wav");
   buildingSound[POTTERSHOP] = makeSample("Sound/Building/CLAY_PIT.wav");
   buildingSound[BARBERSHOP] = makeSample("Sound/Building/BARBER.wav");
   buildingSound[FURNITUREWORKSHOP] = makeSample("Sound/Building/FORUM.wav");
   buildingSound[WINEPRESS] = makeSample("Sound/Building/wine_workshop.wav");
   buildingSound[BATHHOUSE] = makeSample("Sound/Building/BATHS.wav");
   buildingSound[SLAVETRADER] = makeSample("Sound/Building/CLINIC.wav");
   buildingSound[OLIVEPRESS] = makeSample("Sound/Building/MINE.wav");
   buildingSound[ACADEMY] = makeSample("Sound/Building/FORUM.wav");
   buildingSound[TENT] = makeSample("Sound/Building/HOUSING.wav");



   selector = makeBitmap("Images/Selector.gif");


	showLoadingBar("People");


	personPic[0] = makeBitmap("Images/Person0.gif");
	personPic[1] = makeBitmap("Images/Person1.gif");
	personPic[2] = makeBitmap("Images/Person2.gif");
	personPic[3] = makeBitmap("Images/Person3.gif");
	personPic[4] = makeBitmap("Images/Person4.gif");
	personPic[5] = makeBitmap("Images/Person5.gif");

	infoFile = fopen("Text/InfoFile.txt", "w");

	HUD = makeBitmap("Images/HUD.bmp");

   int tY[NUMHUDBUTTONS] = {49, 100, 151, 202, 254, 304, 355, 401, 452, 508, 559, 610, 661, 712};
	for (int i = 0; i < NUMHUDBUTTONS; i++)
	{
		resourceButton[i].left = 968;
		resourceButton[i].top = tY[i];
		resourceButton[i].width = 34;
		resourceButton[i].height = 34;
	}

	menuButton.left = 309;
	menuButton.top = 0;
	menuButton.width = 77;
	menuButton.height = 33;

	buildingButton.left = 802;
	buildingButton.top = 42;
	buildingButton.width = 63;
	buildingButton.height = 115;

	personButton.left = 867;
	personButton.top = 42;
	personButton.width = 63;
	personButton.height = 115;

	placmentOutline = makeBitmap("Images/placmentOutline.bmp");

	showLoadingBar("Ambient Sounds");

   ambientSound[0] = makeSample("Sound/Bird0.wav");
   ambientSound[1] = makeSample("Sound/Bird1.wav");
   ambientSound[2] = makeSample("Sound/Bird2.wav");
   ambientSound[3] = makeSample("Sound/Bird3.wav");
   ambientSound[4] = makeSample("Sound/Bird4.wav");
   ambientSound[5] = makeSample("Sound/Bird5.wav");
   ambientSound[6] = makeSample("Sound/Bird6.wav");
   ambientSound[7] = makeSample("Sound/Bird7.wav");

    font70 = makeFont("Text/Hand70.pcx");
    font15 = makeFont("Text/Hand15.pcx");

	resourceColour[FOOD] = makecol(255, 255,0);
	resourceColour[WATER] = makecol(0,0,255);
	resourceColour[ENTERTAINMENT] = makecol(255 ,0,0);
	resourceColour[RELIGION] = makecol(170, 170, 170);
	resourceColour[POTTERY] = makecol(230, 196, 113);
	resourceColour[BARBER] = makecol(214, 214, 214);
	resourceColour[FURNITURE] = makecol(94, 74,0);
	resourceColour[WINE] = makecol(255, 43, 124);
	resourceColour[BATHING] = makecol(0,0,255);
	resourceColour[SLAVES] = makecol(255,100,100);
	resourceColour[OLIVE_OIL] = makecol(0, 143, 75);
	resourceColour[EDUCATION] = makecol(255,255,255);


	showLoadingBar("Resource Icons");

	resourceIcon[0] = makeBitmap("Images/Icon0.gif");
	resourceIcon[1] = makeBitmap("Images/Icon1.gif");
	resourceIcon[2] = makeBitmap("Images/Icon2.gif");
	resourceIcon[3] = makeBitmap("Images/Icon3.gif");
	resourceIcon[4] = makeBitmap("Images/Icon4.gif");
	resourceIcon[5] = makeBitmap("Images/Icon5.gif");
	resourceIcon[6] = makeBitmap("Images/Icon6.gif");
	resourceIcon[7] = makeBitmap("Images/Icon7.gif");
	resourceIcon[8] = makeBitmap("Images/Icon8.gif");
	resourceIcon[9] = makeBitmap("Images/Icon9.gif");
	resourceIcon[10] = makeBitmap("Images/Icon10.gif");
	resourceIcon[11] = makeBitmap("Images/Icon11.gif");

	draw_sprite(screenBuffer, loadBackground, 0, 0);
	rectfill(screenBuffer, 209, 524, 209 + (int)(((float)((float)loadingPhase)/(float)(loadingPhases)) * (810 - 209)), 531, makecol(255, 0, 0));
	textprintf_centre_ex(screenBuffer, font15, 509, 482, 0, -1, "Loading Complete!       Press any key to continue");
	textprintf_centre_ex(screenBuffer, font15, 510, 480, makecol(255, 255, 255), -1, "Loading Complete!       Press any key to continue");
	draw_sprite(screenBuffer, mousePic, mouseX, mouseY);
	blit(screenBuffer, screen, 0,0,0,0,1024,768);
   clear_bitmap(screenBuffer);
	destroy_bitmap(loadBackground);

	readkey();
}



void initializeMapSquares()
{
    aMapSquare = (SQUARE**)malloc(mapWidth * sizeof(SQUARE*));

	for(int i = 0; i < mapWidth; i++)
	{
	        aMapSquare[i] = (SQUARE*)malloc(mapWidth * sizeof(SQUARE));
	}

    for (int y = 0; y < mapWidth; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
               createSquare(EMPTY, x, y);
        }
    }
}

/*  showLoadingBar(char *stage)

Show the loading screen and the loading bar

*/
void showLoadingBar(char *stage)
{
	loadingPhase++;
	draw_sprite(screenBuffer, loadBackground, 0, 0);
	rectfill(screenBuffer, 209, 524, 209 + (int)(((float)((float)loadingPhase)/(float)(loadingPhases)) * (810 - 209)), 531, makecol(255, 0, 0));
	textprintf_centre_ex(screenBuffer, font15, 509, 482, 0, -1, "Now loading \"%s\"", stage);
	textprintf_centre_ex(screenBuffer, font15, 510, 480, makecol(255, 255, 255), -1, "/*Now loading \"%s\"", stage);
	draw_sprite(screenBuffer, mousePic, mouseX, mouseY);
	blit(screenBuffer, screen, 0,0,0,0,1024,768);
   clear_bitmap(screenBuffer);
}

/*  emptyGame()

Destorys all PERSONs and map SQUAREs

*/
void emptyGame()
{
	PERSON *thePerson = personHeader;
	PERSON *nextPerson;
	while (thePerson != NULL)
	{
      nextPerson = thePerson->next;
		free(thePerson);
		thePerson = nextPerson;
	}
	personHeader = NULL;
   totalWalkers = 0;
	for (int i = 0; i < NUMRESOURCES; i ++)
	{
		totalProducers[i] = 0;
	}
	for (int y = 0; y < mapWidth; y++)
	{
		free(aMapSquare[y]);
	}
}

/*  destroyObjects()

Destorys all OBJECTs
Unloads the game

*/
void destroyObjects()
{
	for (int i = 0; i < totalBuildings; i++)
	{
		free(anObject);
	}
	for (int i = 0; i < 6; i ++)
	{
		destroy_bitmap(personPic[i]);
	}
}


/*  loadEssentials()

Loads support variables such as mousePic, screenBuffer, etc.

*/
void loadEssentials()
{
	screenBuffer = create_bitmap(1024, 768);
	loadBackground = makeBitmap("Images/Loading Background.gif");
	mousePic = makeBitmap("Images/GameMouse.gif");
   font15 = makeFont("Text/Hand15.pcx");
}
