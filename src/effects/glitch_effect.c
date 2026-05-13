/*
GH0ST: 2047 - Glitch Effect Implementation
*/

#include "glitch_effect.h"
#include "config.h"
#include "theme.h"
#include "types.h"

//------------------------------------------------------------------------------------
// Module Variables
//------------------------------------------------------------------------------------
static GlitchParticle glitchParticles[MAX_PARTICLES] = {0};

//------------------------------------------------------------------------------------
// Glitch Effect Functions Implementation
//------------------------------------------------------------------------------------
void InitGlitchEffect(bool won)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        glitchParticles[i].position.x = SCREEN_WIDTH / 2;
        glitchParticles[i].position.y = SCREEN_HEIGHT / 2;
        glitchParticles[i].velocity.x = GetRandomValue(-200, 200);
        glitchParticles[i].velocity.y = GetRandomValue(-200, 200);
        glitchParticles[i].lifetime = 0.0f;
        glitchParticles[i].maxLifetime = GetRandomValue(5, 15) / 10.0f;
        glitchParticles[i].color = won ? COLOR_CYBER_GREEN : COLOR_DANGER_RED;
    }
}

void UpdateGlitchEffect(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        glitchParticles[i].lifetime += GetFrameTime();
        
        if (glitchParticles[i].lifetime < glitchParticles[i].maxLifetime)
        {
            glitchParticles[i].position.x += glitchParticles[i].velocity.x * GetFrameTime();
            glitchParticles[i].position.y += glitchParticles[i].velocity.y * GetFrameTime();
        }
    }
}

void DrawGlitchEffect(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (glitchParticles[i].lifetime < glitchParticles[i].maxLifetime)
        {
            float progress = glitchParticles[i].lifetime / glitchParticles[i].maxLifetime;
            float alpha = 1.0f - progress;
            
            Color color = ColorAlpha(glitchParticles[i].color, alpha * 0.6f);
            DrawCircle(glitchParticles[i].position.x, glitchParticles[i].position.y, 3, color);
        }
    }
}
