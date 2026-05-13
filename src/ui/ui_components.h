/*
GH0ST: 2047 - UI Components Module
Reusable UI components for the cyberpunk interface
*/

#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "raylib.h"
#include <stdbool.h>

//------------------------------------------------------------------------------------
// UI Component Functions
//------------------------------------------------------------------------------------
void UI_DrawGlowText(const char* text, int x, int y, int fontSize, Color color, float glowPulse);
void UI_DrawCyberButton(const char* text, int x, int y, int width, int height, bool hover);
void UI_DrawAlertBox(int x, int y, int width, int height);

#endif // UI_COMPONENTS_H
