/*
GH0ST: 2047 - UI Components Implementation
*/

#include "ui_components.h"
#include "theme.h"
#include <math.h>

//------------------------------------------------------------------------------------
// UI Component Functions Implementation
//------------------------------------------------------------------------------------
void UI_DrawGlowText(const char* text, int x, int y, int fontSize, Color color, float glowPulse)
{
    // Draw shadow/glow layers
    float glow = (sinf(glowPulse * 2.0f) + 1.0f) * 0.5f;
    
    for (int i = 3; i > 0; i--)
    {
        Color glowColor = ColorAlpha(color, glow * 0.2f * i);
        DrawText(text, x - i, y, fontSize, glowColor);
        DrawText(text, x + i, y, fontSize, glowColor);
        DrawText(text, x, y - i, fontSize, glowColor);
        DrawText(text, x, y + i, fontSize, glowColor);
    }
    
    // Draw main text
    DrawText(text, x, y, fontSize, color);
}

void UI_DrawCyberButton(const char* text, int x, int y, int width, int height, bool hover)
{
    Color btnColor = hover ? COLOR_CYBER_GREEN : COLOR_CYBER_GREEN_DARK;
    Color bgColor = hover ? ColorAlpha(COLOR_CYBER_GREEN, 0.2f) : ColorAlpha(COLOR_CYBER_GREEN, 0.05f);
    
    DrawRectangle(x, y, width, height, bgColor);
    DrawRectangleLines(x, y, width, height, btnColor);
    
    if (hover)
    {
        DrawRectangleLines(x - 2, y - 2, width + 4, height + 4, btnColor);
    }
    
    int textWidth = MeasureText(text, 18);
    DrawText(text, x + (width - textWidth) / 2, y + (height - 18) / 2, 18, btnColor);
}

void UI_DrawAlertBox(int x, int y, int width, int height)
{
    DrawRectangle(x, y, width, height, COLOR_BG_BLACK);
    DrawRectangleLines(x, y, width, height, COLOR_CYBER_GREEN_DIM);
}
