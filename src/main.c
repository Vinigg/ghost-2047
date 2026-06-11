/*
GH0ST: 2047 - Neural Guessing System
A cyberpunk-themed number guessing game built with Raylib
REFACTORED: Modular architecture
*/

#include "raylib.h"
#include "resource_dir.h"
#include "config.h"
#include "types.h"
#include "theme.h"
#include "effects/matrix_effect.h"
#include "effects/glitch_effect.h"
#include "ui/screens.h"
#include "game/game_logic.h"
#include "game/logic_questions.h"
#include "audio/music_manager.h"
#include "persistence/statistics.h"
#include "persistence/session_logger.h"
#include <stdlib.h>
#include <time.h>

//------------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------------
static GameScreen currentScreen = SCREEN_MAIN_MENU;
static GameState gameState = {0};
static Statistics stats = {0};
static QuestionBank questionBank = {0};
static LogicQuestion* currentQuestion = NULL;
static DifficultyLevel currentDifficulty = DIFFICULTY_EASY;
static float glowPulse = 0.0f;
static RenderTexture2D virtualScreen = {0};

//------------------------------------------------------------------------------------
// Module Functions Declaration
//------------------------------------------------------------------------------------
static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UnloadGame(void);
static Vector2 GetVirtualMousePosition(void);

//------------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------------
int main(void)
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GH0ST: 2047 - Neural Guessing System");
    
    SearchAndSetResourceDir("resources");
    InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    srand(time(NULL));
    
    InitGame();
    
    while (!WindowShouldClose())
    {
        UpdateGame();
        DrawGame();
    }
    
    UnloadGame();
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}

//------------------------------------------------------------------------------------
// Core Game Functions
//------------------------------------------------------------------------------------
static void InitGame(void)
{
    InitMatrixEffect();
    Stats_Init(&stats);
    Stats_LoadFromCSV(&stats);
    SessionLogger_Init();
    QuestionBank_Load(&questionBank, QUESTIONS_FILE);
    currentScreen = SCREEN_MAIN_MENU;
    
    virtualScreen = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTextureFilter(virtualScreen.texture, TEXTURE_FILTER_BILINEAR);
    
    MusicManager_Init();
    
    // Initialize screen context
    ScreenContext screenCtx = {
        .currentScreen = &currentScreen,
        .gameState = &gameState,
        .stats = &stats,
        .questionBank = &questionBank,
        .currentQuestion = &currentQuestion,
        .currentDifficulty = &currentDifficulty,
        .glowPulse = &glowPulse,
        .getVirtualMousePosition = GetVirtualMousePosition
    };
    Screens_Init(screenCtx);
}

static void UpdateGame(void)
{
    glowPulse += GetFrameTime();
    UpdateMatrixEffect();
    UpdateGlitchEffect();
    MusicManager_Update();
    
    if (IsKeyPressed(KEY_F11))
    {
        ToggleFullscreen();
    }
    
    switch (currentScreen)
    {
        case SCREEN_MAIN_MENU:      Screen_UpdateMainMenu(); break;
        case SCREEN_TUTORIAL:       Screen_UpdateTutorial(); break;
        case SCREEN_DIFFICULTY_SELECT: Screen_UpdateDifficultySelect(); break;
        case SCREEN_GAME:           Screen_UpdateGame(); break;
        case SCREEN_RESULT:         Screen_UpdateResult(); break;
        case SCREEN_STATS:          Screen_UpdateStats(); break;
    }
}

static void DrawGame(void)
{
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    
    float scaleX = (float)windowWidth / SCREEN_WIDTH;
    float scaleY = (float)windowHeight / SCREEN_HEIGHT;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    
    int drawWidth = (int)(SCREEN_WIDTH * scale);
    int drawHeight = (int)(SCREEN_HEIGHT * scale);
    int offsetX = (windowWidth - drawWidth) / 2;
    int offsetY = (windowHeight - drawHeight) / 2;
    
    BeginTextureMode(virtualScreen);
    ClearBackground(COLOR_BG_BLACK);
    
    switch (currentScreen)
    {
        case SCREEN_MAIN_MENU:      Screen_DrawMainMenu(); break;
        case SCREEN_TUTORIAL:       Screen_DrawTutorial(); break;
        case SCREEN_DIFFICULTY_SELECT: Screen_DrawDifficultySelect(); break;
        case SCREEN_GAME:           Screen_DrawGame(); break;
        case SCREEN_RESULT:         Screen_DrawResult(); break;
        case SCREEN_STATS:          Screen_DrawStats(); break;
    }
    
    EndTextureMode();
    
    BeginDrawing();
    ClearBackground(BLACK);
    
    Rectangle source = {0, 0, (float)virtualScreen.texture.width, -(float)virtualScreen.texture.height};
    Rectangle dest = {(float)offsetX, (float)offsetY, (float)drawWidth, (float)drawHeight};
    DrawTexturePro(virtualScreen.texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
    
    EndDrawing();
}

static void UnloadGame(void)
{
    QuestionBank_Unload(&questionBank);
    UnloadRenderTexture(virtualScreen);
    MusicManager_Unload();
}

//------------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------------
static Vector2 GetVirtualMousePosition(void)
{
    Vector2 mousePos = GetMousePosition();
    
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    
    float scaleX = (float)windowWidth / SCREEN_WIDTH;
    float scaleY = (float)windowHeight / SCREEN_HEIGHT;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    
    int drawWidth = (int)(SCREEN_WIDTH * scale);
    int drawHeight = (int)(SCREEN_HEIGHT * scale);
    int offsetX = (windowWidth - drawWidth) / 2;
    int offsetY = (windowHeight - drawHeight) / 2;
    
    Vector2 virtualMouse;
    virtualMouse.x = (mousePos.x - offsetX) / scale;
    virtualMouse.y = (mousePos.y - offsetY) / scale;
    
    return virtualMouse;
}

