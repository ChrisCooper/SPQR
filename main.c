#include <stdlib.h>        //Memory handling prototypes
#include <math.h>          //Math routines
#include <time.h>          //For time(0) function
#include "definitions.h" 	//All structure definitions, #defines, #includes, and ENUMs
#include "globals.h"       //Includes all global variables declared as "extern"


/* THE UGLY MENU-DUCKLING

This was the first part of the game I made... so it's very ugly right now and
probably has some memory leaks

I plan to fix it in the future

I didn't bother commenting it because its so darn UGLY and hopefully I will be
able to re-work it very soon

I plan to work on this over the summer so please excuse the lack of commenting
(I've already done like... over 3000 lines WITHOUT commenting)

*/

#define MENUBUTTON struct menuButton

MENUBUTTON {
   int left, top, width, height;
   BITMAP *picture;
};


typedef struct slider_t {
	int left, top, width, height, values, current, position;
} slider_t;


extern void SPQR_Game();
extern void loadGame();
extern void allegInitializations();
/*
Initializes allegro and its associated packages (algif), as well as installing
hardware settings for mouse, keyboard, sound, timers, and the screen.
Autochecks for errors, and displays message and exits(1) on error.
*/

extern void loadEssentials();
extern void destroyObjects();
extern void emptyGame();
extern void initializeMapSquares();
extern void initializeAllObjects();


void goToNewGameMenu();
int menuClickDetector();
void showIntro();
int menu_mouseIsOver(MENUBUTTON *);
void printCampaignOptions();
void printScenarioOptions();
void printBattleOptions();
void initializeButton(MENUBUTTON *, int, int, int, int, char *);
void initializeSlider(slider_t *, int, int, int, int, int, int, int);
int makeSlider(slider_t *);
void showAssignment();

//PALETTE palette;
BITMAP *makeBitmap(char *aFilename);
SAMPLE *makeSample(char *aFilename);
FONT *makeFont(char *aFilename);


BITMAP *menuBuffer = NULL;
SAMPLE *newAmbient;

volatile long fireCounter = 0;
volatile long fireCheck = 0;

