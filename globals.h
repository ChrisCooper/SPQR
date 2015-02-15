/*    GLOBALS.H

This header file is included in most files, and includes 
all global variables declared as "extern"

*/

#include "Definitions.h" //All structure definitions, #defines, #includes, and ENUMs

/*			Timers
Each "Counter" is the actual timer, the "Checks"
ensure that the logic does not excecute more than
once per increment
*/
extern volatile long speedCounter;        	//For input v.s. printing
extern volatile long speedCounterCheck;
extern volatile long economyCounter;         //For updating resources, housing, people, etc.
extern volatile long economyCounterCheck;
extern volatile long animateCounter;         //For animating people
extern volatile long animateCounterCheck;
extern volatile long updateCounter;          //For updating and sorting people
extern volatile long updateCounterCheck;
extern int moneyNotDone;                     //Functions as a checker for a non-timered uptate

/*BITMAPs and BUTTONs*/
extern BITMAP *screenBuffer;                     //Bitmap for double buffering
extern BITMAP *mousePic;
extern BITMAP *selector;                         //Highlights the selected square
extern BITMAP *HUD;
extern BITMAP *loadBackground;
extern BITMAP *personPic[ANIMATIONFRAMES];
extern BITMAP *resourceIcon[NUMRESOURCES];
extern BUTTON resourceButton[NUMHUDBUTTONS];
extern BUTTON menuButton;
extern BUTTON buildingButton;
extern BUTTON personButton;
extern BITMAP *placmentOutline;                  //Highlights the current icon
extern FILE *infoFile;


/*Malloc-ed structs*/
extern SQUARE **aMapSquare;                      //Pointer to an array of pointers to arrays of SQUAREs
extern OBJECT *anObject;                         //Pointer to the first OBJECT
extern PERSON *personHeader;              //Pointer to the first PERSON in the linked list
extern PERSON personMould;

/*Gameplay variables*/
extern int inGame;                           //Whether the actual SPQR game is in progress
extern int placement;                    //What OBJECT is selected for placeing on the map
extern int selectionX;
extern int selectionY;
extern int squareWidth;                     //The pixel width of one square (height is squareWidth / 2)
extern int mapWidth;                        //The number of SQUAREs to a side of the map
extern int money;
extern int population;
extern int cameraX;                        //The X offest for printing squares and getting input
extern int cameraY;                          //The Y offest for printing squares and getting input
extern int totalWalkers;                     //Number of PERSONs walking on the map
extern int totalProducers[NUMRESOURCES];         //Number of producers of each resource
extern int showPersonInfo;                   //Whether or not to show each PERSON's cargo/cargoIndex
extern int showBuildingInfo;                 //Whether or not to show histograms
extern int resourceColour[NUMRESOURCES];         //The colour associated with each resource
extern int loadingPhase;                     //What phase of loading the game is on
extern int loadingPhases;                   //Total loading phases
extern int totalBuildings;                   //Number of non-house buildings on the map
extern int employees;
extern int employeesNeeded;
extern int convertToResource[21];
extern int IDcounter;

 /*mouse variables*/
extern int mouseX;                               //Pixel X locations
extern int mouseY;                               //Pixel Y locations
extern int mouseGridX;                       //Grid X locations
extern int mouseGridY;                       //Grid Y locations
extern int mouseClick[2];                        //True or false if each mouse button has been clicked once
extern int mouseDown[2];                         //True or false if each button is down

/*Sounds*/
extern SAMPLE *ambientSound[NUMABIENTSOUNDS];    //Array of random sounds to be played
extern SAMPLE *buildingSound[NUMRESOURCES];      //The sounds played when each building is built

/*Text*/
extern FONT *font70;                             //A 70-pt font
extern FONT *font15;                             //A 15-pt font
extern PALETTE palette;                          //A colour-set for loading fonts
