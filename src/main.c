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
#include "ui/ui_components.h"
#include "game/game_logic.h"
#include "game/logic_questions.h"
#include "persistence/statistics.h"
#include "persistence/session_logger.h"
#include "persistence/history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//------------------------------------------------------------------------------------
// Global Variables (to be reduced in future refactoring)
//------------------------------------------------------------------------------------
static GameScreen currentScreen = SCREEN_MAIN_MENU;
static GameState gameState = {0};
static Statistics stats = {0};
static QuestionBank questionBank = {0};
static LogicQuestion* currentQuestion = NULL;
static DifficultyLevel currentDifficulty = DIFFICULTY_EASY;
static float glowPulse = 0.0f;
static bool showLogs = false;
static float logTimer = 0.0f;
static int currentLog = 0;
static RenderTexture2D virtualScreen = {0};  // Virtual resolution render target

// Music system
static Music musicMenu = {0};
static Music musicGame1 = {0};  // First 60s or until 4 attempts
static Music musicGame2 = {0};  // From 5th attempt onwards or after 60s + result screen
static Music* currentMusic = NULL;
static int currentMusicTrack = 0;  // 0=none, 1=menu, 2=game1, 3=game2

static const char* systemLogs[MAX_LOGS] = {
    "> INITIALIZING GHOST PROTOCOL...",
    "> LOADING NEURAL NETWORK...",
    "> PARSING HISTORY FILE...",
    "> TIMESTAMP: 2047-04-23 03:42:17",
    "> SENHA NUMERICA: GERADA",
    "> TENTATIVAS MAXIMAS: 7",
    "> SISTEMA DE ALERTA: ATIVO",
    "> READY FOR INTRUSION."
};

//------------------------------------------------------------------------------------
// Module Functions Declaration
//------------------------------------------------------------------------------------
static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UnloadGame(void);

// Helper function to convert mouse position from window to virtual coordinates
static Vector2 GetVirtualMousePosition(void);

// Screen specific functions
static void UpdateMainMenu(void);
static void DrawMainMenu(void);
static void UpdateDifficultySelect(void);
static void DrawDifficultySelect(void);
static void UpdateGameScreen(void);
static void DrawGameScreen(void);
static void UpdateResultScreen(void);
static void DrawResultScreen(void);
static void UpdateStatsScreen(void);
static void DrawStatsScreen(void);

//------------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------------
int main(void)
{
    // NOTE: Window starts at 1366x768 and is not resizable
    // Press F11 to toggle fullscreen mode
    // MSAA and HighDPI disabled for GPU compatibility
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GH0ST: 2047 - Neural Guessing System");
    
    SearchAndSetResourceDir("resources");
    InitAudioDevice();  // Initialize audio system
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);  // Disable ESC as exit key - game only closes via menu or X button
    srand(time(NULL));
    
    InitGame();
    
    // Main game loop
    while (!WindowShouldClose())
    {
        UpdateGame();
        DrawGame();
    }
    
    UnloadGame();
    CloseAudioDevice();  // Close audio system
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
    Stats_LoadFromCSV(&stats);  // Load from CSV (single source of truth)
    SessionLogger_Init();  // Initialize CSV logging
    QuestionBank_Load(&questionBank, QUESTIONS_FILE);
    currentScreen = SCREEN_MAIN_MENU;
    showLogs = true;
    logTimer = 0.0f;
    currentLog = 0;
    
    // Create virtual screen render texture for resolution scaling
    virtualScreen = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTextureFilter(virtualScreen.texture, TEXTURE_FILTER_BILINEAR);
    
    // Load music files
    musicMenu = LoadMusicStream("menu_music.mp3");
    musicGame1 = LoadMusicStream("game_music_1.mp3");
    musicGame2 = LoadMusicStream("game_music_2.mp3");
    
    // Configure looping
    if (musicMenu.stream.buffer != NULL) musicMenu.looping = true;
    if (musicGame1.stream.buffer != NULL) musicGame1.looping = true;  // Loops during first 60s/4 attempts
    if (musicGame2.stream.buffer != NULL) musicGame2.looping = true;
    
    // Start menu music
    if (musicMenu.stream.buffer != NULL)
    {
        PlayMusicStream(musicMenu);
        currentMusic = &musicMenu;
        currentMusicTrack = 1;
    }
}

static void UpdateGame(void)
{
    glowPulse += GetFrameTime();
    UpdateMatrixEffect();
    UpdateGlitchEffect();
    
    // Update music stream
    if (currentMusic != NULL)
    {
        UpdateMusicStream(*currentMusic);
    }
    
    // Fullscreen toggle
    if (IsKeyPressed(KEY_F11))
    {
        ToggleFullscreen();
    }
    
    switch (currentScreen)
    {
        case SCREEN_MAIN_MENU:
            UpdateMainMenu();
            break;
        case SCREEN_DIFFICULTY_SELECT:
            UpdateDifficultySelect();
            break;
        case SCREEN_GAME:
            UpdateGameScreen();
            break;
        case SCREEN_RESULT:
            UpdateResultScreen();
            break;
        case SCREEN_STATS:
            UpdateStatsScreen();
            break;
    }
}

