/*
GH0ST: 2047 - Screens Module
Update and Draw functions for each game screen
*/

#ifndef SCREENS_H
#define SCREENS_H

#include "types.h"
#include "game/logic_questions.h"
#include "persistence/statistics.h"

//------------------------------------------------------------------------------------
// Shared Screen State (set by main before calling screen functions)
//------------------------------------------------------------------------------------
typedef struct {
    GameScreen* currentScreen;
    GameState* gameState;
    Statistics* stats;
    QuestionBank* questionBank;
    LogicQuestion** currentQuestion;
    DifficultyLevel* currentDifficulty;
    float* glowPulse;
    Vector2 (*getVirtualMousePosition)(void);
} ScreenContext;

//------------------------------------------------------------------------------------
// Screen Functions
//------------------------------------------------------------------------------------
void Screens_Init(ScreenContext ctx);

void Screen_UpdateMainMenu(void);
void Screen_DrawMainMenu(void);

void Screen_UpdateDifficultySelect(void);
void Screen_DrawDifficultySelect(void);

void Screen_UpdateGame(void);
void Screen_DrawGame(void);

void Screen_UpdateResult(void);
void Screen_DrawResult(void);

void Screen_UpdateStats(void);
void Screen_DrawStats(void);

#endif // SCREENS_H
