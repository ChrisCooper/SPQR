/*    GLOBAL DECLARATIONS.H

This header file is included only once, in SPQR.cpp

*/


#include "Definitions.h" //All structure definitions, #defines, #includes, and ENUMs


/*			Timers
Each "Counter" is the actual timer, the "Checks"
ensure that the logic does not excecute more than
once per increment
*/
volatile long speedCounter = 0;        	//For input v.s. printing
volatile long speedCounterCheck = 0;
volatile long economyCounter = 0;         //For updating resources, housing, people, etc.
volatile long economyCounterCheck = 0;
volatile long animateCounter = 0;         //For animating people
volatile long animateCounterCheck = 0;
volatile long updateCounter = 0;          //For updating and sorting people
volatile long updateCounterCheck = 0;


/*BITMAPs and BUTTONs*/
BITMAP *screenBuffer;                     //Bitmap for double buffering
BITMAP *mousePic;
BITMAP *selector;                         //Highlights the selected square
BITMAP *HUD;
BITMAP *loadBackground;
BITMAP *personPic[ANIMATIONFRAMES];
BITMAP *resourceIcon[NUMRESOURCES];
BUTTON resourceButton[NUMHUDBUTTONS];
BUTTON menuButton;
BUTTON buildingButton;
BUTTON personButton;
BITMAP *placmentOutline;                  //Highlights the current icon

FILE *infoFile;


/*Malloc-ed structs*/
SQUARE **aMapSquare;                      //Pointer to an array of pointers to arrays of SQUAREs
OBJECT *anObject;                         //Pointer to the first OBJECT
PERSON *personHeader = NULL;              //Pointer to the first PERSON in the linked list
PERSON personMould;

/*Gameplay variables*/
int inGame = 0;                           //Whether the actual SPQR game is in progress
int placement = EMPTY;                    //What OBJECT is selected for placeing on the map
int selectionX = 0;
int selectionY = 0;
int squareWidth = 56;                     //The pixel width of one square (height is squareWidth / 2)
int mapWidth = 30;                        //The number of SQUAREs to a side of the map
int money = 1000;
int population = 0;
int cameraX = 400;                        //The X offest for printing squares and getting input
int cameraY = 0;                          //The Y offest for printing squares and getting input
int mouseX;                               //Pixel X locations of the mouse
int mouseY;                               //Pixel Y locations of the mouse
int mouseGridX = 0;                       //Grid X locationso f the mouse
int mouseGridY = 0;                       //Grid Y locations of the mouse
int mouseClick[2];                        //True or false if each mouse button has been clicked once
int mouseDown[2];                         //True or false if each button is down
int totalWalkers = 0;                     //Number of PERSONs walking on the map
int totalProducers[NUMRESOURCES];         //Number of producers of each resource
int showPersonInfo = 0;                   //Whether or not to show each PERSON's cargo/cargoIndex
int showBuildingInfo = 0;                 //Whether or not to show histograms
int resourceColour[NUMRESOURCES];         //The colour associated with each resource
int loadingPhase = 0;                     //What phase of loading the game is on
int loadingPhases = 25;                   //Total loading phases
int totalBuildings = 0;                   //Number of non-house buildings on the map
int employees = 0;
int employeesNeeded = 0;
int convertToResource[21];
int moneyNotDone = 0;                     //Functions as a checker for a non-timered uptate
int IDcounter = 0;



SAMPLE *ambientSound[NUMABIENTSOUNDS];    //Array of random sounds to be played

/*Text*/
FONT *font70;                             //A 70-pt font
FONT *font15;                             //A 15-pt font
PALETTE palette;                          //A colour-set for loading fonts

SAMPLE *buildingSound[NUMRESOURCES];      //The sounds played when each building is built