static void DrawGame(void)
{
    // Get actual window size
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    
    // Calculate scaling to fit virtual resolution into window while maintaining aspect ratio
    float scaleX = (float)windowWidth / SCREEN_WIDTH;
    float scaleY = (float)windowHeight / SCREEN_HEIGHT;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    
    // Calculate position to center the scaled game
    int drawWidth = (int)(SCREEN_WIDTH * scale);
    int drawHeight = (int)(SCREEN_HEIGHT * scale);
    int offsetX = (windowWidth - drawWidth) / 2;
    int offsetY = (windowHeight - drawHeight) / 2;
    
    // Render game to virtual screen texture
    BeginTextureMode(virtualScreen);
    ClearBackground(COLOR_BG_BLACK);
    
    switch (currentScreen)
    {
        case SCREEN_MAIN_MENU:
            DrawMainMenu();
            break;
        case SCREEN_DIFFICULTY_SELECT:
            DrawDifficultySelect();
            break;
        case SCREEN_GAME:
            DrawGameScreen();
            break;
        case SCREEN_RESULT:
            DrawResultScreen();
            break;
        case SCREEN_STATS:
            DrawStatsScreen();
            break;
    }
    
    EndTextureMode();
    
    // Draw virtual screen to actual window (scaled and centered)
    BeginDrawing();
    ClearBackground(BLACK);  // Letterbox bars are black
    
    // Draw the scaled texture (flip vertically because render textures are upside down)
    Rectangle source = {0, 0, (float)virtualScreen.texture.width, -(float)virtualScreen.texture.height};
    Rectangle dest = {(float)offsetX, (float)offsetY, (float)drawWidth, (float)drawHeight};
    DrawTexturePro(virtualScreen.texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
    
    EndDrawing();
}

static void UnloadGame(void)
{
    QuestionBank_Unload(&questionBank);
    UnloadRenderTexture(virtualScreen);  // Clean up virtual screen texture
    
    // Unload music
    if (currentMusic != NULL) StopMusicStream(*currentMusic);
    UnloadMusicStream(musicMenu);
    UnloadMusicStream(musicGame1);
    UnloadMusicStream(musicGame2);
}

//------------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------------
static Vector2 GetVirtualMousePosition(void)
{
    // Get actual mouse position in window
    Vector2 mousePos = GetMousePosition();
    
    // Get actual window size
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    
    // Calculate same scaling as DrawGame()
    float scaleX = (float)windowWidth / SCREEN_WIDTH;
    float scaleY = (float)windowHeight / SCREEN_HEIGHT;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    
    // Calculate offset
    int drawWidth = (int)(SCREEN_WIDTH * scale);
    int drawHeight = (int)(SCREEN_HEIGHT * scale);
    int offsetX = (windowWidth - drawWidth) / 2;
    int offsetY = (windowHeight - drawHeight) / 2;
    
    // Convert mouse position from window coordinates to virtual coordinates
    Vector2 virtualMouse;
    virtualMouse.x = (mousePos.x - offsetX) / scale;
    virtualMouse.y = (mousePos.y - offsetY) / scale;
    
    return virtualMouse;
}

static void SwitchMusic(Music* newMusic, int trackNumber)
{
    if (currentMusic != NULL && currentMusic != newMusic)
    {
        StopMusicStream(*currentMusic);
    }
    
    if (newMusic != NULL && newMusic->stream.buffer != NULL)
    {
        PlayMusicStream(*newMusic);
        currentMusic = newMusic;
        currentMusicTrack = trackNumber;
    }
}

//------------------------------------------------------------------------------------
// Main Menu Screen
//------------------------------------------------------------------------------------
static void UpdateMainMenu(void)
{
    // Switch to menu music if not already playing
    if (currentMusicTrack != 1)
    {
        SwitchMusic(&musicMenu, 1);
    }
    
    // Animate system logs
    if (showLogs && currentLog < MAX_LOGS)
    {
        logTimer += GetFrameTime();
        if (logTimer >= 0.3f)
        {
            currentLog++;
            logTimer = 0.0f;
            if (currentLog >= MAX_LOGS)
            {
                showLogs = false;
            }
        }
    }
    
    Vector2 mousePos = GetVirtualMousePosition();
    
    // Button: Start Game
    Rectangle btnStart = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 100, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentScreen = SCREEN_DIFFICULTY_SELECT;
    }
    
    // Button: Statistics
    Rectangle btnStats = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 170, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnStats) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Stats_Calculate(&stats);
        currentScreen = SCREEN_STATS;
    }
    
    // Button: Quit
    Rectangle btnQuit = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 240, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnQuit) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        History_Save(&stats);
        CloseWindow();
    }
}

