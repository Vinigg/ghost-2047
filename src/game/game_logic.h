/*
GH0ST: 2047 - Game Logic Module
Core game rules and mechanics
*/

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "types.h"
#include "game/logic_questions.h"

//------------------------------------------------------------------------------------
// Game Logic Functions
//------------------------------------------------------------------------------------
void Game_StartNew(GameState* state, DifficultyLevel difficulty);
void Game_ProcessGuess(GameState* state, Statistics* stats, int guess, GameScreen* currentScreen);
void Game_AnswerQuestion(GameState* state, int selectedOption, LogicQuestion* currentQuestion);
AlertLevel Game_GetAlertLevel(int attempts);
Color Game_GetAlertColor(AlertLevel level);
const char* Game_GetAlertText(AlertLevel level);
DetailedSession Game_CreateDetailedSession(const GameState* state);

#endif // GAME_LOGIC_H
