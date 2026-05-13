/*
GH0ST: 2047 - Game Logic Module
Core game rules and mechanics
*/

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "types.h"

//------------------------------------------------------------------------------------
// Game Logic Functions
//------------------------------------------------------------------------------------
void Game_StartNew(GameState* state);
void Game_ProcessGuess(GameState* state, Statistics* stats, int guess, GameScreen* currentScreen);
AlertLevel Game_GetAlertLevel(int attempts);
Color Game_GetAlertColor(AlertLevel level);
const char* Game_GetAlertText(AlertLevel level);

#endif // GAME_LOGIC_H