static void DrawMainMenu(void)
{
    DrawMatrixEffect();
    
    Vector2 mousePos = GetVirtualMousePosition();
    
    // Title with glow effect
    UI_DrawGlowText("GH0ST: 2047", SCREEN_WIDTH/2 - 220, 100, 64, COLOR_CYBER_GREEN, glowPulse);
    
    // Subtitle
    const char* subtitle = "NEURAL GUESSING SYSTEM v3.14";
    int subtitleWidth = MeasureText(subtitle, 14);
    DrawText(subtitle, SCREEN_WIDTH/2 - subtitleWidth/2, 180, 14, COLOR_CYBER_GREEN_DARK);
    
    // Warning message
    const char* warning = "WARNING: 7 TENTATIVAS ANTES DO BLOQUEIO TOTAL";
    int warningWidth = MeasureText(warning, 12);
    float warningAlpha = (sinf(glowPulse * 2.0f) + 1.0f) * 0.5f;
    Color warningColor = ColorAlpha(COLOR_DANGER_RED, warningAlpha);
    DrawText(warning, SCREEN_WIDTH/2 - warningWidth/2, 210, 12, warningColor);
    
    // System logs box
    int logBoxY = 260;
    DrawRectangle(SCREEN_WIDTH/2 - 400, logBoxY, 800, 240, COLOR_BG_BLACK);
    DrawRectangleLines(SCREEN_WIDTH/2 - 400, logBoxY, 800, 240, COLOR_CYBER_GREEN_DIM);
    
    DrawText("SYSTEM_LOG.TXT", SCREEN_WIDTH/2 - 380, logBoxY + 15, 12, COLOR_CYBER_GREEN);
    
    for (int i = 0; i < currentLog && i < MAX_LOGS; i++)
    {
        DrawText(systemLogs[i], SCREEN_WIDTH/2 - 380, logBoxY + 50 + i * 20, 12, COLOR_CYBER_GREEN);
    }
    
    // Buttons
    Rectangle btnStart = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 100, 300, 50};
    bool hoverStart = CheckCollisionPointRec(mousePos, btnStart);
    UI_DrawCyberButton(">> INICIAR MISSAO", btnStart.x, btnStart.y, btnStart.width, btnStart.height, hoverStart);
    
    Rectangle btnStats = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 170, 300, 50};
    bool hoverStats = CheckCollisionPointRec(mousePos, btnStats);
    UI_DrawCyberButton(">> ESTATISTICAS", btnStats.x, btnStats.y, btnStats.width, btnStats.height, hoverStats);

    Rectangle btnQuit = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 240, 300, 50};
    bool hoverQuit = CheckCollisionPointRec(mousePos, btnQuit);
    UI_DrawCyberButton(">> ENCERRAR SISTEMA", btnQuit.x, btnQuit.y, btnQuit.width, btnQuit.height, hoverQuit);
    
    // Fullscreen hint
    DrawText("[F11] TELA CHEIA", SCREEN_WIDTH - 150, SCREEN_HEIGHT - 30, 12, COLOR_CYBER_GREEN_DIM);
}

//------------------------------------------------------------------------------------
// Difficulty Selection Screen
//------------------------------------------------------------------------------------
static void UpdateDifficultySelect(void)
{
    // Keep menu music playing
    if (currentMusicTrack != 1)
    {
        SwitchMusic(&musicMenu, 1);
    }
    
    Vector2 mousePos = GetVirtualMousePosition();
    
    // Button: Easy Mode
    Rectangle btnEasy = {SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 - 20, 320, 60};
    if (CheckCollisionPointRec(mousePos, btnEasy) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentDifficulty = DIFFICULTY_EASY;
        QuestionBank_Unload(&questionBank);
        QuestionBank_Load(&questionBank, QUESTIONS_FILE_EASY);
        Game_StartNew(&gameState, currentDifficulty);
        currentQuestion = QuestionBank_GetRandom(&questionBank);
        currentScreen = SCREEN_GAME;
    }
    
    // Button: Hard Mode
    Rectangle btnHard = {SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 + 60, 320, 60};
    if (CheckCollisionPointRec(mousePos, btnHard) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentDifficulty = DIFFICULTY_HARD;
        QuestionBank_Unload(&questionBank);
        QuestionBank_Load(&questionBank, QUESTIONS_FILE_HARD);
        Game_StartNew(&gameState, currentDifficulty);
        currentQuestion = QuestionBank_GetRandom(&questionBank);
        currentScreen = SCREEN_GAME;
    }
    
    // ESC to return to menu
    if (IsKeyPressed(KEY_ESCAPE))
    {
        currentScreen = SCREEN_MAIN_MENU;
    }
}

static void DrawDifficultySelect(void)
{
    DrawMatrixEffect();
    
    Vector2 mousePos = GetVirtualMousePosition();
    
    // Title
    UI_DrawGlowText("SELECIONE A DIFICULDADE", SCREEN_WIDTH/2 - 320, 100, 48, COLOR_CYBER_GREEN, glowPulse);
    
    // Subtitle
    const char* subtitle = "ESCOLHA O NIVEL DE DESAFIO PARA SUA MISSAO";
    int subtitleWidth = MeasureText(subtitle, 14);
    DrawText(subtitle, SCREEN_WIDTH/2 - subtitleWidth/2, 170, 14, COLOR_CYBER_GREEN_DARK);
    
    // Easy Mode Button
    Rectangle btnEasy = {SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 - 20, 320, 60};
    bool hoverEasy = CheckCollisionPointRec(mousePos, btnEasy);
    
    Color easyBorderColor = hoverEasy ? COLOR_CYBER_GREEN : COLOR_CYBER_GREEN_DIM;
    DrawRectangle(btnEasy.x, btnEasy.y, btnEasy.width, btnEasy.height, ColorAlpha(COLOR_CYBER_GREEN, hoverEasy ? 0.15f : 0.05f));
    DrawRectangleLines(btnEasy.x, btnEasy.y, btnEasy.width, btnEasy.height, easyBorderColor);
    DrawRectangleLines(btnEasy.x + 2, btnEasy.y + 2, btnEasy.width - 4, btnEasy.height - 4, ColorAlpha(easyBorderColor, 0.5f));
    
    const char* easyLabel = ">> MODO FACIL";
    int easyLabelWidth = MeasureText(easyLabel, 24);
    DrawText(easyLabel, SCREEN_WIDTH/2 - easyLabelWidth/2, btnEasy.y + 18, 24, COLOR_CYBER_GREEN);
    
    // Easy mode description
    const char* easyDesc = "Questoes de logica basicas | 2 minutos";
    int easyDescWidth = MeasureText(easyDesc, 12);
    DrawText(easyDesc, SCREEN_WIDTH/2 - easyDescWidth/2, btnEasy.y + btnEasy.height + 10, 12, COLOR_CYBER_GREEN_DIM);
    
    // Hard Mode Button
    Rectangle btnHard = {SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 + 60, 320, 60};
    bool hoverHard = CheckCollisionPointRec(mousePos, btnHard);
    
    Color hardBorderColor = hoverHard ? COLOR_DANGER_RED : COLOR_ALERT_YELLOW;
    DrawRectangle(btnHard.x, btnHard.y, btnHard.width, btnHard.height, ColorAlpha(COLOR_DANGER_RED, hoverHard ? 0.15f : 0.05f));
    DrawRectangleLines(btnHard.x, btnHard.y, btnHard.width, btnHard.height, hardBorderColor);
    DrawRectangleLines(btnHard.x + 2, btnHard.y + 2, btnHard.width - 4, btnHard.height - 4, ColorAlpha(hardBorderColor, 0.5f));
    
    const char* hardLabel = ">> MODO DIFICIL";
    int hardLabelWidth = MeasureText(hardLabel, 24);
    DrawText(hardLabel, SCREEN_WIDTH/2 - hardLabelWidth/2, btnHard.y + 18, 24, COLOR_ALERT_YELLOW);
    
    // Hard mode description
    const char* hardDesc = "Questoes de logica avancadas | 2 minutos";
    int hardDescWidth = MeasureText(hardDesc, 12);
    DrawText(hardDesc, SCREEN_WIDTH/2 - hardDescWidth/2, btnHard.y + btnHard.height + 10, 12, COLOR_CYBER_GREEN_DIM);
    
    // Instructions
    DrawText("PRESSIONE ESC PARA VOLTAR", SCREEN_WIDTH/2 - 130, SCREEN_HEIGHT - 50, 14, COLOR_CYBER_GREEN_DIM);
    
    // Fullscreen hint
    DrawText("[F11] TELA CHEIA", SCREEN_WIDTH - 150, SCREEN_HEIGHT - 30, 12, COLOR_CYBER_GREEN_DIM);
}

