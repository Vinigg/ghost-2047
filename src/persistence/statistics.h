/*
GH0ST: 2047 - Statistics Module
Statistics calculation and management
*/

#ifndef STATISTICS_H
#define STATISTICS_H

#include "types.h"
#include <stdbool.h>

//------------------------------------------------------------------------------------
// Statistics Functions
//------------------------------------------------------------------------------------
void Stats_Init(Statistics* stats);
void Stats_LoadFromCSV(Statistics* stats);
void Stats_AddSession(Statistics* stats, int attempts, bool won);
void Stats_Calculate(Statistics* stats);

#endif // STATISTICS_H
