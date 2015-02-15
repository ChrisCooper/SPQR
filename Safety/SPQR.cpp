/*
   SSSSSSSSSSSSSSS PPPPPPPPPPPPPPPPP        QQQQQQQQQ     RRRRRRRRRRRRRRRRR
 SS:::::::::::::::SP::::::::::::::::P     QQ:::::::::QQ   R::::::::::::::::R
S:::::SSSSSS::::::SP::::::PPPPPP:::::P  QQ:::::::::::::QQ R::::::RRRRRR:::::R
S:::::S     SSSSSSSPP:::::P     P:::::PQ:::::::QQQ:::::::QRR:::::R     R:::::R
S:::::S              P::::P     P:::::PQ::::::O   Q::::::Q  R::::R     R:::::R
S:::::S              P::::P     P:::::PQ:::::O     Q:::::Q  R::::R     R:::::R
 S::::SSSS           P::::PPPPPP:::::P Q:::::O     Q:::::Q  R::::RRRRRR:::::R
  SS::::::SSSSS      P:::::::::::::PP  Q:::::O     Q:::::Q  R:::::::::::::RR
    SSS::::::::SS    P::::PPPPPPPPP    Q:::::O     Q:::::Q  R::::RRRRRR:::::R
       SSSSSS::::S   P::::P            Q:::::O     Q:::::Q  R::::R     R:::::R
            S:::::S  P::::P            Q:::::O  QQQQ:::::Q  R::::R     R:::::R
            S:::::S  P::::P            Q::::::O Q::::::::Q  R::::R     R:::::R
SSSSSSS     S:::::SPP::::::PP          Q:::::::QQ::::::::QRR:::::R     R:::::R
S::::::SSSSSS:::::SP::::::::P           QQ::::::::::::::Q R::::::R     R:::::R
S:::::::::::::::SS P::::::::P             QQ:::::::::::Q  R::::::R     R:::::R
 SSSSSSSSSSSSSSS   PPPPPPPPPP               QQQQQQQQ::::QQRRRRRRRR     RRRRRRR
                                                    Q:::::Q
                                                     QQQQQ
               _    _
            _ ( )_ ( )
 _   _   _ (_)| ,_)| |__
( ) ( ) ( )| || |  |  _ `\
| \_/ \_/ || || |_ | | | |
`\___x___/'(_)`\__)(_) (_)  ()  ()  ()

         ______   ___    ___
        /\  _  \ /\_ \  /\_ \
        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
                                           /\____/
                                           \_/__/


      By Chris Cooper.


      See in-game instructions for game information.


*/


/*   MAIN CONCEPTS

-Objects  (anObject[21]):
	Each object is a template definition for the different possible contents of a tile, in a stucture format.
	When checking the attributes of the tile contents (ex. a FARM), this list is referred to.
	EX>
		To check the maximum supply of resource[index]:

		anObject[aMapSquare[x][y].objectType].maxSupply



-Squares (aMapSquare[columns][rows]):
	The dimensions of this array corresponds to the dimensions of the map. Each member of the array contains
	the values that a tile needs to know about itself; i.e. how much of each resource it has, what its population
	is, etc.



-People:
	People are the transporters of goods that wander the town. They are stored in a linked list, sorted in the
	order they should be printed in (from left to right, top to bottom). When they need to be printed or process,
	the list is stepped through and they are individually evaluated. In this way, they do not need to be
	individually tracked, and there can be any number of them at once. To create a person, the function "sendOccupant
	it used. It automatically inserts the person at the top of the list, then sorts the list. The function
	"destroyPerson" removes the person it is passed, and resets the appropriate pointers so the rest of the list
	is not cut off. The structure "PERSON" contains information about their location, cargo, etc.
	EX>
		to do something with all of the people:

	PERSON *tempPerson = personHeader;
	while (tempPerson != NULL)
	{
		doSomething(thePerson);
		thePerson = thePerson->next;
	}

*/


#include "definitions.h"
/*
"definitions.h" includes definitions for all structures used in SPQR, and
all #define statements and ENUMs for the constants used universally in the game.
*/

#include "global_declaration.h"
/*
"global_declaration.h" includes the declarations for all global variables.
This header file is only included once in "SPQR.cpp" so that they are only
declared once.
*/

extern void allegInitializations();
/*
Initializes allegro and its associated packages (algif), as well as installing
hardware settings for mouse, keyboard, sound, timers, and the screen.
Autochecks for errors, and displays message and exits(1) on error.
*/