//------------------------------------------------------------------------------------
// Game Screen
//------------------------------------------------------------------------------------
static void UpdateGameScreen(void)
{
    if (gameState.gameOver) return;
    
    // Update game timer (runs continuously, even during questions)
    gameState.gameTimer -= GetFrameTime();
    if (gameState.gameTimer <= 0.0f)
    {
        gameState.gameTimer = 0.0f;
        gameState.timedOut = true;
        gameState.gameOver = true;
        gameState.won = false;
        
        // Log session
        Stats_AddSession(&stats, gameState.attempts, false);
        DetailedSession detailedSession = Game_CreateDetailedSession(&gameState);
        SessionLogger_Append(&detailedSession);
        
        InitGlitchEffect(false);
        currentScreen = SCREEN_RESULT;
        return;
    }
    
    // Dynamic music switching based on time and attempts
    if (!gameState.gameOver)
    {
        float elapsedTime = gameState.maxGameTime - gameState.gameTimer;
        
        // Music 2: After 60s OR after 4 attempts (5th attempt onwards)
        if ((elapsedTime >= 60.0f || gameState.attempts >= 5) && currentMusicTrack != 3)
        {
            SwitchMusic(&musicGame2, 3);
        }
        // Music 1: Start of game (first 60s or first 4 attempts)
        else if (currentMusicTrack != 2 && currentMusicTrack != 3)
        {
            SwitchMusic(&musicGame1, 2);
        }
    }
    
    // Update error timer
    if (gameState.errorTimer > 0.0f)
    {
        gameState.errorTimer -= GetFrameTime();
    }
    
    // Handle question feedback timer (transition from ANSWERED -> IDLE)
    if (gameState.questionState == QUESTION_ANSWERED_CORRECT || 
        gameState.questionState == QUESTION_ANSWERED_WRONG)
    {
        gameState.questionFeedbackTimer -= GetFrameTime();
        if (gameState.questionFeedbackTimer <= 0.0f)
        {
            gameState.questionState = QUESTION_IDLE;
        }
        return; // Don't process other input during feedback
    }
    
    // Handle question showing state - player must answer before guessing
    if (gameState.questionState == QUESTION_SHOWING)
    {
        // Load a question if we don't have one
        if (currentQuestion == NULL && questionBank.loaded)
        {
            currentQuestion = QuestionBank_GetRandom(&questionBank);
        }
        
        // If no questions available (file didn't load), skip to idle with hint unlocked
        if (!questionBank.loaded)
        {
            gameState.hintUnlocked = true;
            gameState.questionState = QUESTION_IDLE;
            return;
        }
        
        // Handle keyboard input (1-4 keys)
        if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1))
        {
            Game_AnswerQuestion(&gameState, 0, currentQuestion);
            currentQuestion = NULL;
        }
        else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2))
        {
            Game_AnswerQuestion(&gameState, 1, currentQuestion);
            currentQuestion = NULL;
        }
        else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3))
        {
            Game_AnswerQuestion(&gameState, 2, currentQuestion);
            currentQuestion = NULL;
        }
        else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4))
        {
            Game_AnswerQuestion(&gameState, 3, currentQuestion);
            currentQuestion = NULL;
        }
        
        // Handle mouse clicks on option buttons
        Vector2 mousePos = GetVirtualMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentQuestion != NULL)
        {
            int overlayX = SCREEN_WIDTH/2 - 350;
            int overlayY = SCREEN_HEIGHT/2 - 200;
            
            for (int i = 0; i < 4; i++)
            {
                Rectangle optBtn = {overlayX + 30, overlayY + 120 + i * 60, 640, 45};
                if (CheckCollisionPointRec(mousePos, optBtn))
                {
                    Game_AnswerQuestion(&gameState, i, currentQuestion);
                    currentQuestion = NULL;
                    break;
                }
            }
        }
        
        // ESC to return to menu (still works)
        if (IsKeyPressed(KEY_ESCAPE))
        {
            currentQuestion = NULL;
            currentScreen = SCREEN_MAIN_MENU;
        }
        return; // Block number input while question is showing
    }
    
    // QUESTION_IDLE: Normal game input
    // Handle text input for numbers
    int key = GetCharPressed();
    while (key > 0)
    {
        if ((key >= '0' && key <= '9') && strlen(gameState.inputBuffer) < MAX_INPUT_LENGTH)
        {
            int len = strlen(gameState.inputBuffer);
            gameState.inputBuffer[len] = (char)key;
            gameState.inputBuffer[len + 1] = '\0';
        }
        key = GetCharPressed();
    }
    
    // Handle backspace
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        int len = strlen(gameState.inputBuffer);
        if (len > 0)
        {
            gameState.inputBuffer[len - 1] = '\0';
        }
    }
    
    // Handle Enter key to submit guess
    if (IsKeyPressed(KEY_ENTER) && strlen(gameState.inputBuffer) > 0)
    {
        int guess = atoi(gameState.inputBuffer);
        Game_ProcessGuess(&gameState, &stats, guess, &currentScreen);
        gameState.inputBuffer[0] = '\0';
    }
    
    // ESC to return to menu
    if (IsKeyPressed(KEY_ESCAPE))
    {
        currentScreen = SCREEN_MAIN_MENU;
    }
}

