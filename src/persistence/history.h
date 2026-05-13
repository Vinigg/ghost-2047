/*
GH0ST: 2047 - History Module
File persistence for game statistics
*/

#ifndef HISTORY_H
#define HISTORY_H

#include "types.h"
#include <stdbool.h>

//------------------------------------------------------------------------------------
// History Functions
//------------------------------------------------------------------------------------
bool History_Save(const Statistics* stats);
bool History_Load(Statistics* stats);

#endif // HISTORY_H
