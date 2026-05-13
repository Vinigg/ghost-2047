/*
GH0ST: 2047 - Glitch Effect Module
Handles particle explosion effects on result screen
*/

#ifndef GLITCH_EFFECT_H
#define GLITCH_EFFECT_H

#include "raylib.h"
#include <stdbool.h>

//------------------------------------------------------------------------------------
// Glitch Effect Functions
//------------------------------------------------------------------------------------
void InitGlitchEffect(bool won);
void UpdateGlitchEffect(void);
void DrawGlitchEffect(void);

#endif // GLITCH_EFFECT_H