static void DrawGameScreen(void)
{
    DrawMatrixEffect();
    
    // Header
    UI_DrawGlowText("GH0ST: 2047", 50, 30, 32, COLOR_CYBER_GREEN, glowPulse);
    
    // Alert status
    Color alertColor = Game_GetAlertColor(gameState.alertLevel);
    const char* alertText = Game_GetAlertText(gameState.alertLevel);
    int alertWidth = MeasureText(alertText, 20);
    DrawRectangle(SCREEN_WIDTH - alertWidth - 80, 30, alertWidth + 60, 40, ColorAlpha(alertColor, 0.1f));
    DrawRectangleLines(SCREEN_WIDTH - alertWidth - 80, 30, alertWidth + 60, 40, alertColor);
    DrawText(alertText, SCREEN_WIDTH - alertWidth - 50, 42, 20, alertColor);
    
    // Timer display
    int timerMinutes = (int)gameState.gameTimer / 60;
    int timerSeconds = (int)gameState.gameTimer % 60;
    char timerText[16];
    sprintf(timerText, "TEMPO: %02d:%02d", timerMinutes, timerSeconds);
    
    // Timer color based on remaining time
    float timeRatio = gameState.gameTimer / gameState.maxGameTime;
    Color timerColor;
    if (timeRatio > 0.5f) timerColor = COLOR_CYBER_GREEN;
    else if (timeRatio > 0.25f) timerColor = COLOR_ALERT_YELLOW;
    else {
        float pulse = (sinf(glowPulse * 8.0f) + 1.0f) * 0.5f;
        timerColor = ColorAlpha(COLOR_DANGER_RED, 0.5f + pulse * 0.5f);
    }
    
    int timerWidth = MeasureText(timerText, 20);
    DrawRectangle(SCREEN_WIDTH/2 - timerWidth/2 - 20, 30, timerWidth + 40, 40, ColorAlpha(timerColor, 0.1f));
    DrawRectangleLines(SCREEN_WIDTH/2 - timerWidth/2 - 20, 30, timerWidth + 40, 40, timerColor);
    DrawText(timerText, SCREEN_WIDTH/2 - timerWidth/2, 42, 20, timerColor);
    
    // Attempts counter
    char attemptsText[32];
    sprintf(attemptsText, "TENTATIVAS: %d/%d", gameState.attempts, gameState.maxAttempts);
    DrawText(attemptsText, 50, 90, 20, COLOR_CYBER_GREEN);
    
    // Range info
    char rangeText[64];
    sprintf(rangeText, "RANGE: %d - %d", gameState.minRange, gameState.maxRange);
    DrawText(rangeText, 50, 120, 20, COLOR_CYBER_GREEN_DARK);
    
    // Suggested guess (binary search)
    int suggested = (gameState.minRange + gameState.maxRange) / 2;
    char suggestedText[64];
    sprintf(suggestedText, "SUGESTAO IA: %d", suggested);
    DrawText(suggestedText, 50, 150, 16, COLOR_CYBER_GREEN_DIM);
    
    // Input box
    int inputBoxY = SCREEN_HEIGHT/2 - 50;
    DrawRectangle(SCREEN_WIDTH/2 - 200, inputBoxY, 400, 80, COLOR_BG_BLACK);
    DrawRectangleLines(SCREEN_WIDTH/2 - 200, inputBoxY, 400, 80, COLOR_CYBER_GREEN);
    
    DrawText("DIGITE O NUMERO:", SCREEN_WIDTH/2 - 180, inputBoxY + 15, 16, COLOR_CYBER_GREEN_DARK);
    
    // Input display with cursor
    char displayText[16];
    sprintf(displayText, "%s_", gameState.inputBuffer);
    int textWidth = MeasureText(displayText, 32);
    DrawText(displayText, SCREEN_WIDTH/2 - textWidth/2, inputBoxY + 40, 32, COLOR_CYBER_GREEN);
    
    // Error message
    if (gameState.errorTimer > 0.0f)
    {
        int errorWidth = MeasureText(gameState.errorMsg, 16);
        DrawText(gameState.errorMsg, SCREEN_WIDTH/2 - errorWidth/2, inputBoxY + 90, 16, COLOR_DANGER_RED);
    }
    
    // History panel
    UI_DrawAlertBox(50, SCREEN_HEIGHT - 320, 400, 280);
    
    DrawText("HISTORICO DE TENTATIVAS:", 70, SCREEN_HEIGHT - 300, 16, COLOR_CYBER_GREEN);
    
    for (int i = 0; i < gameState.attempts && i < MAX_HISTORY; i++)
    {
        char historyText[64];
        sprintf(historyText, "%d. VALOR: %d -> %s", 
                i + 1, 
                gameState.history[i].value, 
                gameState.history[i].feedback);
        
        Color historyColor = (strcmp(gameState.history[i].feedback, "ACERTO!") == 0) 
                             ? COLOR_CYBER_GREEN : COLOR_CYBER_GREEN_DARK;
        
        DrawText(historyText, 70, SCREEN_HEIGHT - 270 + i * 30, 14, historyColor);
    }
    
    // Alert legend
    UI_DrawAlertBox(SCREEN_WIDTH - 450, SCREEN_HEIGHT - 320, 400, 280);
    DrawText("SISTEMA DE ALERTAS", SCREEN_WIDTH - 430, SCREEN_HEIGHT - 300, 16, COLOR_CYBER_GREEN);
    
    const char* alertLevels[] = {
        "1-2: SISTEMA ESTAVEL",
        "3-4: ALERTA DETECTADO",
        "5-6: RASTREAMENTO ATIVO",
        "  7: BLOQUEADO"
    };
    
    Color alertColors[] = {COLOR_CYBER_GREEN, COLOR_ALERT_YELLOW, COLOR_DANGER_RED, COLOR_LOCKED_RED};
    
    for (int i = 0; i < 4; i++)
    {
        DrawRectangle(SCREEN_WIDTH - 430, SCREEN_HEIGHT - 260 + i * 50, 360, 40, ColorAlpha(alertColors[i], 0.1f));
        DrawRectangleLines(SCREEN_WIDTH - 430, SCREEN_HEIGHT - 260 + i * 50, 360, 40, alertColors[i]);
        DrawText(alertLevels[i], SCREEN_WIDTH - 410, SCREEN_HEIGHT - 245 + i * 50, 14, alertColors[i]);
    }
    
    // Instructions
    if (gameState.questionState == QUESTION_IDLE)
    {
        DrawText("PRESSIONE ENTER PARA CONFIRMAR | ESC PARA SAIR", 
                 SCREEN_WIDTH/2 - 250, SCREEN_HEIGHT - 30, 14, COLOR_CYBER_GREEN_DIM);
    }
    
    // Question overlay
    if (gameState.questionState == QUESTION_SHOWING && currentQuestion != NULL)
    {
        // Dim background
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.75f));
        
        int overlayX = SCREEN_WIDTH/2 - 350;
        int overlayY = SCREEN_HEIGHT/2 - 200;
        int overlayW = 700;
        int overlayH = 400;
        
        // Overlay box
        DrawRectangle(overlayX, overlayY, overlayW, overlayH, COLOR_BG_BLACK);
        DrawRectangleLines(overlayX, overlayY, overlayW, overlayH, COLOR_CYBER_GREEN);
        DrawRectangleLines(overlayX + 2, overlayY + 2, overlayW - 4, overlayH - 4, COLOR_CYBER_GREEN_DIM);
        
        // Title
        DrawText(">> DESAFIO LOGICO <<", overlayX + 30, overlayY + 20, 20, COLOR_CYBER_GREEN);
        DrawText("Responda para desbloquear a dica:", overlayX + 30, overlayY + 50, 14, COLOR_CYBER_GREEN_DARK);
        
        // Question text
        DrawText(currentQuestion->question, overlayX + 30, overlayY + 85, 18, COLOR_CYBER_GREEN);
        
        // Options
        Vector2 mousePos = GetVirtualMousePosition();
        const char* labels[] = {"[1]", "[2]", "[3]", "[4]"};
        
        for (int i = 0; i < 4; i++)
        {
            Rectangle optBtn = {overlayX + 30, overlayY + 120 + i * 60, 640, 45};
            bool hover = CheckCollisionPointRec(mousePos, optBtn);
            
            Color btnBg = hover ? ColorAlpha(COLOR_CYBER_GREEN, 0.15f) : ColorAlpha(COLOR_CYBER_GREEN, 0.05f);
            Color btnBorder = hover ? COLOR_CYBER_GREEN : COLOR_CYBER_GREEN_DIM;
            
            DrawRectangle(optBtn.x, optBtn.y, optBtn.width, optBtn.height, btnBg);
            DrawRectangleLines(optBtn.x, optBtn.y, optBtn.width, optBtn.height, btnBorder);
            
            char optText[160];
            sprintf(optText, "%s %s", labels[i], currentQuestion->options[i]);
            DrawText(optText, optBtn.x + 15, optBtn.y + 14, 16, hover ? COLOR_CYBER_GREEN : COLOR_CYBER_GREEN_DARK);
        }
        
        DrawText("USE TECLAS 1-4 OU CLIQUE NA OPCAO", overlayX + 30, overlayY + overlayH - 30, 12, COLOR_CYBER_GREEN_DIM);
    }
    
    // Question feedback overlay
    if (gameState.questionState == QUESTION_ANSWERED_CORRECT)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.7f));
        const char* msg = ">> DICA DESBLOQUEADA <<";
        int msgW = MeasureText(msg, 36);
        float alpha = (sinf(glowPulse * 6.0f) + 1.0f) * 0.3f + 0.7f;
        DrawText(msg, SCREEN_WIDTH/2 - msgW/2, SCREEN_HEIGHT/2 - 18, 36, ColorAlpha(COLOR_CYBER_GREEN, alpha));
    }
    else if (gameState.questionState == QUESTION_ANSWERED_WRONG)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.7f));
        const char* msg = ">> DICA NEGADA <<";
        int msgW = MeasureText(msg, 36);
        float alpha = (sinf(glowPulse * 6.0f) + 1.0f) * 0.3f + 0.7f;
        DrawText(msg, SCREEN_WIDTH/2 - msgW/2, SCREEN_HEIGHT/2 - 18, 36, ColorAlpha(COLOR_DANGER_RED, alpha));
    }
}

