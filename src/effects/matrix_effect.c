/*
GH0ST: 2047 - Matrix Effect Implementation
*/

#include "matrix_effect.h"
#include "config.h"
#include "theme.h"
#include "types.h"
#include <stdio.h>

//------------------------------------------------------------------------------------
// Module Variables
//------------------------------------------------------------------------------------
static MatrixParticle matrixParticles[MAX_PARTICLES] = {0};

//------------------------------------------------------------------------------------
// Matrix Effect Functions Implementation
//------------------------------------------------------------------------------------
void InitMatrixEffect(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        matrixParticles[i].position.x = GetRandomValue(0, SCREEN_WIDTH);
        matrixParticles[i].position.y = GetRandomValue(-SCREEN_HEIGHT, 0);
        matrixParticles[i].speed = GetRandomValue(100, 300);
        matrixParticles[i].alpha = GetRandomValue(50, 200) / 255.0f;
        
        // Random binary digits
        int randNum = GetRandomValue(0, 255);
        sprintf(matrixParticles[i].text, "%d", randNum % 2);
    }
}

void UpdateMatrixEffect(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        matrixParticles[i].position.y += matrixParticles[i].speed * GetFrameTime();
        
        if (matrixParticles[i].position.y > SCREEN_HEIGHT)
        {
            matrixParticles[i].position.x = GetRandomValue(0, SCREEN_WIDTH);
            matrixParticles[i].position.y = -20;
            matrixParticles[i].speed = GetRandomValue(100, 300);
        }
    }
}

void DrawMatrixEffect(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        Color color = ColorAlpha(COLOR_CYBER_GREEN, matrixParticles[i].alpha * 0.3f);
        DrawText(matrixParticles[i].text, 
                 matrixParticles[i].position.x, 
                 matrixParticles[i].position.y, 
                 10, color);
    }
}
