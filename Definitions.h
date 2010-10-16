/*		DEFINITIONS.H

This header is included in ALL other files, it contains the include files, as well as all
definitions of structures, ENUMs, and #define statements.

*/

#include <stdio.h>               //Standard functions prototypes
#include <string.h>              //Prototypes related to strings
#include <allegro.h>             //Allegro MACROs, defines, and prototypes
#include <algif.h>               //Used for support of gif files

#define NUMRESOURCES 12          //Total number of resources
#define MAXPICS 16               //Maximum varieties of one object
#define ANIMATIONFRAMES 6        //Frames of person animations
#define NUMABIENTSOUNDS 8        //Maximum ambient sounds to be randomized from
#define NUMHUDBUTTONS 14         // Number of buttons on the HUD

#define SQUARE struct square     //Makes referring to the structure type "stuct square" more obvious
#define OBJECT struct object     //Makes referring to the structure type "stuct object" more obvious
#define PERSON struct person     //Makes referring to the structure type "stuct person" more obvious
#define BUTTON struct button     //Makes referring to the structure type "stuct square" more obvious

/*
SELECTIVE DECLARATIONS
The following two lines of code are used to prevent multiple definitions and ENUMS
*/
#ifndef SPQR_DEF                 //If SPQR_DEF has not been defined then...
#define SPQR_DEF                 //DefineS PQR_DEF

//Resources 0-11
enum resources{
	FOOD,
	WATER,
	ENTERTAINMENT,
	RELIGION,
	POTTERY,
	BARBER,
	FURNITURE,
	WINE,
	BATHING,
	SLAVES,
	OLIVE_OIL,
	EDUCATION
};

//Objects 0 - 20
enum squareTypes{
	EMPTY,
	DEBRIS,
	ROAD,
	MARKET,
	FARM,
	FOUNTAIN,
	THEATRE,
	TEMPLE,
	POTTERSHOP,
	BARBERSHOP,
	FURNITUREWORKSHOP,
	WINEPRESS,
	BATHHOUSE,
	SLAVETRADER,
	OLIVEPRESS,
	ACADEMY,
	TENT,
	COTTAGE,
	HOUSE,
	APARTMENT,
	PALACE
};

//Directions 0 - 3 (for moving people)
enum directions{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

//SQUARES
//Each structure containst he values that a tile needs to know about itself;
//i.e. how much of each resource it has, what its population is, etc.
SQUARE{
       int objectType, variety;              //Index of the corresponding OBJECT, which variety of picture
       int people;                           //Population or employees on this square
       int currentSupply[NUMRESOURCES];      //Current supply of all 12 resources
       int percentageFull[NUMRESOURCES];     //How much oiut of the maximum of each resource this square has
		 int supplyBarOffset;                  //Offsets histograms so they do not cover eachother - random value
       PERSON *walker[NUMRESOURCES];         //Whether or not there is a person available to send for each resource
		BITMAP *picture;                       //A pointer tothecorresponding object's picture
};

//OBJECTS
//Each object is a template definition for the different possible contents of a tile, in a stucture format.
//When checking the attributes of the tile contents (ex. a FARM), this list is referred to.
OBJECT{
       BITMAP *picture[MAXPICS];          //A picture for each variety of a single OBJECT
       int printOffset;                   //How tall the picture is - printing starts at top left of all pictures
       int cost, varieties;               //Cost to build, number of varieties of this OBJECT
       int productionRate[NUMRESOURCES];  //How fast this building produces each resource
       int consumptionRate[NUMRESOURCES]; //How fast this building consumes each resource
       int maxSupply[NUMRESOURCES];       //How much of each resource this building can store
       int supplyThreshold[NUMRESOURCES]; //How low the supply of each resource must drop before a person will be sent
       int maxPeople;                     //Maximum population or employees for this square
       char name[32], blurb[250];         //The name and description of this OBJECT
       int products;								//an int where each bit acts as a boolean representing
															//whether or not this OBJECT produces a certain resource
};

//PEOPLE
//People are the transporters of goods that wander the town. They are stored in a linked list, sorted in the
//order they should be printed in (from left to right, top to bottom). Their structure contains
//information about their location, cargo, etc.
PERSON{
       int x, y, height, gridX, gridY;                   //The persons location in pixels and grid coordinates
       int homeX, homeY, nextX, nextY, destinX, destinY; //The person's house location, square to walk to, final destination
       BITMAP *picture[ANIMATIONFRAMES];                 //The picture for each frame of animation
		int animationCounter;                              //What frame of animation the person is on
       int cargo;                                        //How much the person is carrying
		 int cargoIndex;                                   //What type of resource the person is carrying
		int homeComing, hasDestination;                    //If the person has enough resource, and if they are going somewhere
		int ID;
		PERSON *next, *previous;                           //Pointers to the next and previous people in the list
};

//Buttons for HUD
BUTTON{
		int top, left, width, height;                      //Dimensions and location of the button
};

#endif                                                   //End of selective declarations