int main(int argc, char *argv[])
{
	allegInitializations();
	loadEssentials();

    int i = 0;

    MENUBUTTON aButton[5];
    initializeButton(&aButton[0], 359, 356, 305, 55, "Menu_Images/Check Box.bmp");
    initializeButton(&aButton[1], 359, 432, 305, 55, "Menu_Images/Check Box.bmp");
    initializeButton(&aButton[2], 359, 507, 305, 55, "Menu_Images/Check Box.bmp");
    initializeButton(&aButton[3], 359, 583, 305, 55, "Menu_Images/Check Box.bmp");
    initializeButton(&aButton[4], 359, 659, 305, 55, "Menu_Images/Check Box.bmp");

    int buttonClicked = 0;
    menuBuffer = create_bitmap(1024,768);
    BITMAP *menuBackground[7];
    menuBackground[0] = makeBitmap("Menu_Images/Menu Background 04.bmp");
    menuBackground[1] = makeBitmap("Menu_Images/Menu Background 05.bmp");
    menuBackground[2] = makeBitmap("Menu_Images/Menu Background 06.bmp");
    menuBackground[3] = makeBitmap("Menu_Images/Menu Background 07.bmp");
    menuBackground[4] = makeBitmap("Menu_Images/Menu Background 08.bmp");
    menuBackground[5] = makeBitmap("Menu_Images/Menu Background 09.bmp");
    menuBackground[6] = makeBitmap("Menu_Images/Menu Background 10.bmp");
    BITMAP *highlighter = makeBitmap("Menu_Images/Menu Selector.bmp"); // Load our picture
    BITMAP *mouse = makeBitmap("Menu_Images/Mouse.bmp"); // Load our picture
    BITMAP *mouseH = makeBitmap("Menu_Images/MouseH.bmp"); // Load our picture
    int animationCounter1 = 0;
    SAMPLE *menuClick[5];
    menuClick[0] = makeSample("Menu_Sounds/Menu Select 1.wav");
    menuClick[1] = makeSample("Menu_Sounds/Menu Select 2.wav");
    menuClick[2] = makeSample("Menu_Sounds/Menu Select 3.wav");
    menuClick[3] = makeSample("Menu_Sounds/Menu Select 4.wav");
    menuClick[4] = makeSample("Menu_Sounds/Menu Select 5.wav");
    double startTime = 0;
    SAMPLE *menuButtonClick[5];
    menuButtonClick[0] = makeSample("Menu_Sounds/New Game.wav");
    menuButtonClick[1] = makeSample("Menu_Sounds/Load Game.wav");
    menuButtonClick[2] = makeSample("Menu_Sounds/Options.wav");
    menuButtonClick[3] = makeSample("Menu_Sounds/Credits.wav");
    menuButtonClick[4] = makeSample("Menu_Sounds/Quit Game.wav");

    SAMPLE *menuAmbient = makeSample("Menu_Sounds/Menu Ambient.wav");

    int randomizer = 0;


    int highlightedOption = 1;
    int nextKey = -1;
    srand(time(0));

    showIntro();

    draw_sprite(menuBuffer, menuBackground[0], 0, 0);
    draw_sprite(menuBuffer, highlighter, 6, 186);
    blit(menuBuffer, screen, 0,0,0,0,1024,768);//Draw the buffer to the screen
    play_sample(menuAmbient, 255, 0, 1000, 1);

    while (true)
    {
        if (fireCounter != fireCheck)
        {
           randomizer = rand() % 7;
				fireCheck = fireCounter;

        }
        draw_sprite(menuBuffer, menuBackground[randomizer], 0, 0);
        draw_sprite(menuBuffer, mouse, mouse_x, mouse_y);

        if (mouse_b)
        {
           draw_sprite(menuBuffer, mouseH, mouse_x, mouse_y);
           for (i = 0; i <= 4; i++)
               {
                  if (menu_mouseIsOver(&aButton[i]))
                  {
                     buttonClicked = i;
                  }
               }
          switch (buttonClicked)
          {
                 case 0:
                      play_sample(menuButtonClick[0], 200, 0, 1000, 0);
                      stop_sample(menuClick[randomizer]);
                      stop_sample(menuAmbient);
                      goToNewGameMenu();
                      play_sample(menuAmbient, 255, 0, 1000, 1);
                      buttonClicked = -1;
                 break;
                 case 1:
                      play_sample(menuButtonClick[1], 200, 0, 1000, 0);
                      //loadGame();
                 break;
                 case 2:
                      play_sample(menuButtonClick[2], 200, 0, 1000, 0);
                 break;
                 case 3:
                      play_sample(menuButtonClick[3], 50, 0, 1000, 0);
                 break;
                 case 4:
                      stop_sample(menuAmbient);
                      play_sample(menuButtonClick[4], 255, 0, 1000, 0);
                      startTime = time(0);
                      while (startTime + 4 > time(0))
                      {
                      }
                      return 0;
                 break;
                 default:
                         stop_sample(menuClick[randomizer]);
                         randomizer = rand() % 5;
                         play_sample(menuClick[randomizer], 255, 0, 500, 0);
                 break;
          }
        }
        else
        {
            buttonClicked = -1;
        }
        blit(menuBuffer, screen, 0,0,0,0,1024,768);//Draw the buffer to the screen
        clear_bitmap(menuBuffer); // Clear the contents of the buffer bitmap

     }

}END_OF_MAIN()

void showIntro()
{
     SAMPLE *copyAmbient = makeSample("Menu_Sounds/Intro Wind.wav");
     BITMAP *copyBackground = makeBitmap("Menu_Images/Copyright Intro Screen.bmp");
     double startTime;

     blit(copyBackground, screen, 0,0,0,0,1024,768);//Draw the buffer to the screen
      play_sample(copyAmbient, 255, 0, 1000, 0);
      startTime = time(0);
      while (startTime + 10 > time(0) && !key[KEY_ESC])
      {
      }
      stop_sample(copyAmbient);
		destroy_sample(copyAmbient);
		destroy_bitmap(copyBackground);
}

