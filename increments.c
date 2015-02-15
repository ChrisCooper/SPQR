/*  INCREMENTS.cpp

This contains the functions passed to timer routins.
All that they do is increment the selected variable

*/

#include "definitions.h" //All structure definitions, #defines, #includes, and ENUMs
#include "globals.h"     //Includes all global variables declared as "extern"

extern volatile long fireCounter;

void incrementTimer()
{
  speedCounter++;
}
void incrementEconomyCounter()
{
  economyCounter++;
}
void incrementAnimateCounter()
{
  animateCounter++;
}
void incrementUpdateCounter()
{
  updateCounter++;
}
void incrementFireCounter()
{
  fireCounter++;
}