extern void getInput();
/*
Moves the camera, set the mouse variables, checks for hotkeys and button-presses.
*/

extern void gameVisuals();
/*
Draws the grid, people, histograms, HUD, and mouse.
*/

extern void sortPeople();
/*
Sorts the linked list of people so that they are in printing order.
*/

extern void animatePeople();
/*
Advances the individual frame of each person.
*/

extern void updatePeople();
/*
Searches for a destination for each person, makes people
drop off or pick up goods depending on their location.
*/

extern void movePeople();
/*
Moves people towards their destination.
*/

extern void playAmbientSound();
/*
Plays a random ambient sound.
*/

extern void checkThresholds();
/*
For every square, checks if the house is running low on any resource; if it is and there is a functioning
producer of that resource nearby, sends an occupant to pick up that resource.
*/

extern void updateResources();
/*
Increases each building's supply of each resource by its productions rate, or decreases it by its consumption
rate.
*/

extern void calculatePopulation();
/*
Sums up all people in all buildings on the map, also allocates employees to each building
*/

extern void updateHousing();
/*
Checks if each house should upgrade or downgrade; upgrades and downgrades the houses if necessary
*/

extern void calculatePercentages();
/*
Calculates, for each square and each of its resources, how full the supply is, and sets the variable
"aMapSquare[x][y].percentageFull[resourceType]" to that percentage
*/
extern void initializeAllObjects();
/*
Loads all objects from a file and creates (using malloc) the map to
the variable dimensions set in Initializes allegro and its associated packages (algif), as well as installing
hardware settings for mouse, keyboard, sound, timers, and the screen.
Autochecks for errors, and displays message and exits(1) on error.
*/

extern void	updateMoney();
/*
Increases the user's money by "revenue", decreases it by "expenses"
*/



int SPQR_Game()
{
	//initializeAllObjects();

	int randomizer = 0;                    //Used for random timing of events

   inGame = 1;
	//The main game loop
	while (inGame)
    {

/*
	All timers use the format:

	if (timerVar less than 'X' and not the same as last time)
	{
		do_something;
	}
	else if (more than 'X')
	{
		do_something_else;
		reset timerVar;
	}

*/

			//Timer for updating money
			if (time(0) % 3 == 0 && moneyNotDone)//excecute every 3 seconds
			{
				updateMoney();
				moneyNotDone = 0;
			}
			else if (time(0) % 3 != 0)
			{
            moneyNotDone = 1;
			}

         /*Counter for playing ambient sounds, and updating resources     60 BPM*/
         if (economyCounter != economyCounterCheck)
			{
            checkThresholds();
            updateResources();
				calculatePopulation();
				if (!(economyCounter % 5)) // every 5 cycles
				{
					updateHousing();
				}
            economyCounterCheck = economyCounter;
				//playAmbientSound();
				if (!(randomizer = rand() % 2))
				{
					play_sample(ambientSound[rand() % NUMABIENTSOUNDS], 255, 127, 1000, 0);
				}
			}

			/*Counter for updating atributes of people        4 BPS*/
          if (updateCounter < 100000 && updateCounterCheck != updateCounter)
          {
              updatePeople();
					sortPeople();
              updateCounterCheck = updateCounter;
          }
          else
          {
              updateCounter = 0;
          }

			/*Counter for animation of people                 8 BPS*/
          if (animateCounter < 100000 && animateCounterCheck != animateCounter)
          {
              animatePeople();
					movePeople();
					sortPeople();
					calculatePercentages();
              animateCounterCheck = animateCounter;
          }
          else
          {
              animateCounter = 0;
          }

			/*Main logic vs. visuals loop                      60 BPS*/
			if (speedCounterCheck == speedCounter)
			{
			}
			else if (speedCounter > 1)
			{
              speedCounter = 0;
              gameVisuals();
			}
         else if (speedCounterCheck != speedCounter)
         {
              getInput();
              speedCounterCheck = speedCounter;
         }
    }
	return 0;
}

//TO-DO
/*

*Change loading of object picture to scan "Varieties" pictures

*Print expected revenue

*Re-do menu

*Make people follow roads

*Make houses know about their people
	-(change "int freePerson[NUMRESOURCES]" to "PERSON *aWalker[NUMRESOURCES]"

*Draw lines to the people of a selected house

*/