void goToNewGameMenu()
{
     int i = 0;
     int finishedSelecting = 0;

    BITMAP *check =  makeBitmap("Menu_Images/Checked Box.bmp");
    BITMAP *empty =  makeBitmap("Menu_Images/Check Box.bmp");

    MENUBUTTON aToggle[3];
           initializeButton(&aToggle[0], 65, 38, 314, 46, "Menu_Images/Check Box.bmp");
           initializeButton(&aToggle[1], 65, 111, 314, 46, "Menu_Images/Check Box.bmp");
           initializeButton(&aToggle[2], 65, 186, 314, 46, "Menu_Images/Check Box.bmp");

   MENUBUTTON backArrow;
       initializeButton(&backArrow, 10, 715, 50, 50, "Menu_Images/Back Arrow.bmp");


     int toggleClicked = -1;


     BITMAP *newBackground = makeBitmap("Menu_Images/New Game Initial.bmp");

     BITMAP *mouse =  makeBitmap("Menu_Images/Mouse.bmp");
     BITMAP *mouseH = makeBitmap("Menu_Images/MouseH.bmp");
     newAmbient = makeSample("Menu_Sounds/Battle Drums.wav");
     SAMPLE *toggleNoise = makeSample("Menu_Sounds/ToggleNoise.wav");

     play_sample(newAmbient, 100, 0, 1000, 1);

     while (1)
     {
           if (mouse_b)
           {
                for (i = 0; i <= 2; i++)
                {
                          if (menu_mouseIsOver(&aToggle[i]))
                          {
                             toggleClicked = i;
                             aToggle[i].picture = check;
                             stop_sample(toggleNoise);
                             play_sample(toggleNoise, 255, 0, 1000, 0);
                             finishedSelecting = 1;
                             //newBackground = load_bitmap("New Game Background 01.bmp", NULL);
                          }


                 }

                 if (menu_mouseIsOver(&backArrow))
                 {
                    stop_sample(newAmbient);
                    return;
                 }
           }
           for (i = 0; i <= 2; i++)
           {
               if (toggleClicked != i)
               {
                    aToggle[i].picture = empty;
               }
           }

           draw_sprite(menuBuffer, newBackground, 0, 0);

           if (finishedSelecting)
           {
               if (toggleClicked == 0)
               {
                  textprintf_centre_ex(menuBuffer, font, 500, 300, makecol(255, 255, 255), -1, "Not availible in the demo version!");
                  //printCampaignOptions();
               }
               if (toggleClicked == 1)
               {
                  printScenarioOptions();
               }
               if (toggleClicked == 2)
               {
                  textprintf_centre_ex(menuBuffer, font, 500, 300, makecol(255, 255, 255), -1, "Not availible in the demo version!");
                  //printBattleOptions();
               }
           }
           if (menu_mouseIsOver(&backArrow))
           {
                 backArrow.picture = makeBitmap("Menu_Images/Back ArrowH.bmp");
           }
           else
           {
               backArrow.picture = makeBitmap("Menu_Images/Back Arrow.bmp");
           }
           draw_sprite(menuBuffer, backArrow.picture, backArrow.left, backArrow.top);
           for (i = 0; i <= 2; i++)
           {
                draw_sprite(menuBuffer, aToggle[i].picture, aToggle[i].left, aToggle[i].top);
           }
           draw_sprite(menuBuffer, mouse, mouse_x, mouse_y);
           blit(menuBuffer, screen, 0,0,0,0,1024,768);
           clear_bitmap(menuBuffer);
     }
     stop_sample(newAmbient);
}