//------------------------------------------------------------------------------------
// Result Screen
//------------------------------------------------------------------------------------
static void UpdateResultScreen(void)
{
    // Keep game music 2 playing (already looping) or switch to it
    if (currentMusicTrack != 3)
    {
        SwitchMusic(&musicGame2, 3);
    }
    
    Vector2 mousePos = GetVirtualMousePosition();
    
    // Button: Play Again
    Rectangle btnPlayAgain = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 150, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnPlayAgain) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentScreen = SCREEN_DIFFICULTY_SELECT;
    }
    
    // Button: Menu
    Rectangle btnMenu = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 220, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnMenu) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentScreen = SCREEN_MAIN_MENU;
    }
}

static void DrawResultScreen(void)
{
    DrawGlitchEffect();
    
    Vector2 mousePos = GetVirtualMousePosition();
    
    // Main result box
    Color borderColor = gameState.won ? COLOR_CYBER_GREEN : COLOR_DANGER_RED;
    DrawRectangle(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 300, 800, 500, COLOR_BG_BLACK);
    DrawRectangleLines(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 300, 800, 500, borderColor);
    DrawRectangleLines(SCREEN_WIDTH/2 - 397, SCREEN_HEIGHT/2 - 297, 794, 494, borderColor);
    
    // Result icon and title
    const char* resultTitle;
    if (gameState.won) {
        resultTitle = "SENHA DESBLOQUEADA";
    } else if (gameState.timedOut) {
        resultTitle = "TEMPO ESGOTADO";
    } else {
        resultTitle = "SISTEMA BLOQUEADO";
    }
    
    int titleWidth = MeasureText(resultTitle, 48);
    UI_DrawGlowText(resultTitle, SCREEN_WIDTH/2 - titleWidth/2, SCREEN_HEIGHT/2 - 230, 48, borderColor, glowPulse);
    
    // Attempts info
    DrawRectangle(SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 100, 600, 100, ColorAlpha(COLOR_CYBER_GREEN, 0.05f));
    DrawRectangleLines(SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 100, 600, 100, COLOR_CYBER_GREEN_DIM);
    
    DrawText("TENTATIVAS UTILIZADAS:", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 - 80, 14, COLOR_CYBER_GREEN_DARK);
    
    char attemptsText[8];
    sprintf(attemptsText, "%d", gameState.attempts);
    int attemptsWidth = MeasureText(attemptsText, 48);
    DrawText(attemptsText, SCREEN_WIDTH/2 - attemptsWidth/2, SCREEN_HEIGHT/2 - 50, 48, COLOR_CYBER_GREEN);
    
    // High score indicator
    if (gameState.won && gameState.attempts <= stats.bestScore)
    {
        const char* newRecord = "*** NOVO RECORDE ***";
        int recordWidth = MeasureText(newRecord, 20);
        float recordAlpha = (sinf(glowPulse * 4.0f) + 1.0f) * 0.5f;
        DrawText(newRecord, SCREEN_WIDTH/2 - recordWidth/2, SCREEN_HEIGHT/2 + 20, 20, 
                 ColorAlpha(COLOR_ALERT_YELLOW, recordAlpha));
    }
    
    // Target number reveal
    char targetText[32];
    sprintf(targetText, "SENHA: %d", gameState.targetNumber);
    int targetWidth = MeasureText(targetText, 24);
    DrawText(targetText, SCREEN_WIDTH/2 - targetWidth/2, SCREEN_HEIGHT/2 + 60, 24, COLOR_CYBER_GREEN_DARK);
    
    // Buttons
    Rectangle btnPlayAgain = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 150, 300, 50};
    bool hoverPlayAgain = CheckCollisionPointRec(mousePos, btnPlayAgain);
    UI_DrawCyberButton(">> TENTAR NOVAMENTE", btnPlayAgain.x, btnPlayAgain.y, 
                    btnPlayAgain.width, btnPlayAgain.height, hoverPlayAgain);
    
    Rectangle btnMenu = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 220, 300, 50};
    bool hoverMenu = CheckCollisionPointRec(mousePos, btnMenu);
    UI_DrawCyberButton(">> MENU PRINCIPAL", btnMenu.x, btnMenu.y, btnMenu.width, btnMenu.height, hoverMenu);
}

