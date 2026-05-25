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
static float glowPulse = 0.0f;
static bool showLogs = false;
static float logTimer = 0.0f;
static int currentLog = 0;

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

// Screen specific functions
static void UpdateMainMenu(void);
static void DrawMainMenu(void);
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
    // NOTE: For Virtual Machines, you may need to disable HighDPI and MSAA
    // If window doesn't appear, comment out FLAG_WINDOW_HIGHDPI and FLAG_MSAA_4X_HINT
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GH0ST: 2047 - Neural Guessing System");
    
    SearchAndSetResourceDir("resources");
    SetTargetFPS(60);
    srand(time(NULL));
    
    InitGame();
    
    // Main game loop
    while (!WindowShouldClose())
    {
        UpdateGame();
        DrawGame();
    }
    
    UnloadGame();
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
}

static void UpdateGame(void)
{
    glowPulse += GetFrameTime();
    UpdateMatrixEffect();
    UpdateGlitchEffect();
    
    switch (currentScreen)
    {
        case SCREEN_MAIN_MENU:
            UpdateMainMenu();
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
    BeginDrawing();
    ClearBackground(COLOR_BG_BLACK);
    
    switch (currentScreen)
    {
        case SCREEN_MAIN_MENU:
            DrawMainMenu();
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
    
    EndDrawing();
}

static void UnloadGame(void)
{
    QuestionBank_Unload(&questionBank);
}

//------------------------------------------------------------------------------------
// Main Menu Screen
//------------------------------------------------------------------------------------
static void UpdateMainMenu(void)
{
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
    
    Vector2 mousePos = GetMousePosition();
    
    // Button: Start Game
    Rectangle btnStart = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 100, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Game_StartNew(&gameState);
        currentQuestion = QuestionBank_GetRandom(&questionBank);
        currentScreen = SCREEN_GAME;
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
    
    Vector2 mousePos = GetMousePosition();
    
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
}

//------------------------------------------------------------------------------------
// Game Screen
//------------------------------------------------------------------------------------
static void UpdateGameScreen(void)
{
    if (gameState.gameOver) return;
    
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
        Vector2 mousePos = GetMousePosition();
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
        Vector2 mousePos = GetMousePosition();
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
    Vector2 mousePos = GetMousePosition();
    
    // Button: Play Again
    Rectangle btnPlayAgain = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 150, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnPlayAgain) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Game_StartNew(&gameState);
        currentQuestion = QuestionBank_GetRandom(&questionBank);
        currentScreen = SCREEN_GAME;
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
    
    Vector2 mousePos = GetMousePosition();
    
    // Main result box
    Color borderColor = gameState.won ? COLOR_CYBER_GREEN : COLOR_DANGER_RED;
    DrawRectangle(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 300, 800, 500, COLOR_BG_BLACK);
    DrawRectangleLines(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 300, 800, 500, borderColor);
    DrawRectangleLines(SCREEN_WIDTH/2 - 397, SCREEN_HEIGHT/2 - 297, 794, 494, borderColor);
    
    // Result icon and title
    const char* resultTitle = gameState.won ? "SENHA DESBLOQUEADA" : "SISTEMA BLOQUEADO";
    
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