void printCampaignOptions()
{
     int i = 0;

    static int firstTime = 1;
    static int timeClicked = 0;
    static int sliderClicked = 1;

    static BITMAP *check =  makeBitmap("Menu_Images/Checked Box.bmp");
    static BITMAP *empty =  makeBitmap("Menu_Images/Check Box.bmp");

    static MENUBUTTON aSlider[3];


    static MENUBUTTON forwardArrow;


    static MENUBUTTON tutorial;

    static BITMAP *newBackground = makeBitmap("Menu_Images/Campaign Menu 01.bmp");


   static BITMAP *slider =  makeBitmap("Menu_Images/Slider.bmp");
   static BITMAP *difficulties[3];

   static SAMPLE *slideNoise = makeSample("Menu_Sounds/SliderNoise.wav");

    if (firstTime)
    {
       initializeButton(&aSlider[0], 490, 180, 150, 50, "Menu_Images/Slider.bmp");
       initializeButton(&aSlider[1], 625, 180, 150, 50, "Menu_Images/Slider.bmp");
       initializeButton(&aSlider[2], 810, 180, 150, 50, "Menu_Images/Slider.bmp");
       initializeButton(&forwardArrow, 920, 677, 57, 50, "Menu_Images/Forward Arrow.bmp");
       initializeButton(&tutorial, 530, 340, 290, 46, "Menu_Images/Checked Box.bmp");
       difficulties[0] = makeBitmap("Menu_Images/Senator.bmp");
       difficulties[1] = makeBitmap("Menu_Images/Consul.bmp");
       difficulties[2] = makeBitmap("Menu_Images/Emperor.bmp");
    }

    draw_sprite(menuBuffer, newBackground, 0, 0);

    if (menu_mouseIsOver(&forwardArrow))
    {
                 forwardArrow.picture = makeBitmap("Menu_Images/Forward ArrowH.bmp");
    }
    else if (!menu_mouseIsOver(&forwardArrow))
    {
               forwardArrow.picture = makeBitmap("Menu_Images/Forward Arrow.bmp");
    }

    switch(sliderClicked)
    {
        case 0:
             draw_sprite(menuBuffer, slider, 498, 197);
             draw_sprite(menuBuffer, difficulties[0], 617, 255);
        break;
        case 1:
             draw_sprite(menuBuffer, slider, 700, 197);
             draw_sprite(menuBuffer, difficulties[1], 617, 255);
        break;
        case 2:
             draw_sprite(menuBuffer, slider, 913, 197);
             draw_sprite(menuBuffer, difficulties[2], 617, 255);
        break;
    }

        if (mouse_b)
        {
            for (i = 0; i <= 2; i++)
            {
                      if (menu_mouseIsOver(&aSlider[i]))
                      {
                         sliderClicked = i;
                         stop_sample(slideNoise);
                         play_sample(slideNoise, 155, 0, 1000, 0);
                         break;
                      }
             }
             if (menu_mouseIsOver(&forwardArrow))
             {
                return;
             }
             if (menu_mouseIsOver(&tutorial) && timeClicked < time(0))
             {
                timeClicked = time(0);
                if (tutorial.picture == empty)
                {
                   tutorial.picture = check;
                }
                else
                {
                    tutorial.picture = empty;
                }
             }
        }
        draw_sprite(menuBuffer, tutorial.picture, tutorial.left, tutorial.top);
        draw_sprite(menuBuffer, forwardArrow.picture, forwardArrow.left, forwardArrow.top);
        firstTime = 0;
}