//------------------------------------------------------------------------------------
// Statistics Screen
//------------------------------------------------------------------------------------
static void UpdateStatsScreen(void)
{
    if (IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentScreen = SCREEN_MAIN_MENU;
    }
}

static void DrawStatsScreen(void)
{
    DrawMatrixEffect();
    
    // Header
    UI_DrawGlowText("ANALISE ESTATISTICA", SCREEN_WIDTH/2 - 260, 50, 48, COLOR_CYBER_GREEN, glowPulse);
    
    char sessionsText[64];
    sprintf(sessionsText, "PROCESSANDO DADOS DE %d SESSOES...", stats.sessionCount);
    int sessionsWidth = MeasureText(sessionsText, 14);
    DrawText(sessionsText, SCREEN_WIDTH/2 - sessionsWidth/2, 120, 14, COLOR_CYBER_GREEN_DARK);
    
    if (stats.sessionCount == 0)
    {
        const char* noData = "NENHUM DADO DISPONIVEL.";
        const char* startMsg = "INICIE UMA SESSAO PARA COLETAR ESTATISTICAS.";
        
        int noDataWidth = MeasureText(noData, 24);
        int startWidth = MeasureText(startMsg, 16);
        
        DrawRectangle(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 100, 800, 200, ColorAlpha(COLOR_CYBER_GREEN, 0.05f));
        DrawRectangleLines(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 100, 800, 200, COLOR_CYBER_GREEN_DIM);
        
        DrawText(noData, SCREEN_WIDTH/2 - noDataWidth/2, SCREEN_HEIGHT/2 - 20, 24, COLOR_CYBER_GREEN_DARK);
        DrawText(startMsg, SCREEN_WIDTH/2 - startWidth/2, SCREEN_HEIGHT/2 + 20, 16, COLOR_CYBER_GREEN_DIM);
    }
    else
    {
        // Stats cards
        int cardY = 180;
        int cardWidth = 250;
        int cardHeight = 120;
        int spacing = 30;
        int startX = (SCREEN_WIDTH - (cardWidth * 3 + spacing * 2)) / 2;
        
        // Card 1: Total Sessions
        DrawRectangle(startX, cardY, cardWidth, cardHeight, ColorAlpha(COLOR_CYBER_GREEN, 0.05f));
        DrawRectangleLines(startX, cardY, cardWidth, cardHeight, COLOR_CYBER_GREEN);
        DrawText("TOTAL SESSOES", startX + 20, cardY + 20, 14, COLOR_CYBER_GREEN_DARK);
        char totalText[16];
        sprintf(totalText, "%d", stats.sessionCount);
        DrawText(totalText, startX + 20, cardY + 50, 42, COLOR_CYBER_GREEN);
        
        // Card 2: Win Rate
        DrawRectangle(startX + cardWidth + spacing, cardY, cardWidth, cardHeight, 
                     ColorAlpha(COLOR_ALERT_YELLOW, 0.05f));
        DrawRectangleLines(startX + cardWidth + spacing, cardY, cardWidth, cardHeight, COLOR_ALERT_YELLOW);
        DrawText("TAXA VITORIA", startX + cardWidth + spacing + 20, cardY + 20, 14, COLOR_CYBER_GREEN_DARK);
        char winRateText[16];
        sprintf(winRateText, "%.1f%%", stats.winRate);
        DrawText(winRateText, startX + cardWidth + spacing + 20, cardY + 50, 42, COLOR_ALERT_YELLOW);
        
        // Card 3: Best Score
        DrawRectangle(startX + (cardWidth + spacing) * 2, cardY, cardWidth, cardHeight, 
                     ColorAlpha(COLOR_DANGER_RED, 0.05f));
        DrawRectangleLines(startX + (cardWidth + spacing) * 2, cardY, cardWidth, cardHeight, COLOR_DANGER_RED);
        DrawText("MELHOR SCORE", startX + (cardWidth + spacing) * 2 + 20, cardY + 20, 14, COLOR_CYBER_GREEN_DARK);
        char bestText[16];
        sprintf(bestText, "%d", stats.bestScore <= MAX_ATTEMPTS ? stats.bestScore : 0);
        DrawText(bestText, startX + (cardWidth + spacing) * 2 + 20, cardY + 50, 42, COLOR_DANGER_RED);
        
        // Additional stats
        cardY += 170;
        DrawRectangle(startX, cardY, cardWidth, cardHeight, ColorAlpha(COLOR_CYBER_GREEN, 0.05f));
        DrawRectangleLines(startX, cardY, cardWidth, cardHeight, COLOR_CYBER_GREEN_DIM);
        DrawText("MEDIA TENTATIVAS", startX + 20, cardY + 20, 14, COLOR_CYBER_GREEN_DARK);
        char avgText[16];
        sprintf(avgText, "%.2f", stats.avgAttempts);
        DrawText(avgText, startX + 20, cardY + 50, 42, COLOR_CYBER_GREEN);
        
        // Recent sessions history
        int historyY = cardY + 170;
        DrawText("ULTIMAS SESSOES:", startX, historyY, 18, COLOR_CYBER_GREEN);
        
        int displayCount = (stats.sessionCount < 10) ? stats.sessionCount : 10;
        for (int i = 0; i < displayCount; i++)
        {
            int idx = stats.sessionCount - 1 - i;
            GameSession* session = &stats.sessions[idx];
            
            char sessionText[128];
            sprintf(sessionText, "#%d [%s] - %d tent. - %s", 
                    idx + 1,
                    session->date,
                    session->attempts,
                    session->won ? "VITORIA" : "DERROTA");
            
            Color sessionColor = session->won ? COLOR_CYBER_GREEN : COLOR_DANGER_RED;
            DrawText(sessionText, startX, historyY + 40 + i * 30, 14, sessionColor);
        }
    }
    
    // Instructions
    DrawText("PRESSIONE ESC OU CLIQUE PARA VOLTAR", 
             SCREEN_WIDTH/2 - 180, SCREEN_HEIGHT - 40, 14, COLOR_CYBER_GREEN_DIM);
}