void printScenarioOptions()
{
     int i = 0;
    static int firstTime = 1;
    static struct slider moneySlider;

     if (firstTime)
     {
         initializeSlider(&moneySlider, 493, 190, 442, 50, 4, 1, 700);
     }
    static MENUBUTTON forwardArrow;
    initializeButton(&forwardArrow, 920, 677, 57, 50, "Menu_Images/Forward Arrow.bmp");

    static BITMAP *newBackground = makeBitmap("Menu_Images/Scenario Menu 01.bmp");

    draw_sprite(menuBuffer, newBackground, 0, 0);
    makeSlider(&moneySlider);

    if (menu_mouseIsOver(&forwardArrow))
           {
                 forwardArrow.picture = makeBitmap("Menu_Images/Forward ArrowH.bmp");
           }
           else if (!menu_mouseIsOver(&forwardArrow))
           {
               forwardArrow.picture = makeBitmap("Menu_Images/Forward Arrow.bmp");
           }

        if (mouse_b)
        {
             if (menu_mouseIsOver(&forwardArrow))
             {
                showAssignment();
               stop_sample(newAmbient);
               loadingPhase = 0;
               inGame = 1;
               initializeAllObjects();
               initializeMapSquares();
                SPQR_Game();
					emptyGame();
             }
        }
        draw_sprite(menuBuffer, forwardArrow.picture, forwardArrow.left, forwardArrow.top);
        firstTime = 0;
}
void printBattleOptions()
{

     static slider_t aBattleSlider[3];

     static int firstTime = 1;
     static int characterClicked = 0;
    static BITMAP *godNames[3];

     if (firstTime)
     {
         initializeSlider(&aBattleSlider[0], 493, 190, 442, 50, 4, 1, 700);
         initializeSlider(&aBattleSlider[1], 493, 315, 442, 50, 4, 1, 700);
         initializeSlider(&aBattleSlider[2], 493, 600, 442, 50, 4, 1, 700);
         firstTime = 0;

    godNames[0] = makeBitmap("Menu_Images/Jupiter Name.bmp");
    godNames[1] = makeBitmap("Menu_Images/Mars Name.bmp");
    godNames[2] = makeBitmap("Menu_Images/Neptune Name.bmp");
     }

     static BITMAP *newBackground = makeBitmap("Menu_Images/Single Battle Menu 01.bmp");
    int i = 0;
    int j = 0;
    static MENUBUTTON forwardArrow;
    initializeButton(&forwardArrow, 920, 677, 57, 50, "Menu_Images/Forward Arrow.bmp");

    static MENUBUTTON aGod[3];
    initializeButton(&aGod[0], 475, 434, 129, 66, "Menu_Images/Zeus.bmp");
    initializeButton(&aGod[1], 649, 434, 129, 66, "Menu_Images/Mars.bmp");
    initializeButton(&aGod[2], 825, 434, 129, 66, "Menu_Images/Neptune.bmp");

    static SAMPLE *slideNoise = makeSample("Menu_Sounds/SliderNoise.wav");

    static SAMPLE *aGodSound[3];
    aGodSound[0] = makeSample("Menu_Sounds/Jupiter.wav");
    aGodSound[1] = makeSample("Menu_Sounds/Mars.wav");
    aGodSound[2] = makeSample("Menu_Sounds/Neptune.wav");

    draw_sprite(menuBuffer, newBackground, 0, 0);

     for(i = 0; i <= 2; i++)
     {
     makeSlider(&aBattleSlider[i]);
     }

     if (menu_mouseIsOver(&forwardArrow))
           {
                 forwardArrow.picture = makeBitmap("Menu_Images/Forward ArrowH.bmp");
           }
           else
           {
               forwardArrow.picture = makeBitmap("Menu_Images/Forward Arrow.bmp");
           }

        if (mouse_b)
        {
           for (i=0; i<=2; i++)
           {
                 if (menu_mouseIsOver(&aGod[i]))
                 {
                       characterClicked = i;
                       for (j=0; j<=2; j++)
                       {
                            stop_sample(aGodSound[i]);
                       }
                       play_sample(aGodSound[i], 155, 0, 1000, 0);
                 }
           }
             if (menu_mouseIsOver(&forwardArrow))
             {
                return;
             }
        }
        aGod[0].picture = makeBitmap("Menu_Images/Zeus.bmp");
        aGod[1].picture = makeBitmap("Menu_Images/Mars.bmp");
        aGod[2].picture = makeBitmap("Menu_Images/Neptune.bmp");
        switch(characterClicked)
        {
            case 0:
                 aGod[0].picture = makeBitmap("Menu_Images/ZeusH.bmp");
            break;
            case 1:
                 aGod[1].picture = makeBitmap("Menu_Images/MarsH.bmp");
            break;
            case 2:
                 aGod[2].picture = makeBitmap("Menu_Images/NeptuneH.bmp");
            break;
        }
        for (i=0; i<=2; i++)
        {
                 draw_sprite(menuBuffer, aGod[i].picture, aGod[i].left, aGod[i].top);
        }
        draw_sprite(menuBuffer, godNames[characterClicked], 616, 515);
        draw_sprite(menuBuffer, forwardArrow.picture, forwardArrow.left, forwardArrow.top);
}

void initializeButton(MENUBUTTON *theButton, int left, int top, int width, int height, char *fileName)
{
     theButton->left = left;
     theButton->top = top;
     theButton->width = width;
     theButton->height = height;
     theButton->picture = makeBitmap(fileName);
}
void initializeSlider(slider_t *theSlider, int left, int top, int width, int height, int values, int current, int position)
{
     theSlider->left = left;
     theSlider->top = top;
     theSlider->width = width;
     theSlider->height = height;
     theSlider->values = values;
     theSlider->current = current;
     theSlider->position = position;
}

int menu_mouseIsOver(MENUBUTTON *theButton)
{
    return (mouse_x > theButton->left && mouse_x < theButton->left + theButton->width && mouse_y > theButton->top && mouse_y < theButton->top + theButton->height);
}
int menu_mouseIsOverSlider(slider_t *theSlider)
{
    return (mouse_x > theSlider->left && mouse_x < theSlider->left + theSlider->width && mouse_y > theSlider->top && mouse_y < theSlider->top + theSlider->height);
}

int makeSlider(slider_t *theSlider)
{
    static SAMPLE *slideNoise = makeSample("Menu_Sounds/SliderNoise.wav");
	static BITMAP *bar = makeBitmap("Menu_Images/Slider.bmp");

	if (mouse_b && menu_mouseIsOverSlider(theSlider))
	{
			theSlider->position += (mouse_x - theSlider->position)/ 4;
            stop_sample(slideNoise);
            play_sample(slideNoise, 155, 0, 1000, 0);
	}
	draw_sprite(menuBuffer, bar, theSlider->position - 10, theSlider->top + 5);

    theSlider->current = (int)(((theSlider->position - theSlider->left) * theSlider->values)/theSlider->width) + 1;

    if (theSlider->current > theSlider->values)
    {
       theSlider->current = theSlider->values;
    }
    else if (theSlider->current < 1)
    {
       theSlider->current = 1;
    }
	return (theSlider->current);
}

void showAssignment()
{
     BITMAP *background = makeBitmap("Menu_Images/Instructions0.gif");

     draw_sprite(menuBuffer, background, 0, 0);
     blit(menuBuffer, screen, 0,0,0,0,1024,768);
		clear_keybuf();
		readkey();
		background = makeBitmap("Menu_Images/Instructions1.gif");
     draw_sprite(menuBuffer, background, 0, 0);
     blit(menuBuffer, screen, 0,0,0,0,1024,768);
		clear_keybuf();
		readkey();
}


/*THE USEFUL FUNCTIONS.........*/

//returns a pointer to BITMAP (char *aFilename) and shows
//error message on failure
BITMAP *makeBitmap(char *aFilename)
{
	BITMAP *bitmapPointer;
	if (!(bitmapPointer = load_bitmap(aFilename, NULL)))
	{
      set_gfx_mode(GFX_TEXT, 1024,768,0,0);
		allegro_message("Error! Could not find the file \"%s\"!", aFilename);
		exit(1);
	}
	return(bitmapPointer);
}

//returns a pointer to SAMPLE (char *aFilename) and shows
//error message on failure
SAMPLE *makeSample(char *aFilename)
{
	SAMPLE *samplePointer;
	if (!(samplePointer = load_sample(aFilename)))
	{
      set_gfx_mode(GFX_TEXT, 1024,768,0,0);
		allegro_message("Error! Could not find the file \"%s\"!", aFilename);
		exit(1);
	}
	return(samplePointer);
}

//returns a pointer to FONT (char *aFilename) and shows
//error message on failure
FONT *makeFont(char *aFilename)
{
	FONT *fontPointer;
	if (!(fontPointer = load_font(aFilename, palette, NULL)))
	{
      set_gfx_mode(GFX_TEXT, 1024,768,0,0);
		allegro_message("Error! Could not find the file \"%s\"!", aFilename);
		exit(1);
	}
	return(fontPointer);
}
